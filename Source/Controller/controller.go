package main

import (
	"github.com/rs/cors"
	"log"
	"net/http"
)

func main() {
	log.Println("Starting DConsole Controller")
	//runtime.GOMAXPROCS(2)
	router := createRouter()
	handler := cors.Default().Handler(router)
	//log.Fatal(http.ListenAndServe(":"+os.Getenv("PORT"), handler))

	srv := &http.Server{
		Handler: handler,
		Addr:    "127.0.0.1:8080",
		// Good practice: enforce timeouts for servers you create!
	}
	log.Fatal(srv.ListenAndServe())
}
