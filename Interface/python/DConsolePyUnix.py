import socket
import sys
import os
from utils import *

DCONSOLE_SERVER = "/tmp/dconsole-server.socket"


static_client_socket = FakeStaticVar()


def first_time_connect():
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
    try:
        client_socket.connect(DCONSOLE_SERVER)
    except socket.error as msg:
        error_helper("Connection Failed:" + str(msg))

    static_client_socket(client_socket)


def send_base(buffer):
    con_c = 0
    while con_c < 25:
        if static_client_socket.tVar == None:
            first_time_connect()
        try:
            static_client_socket.tVar.sendall(buffer)
            break
        except Exception as e:
            con_c += 1

    if con_c >= 25:
        error_helper("Max Connect attempts reached")

    debug_helper("Successfully sent")
