CC = g++
CFLAGS = -g -DGL_GLEXT_PROTOTYPE -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses -Wno-parentheses-equality
INCFLAGS = -I../glad/include -I/usr/local/Cellar/glfw/3.2.1/include \
	   -I/usr/X11/include -I./glm 
LDFLAGS = -framework GLUT -framework OpenGL \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-L/usr/local/Cellar/glfw/3.2.1/lib \
		-lGL -lglfw -lstdc++ 
RM = /bin/rm -f
all: ex6
ex6:  main.o shaders.o objeto.o glad.o
	$(CC) $(CFLAGS) -o ex6 glad.o objeto.o shaders.o main.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
objeto.o: objeto.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c objeto.cpp
glad.o: ../glad/src/glad.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c ../glad/src/glad.c	
clean: 
	$(RM) *.o ex6
 
