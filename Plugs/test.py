import asyncio
import websockets
import logging
import json

logger = logging.getLogger('websockets')
logger.setLevel(logging.INFO)
logger.addHandler(logging.StreamHandler())

async def hello():
    uri = "ws://localhost:8080/processConnection"
    async with websockets.connect(uri) as websocket:
        await websocket.send(json.dumps({
            "tab":"default",
            "body":"Hello world!"
            }))
        # await websocket.send("Hello world!")
        async for message in websocket:
            print(message)

asyncio.get_event_loop().run_until_complete(hello())