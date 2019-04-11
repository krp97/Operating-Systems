# Operating-Systems
Multithreading projects implemented during an OS course using C++.
## Dependencies
  - ncurses
  - pthreads
  - cmake
## Build process
In order to generate the makefiles for your compiler use the following:
1. Create a **build** directory within the top level of a source tree `$ mkdir build`.
2. Enter the directory `$ cd build`.
3. Run an out of source build `$ cmake ..` with the default compiler.
4. Compile the project with `$ make`.

    You can also build each task separately with `$ make task1` or `$ make task2`.

## Tasks
### Task 1
The program is a basic visualization of threads with ncurses. Each thread is a circle on screen which bounces around with a set speed. This task only implements the basic movement of circles and synchronized access to the shared resource - terminal screen.

### Task 2
Task 2 is an extension of the first, implementing handling of condition variables. The exercise requirements are as follows:
 - If a circle hits the ceiling of the box, it has to freeze.
 - The circle stays frozen until another circle hits the ceiling, at which point they switch states. The first one resumes movement and the other freezes.