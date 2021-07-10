import asyncio
import websockets
import logging
import json
import time
from datetime import datetime
import random

logger = logging.getLogger('websockets')
logger.setLevel(logging.INFO)
logger.addHandler(logging.StreamHandler())

async def hello():
    uri = "ws://localhost:8080/processConnection"
    async with websockets.connect(uri) as websocket:
        # await websocket.send(json.dumps({
        #     "tab":"default",
        #     "body":"Hello-"+str(datetime.now())
        # }))
        # # async for message in websocket:
        # #     print(message)

        # await websocket.send(json.dumps({
        #     "tab":"default",
        #     "body":"HelloNew-"+str(datetime.now())
        # }))
        # await websocket.send(json.dumps({
        #     "tab":"default",
        #     "body":"HelloNew123-"+str(datetime.now())
        # }))
        # async for message in websocket:
        #     print(message)
        while True:
            await websocket.send(json.dumps({
                "tab":"overload",
                "body":"Hello-"+str(datetime.now())
                }))

            #time.sleep(2)
            # await websocket.send("Hello world!")
            # async for message in websocket:
            #     print(message)

asyncio.get_event_loop().run_until_complete(hello())