#ifndef OBJETO_H
#define OBJETO_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>

// O objeto a ser renderizado deve ser informado como um arquivo OBJ.
// Estude sobre o formato de arquivos OBJ na Wikipedia eh suficiente.
extern std::vector <glm::vec3> vertices;
extern std::vector <glm::vec3> normais;  //nao usado nesse exemplo
extern std::vector <unsigned int> indices;
void parse(const char*); //faz o parser do arquivo OBJ

#endif
