from pynput import mouse

def on_move(x, y):
    f = open("mouse.txt", "w")
    x, y= int(x), int(y)
    f.write(f"{x},{y}")
    f.close()
    
with mouse.Listener(on_move=on_move) as listener:
    listener.join()