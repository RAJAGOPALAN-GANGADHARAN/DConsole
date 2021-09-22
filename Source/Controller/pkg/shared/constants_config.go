package shared

import (
	"bufio"
	"io"
	"log"
	"os"
	"strconv"
	"strings"
)

type Config map[string]string

var ConstantsConfig Config

func readConfig(conf *Config, filename string) {
	file, err := os.Open(filename)
	if err != nil {
		return
	}
	defer file.Close()

	reader := bufio.NewReader(file)

	for {
		line, err := reader.ReadString('\n')
		if equal := strings.Index(line, "="); equal >= 0 {
			if key := strings.TrimSpace(line[:equal]); len(key) > 0 {
				value := ""
				if len(line) > equal {
					value = strings.TrimSpace(line[equal+1:])
				}
				ConstantsConfig[key] = value
			}
		}
		if err == io.EOF {
			break
		}
		if err != nil {
			return
		}
	}
}

func ConfigInit() {
	ConstantsConfig = make(Config)
	configPath, err := SearchAndGetFromPaths("constants.shared")
	if err != nil {
		log.Fatal(err)
		os.Exit(-1)
	}
	readConfig(&ConstantsConfig, configPath)
	log.Println(ConstantsConfig["TAB_CREATION_CODE"])
}

func GetProperty(property string) string {
	return ConstantsConfig[property]
}
func GetPropertyAsInt(property string) int{
	val,_ := strconv.Atoi(ConstantsConfig[property])
	return val
}
