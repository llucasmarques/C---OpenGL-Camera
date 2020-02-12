#include <glad/glad.h>  //ja inclui OpenGL includes
#include <GLFW/glfw3.h> //para criacao de janelas
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shaders.h"
#include "objeto.h"
#include <iostream>
#include "camera.h"
#include "transformacoes.h"


//Variaveis globais
//------------------------------------------------------------------------------------
GLint program; //referência pra o programa dos shaders
GLuint VAO; //referencia ao Vertex Array Object que gerenciara os vertices
GLuint VBO; //referencia para o Vertex Buffer Object (buffer interno de vertices)
GLuint EBO; //referencia para o Element Buffer Object (buffer interno de indices)
GLuint modelLoc, viewLoc, projectionLoc; //localizacao da matriz de transformacao no shader
glm::mat4 model, view, projection;

float angulo = 0.0f;
float rodar = 0.0f;
float Zoom = 45.0f;
using namespace std;


/* Variáveis de Camera*/

//callbacks:
//--------------------------------------------------------------------------------------
void callback_teclado(GLFWwindow* win, int key, int scancode, int action, int mods)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	  glfwSetWindowShouldClose(win, true);

  //Direita, eixo x aumenta
  if (key == GLFW_KEY_RIGHT) {
	  angulo += 0.1f;

	 
  }

	
  //Esquerda, eixo x diminui
  if (key == GLFW_KEY_LEFT ) {
	  angulo -= 0.1f;
  }


  //Cima, eixo y aumenta
  if (key == GLFW_KEY_UP) {
	  rodar += 1.0f;
  }


  //Baixo, eixo y diminui
  if (key == GLFW_KEY_DOWN) {
	  rodar -= 1.0f;
  }


  //Zoom +, eixo z aumenta
  if (key == GLFW_KEY_A) {
	  if (Zoom >= 1.0f && Zoom <= 45.0f)
		  Zoom -= 1.0f;
	  if (Zoom <= 1.0f)
		  Zoom = 1.0f;
	  if (Zoom >= 45.0f)
		  Zoom = 45.0f;
  }


  //Zoom -, eixo z diminui
  if (key == GLFW_KEY_D) {
	  if (Zoom >= 1.0f && Zoom <= 45.0f)
		  Zoom += 1.0f;
	  if (Zoom <= 1.0f)
		  Zoom = 1.0f;
	  if (Zoom >= 45.0f)
		  Zoom = 45.0f;
  }


  //Posição Inicial
  if (key == GLFW_KEY_R && action == GLFW_PRESS) {

	  Zoom = 45.0f;
	  angulo = 0.0f;
	  rodar = 0.0f;


  }
 


}

void callback_redimensionamento(GLFWwindow* win, int width, int height)
{
    glfwGetFramebufferSize(win, &width, &height); //comentar se nao for tela de retina
    glViewport(0, 0, width, height); //OpenGL: transformacao de viewport
    projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

}  

// function to display 4x4 matrix
void display_4x4(string tag, glm::mat4 m4)
{
	cout << tag << '\n';
	for (int col = 0; col < 4; ++col) {
		cout << "| ";
		for (int row = 0; row < 4; ++row) {
			cout << m4[row][col] << '\t';
		}
		cout << '\n';
	}
	cout << '\n';
}

// function to display 3-component vector
void display_v3(string tag, glm::vec3 v3)
{
	cout
		<< tag
		<< "\n| "
		<< v3.x << '\t'
		<< v3.y << '\t'
		<< v3.z << '\t'
		<< "\n"
		;
}

