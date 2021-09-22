import { useState, useEffect } from "react";
import { Tab, Tabs, TabList, TabPanel } from 'react-tabs';
import { TabView } from "./TabView";
import 'react-tabs/style/react-tabs.css';

export function Home() {

	let [tabList, tabListUpdate] = useState([]);
	let [tabPanel, tabPanelUpdate] = useState([]);

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
			if (res.type == "100")
			{
				tabListUpdate(state => [...state,
					<Tab>{ res.body }</Tab>
				])
				tabPanelUpdate(state => [...state,
					<TabPanel>
						<TabView tabName={res.body} />
					</TabPanel>
				])
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

			<Tabs>
				<TabList>
					{tabList}
				</TabList>
				{tabPanel}
			</Tabs>
		</div>
	);
}
