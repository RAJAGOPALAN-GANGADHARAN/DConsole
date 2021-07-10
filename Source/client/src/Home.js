import { useState, useEffect } from "react";
import { Tabs, Tab, Button } from "react-bootstrap";
import { TabView } from "./TabView";

export function Home() {
	

	let [tabViews, tabViewsUpdate] = useState([]);
	
	useEffect(() => {
		var masterSocket = new WebSocket(
			"ws://localhost:8080/masterSocketConnection"
		);
		masterSocket.onopen = () => {
			console.log("Successfully Connected");
		};

		masterSocket.onmessage = (msg) => {
			console.log(msg.data)
			let res = JSON.parse(msg.data)
			console.log(res)
			if (res.type == 100)
			{
				tabViewsUpdate(state => [...state,
					<Tab eventKey={res.body} title={res.body}>
						<TabView tabName={res.body} />
					</Tab>])
			}

			
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
