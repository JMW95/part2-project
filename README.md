# Graphics Processor for CPU + FPGA
Hardware acceleration of graphics tasks for CPU + FPGA platforms.

## Hardware
The included hardware design is for an Altera SoC FPGA platform.
1, 2, 4 or 8 rendering cores are supported.
Firmware for the Clarvi soft processor is included to implement triangle-rendering on the FPGA.

## Software
Two Linux kernel drivers for the host CPU are included:
* workqueue: Workqueue driver, for sending rendering commands to the graphics processor core(s).
* vsync: Interrupt-driven double-buffering and control over the 256-colour palette.

A C++ 3D graphics library (3dlib) provides abstraction over these drivers, and supports rendering on the host CPU for comparison.

Several demo programs are included:
* 3ddemo: render a pair of 3D models to test 3D rendering performance.
* game: a 3D castle scene with movement controls (if enabled in the hardware design)
* triangles: 2D random triangle rendering to test 2D rendering performance.


## Scripts

### build.sh
``` ./build.sh <num_cores>```
Configure the hardware design for the chosen number of rendering cores, and compile it.

### test.sh
Compile and run benchmarks with each rendering algorithm on the remote hardware.

### run_test.sh
Setup the device drivers and execute the set of benchmarks.

### hardware_setup.sh
Compile and re-insert both kernel drivers. Used by the run_test.sh script.
