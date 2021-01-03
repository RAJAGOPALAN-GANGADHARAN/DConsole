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


def fill_str_buff(msg):
    buf = [''] * 128
    for i in range(len(msg)):
        buf[i] = msg[i]
    buf[range(len(msg))] = '\0'
    buf = ''.join(buf)
    return bytes(buf)

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

def DConsoleSend(console_msg):
    byte_buffer = fill_str_buff(str(console_msg))
    send_base(byte_buffer)
