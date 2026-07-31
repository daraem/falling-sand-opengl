gcc -fPIC -c utils/shader_util.c -o utils/shader_util.o
gcc -fPIC -c utils/texture_util.c -o utils/texture_util.o
gcc -fPIC -c utils/geometry_util.c -o utils/geometry_util.o

gcc -fPIC -c src/libgame.c -I./utils -o build/libgame.o
gcc -fPIC -c src/window.c -I./utils -o build/window.o
gcc -fPIC -c src/window_loop.c -I./utils -o build/window_loop.o

gcc -shared -o lib/libgame.so utils/shader_util.o utils/texture_util.o utils/geometry_util.o build/libgame.o -lX11 -lGL -lm -I./utils 
gcc -shared -o lib/window.so build/window.o -lGL -lm -lX11 -I./utils
gcc -shared -o lib/window_loop.so build/window_loop.o -lX11 -lGL -lm -I./utils