// funcao de inicializacao: tudo que deve ser feito apenas uma vez vem aqui
// --------------------------------------------------------------------------------------
void init()
{	
	// Compila e linka os shaders
	GLint vertShader  = compileShader(GL_VERTEX_SHADER,   "./shader.ver");
    GLint fragShader  = compileShader(GL_FRAGMENT_SHADER, "./shader.fra");
   	program           = createProgram(vertShader, fragShader);

	// le o arquivo OBJ 
	parse("./teapot.obj");
        
	// recupera a localizacao da transformacao
    projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	
	// cria um Vertex Array Object, um Vertex Buffer Object e um Element Buffer Object
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
	
	//ativa Vertex Array Object VAO
	glBindVertexArray(VAO);
	
	//povoa o VBO com os vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//povoa o EBO com os indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	
	// configura VAO a transferencia dados do buffer VBO
	// para o atributo 0 do vertex shader aPos (location = 0)
    glEnableVertexAttribArray(0); // 0 -> location do atributo pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
	glBindVertexArray(0); //0 -> unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

glm::mat4 minhaEscala(glm::mat4 mIndent, glm::vec3 coordenadas) {
	glm::mat4 result;
	result[0] = mIndent[0] * coordenadas[0];
	result[1] = mIndent[1] * coordenadas[1];
	result[2] = mIndent[2] * coordenadas[2];
	result[3] = mIndent[3];
	return result;
}

/*glm::mat4 minhaLookAt(glm::vec3, glm::vec3, glm::vec3) {


}*/



// renderiza (desenha) a cena desejada 
// ---------------------------------------------------------------------------------
void renderiza()
{
  	// preenche buffer (pinta os pixels) associado ao fundo com branco
  	GLfloat cor[]= {1.0f, 1.0f, 1.0f, 1.0f};
  	glClearBufferfv(GL_COLOR,0,cor);

  	// usa os shaders criados anteriormente em init()
  	glUseProgram(program);
	glBindVertexArray(VAO);

	float radius = 10.0f;
	float camX = sin(angulo) * radius;
	float camY = rodar;
	float camZ = cos(angulo) * radius;
	

    //monta a matriz modelo
	// model = glm::scale(glm::mat4(1.0f), glm::vec3(escala, escala, escala));
    model = minhaEscala(glm::mat4(1.0f), glm::vec3(2.5f, 2.5f, 2.5f));



	//display_4x4("matriz scala", model);
	//display_4x4("matriz mat4", glm::mat4(1.0f));
	//display_v3("vetor na escala, ", glm::vec3(escala, escala, escala));

	glm::vec3 Pos(camX, 1.0f, camZ);
	
	glm::vec3 Target(0.0f, 0.0f, 0.0f);
	
	glm::vec3 Up(0.0, 1.0f, 0.0f);

	//esquerda(60.0f, Pos, Up);
	paraCima(camY, Pos, Up);


    view = myLookAt(Pos, Up);
	view = glm::transpose(view);
   
	/* 
	1 - angulo de visao, perto diminui, longe aumenta
	2 - apecto, largura/altura
	3 - perto, 
	*/
	projection = glm::perspective(glm::radians(Zoom), 800.0f/600.0f, 0.1f, 100.0f);

	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));



		
	// passa a matrizes para o vertex shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));	
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));	
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);//varios triangulos!
	glBindVertexArray(0);// unbind VAO
}

//codigo principal
//--------------------------------------------------------------------------------
int main()
{
   //GLFW: configuracao de atributos da janela
   //------------------------------------------------------------------------------
    glfwInit(); //inicializa GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //define versao OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //define versao OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //OpenGL moderno
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // especifico para MAC
   
   //GLFW: Criacao da janela
   //------------------------------------------------------------------------------
    GLFWwindow* win = glfwCreateWindow(800, 600, "Janela 1", NULL, NULL);
    if (win == NULL)
    {
        std::cout << "Falha ao criar GLFW window: Janela 1" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);//torna a janela o contexto OpenGL da thread chamadora

   //GLAD: carrega todos os ponteiros de funcoes OpenGL para uso posterior
   //------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
       std::cout << "Falha inicializando glad! " << std::endl;
        return -1;
    }

   //Registro de callbacks: sempre depois da criacao da janela e antes do loop
   //------------------------------------------------------------------------------
   glfwSetKeyCallback(win, callback_teclado); //evento de teclado
   glfwSetFramebufferSizeCallback(win,callback_redimensionamento);//redim. janela 

   
   //Inicializacao: tudo que eh feito um unica vez (e.g., compilacao de shaders)
   //------------------------------------------------------------------------------
    init();

   //Loop de renderizacao: redesenha imagens e aguarda entrada de usuario
   //------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(win))
    {
        // renderiza a cena
        // ------------------------------------------------------------------------
        renderiza();

        // glfw: swap buffers e inspeciona eventos de I/O
        // ------------------------------------------------------------------------
	glfwSwapBuffers(win);
	glfwWaitEvents();            //aguarda evento; glfwPostEmptyEvent() desbloqueia
        //glfwPollEvents();          //checa evento e retorna
	//glfwWaitEventsTimeout(0.5);//aguarda evento e ou temporizador
    }

    // Finaliza, liberando os recursos alocados
    // ----------------------------------------------------------------------------
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}


