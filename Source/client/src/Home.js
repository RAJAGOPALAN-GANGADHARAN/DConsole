import { useState, useEffect } from 'react';
import { Tabs, Tab, Button } from 'react-bootstrap';
import { TabView } from './TabView';

export function Home() {
    let [tabViews, tabViewsUpdate] = useState([
        <Tab eventKey="default" title="Home">
            <TabView tabName={"default"} />
        </Tab>,
        <Tab eventKey="default2" title="New2">
            <TabView tabName={"default2"} />
        </Tab>
    ]);
    return (
        <div>
            <Tabs defaultActiveKey="default" id="uncontrolled-tab-example" unmountOnExit={true}>
                {tabViews}
            </Tabs>
        </div>
    );
}