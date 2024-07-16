from pynput import mouse
import os

def on_move(x, y):
    f = open("mouse.txt", "w+")
    x, y= int(x), int(y)
    f.write(f"{x},{y}")
    f.close()

# f = open("pid.txt", "w+")
# f.write(f"{os.getpid()}")
# f.close()

with mouse.Listener(on_move=on_move) as listener:
    listener.join()