GLFW=C:/Users/fredr/scoop/apps/glfw/current
CFLAGS=-I${GLFW}/include -Iinclude
LDFLAGS=-L${GLFW}/lib-mingw-w64 -lglfw3 -lgdi32 -lopengl32 -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64"


main:
	gcc -Wall -g -o main.exe $(CFLAGS) src/main.c src/glad.c ${GLFW}/lib-mingw-w64/libglfw3.a $(LDFLAGS)
