import sys


class FakeStaticVar(object):
    tVar = None

    def __call__(self, fVar):
        FakeStaticVar.tVar = fVar


def error_helper(msg):
    print(msg, file=sys.stderr)
    exit(1)


def debug_helper(msg):
    if "DEBUG" in sys.argv:
        print(msg, file=sys.stderr)


def fill_str_buff(msg, size):
    buf = ['\0'] * size
    for i in range(len(msg)):
        buf[i] = msg[i]
    return ''.join(buf)


def get_buff(msg, tab, color):
    tab = fill_str_buff(tab, 16)
    msg = fill_str_buff(msg, 128)
    color = fill_str_buff(color, 16)
    return bytes(color + tab + msg, encoding='ascii')
