package main

import (
	"DConsole/pkg/websocket"
	"fmt"
	"net/http"

	"github.com/gorilla/mux"
)

var trackCnt int = 0

func masterSocketConnections(masterQueue *websocket.ProcessMessageMaster, pool *websocket.Pool, w http.ResponseWriter, r *http.Request) {
	fmt.Println("New incoming master Socket Connection")
	conn, err := websocket.Upgrade(w, r)
	if err != nil {
		fmt.Fprintf(w, "%+v\n", err)
	}

	trackCnt += 1
	client := &websocket.Client{
		ID:   "master_socket_" + fmt.Sprint(trackCnt),
		Conn: conn,
		Pool: pool,
	}

	fmt.Println("Assigned ID:" + client.ID)
	pool.Register <- client
	masterQueue.NotifyNew <- client
}

func handleProcessConnection(masterQueue *websocket.ProcessMessageMaster, pool *websocket.Pool, w http.ResponseWriter, r *http.Request) {
	fmt.Println("New incoming process Connection")
	conn, err := websocket.Upgrade(w, r)
	if err != nil {
		fmt.Fprintf(w, "%+v\n", err)
	}

	trackCnt += 1
	client := &websocket.Client{
		ID:   "process_socket_" + fmt.Sprint(trackCnt),
		Conn: conn,
		Pool: pool,
	}

	fmt.Println("Assigned ID:" + client.ID)
	//pool.Register <- client

	client.ListenProcess(masterQueue)
}

func tabSocketConnection(masterQueue *websocket.ProcessMessageMaster, pool *websocket.Pool, w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)

	fmt.Println("New Tab socket Connection:" + vars["tabName"])
	conn, err := websocket.Upgrade(w, r)
	if err != nil {
		fmt.Fprintf(w, "%+v\n", err)
	}

	trackCnt += 1
	client := &websocket.Client{
		ID:      "tab_socket_" + fmt.Sprint(trackCnt),
		Conn:    conn,
		Pool:    pool,
		TabName: vars["tabName"],
	}

	fmt.Println("Assigned ID:" + client.ID)
	masterQueue.Notify <- client
	fmt.Println("Notified queue")
	client.TabChannel(vars["tabName"])
}

func setupRoutes(router *mux.Router) {

	clientPool := websocket.NewPool()
	go clientPool.HandleClient()

	processPool := websocket.NewPool()
	go processPool.HandleProcess()

	messageQueueMaster := websocket.NewProcessMessageMaster()
	go messageQueueMaster.MainLoop(clientPool)

	router.HandleFunc("/masterSocketConnection", func(w http.ResponseWriter, r *http.Request) {
		masterSocketConnections(messageQueueMaster, clientPool, w, r)
	})
	router.HandleFunc("/tabSocketConnection/{tabName}", func(rw http.ResponseWriter, r *http.Request) {
		tabSocketConnection(messageQueueMaster, clientPool, rw, r)
	})
	router.HandleFunc("/processConnection", func(rw http.ResponseWriter, r *http.Request) {
		handleProcessConnection(messageQueueMaster, processPool, rw, r)
	})

	go websocket.RawSocketServer()

}

func createRouter() *mux.Router {
	router := mux.NewRouter()

	router.Handle("/", http.FileServer(http.Dir("./build")))
	serveStatic(router, "/static", "/build")
	setupRoutes(router)

	return router
}
