package main

import (
	"encoding/json"
	"fmt"
	"net/http"

	"github.com/gorilla/mux"
)

func addToChannel(messageDump []Message, id string) {
	_, present := channels[id]
	if !present {
		fmt.Println("Creating a new channel for id:" + id)
		channels[id] = make(chan Message, 10)
	}

	for _, message := range messageDump {
		channels[id] <- message
	}
	fmt.Println("Dump Routine success")
}

func addToStash(messageDump []Message, id string) {
	_, present := memoryStash[id]
	if !present {
		fmt.Println("Creating a new channel for id:" + id)
		memoryStash[id] = make([]Message, 0)
	}

	memoryStash[id] = append(memoryStash[id], messageDump...)
	fmt.Println("Data dump to stash success")
}

func getLogsForId(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	params := mux.Vars(r)
	//Check if id exist
	memory, present := memoryStash[params["id"]]
	if !present {
		json.NewEncoder(w).Encode(make([]Message, 0))
		return
	}

	fmt.Println("Get ID:" + params["id"])
	json.NewEncoder(w).Encode(memory)
}

func dumpLogsForId(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	params := mux.Vars(r)
	fmt.Println("Post ID:" + params["id"])

	var messageDump []Message
	json.NewDecoder(r.Body).Decode(&messageDump)
	fmt.Println(messageDump)

	go addToChannel(messageDump, params["id"])
	//  Directly add to map  //
	addToStash(messageDump, params["id"])
	//    //
	json.NewEncoder(w).Encode("Dumped successfully for id:" + params["id"])
}

func readFromChannel(w http.ResponseWriter, r *http.Request) {

	// w.Header().Set("Content-Type", "text/event-stream")
	// w.Header().Set("Connection", "keep-alive")
	// w.Header().Set("Cache-Control", "no-cache")
	// w.WriteHeader(200)

	w.Header().Set("Content-Type", "application/json")
	id := mux.Vars(r)["id"]

	dataChannel, present := channels[id]
	msgArr := make([]Message, 0)
	if !present {
		json.NewEncoder(w).Encode(msgArr)
		return
	}

	for {
		select {
		case msg, ok := <-dataChannel:
			if ok {
				msgArr = append(msgArr, msg)
			} else {
				goto dirty
			}
		default:
			goto dirty
		}
	}
dirty:
	json.NewEncoder(w).Encode(msgArr)
}

func createRouter() *mux.Router {
	router := mux.NewRouter()

	router.HandleFunc("/api/logs/{id}", getLogsForId).Methods("GET")
	router.HandleFunc("/api/logs/{id}", dumpLogsForId).Methods("POST")
	router.HandleFunc("/api/stream/{id}", readFromChannel).Methods("GET")
	return router
}
