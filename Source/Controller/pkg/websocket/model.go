package websocket

import (
	"bufio"
	"encoding/json"
	"fmt"
	"log"
	"net"
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
type SocketClient struct {
	ID   string
	Pool *SocketPool
	Conn net.Conn
	Data chan []byte
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
		fmt.Printf("Message Received: %+v\n", message)

	}
}

func (c *SocketClient) ListenProcess(masterQueue *ProcessMessageMaster) {

	defer func() {
		fmt.Println("Defering Listen Process")
		c.Pool.Unregister <- c
		c.Conn.Close()
	}()

	decoder := json.NewDecoder(c.Conn)

	for {
		// byteArray := make([]byte, 4096)

		// _, err := c.Conn.Read(byteArray)
		var message Message
		err := decoder.Decode(&message)
		if err != nil {
			fmt.Println("error occured:" + err.Error())
			log.Println(err)
			return
		}
		// fmt.Print(string(byteArray))
		// json.Unmarshal(byteArray, &message)
		masterQueue.Register <- message

		fmt.Printf("Message Received: %d %s\n", message.Type, message.Body)
		fmt.Printf("Message Received: %+v\n", message)
	}
}

func (c *Client) TabChannel(tabName string) {
	fmt.Println("Opening file:" + tabName)
	fileSeek, _ := os.Open(ConstructLogPath(tabName))
	scanner := bufio.NewScanner(fileSeek)
	defer func() {
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
