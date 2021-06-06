package main

import (
	"fmt"
	"log"
	"net/http"

	"github.com/rs/cors"
)

func main() {
	fmt.Println("Starting a webserver")
	channels = make(map[string]chan Message)
	memoryStash = make(map[string][]Message)

	router := createRouter()
	handler := cors.Default().Handler(router)
	//log.Fatal(http.ListenAndServe(":"+os.Getenv("PORT"), handler))
	log.Fatal(http.ListenAndServe(":8080", handler))
}
