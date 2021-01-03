import socket
import sys
import os

DCONSOLE_SERVER = "/tmp/dconsole-server.socket"

class FakeStaticVar(object):
    tSock = socket.socket()
    def __call__(self, fSock):
        FakeStaticVar.tSock = fSock

static_client_socket = FakeStaticVar()

def error_helper(msg):
    print(msg, file=sys.stderr)
    exit(1)

def debug_helper(msg):
    if "DEBUG" in sys.argv:
        print(msg, file=sys.stderr)

def first_time_connect():
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
    try:
        client_socket.connect(DCONSOLE_SERVER)
    except socket.error as msg:
        error_helper("Connection Failed:" + str(msg))

    static_client_socket(client_socket)


def fill_str_buff(msg, size):
    buf = ['\0'] * size
    for i in range(len(msg)):
        buf[i] = msg[i]
    return ''.join(buf)

def get_buff(msg, tab, color):
    tab = fill_str_buff(tab,16)
    msg = fill_str_buff(msg, 128)
    color = fill_str_buff(color, 16)
    return bytes(color + tab + msg, encoding='utf-8')

def send_base(buffer):
    con_c = 0
    while con_c < 25:
        try:
            static_client_socket.tSock.sendall(buffer)
            break
        except Exception as e:
            first_time_connect()
            con_c += 1

    if con_c >= 25:
        error_helper("Max Connect attempts reached")

    debug_helper("Successfully sent")

def DConsoleSend(console_msg,color='white',tab_name='default'):
    byte_buffer = get_buff(str(console_msg), color, tab_name)
    send_base(byte_buffer)
