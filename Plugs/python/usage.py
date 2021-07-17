from DConsole import DConsoleIO
import sys
import random

dio = DConsoleIO()
sys.stdout =dio
sys.__stdout__ =dio

def add(x,y):
    return x+y

def sub(x,y):
    return x-y

def mul(x,y):
    return x*y

def div(x,y):
    return x/y

for i in range(1,30):
    r=random.randint(1,10000)
    c=random.randint(1,4)
    if c==1:
        print(add(r,r+65))
    elif c==2:
        print(sub(r,random.randint(1,10000)))
    elif c==3:
        print(mul(r,r+65))
    else:
        print(div(r,r+65))
