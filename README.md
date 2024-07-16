# Cube
Terminal cube in C (with a little Python)

## Instructions
First, make sure you have pynput installed in python. This can easily be done with `pip install pynput`.

Then, run `make` to compile the program. And finally, run `./cube` to run the code. 

If your terminal is in light mode, run `./cube light` to reverse the lighting conditions.

### Keybinds and Modes
Once running, it will start in Auto mode. You can use the keyboard to change the mode and some other settings.  

**Global Keybinds + Auto Mode**

- <kbd>i</kbd>: Toggle Information (on by default)  
- <kbd>m</kbd>: Change Mode  
- <kbd>l</kbd>: Toggle Lighting (on by default)  
- <kbd>=</kbd>/<kbd>-</kbd>: Zoom In/Out, think + and - keys, but no need to hold shift  
- <kbd>space</kbd>: Pause  
- <kbd>r</kbd>: Reset Cube Position  
- <kbd>p</kbd>: Quit  

There are some more controls in the other modes. 

**Acceleration Control Mode**  

- <kbd>w</kbd>/<kbd>s</kbd>: x-axis acceleration  
- <kbd>a</kbd>/<kbd>d</kbd>: y-axis acceleration  
- <kbd>q</kbd>/<kbd>e</kbd>: x-axis acceleration  

**Mouse Control Mode**  

Use the mouse to control the direction the cube is facing.
- Note that the mouse control is based on your mouse's _absolute_ position, so this will only work accurately when the cube is near the middle of your screen.  
- Mouse control is also resolution-dependent and is defaulted to 1440x900. If you want this to work on a different size display, change the 'xres' and 'yres' variables near the top of cube.c. 


