import asyncio
import json
import sys
from io import TextIOBase

import websockets

class DConsoleIO(TextIOBase):
    def __init__(self) -> None:
        super().__init__()
        self.uri = "ws://localhost:8080/processConnection"
        self.websocket = None

    async def connect(self):
        self.websocket = await websockets.connect(self.uri)
    async def writeAsync(self,__s):
        if self.websocket == None:
            await self.connect()
        if __s.rstrip():
            await self.websocket.send(json.dumps({
                "tab":"pythonApp",
                "body":__s.rstrip()
                }))
    def write(self, __s: str) -> int:
        asyncio.get_event_loop().run_until_complete(self.writeAsync(__s))
        return 1


