package websocket

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"
)

type Pool struct {
	Register        chan *Client
	Unregister      chan *Client
	Clients         map[*Client]bool
	Broadcast       chan Message
	MasterBroadcast chan Message
	Stream          chan Message
}

func NewPool() *Pool {
	return &Pool{
		Register:        make(chan *Client),
		Unregister:      make(chan *Client),
		Clients:         make(map[*Client]bool),
		Broadcast:       make(chan Message),
		MasterBroadcast: make(chan Message),
		Stream:          make(chan Message),
	}
}

func (pool *Pool) HandleClient() {
	for {
		select {
		case client := <-pool.Register:
			pool.Clients[client] = true
			fmt.Println("Size of Connection Pool: ", len(pool.Clients))
			for client, _ := range pool.Clients {
				fmt.Println(client)
				client.Conn.WriteJSON(Message{Type: 1, Body: "New User Joined..."})
			}
			break
		case client := <-pool.Unregister:
			delete(pool.Clients, client)
			fmt.Println("Size of Connection Pool: ", len(pool.Clients))
			for client, _ := range pool.Clients {
				client.Conn.WriteJSON(Message{Type: 1, Body: "User Disconnected..."})
			}
			break
		case message := <-pool.Broadcast:
			fmt.Println("Sending message to all clients in Pool")
			for client, _ := range pool.Clients {
				if err := client.Conn.WriteJSON(message); err != nil {
					fmt.Println(err)
					return
				}
			}
			break

		case masterMessage := <-pool.MasterBroadcast:
			fmt.Println("Sending new tab data to all master Sockets")
			for client, _ := range pool.Clients {
				if !strings.Contains(client.ID, "master_socket_") {
					continue
				}
				if err := client.Conn.WriteJSON(masterMessage); err != nil {
					fmt.Println(err)
					return
				}
			}
		}
	}
}

func (pool *Pool) HandleProcess() {
	for {
		select {
		case client := <-pool.Register:
			pool.Clients[client] = true
			fmt.Println("Size of Connection Pool: ", len(pool.Clients))
			for client, _ := range pool.Clients {
				fmt.Println(client)
				client.Conn.WriteJSON(Message{Type: 1, Body: "New Process Connected..."})
			}
			break
		case client := <-pool.Unregister:
			delete(pool.Clients, client)
			fmt.Println("Size of Connection Pool: ", len(pool.Clients))
			for client, _ := range pool.Clients {
				client.mu.Lock()
				client.Conn.WriteJSON(Message{Type: 1, Body: "User Disconnected..."})
				client.mu.Unlock()
			}
			break
		case message := <-pool.Stream:
			file, _ := os.OpenFile(ConstructLogPath(message.Tab), os.O_RDWR|os.O_APPEND|os.O_CREATE, 0660)
			fmt.Println("Writing message:" + message.Body)
			file.WriteString(message.Body + "\n")
			file.Close()
			break
		}
	}
}

type ProcessMessageQ struct {
	connectedClients map[*Client]bool
	FileName         string
	Register         chan *Client
	Unregister       chan *Client
	Broadcast        chan *Message
}

func NewProcessMessageQ(fileName string) *ProcessMessageQ {
	return &ProcessMessageQ{
		connectedClients: make(map[*Client]bool),
		FileName:         fileName,
		Register:         make(chan *Client),
		Unregister:       make(chan *Client),
		Broadcast:        make(chan *Message),
	}
}

func (queue *ProcessMessageQ) MainLoop() {
	for {
		select {
		case client := <-queue.Register:
			queue.connectedClients[client] = true
			break
		case client := <-queue.Unregister:
			delete(queue.connectedClients, client)
			break
		case message := <-queue.Broadcast:
			for client, _ := range queue.connectedClients {
				client.mu.Lock()
				client.Conn.WriteJSON(message)
				client.mu.Unlock()
			}

			file, _ := os.OpenFile(queue.FileName, os.O_RDWR|os.O_APPEND|os.O_CREATE, 0660)
			fmt.Println("Writing message:" + message.Body)
			messageJson, _ := json.Marshal(message)
			file.Write(messageJson)
			file.WriteString("\n")
			file.Close()
			break
		}
	}
}

type ProcessMessageMaster struct {
	messageQueues map[string]*ProcessMessageQ
	Register      chan Message
	Unregister    chan string
	Notify        chan *Client
	NotifyNew     chan *Client
}

func NewProcessMessageMaster() *ProcessMessageMaster {
	return &ProcessMessageMaster{
		messageQueues: make(map[string]*ProcessMessageQ),
		Register:      make(chan Message),
		Unregister:    make(chan string),
		Notify:        make(chan *Client),
		NotifyNew:     make(chan *Client),
	}
}

func (master *ProcessMessageMaster) MainLoop(pool *Pool) {
	for {
		select {
		case message := <-master.Register:
			if _, ok := master.messageQueues[message.Tab]; !ok {
				master.messageQueues[message.Tab] = NewProcessMessageQ(ConstructLogPath(message.Tab))
				go master.messageQueues[message.Tab].MainLoop()
				tabMessage := &Message{
					Type: 100, //100 is a tab spawn message
					Body: message.Tab,
				}
				pool.Broadcast <- *tabMessage
				//do something here
			}
			fmt.Printf("Broadcast Message %+v", message)
			master.messageQueues[message.Tab].Broadcast <- &message
			break
		case client := <-master.Notify:
			fmt.Println("Registering a new client for tab" + client.TabName)
			master.messageQueues[client.TabName].Register <- client
			break
		case client := <-master.NotifyNew:
			fmt.Println("Send all tab details to new master Socket")
			for tab, _ := range master.messageQueues {
				client.Conn.WriteJSON(&Message{
					Type: 100,
					Body: tab,
				})
				fmt.Println(tab)
			}
			break
		}
	}
}
