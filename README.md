# Cube
A quick spinning cube project. Runs raw in the terminal with no real C libraries like curses. Takes keyboard input using putchar(). Takes a simple spinning cube and adds a ton of features, including zooming, terminal resizing, and mouse and keyboard control. It also uses a small Python script to get your mouse position for mouse control but is otherwise all written in C. It Probably only works on Mac.

Built off of this spinning C cube: https://github.com/servetgulnaroglu/cube.c  

Lighting is based on this spinning C donut: https://www.a1k0n.net/2011/07/20/donut-math.html

Thanks and huge credit to both of these projects.

## Features  
 - Cube fits as you adjust the size of the terminal window
 - Lighting calculated from surface normals of each side (press <kbd>l</kbd> to toggle light on and off to see the difference)
 - Zoom in and out
 - Reset cube position
 - Kinematics-based acceleration mode that lets you accelerate the cube on all three axes
 - Mouse control mode that makes the cube face your mouse

## Instructions
First, make sure you have pynput installed in python. This can easily be done with `pip install pynput`.

Then, run `make` to compile the program. And finally, run `./cube` to run the code. 

If your terminal is in light mode, run `./cube light` to reverse the lighting conditions.

### Keybinds and Modes
Once running, it will start in Auto mode. You can use the keyboard to change the mode and some other settings.  

**Global Keybinds + Auto Mode**

Auto mode has the same default behavior as the original spinning cube  
- <kbd>i</kbd>: Toggle Information (on by default)  
- <kbd>m</kbd>: Change Mode  
- <kbd>l</kbd>: Toggle Lighting (on by default)  
- <kbd>=</kbd>/<kbd>-</kbd>: Zoom In/Out, think + and - keys, but no need to hold shift  
- <kbd>space</kbd>: Pause  
- <kbd>r</kbd>: Reset Cube Position  
- <kbd>p</kbd>: Quit  

There are some more controls in the other modes. 

**Acceleration Control Mode**  

This mode lets you control which direction the cube is spinning using simple kinematic equations
- <kbd>w</kbd>/<kbd>s</kbd>: x-axis acceleration  
- <kbd>a</kbd>/<kbd>d</kbd>: y-axis acceleration  
- <kbd>q</kbd>/<kbd>e</kbd>: x-axis acceleration  

**Mouse Control Mode**  

Use the mouse to control the direction the cube is facing.
- Note that the mouse control is based on your mouse's _absolute_ position, so this will only work accurately when the cube is near the middle of your screen.  
- Mouse control is also resolution-dependent and is defaulted to 1440x900. If you want this to work on a different size display, change the 'xres' and 'yres' variables near the top of cube.c. 


