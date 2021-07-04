package websocket

import (
	"fmt"
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
			// case message := <-pool.Stream:
			// 	for client, _ := range pool.Clients {
			// 		if err := client.Conn.WriteJSON(message); err != nil {
			// 			fmt.Println(err)
			// 			return
			// 		}
			// 	}
		}
	}
}
