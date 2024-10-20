GLFW=C:/Users/fredr/scoop/apps/glfw/current
CGLM=C:/Users/fredr/source/cglm/cglm-0.9.4
CFLAGS=-I${GLFW}/include -Iinclude
#-I${CGLM}/include
LDFLAGS=-L${GLFW}/lib-mingw-w64 -lglfw3 -lgdi32 -lopengl32 -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64" -L${CGLM}/build/Release -lcglm


main:
	if not exist "bin" mkdir bin
	if not exist "obj" mkdir obj
	if not exist "bin/shaders" mkdir "bin/shaders"
	if not exist "bin/textures" mkdir "bin/textures"
	gcc -c -o obj/glad.o $(CFLAGS) src/glad.c $(LDFLAGS)
	gcc -Wall -Wextra -pedantic -g -o bin/main.exe $(CFLAGS) src/main.c src/read_file.c src/shader.c obj/glad.o ${GLFW}/lib-mingw-w64/libglfw3.a $(LDFLAGS)
	copy /Y resources\shaders\* bin\shaders
	copy /Y resources\textures\* bin\textures

