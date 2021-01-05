import sys
from sys import platform
from utils import get_buff

if platform.startswith('lin'):
    import DConsolePyUnix as DConsoleIn
elif platform.startswith('win'):
    import DConsolePyWin as DConsoleIn


def DConsoleSend(console_msg, color='white', tab_name='default'):
    byte_buffer = get_buff(str(console_msg), color, tab_name)
    DConsoleIn.send_base(byte_buffer)
