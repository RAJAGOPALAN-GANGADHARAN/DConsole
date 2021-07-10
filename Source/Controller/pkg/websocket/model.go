package websocket

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"sync"

	"github.com/gorilla/websocket"
)

type Client struct {
	ID      string
	Conn    *websocket.Conn
	Pool    *Pool
	mu      sync.Mutex
	TabName string
}

type Message struct {
	Type int    `json:"type"`
	Body string `json:"body"`
	Tab  string `json:"tab"`
}

func (c *Client) Read() {
	defer func() {
		c.Pool.Unregister <- c
		c.Conn.Close()
	}()

	for {
		messageType, p, err := c.Conn.ReadMessage()
		if err != nil {
			log.Println(err)
			return
		}
		fmt.Printf("Message Received: %d %s\n", messageType, p)

		message := Message{Type: messageType, Body: string(p)}
		//c.Pool.Broadcast <- message
		fmt.Printf("Message Received: %+v\n", message)

	}
}

func (c *Client) ListenProcess(masterQueue *ProcessMessageMaster) {
	defer func() {
		fmt.Println("Defering Listen Process")
		c.Pool.Unregister <- c
		c.Conn.Close()
	}()

	for {
		var message Message

		err := c.Conn.ReadJSON(&message)
		if err != nil {
			fmt.Println("error occured:" + err.Error())
			log.Println(err)
			return
		}
		masterQueue.Register <- message

		fmt.Printf("Message Received: %d %s\n", message.Type, message.Body)
		//var message Message
		// json.Unmarshal(p, &message)
		//c.Pool.Broadcast <- message
		// c.Pool.Stream <- message
		fmt.Printf("Message Received: %+v\n", message)

	}
}

func (c *Client) TabChannel(tabName string) {
	fmt.Println("Opening file:" + tabName)
	fileSeek, _ := os.Open(tabName + "_logBase.log")
	scanner := bufio.NewScanner(fileSeek)
	defer func() {
		// c.Pool.Unregister <- c
		// c.Conn.Close()
		fileSeek.Close()
	}()
	fmt.Println("************* File Streaming to Tab *********")
	for {
		if scanner.Scan() {
			c.Conn.WriteMessage(1, []byte(scanner.Text()))
		} else {
			return
		}
	}
}
