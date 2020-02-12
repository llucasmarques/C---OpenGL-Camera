//implementation

#include "transformacoes.h"

//implement as seguintes funcoes:

// funcao de rotacao.  
mat3 rotaciona(const float degree, const glm::vec3& axis) {
  // implemente usando a forma eixo-angulo apresentada em sala
	glm::vec3 a = glm::normalize(axis);
	float sine = sin(glm::radians(degree));
	float cosine = cos(glm::radians(degree));
	float x = a.x, y = a.y, z = a.z;

	glm::mat3 aat = glm::mat3(x*x, y*x, z*x,
		x*y, y*y, z*y,
		x*z, y*z, z*z);

	glm::mat3 aStar = glm::mat3(0, -z, y,
		z, 0, -x,
		-y, x, 0);

	glm::mat3 rotateM = glm::mat3(cosine) + (1 - cosine) * aat + sine * aStar;

	return rotateM;
}

// rotaciona a camera pela esquerda da "bola de cristal"
void esquerda(float degrees, glm::vec3& eye, glm::vec3& up) {
	glm::mat3 rotationMatrix = rotaciona(degrees, up);
	eye = eye * rotationMatrix;
}

// rotaciona a camera por cima da "bola de cristal"
void paraCima(float degrees, glm::vec3& eye, glm::vec3& up) {
	glm::mat3 rotationMatrix = rotaciona(degrees, glm::cross(eye, up));
	eye = eye * rotationMatrix;
	up = up * rotationMatrix;
}

// sua implementacao de glm::lookAt
// sempre olhando para a origem
glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 up) {
	glm::vec3 w = glm::normalize(eye);
	glm::vec3 u = glm::normalize(glm::cross(up, w));
	glm::vec3 v = glm::cross(w, u);
	glm::mat4 transformM = 
		glm::mat4(
		u.x, u.y, u.z, -glm::dot(u, eye),
		v.x, v.y, v.z, -glm::dot(v, eye),
		w.x, w.y, w.z, -glm::dot(w, eye),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	return transformM;
}

