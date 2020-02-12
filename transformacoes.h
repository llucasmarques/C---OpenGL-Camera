// Define os cabecalhos para as transformacoes 

// You need to implement left, up and lookAt
// Rotate is a helper function

// Include the helper glm library, including matrix transform extensions

#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Typedefs para deixar o codigo mais limpo 
typedef glm::mat3 mat3;
typedef glm::mat4 mat4; 
typedef glm::vec3 vec3; 
typedef glm::vec4 vec4; 
const float pi = 3.14159265;
void esquerda(float degrees, glm::vec3& eye, glm::vec3& up);
void paraCima(float degrees, glm::vec3& eye, glm::vec3& up);
glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 up);
glm::mat3 rotaciona(const float degrees, const glm::vec3& axis);
//mat3 translado(...);


#endif