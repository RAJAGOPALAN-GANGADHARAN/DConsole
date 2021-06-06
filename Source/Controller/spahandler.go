package main

import (
	"fmt"
	"net/http"

	"github.com/gorilla/mux"
)

func serveStatic(router *mux.Router, staticDir string, baseDir string) {
	staticPaths := map[string]string{
		"/css/": staticDir + "/css/",
		"/js/":  staticDir + "/js/",
	}
	for _, pathValue := range staticPaths {
		fmt.Println(baseDir + pathValue)
		fmt.Println(pathValue)
		router.PathPrefix(pathValue).Handler(http.StripPrefix(pathValue,
			http.FileServer(http.Dir("."+baseDir+pathValue))))
	}

}
