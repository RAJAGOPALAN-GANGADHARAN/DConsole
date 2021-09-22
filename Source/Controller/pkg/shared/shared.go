package shared

import (
	"log"
	"os"
	"path/filepath"
)

var paths = [...]string{
	"./",
	"../Shared/",
}

func ConstructLogPath(file string) string {
	return filepath.Join("logs", file+"DConsole.log")
}

func SearchAndGetFromPaths(filename string) (string, error) {
	dir, err := os.Getwd()
	if err != nil {
		log.Fatal(err)
	}
	for path := range paths {
		constructed := filepath.Clean(filepath.Join(dir, paths[path],
			filename))
		if _, err := os.Stat(constructed); !os.IsNotExist(err) {
			return constructed, nil
		}
	}
	return "", err
}
