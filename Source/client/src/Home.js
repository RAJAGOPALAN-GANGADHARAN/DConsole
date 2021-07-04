import { useState, useEffect } from "react";
import { Tabs, Tab, Button } from "react-bootstrap";
import { TabView } from "./TabView";

export function Home() {
	var masterSocket = new WebSocket(
		"ws://localhost:8080/masterSocketConnection"
	);

	let [tabViews, tabViewsUpdate] = useState([
		<Tab eventKey="default" title="default">
			<TabView tabName={"default"} />
		</Tab>
	]);
	
	useEffect(() => {
		masterSocket.onopen = () => {
			console.log("Successfully Connected");
		};

		masterSocket.onmessage = (msg) => {
			console.log("Master Socket:");
			console.log(msg.data)
			
		};

		masterSocket.onclose = (event) => {
			console.log("Socket Closed Connection: ", event);
		};

		masterSocket.onerror = (error) => {
			console.log("Socket Error: ", error);
		};
	}, []);

	
	return (
		<div>
			<Tabs
				defaultActiveKey="default"
				id="uncontrolled-tab-example"
				unmountOnExit={true}
			>
				{tabViews}
			</Tabs>
		</div>
	);
}
