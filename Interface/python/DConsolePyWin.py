import time
import win32pipe
import win32file
import pywintypes
from utils import *

DCONSOLE_SERVER = r"\\.\\pipe\\dconsole_pipe"


static_server_handle = FakeStaticVar()


def first_time_connect():
    server_handle = win32file.CreateFile(DCONSOLE_SERVER, win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                                         0, None, win32file.OPEN_EXISTING, win32file.FILE_ATTRIBUTE_NORMAL, None)

    res = win32pipe.SetNamedPipeHandleState(
        server_handle, win32pipe.PIPE_READMODE_MESSAGE, None, None)
    if res == 0:
        # if function fails, the return value will be zero
        error_helper(f"SetNamedPipeHandleState Return Code: {res}")

    static_server_handle(server_handle)


def send_base(buffer):
    con_c = 0
    while con_c < 25:
        if static_server_handle.tVar == None:
            first_time_connect()
        try:
            resp = win32file.WriteFile(static_server_handle.tVar, buffer)
            break
        except Exception as e:
            con_c += 1

    if con_c >= 25:
        error_helper("Max Connect attempts reached")

    debug_helper("Successfully sent")
