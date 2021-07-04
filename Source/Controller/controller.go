package main

import (
	"fmt"
	"log"
	"net/http"
	"time"

	"github.com/rs/cors"
)

func main() {
	fmt.Println("Starting a webserver")

	router := createRouter()
	handler := cors.Default().Handler(router)
	//log.Fatal(http.ListenAndServe(":"+os.Getenv("PORT"), handler))

	srv := &http.Server{
		Handler: handler,
		Addr:    "127.0.0.1:8080",
		// Good practice: enforce timeouts for servers you create!
		WriteTimeout: 15 * time.Second,
		ReadTimeout:  15 * time.Second,
	}
	log.Fatal(srv.ListenAndServe())
}
