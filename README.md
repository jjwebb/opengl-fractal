# opengl-fractal
This is my OpenGL fractal explorer program. It was developed for the Rasberry Pi 4B (OpenGL ES 3.1) to put into a coffee table, but it can perfectly well be used on a desktop or laptop. It is controlled via the keyboard, with a Python script to map rotary encoders to keyboard keys.

![Multijulia set](screenshots/multijulia-11-16-20.png?raw=true)  

## Run:  
This repo contains precompiled binaries for Windows 10 and Raspberry Pi OS, which should (hopefully) work. The binaries are in ***opengl-fractal/opengl-fractal***. If you get a .dll error on Windows, you may need to install the latest <a href="https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads">Visual C++ Redistributable</a>  

## Build:  
#### Windows:  
If you have some flavor of Microsoft Visual Studio 2019, go to ***File -> Open -> Cmake...*** and open the *CMakeLists.txt* in the root directory of the project. Choose *x64-Debug* or *x64-Release* in the **Configuration** dropdown and *fractal.exe* in the **Startup Item** dropdown and compile.  

#### Raspberry Pi 4/Linux:
Run the following commands in the project root directory:  
`$ cmake . -DCMAKE_BUILD_TYPE=`(`Release`/`Debug`)  
`$ make fractal`  
  
Fractal binary will be written to ***opengl-fractal/opengl-fractal***  

#### To run the Python script (on a Pi):  
`sudo pip3 install python-uinput`  
`sudo modprobe uinput`  
`sudo python3 (install folder)/opengl-fractal/opengl-fractal/src/rotaryencoder.py`  
  
  ## Controls:  
 `UP` `DOWN` `LEFT` `RIGHT` move the crosshair around  
 `W` `A` `S` `D` move the crosshair around just like the arrow keys in Mandelbrot mode. In Julia set mode, they move the crosshair on the Mandelbrot set, which changes the set               displayed  
 `Enter` zooms in  
 `Backspace`  zooms out  
 `J` displays the Julia set pointed to by the crosshair in-place -- i.e., The Z value is the same as the C value from the Mandelbrot set and the zoom level is maintained  
 `H` displays the whole Julia set for the point at the crosshair -- the view is set to the origin (0 + 0i) and a zoom level of 1. Pressing H again always returns you to where you were when you first pressed it, even if you change the Julia set. J will return you back to the Mandelbrot in-place.  
 |                     Mandelbrot                       |            `     J     `                             |                 `       H                    `      
 |:------------------------------------------------------:|----------------------------------------------------|---------------------------------------------------
<img src="screenshots/jh-demo-1.png?raw=true" width="100%"> | <img src="screenshots/jh-demo-2.png?raw=true" width="100%"> | ![Julia set H](screenshots/jh-demo-3.png?raw=true)  
  
  `N` and `P` respectively raise and lower the exponent of the fractal function by an interval specified by `.` (+- .01, .05, .25, or 1.0)
| `N`  | \- | `P` |
| :-: | :-: | :-: |
| z ↦ z^4.10 + c  | z ↦ z^2 + c (Mandelbrot) | z ↦ z^1.90 + c |
| <img src="screenshots/multibrot.png?raw=true" width="100%"> | <img src="screenshots/mandelbrot.png?raw=true" width="100%"> | <img src="screenshots/multibrot1.png?raw=true" width="100%"> |  
  
  `R` resets to 1x zoom and the origin (0 + 0i) on any set  
  `C` centers the screen at the crosshair  
  `I` toggles the text info on/off  
  `T` toggles displaying the render time for the last frame (for diagnostics)  
  `Esc` exits the program  
    
  There's lots to explore, have fun!
