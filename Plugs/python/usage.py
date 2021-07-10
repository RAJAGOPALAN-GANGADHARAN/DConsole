from DConsole import DConsoleIO
import sys

dio = DConsoleIO()
sys.stdout =dio
sys.__stdout__ =dio

print("Hello")
print("Yolo")

for i in range(200):
    print("Hello")