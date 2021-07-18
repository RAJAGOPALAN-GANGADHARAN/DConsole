package websocket

type SocketClientManager struct {
	clients    map[*SocketClient]bool
	broadcast  chan []byte
	register   chan *SocketClient
	unregister chan *SocketClient
}

// type SocketClient struct {
// 	socket net.Conn
// 	data   chan []byte
// }

// func (manager *SocketClientManager) start() {
// 	for {
// 		select {
// 		case connection := <-manager.register:
// 			manager.clients[connection] = true
// 			fmt.Println("Added new connection!")
// 		case connection := <-manager.unregister:
// 			if _, ok := manager.clients[connection]; ok {
// 				close(connection.data)
// 				delete(manager.clients, connection)
// 				fmt.Println("A connection has terminated!")
// 			}
// 		case message := <-manager.broadcast:
// 			for connection := range manager.clients {
// 				select {
// 				case connection.data <- message:
// 				default:
// 					close(connection.data)
// 					delete(manager.clients, connection)
// 				}
// 			}
// 		}
// 	}
// }

// func (manager *SocketClientManager) receive(client *SocketClient) {
// 	for {
// 		message := make([]byte, 4096)
// 		length, err := client.socket.Read(message)
// 		if err != nil {
// 			manager.unregister <- client
// 			client.socket.Close()
// 			break
// 		}
// 		if length > 0 {
// 			fmt.Println("RECEIVED: " + string(message))
// 			manager.broadcast <- message
// 		}
// 	}
// }

// func (manager *SocketClientManager) send(client *SocketClient) {
// 	defer client.socket.Close()
// 	for {
// 		select {
// 		case message, ok := <-client.data:
// 			if !ok {
// 				return
// 			}
// 			client.socket.Write(message)
// 		}
// 	}
// }
