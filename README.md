# cube-web
The spinning cube is now in your browser with the power of WASM  
[ronthekiehn.github.io/cube-web/](ronthekiehn.github.io/cube-web/)

## Features  
 - Dark mode support!!
 - Lighting calculated from surface normals of each side (press <kbd>l</kbd> to toggle light on and off to see the difference)
 - Zoom in and out
 - Reset cube position
 - Kinematics-based acceleration mode that lets you accelerate the cube on all three axes
 - Mouse control mode that makes the cube face your mouse

### Keybinds and Modes
All of the keybinds and modes can now be manipulated thanks to my one true love: javascript. The keybinds can still be used though. 'wasdqe' will still be used for acceleration control. 

**Global Keybinds + Auto Mode**
Auto mode has the same default behavior as the original spinning cube  
- <kbd>m</kbd>: Change Mode  
- <kbd>l</kbd>: Toggle Lighting (on by default)  
- <kbd>=</kbd>/<kbd>-</kbd>: Zoom In/Out, think + and - keys, but no need to hold shift  
- <kbd>r</kbd>: Reset Cube Position  

There are some more controls in the other modes. 

**Acceleration Control Mode**  

This mode lets you control which direction the cube is spinning using simple kinematic equations
- <kbd>w</kbd>/<kbd>s</kbd>: x-axis acceleration  
- <kbd>a</kbd>/<kbd>d</kbd>: y-axis acceleration  
- <kbd>q</kbd>/<kbd>e</kbd>: x-axis acceleration  

**Mouse Control Mode**  
Use the mouse to control the direction the cube is facing.



## Credit
See the original cube I made for the terminal: [https://github.com/ronthekiehn/Cube](https://github.com/ronthekiehn/Cube)

A lot of credit goes to these two!! 
Built off of this spinning C cube: [https://github.com/servetgulnaroglu/cube.c](https://github.com/servetgulnaroglu/cube.c)

Lighting is based on this spinning C donut: [https://www.a1k0n.net/2011/07/20/donut-math.html](https://www.a1k0n.net/2011/07/20/donut-math.html)

Thanks and huge credit to both of these projects.

