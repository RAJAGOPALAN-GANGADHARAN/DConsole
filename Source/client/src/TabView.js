import { useEffect, useState } from "react";

var apiLogBase = "http://localhost:8080/api/logs/";
var apiStreamBase = "http://localhost:8080/api/stream/";

// export function TabView(props) {
//     let [data, dataUpdate] = useState([<p>Hello123</p>, <p>Hello234</p>]);
//     // let streamSource = new EventSource(apiStreamBase+props.tabName);
//     useEffect(() => {
//         console.log("initial data and stream setup")
//         // streamSource.onmessage=e=>handleStream(JSON.parse(e.data()))
//         fetch(apiLogBase + props.tabName)
//             .then(res => res.json())
//             .then(res => {
//                 let result = []

//                 for (let data in res) {
//                     console.log(data);
//                     result.push(<p>{res[data]['detail']}</p>)
//                 }
//                 dataUpdate(state=>[...state,result])
//                 })
//     }, []);

//     useEffect(() => {
//         console.log("Events")
//         fetch(apiStreamBase + props.tabName)
//             .then(res => res.json())
//             .then(res => {
//                 let result = []

//                 for (let data in res) {
//                     console.log(data);
//                     result.push(<p>{res[data]['detail']}</p>)
//                 }
//                 dataUpdate(state=>[...state,result])
//                 })
//       });
//     return (
//         <div>
//             {data}
//         </div>
//     )
// }

export function TabView(props) {
	
	let [messageList, messageListUpdate] = useState([<p>Jellp</p>])
	useEffect(() => {
		var socket = new WebSocket("ws://localhost:8080/tabSocketConnection/" + props.tabName);
		console.log("HEtt");
		socket.onopen = () => {
			console.log("Successfully Connected");
		};
	
		socket.onmessage = (msg) => {
			//console.log(msg);
			messageListUpdate(state => [...state, <p>{msg.data}</p>])
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
