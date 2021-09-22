import { useEffect, useState } from "react";

export function TabView(props) {
	
	let [messageList, messageListUpdate] = useState([])
	useEffect(() => {
		var socket = new WebSocket("ws://localhost:8080/tabSocketConnection/" + props.tabName);
		socket.onopen = () => {
			console.log("Successfully Connected");
		};
	
		socket.onmessage = (msg) => {
			let data = JSON.parse(msg.data);
			data.body = data.body.replaceAll(`\"`, '"');
			messageListUpdate(state => [...state, <p>{data.body}</p>])
		};
	
		socket.onclose = (event) => {
			console.log("Socket Closed Connection: ", event);
		};
	
		socket.onerror = (error) => {
			console.log("Socket Error: ", error);
		};
	}, []);

	return (
		<div>
			{messageList}
		</div>
	);
}
