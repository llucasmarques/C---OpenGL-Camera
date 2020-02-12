#include <iostream>
#include <fstream>
#include <cstring>
#include <glad/glad.h>
#include "shaders.h"

//#include <GL/glew.h>
//#include <GL/glut.h>


using namespace std; 

// le o arquivo fonte do shader para uma string c++.
string shaderSrcRead (const char * filename) {
	string str,ret = ""; 
	ifstream in; 
	in.open(filename); 
	if (in.is_open()) 
	{
		getline (in,str); 
		while (in) {
			ret += str + "\n"; //concatena as linhas do arquivo
			getline (in,str); 
		} 
		return ret; 
	}else {
		cerr << "Erro ao abrir o arquivo " << filename << "\n"; 
		throw 2; 
	}
}


// compila um shader (vertex ou fragment)
GLuint compileShader (GLenum type,const char *filename) 
{
	string str = shaderSrcRead (filename); // le o arquivo fonte
	const GLchar *cstr = str.c_str(); // converte para um char[] com  \0 no final
	
	GLuint shader = glCreateShader(type); // cria o shader (vertex ou fragment)
	glShaderSource (shader,1,&cstr,NULL); // copia o codigo no objeto
	glCompileShader (shader);             // compila o shader
		
	// verifica erros de compilacao
	GLint compiled; 
	glGetShaderiv (shader,GL_COMPILE_STATUS,&compiled); 
	if (!compiled) { 
		shaderErrors (shader); 
		throw 3; 
	}
	return shader; 
}

void shaderErrors (const GLint shader) {
	GLint length; 
	glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length); // recupera o tam do log de erro 
	
	GLchar * log; 
	log = new GLchar[length+1];
	glGetShaderInfoLog(shader,length,&length,log); 
	cout << "Erro de compilacao abaixo:\n" << log << "\n"; 
	delete [] log; 
}

GLuint createProgram (GLuint vertexshader,GLuint fragmentshader) 
{
	GLuint program = glCreateProgram();     // cria o programa 
	glAttachShader(program,vertexshader);   // anexa o vertex shader 
	glAttachShader(program,fragmentshader); // anexa o fragment shader
	glLinkProgram(program);                 // linka e gera o executavel

        GLint linked; 
	glGetProgramiv(program,GL_LINK_STATUS,&linked); // recupera o status 
	if (!linked){ // tudo ok? 
	   programErrors(program); 
	   throw 4; 
	}
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	return program; 
}

void programErrors (const GLint program) {
	GLint length; 
	glGetProgramiv(program,GL_INFO_LOG_LENGTH,&length); 
		
	GLchar * log; 
	log = new GLchar[length+1];
	glGetProgramInfoLog(program,length,&length,log); 
	cout << "Erro de linkagem abaixo\n" << log << "\n"; 
	delete [] log; 
}

