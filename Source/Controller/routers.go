package main

import (
	"DConsole/pkg/websocket"
	"fmt"
	"net/http"

	"github.com/gorilla/mux"
)

var trackCnt int = 0

func masterSocketConnections(pool *websocket.Pool, w http.ResponseWriter, r *http.Request) {
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
}

func handleProcessConnection(pool *websocket.Pool, w http.ResponseWriter, r *http.Request) {
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

	client.ListenProcess()
}

func tabSocketConnection(pool *websocket.Pool, w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)

	fmt.Println("New Tab socket Connection:" + vars["tabName"])
	conn, err := websocket.Upgrade(w, r)
	if err != nil {
		fmt.Fprintf(w, "%+v\n", err)
	}

	trackCnt += 1
	client := &websocket.Client{
		ID:   "tab_socket_" + fmt.Sprint(trackCnt),
		Conn: conn,
		Pool: pool,
	}

	fmt.Println("Assigned ID:" + client.ID)
	//pool.Register <- client

	client.TabChannel(vars["tabName"])
}

func setupRoutes(router *mux.Router) {
	clientPool := websocket.NewPool()
	go clientPool.HandleClient()

	processPool := websocket.NewPool()
	go processPool.HandleProcess()

	router.HandleFunc("/masterSocketConnection", func(w http.ResponseWriter, r *http.Request) {
		masterSocketConnections(clientPool, w, r)
	})
	router.HandleFunc("/tabSocketConnection/{tabName}", func(rw http.ResponseWriter, r *http.Request) {
		tabSocketConnection(clientPool, rw, r)
	})
	router.HandleFunc("/processConnection", func(rw http.ResponseWriter, r *http.Request) {
		handleProcessConnection(processPool, rw, r)
	})

}

func createRouter() *mux.Router {
	router := mux.NewRouter()

	router.Handle("/", http.FileServer(http.Dir("./build")))
	serveStatic(router, "/static", "/build")
	setupRoutes(router)

	return router
}
