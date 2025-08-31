//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* fShaderRed = "shaders/red.frag";
static const char* fShaderGreen = "shaders/green.frag";
static const char* fShaderBlue = "shaders/blue.frag";
static const char* fShaderBrown = "shaders/brown.frag";
static const char* fShaderDarkG = "shaders/darkgreen.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{

	GLfloat vertices_A[] = {
		// x, y, z,   r, g, b
		//LETRA A
		//T1
		-1.0f,0.4f, 0.5f, 0.0f, 0.0f, 1.0f,//1
		-1.0f,-0.4f,0.5f, 0.0f, 0.0f, 1.0f,//2
		-0.8f,0.4f,0.5f, 0.0f, 0.0f, 1.0f,//3
		//T2
		-1.0f,-0.4f,0.5f, 0.0f, 0.0f, 1.0f,//2
		-0.8f,0.4f,0.5f, 0.0f, 0.0f, 1.0f,//3
		-0.8f,-0.4f,0.5f, 0.0f, 0.0f, 1.0f,//4
		//T3
		-0.8f,0.4f,0.5f, 0.0f, 0.0f, 1.0f,//3
		-0.6f,0.4f,0.5f, 0.0f, 0.0f, 1.0f,//5
		-0.8f,0.2f,0.5f, 0.0f, 0.0f, 1.0f,//6
		//T4
		-0.6f,0.4f,0.5f, 0.0f, 0.0f, 1.0f,//5
		-0.8f,0.2f,0.5f, 0.0f, 0.0f, 1.0f,//6
		-0.6F,0.2F,0.5F, 0.0f, 0.0f, 1.0f,//7
		//T5
		-0.8f,0.0f,0.5f, 0.0f, 0.0f, 1.0f,//8
		-0.6f,0.0f,0.5f, 0.0f, 0.0f, 1.0f,//9
		-0.8f,-0.2f,0.5f, 0.0f, 0.0f, 1.0f,//10
		//T6
		-0.6f,0.0f,0.5f, 0.0f, 0.0f, 1.0f,//9
		-0.8f,-0.2f,0.5f, 0.0f, 0.0f, 1.0f,//10
		-0.6F,-0.2F,0.5F, 0.0f, 0.0f, 1.0f,//11
		//T7
		-0.6f,0.4f, 0.5f, 0.0f, 0.0f, 1.0f,//5
		-0.6f,-0.4f,0.5f, 0.0f, 0.0f, 1.0f,//12
		-0.4f,0.4f,0.5f, 0.0f, 0.0f, 1.0f,//13
		//T8
		-0.6f,-0.4f,0.5f, 0.0f, 0.0f, 1.0f,//12
		-0.4f,0.4f,0.5f, 0.0f, 0.0f, 1.0f,//13
		-0.4f,-0.4f,0.5f, 0.0f, 0.0f, 1.0f,//4
	};
	MeshColor* letraA = new MeshColor();
	letraA->CreateMeshColor(vertices_A, sizeof(vertices_A) / sizeof(vertices_A[0]));
	meshColorList.push_back(letraA);

	GLfloat vertices_B[] = {
		// x, y, z,   r, g, b
		//LETRA Q
		//T9
		-0.3f,0.4f, 0.5f,0.0f, 1.0f, 0.0f,
		-0.3f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		-0.1f,0.4f,0.5f,0.0f, 1.0f, 0.0f,
		//T10
		-0.3f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		-0.1f,0.4f,0.5f,0.0f, 1.0f, 0.0f,
		-0.1f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		//T11
		-0.1f,0.4f,0.5f,0.0f, 1.0f, 0.0f,
		0.1f,0.4f,0.5f,0.0f, 1.0f, 0.0f,
		-0.1f,0.2f,0.5f,0.0f, 1.0f, 0.0f,
		//T12
		0.1f,0.4f,0.5f,0.0f, 1.0f, 0.0f,
		-0.1f,0.2f,0.5f,0.0f, 1.0f, 0.0f,
		0.1f,0.2f,0.5f,0.0f, 1.0f, 0.0f,
		//T13
		-0.1f,-0.2f,0.5f,0.0f, 1.0f, 0.0f,
		0.1f,-0.2f,0.5f,0.0f, 1.0f, 0.0f,
		-0.1f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		//T14
		0.1f,-0.2f,0.5f,0.0f, 1.0f, 0.0f,
		-0.1f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		0.1f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		//T9
		0.1f,0.4f, 0.5f,0.0f, 1.0f, 0.0f,
		0.1f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		0.3f,0.4f,0.5f,0.0f, 1.0f, 0.0f,
		//T10
		0.1f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		0.3f,0.4f,0.5f,0.0f, 1.0f, 0.0f,
		0.3f,-0.4f,0.5f,0.0f, 1.0f, 0.0f,
		//T11
		0.3f,-0.2f, 0.5f,0.0f, 1.0f, 0.0f,
		0.3f,-0.5f,0.5f,0.0f, 1.0f, 0.0f,
		0.4f,-0.5f,0.5f,0.0f, 1.0f, 0.0f,
		//T10
		0.1f, -0.4f, 0.5f,0.0f, 1.0f, 0.0f,
		0.3f, -0.4f, 0.5f,0.0f, 1.0f, 0.0f,
		0.3f, -0.5f, 0.5f,0.0f, 1.0f, 0.0f,

	};
	MeshColor* letraB = new MeshColor();
	letraB->CreateMeshColor(vertices_B, sizeof(vertices_B) / sizeof(vertices_B[0]));
	meshColorList.push_back(letraB);

	GLfloat vertices_C[] = {
		// x, y, z,   r, g, b
		//LETRA D
		//T1
		1.0f,0.2f, 0.5f, 1.0f, 0.0f, 0.0f,
		1.0f,-0.2f,0.5f, 1.0f, 0.0f, 0.0f,//2
		0.8f,0.2f,0.5f, 1.0f, 0.0f, 0.0f,//3
		//T2
		1.0f,-0.2f,0.5f, 1.0f, 0.0f, 0.0f,//2
		0.8f,0.2f,0.5f, 1.0f, 0.0f, 0.0f,//3
		0.8f,-0.2f,0.5f, 1.0f, 0.0f, 0.0f,//4
		//T3
		0.8f,0.4f,0.5f, 1.0f, 0.0f, 0.0f,//3
		0.6f,0.4f,0.5f, 1.0f, 0.0f, 0.0f,//5
		0.8f,0.2f,0.5f, 1.0f, 0.0f, 0.0f,//6
		//T4
		0.6f,0.4f,0.5f, 1.0f, 0.0f, 0.0f,//5
		0.8f,0.2f,0.5f, 1.0f, 0.0f, 0.0f,//6
		0.6F,0.2F,0.5F, 1.0f, 0.0f, 0.0f,//7
		//T5
		0.8f,-0.2f,0.5f, 1.0f, 0.0f, 0.0f,//8
		0.6f,-0.2f,0.5f, 1.0f, 0.0f, 0.0f,//9
		0.8f,-0.4f,0.5f, 1.0f, 0.0f, 0.0f,//10
		//T6
		0.6f,-0.2f,0.5f, 1.0f, 0.0f, 0.0f,//9
		0.8f,-0.4f,0.5f, 1.0f, 0.0f, 0.0f,//10
		0.6F,-0.4F,0.5F, 1.0f, 0.0f, 0.0f,//11
		//T7
		0.6f,0.4f, 0.5f, 1.0f, 0.0f, 0.0f,//5
		0.6f,-0.4f,0.5f, 1.0f, 0.0f, 0.0f,//12
		0.4f,0.4f,0.5f, 1.0f, 0.0f, 0.0f,//13
		//T8
		0.6f,-0.4f,0.5f, 1.0f, 0.0f, 0.0f,//12
		0.4f,0.4f,0.5f, 1.0f, 0.0f, 0.0f,//13
		0.4f,-0.4f,0.5f, 1.0f, 0.0f, 0.0f,//4
		//T9
		0.8f, 0.4f, 0.5f, 1.0f, 0.0f, 0.0f,//5
		0.8f, 0.2f, 0.5f, 1.0f, 0.0f, 0.0f,//12
		1.0f, 0.2f, 0.5f, 1.0f, 0.0f, 0.0f,//13
		//T10
		1.0f, -0.2f, 0.5f, 1.0f, 0.0f, 0.0f,//12
		0.8f, -0.2f, 0.5f, 1.0f, 0.0f, 0.0f,//13
		0.8f, -0.4f, 0.5f, 1.0f, 0.0f, 0.0f,//4

	};
	MeshColor* letraC = new MeshColor();
	letraC->CreateMeshColor(vertices_C, sizeof(vertices_C) / sizeof(vertices_C[0]));
	meshColorList.push_back(letraC);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	// Triángulo azul
	GLfloat vertices_trianguloazul[] = {
		-1.0f, -1.0f, 0.5f,   0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.5f,   0.0f, 0.0f, 1.0f,
		 0.0f,  1.0f, 0.5f,   0.0f, 0.0f, 1.0f,
	};
	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	// Triángulo verde
	GLfloat vertices_trianguloverde[] = {
		-1.0f, -1.0f, 0.5f,   0.0f, 0.5f, 0.0f,
		 1.0f, -1.0f, 0.5f,   0.0f, 0.5f, 0.0f,
		 0.0f,  1.0f, 0.5f,   0.0f, 0.5f, 0.0f,
	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	// Cuadrado rojo
	GLfloat vertices_cuadradorojo[] = {
		-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
	};
	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	// Cuadrado café
	GLfloat vertices_cuadradocafe[] = {
		-0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		-0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
		-0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shaderRed = new Shader();
	shaderRed->CreateFromFiles(vShader, fShaderRed);//shader para usar color fijo rojo
	shaderList.push_back(*shaderRed);

	Shader* shaderGreen = new Shader();
	shaderGreen->CreateFromFiles(vShader, fShaderGreen);//shader para usar color fijo verde
	shaderList.push_back(*shaderGreen);

	Shader* shaderBlue = new Shader();
	shaderBlue->CreateFromFiles(vShader, fShaderBlue);//shader para usar color fijo azul
	shaderList.push_back(*shaderBlue);

	Shader* shaderBrown = new Shader();
	shaderBrown->CreateFromFiles(vShader, fShaderBrown);//shader para usar color fijo café
	shaderList.push_back(*shaderBrown);

	Shader* shaderDarkG = new Shader();
	shaderDarkG->CreateFromFiles(vShader, fShaderDarkG);//shader para usar color fijo verde oscuro
	shaderList.push_back(*shaderDarkG);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		//shaderList[0].useShader(); 
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();
		//angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[1]->RenderMesh();

		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		//shaderList[1].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//------------------Dibujar letras------------------
		//shaderList[1].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//No fue necesario utilizar ya que previamente se había calculado las coordenadas
		//de otra forma 
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.5f)); // posición A
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshColorList[0]->RenderMeshColor();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.5f)); // posición Q
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshColorList[1]->RenderMeshColor();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.5f)); // posición D
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshColorList[2]->RenderMeshColor();
		//----------------------------------------------------

		//------------------Dibujar casita------------------
		
		// Usamos projection y model uniform de cada shader
		// Techo = pirámide azul
		shaderList[4].useShader(); //[4] es el azul
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.62f, -4.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.75f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // pirámide

		// Paredes = cubo rojo
		shaderList[2].useShader(); // rojo
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.38f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // cubo

		// Ventana izquierda = cubo verde
		shaderList[3].useShader(); // verde
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.23f, -0.05f, -3.5f));
		model = glm::scale(model, glm::vec3(0.32f, 0.38f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // cubo

		// Ventana derecha = cubo verde
		shaderList[3].useShader(); // verde
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.23f, -0.05f, -3.5f));
		model = glm::scale(model, glm::vec3(0.32f, 0.38f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // cubo

		// Puerta = cubo verde
		shaderList[3].useShader(); // verde
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -3.5f));
		model = glm::scale(model, glm::vec3(0.32f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // cubo

		// Árbol tronco = cubo café
		shaderList[5].useShader(); // café
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.78f, -0.88f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.24f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // cubo

		// Árbol hojas = pirámide verde obs
		shaderList[6].useShader(); // verde obs
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.78f, -0.45f, -4.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.62f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // pirámide

		// Árbol tronco = cubo café
		shaderList[5].useShader(); // café
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.78f, -0.88f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.24f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // cubo

		// Árbol hojas = pirámide verde obs
		shaderList[6].useShader(); // verde obs
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.78f, -0.45f, -4.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.62f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // pirámide

		// --- Hojas del árbol ---
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f, -0.45f, -4.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.3f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshColorList[4]->RenderMeshColor(); // triángulo verde

		// --- Tronco café ---
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f, -0.87f, -4.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.25f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshColorList[6]->RenderMeshColor(); // cuadrado café

		// --- Hojas del árbol ---
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.8f, -0.45f, -4.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.3f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshColorList[4]->RenderMeshColor(); // triángulo verde

		// --- Tronco café ---
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.8f, -0.87f, -4.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.25f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshColorList[6]->RenderMeshColor(); // cuadrado café

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/