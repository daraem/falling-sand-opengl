# Falling sand
Celular automation basic falling sand simulation. OpenGL & X11

Project structure is developed with hotloading in mind, which comes enable by default and cannot be disabled (tho it might be worth for performance).

# Files
- **libgame.c** contains game logic
- **main.c** needs to be compiled on its own, code for executable program
- **window.c** X11 window setup
- **window_loop.c** X11 window loop

# Build and run
I have to learn Make :(

1. Create 'build' and 'lib' folders
2. Compile `gcc src/main.c -I./utils -o [whatever]`
3. Run compiled executable
