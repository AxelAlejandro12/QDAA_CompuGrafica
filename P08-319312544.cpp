/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Lamp_M;
Model Auto_Cofre;
Model Rico_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
bool luzAntorchaEncendida = true;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	// Índices: 8 caras triangulares (cada cara usa 3 vértices únicos)
	unsigned int octaedro_indices[] = {
	0, 1, 2, // cara 1
	3, 4, 5, // cara 2
	6, 7, 8, // cara 3
	9, 10, 11, // cara 4
	12, 13, 14, // cara 5
	15, 16, 17, // cara 6
	18, 19, 20, // cara 7
	21, 22, 23 // cara 8
	};

	// 24 vértices (3 por cara)
	// Formato: x, y, z, S, T, NX, NY, NZ
	GLfloat octaedro_vertices[] = {
		// --- CARA 1 --- (4)
		0.0f,  0.5f,  0.0f,  0.486328f, 0.755859f,  +0.577f, -0.577f, -0.577f,
		0.0f,  0.0f,  0.5f,  0.011719f, 0.755859f,  +0.577f, -0.577f, -0.577f,
	   -0.5f,  0.0f,  0.0f,  0.248047f, 0.994141f,  +0.577f, -0.577f, -0.577f,

	   // --- CARA 2 --- (1)
	   0.0f,  0.5f,  0.0f,  0.486328f, 0.746094f,  -0.577f, -0.577f, -0.577f,
	   0.5f,  0.0f,  0.0f,  0.250000f, 0.509766f,  -0.577f, -0.577f, -0.577f,
	   0.0f,  0.0f,  0.5f,  0.013672f, 0.746094f,  -0.577f, -0.577f, -0.577f,

	   // --- CARA 3 --- (8)
	   0.0f,  0.5f,  0.0f,  0.505859f, 0.744141f,  -0.577f, -0.577f, +0.577f,
	   0.0f,  0.0f, -0.5f,  0.744141f, 0.505859f,  -0.577f, -0.577f, +0.577f,
	   0.5f,  0.0f,  0.0f,  0.255859f, 0.505859f,  -0.577f, -0.577f, +0.577f,

	   // --- CARA 4 --- (5)
	   0.0f,  0.5f,  0.0f,  0.509766f, 0.746094f,  +0.577f, -0.577f, +0.577f,
	  -0.5f,  0.0f,  0.0f,  0.984375f, 0.746094f,  +0.577f, -0.577f, +0.577f,
	   0.0f,  0.0f, -0.5f,  0.748047f, 0.507812f,  +0.577f, -0.577f, +0.577f,

	   // --- CARA 5 --- (6)
	   0.0f, -0.5f,  0.0f,  0.490234f, 0.255859f,  -0.577f, +0.577f, -0.577f,
	   0.0f,  0.0f,  0.5f,  0.246094f, 0.496094f,  -0.577f, +0.577f, -0.577f,
	   0.5f,  0.0f,  0.0f,  0.009766f, 0.255859f,  -0.577f, +0.577f, -0.577f,

	   // --- CARA 6 --- (3)
	   0.0f, -0.5f,  0.0f,  0.498047f, 0.259766f,  -0.577f, +0.577f, +0.577f,
	   0.5f,  0.0f,  0.0f,  0.257812f, 0.500000f,  -0.577f, +0.577f, +0.577f,
	   0.0f,  0.0f, -0.5f,  0.738281f, 0.500000f,  -0.577f, +0.577f, +0.577f,

	   // --- CARA 7 --- (2)
	   0.0f, -0.5f,  0.0f,  0.507812f, 0.255859f,  +0.577f, +0.577f, +0.577f,
	  -0.5f,  0.0f,  0.0f,  0.986328f, 0.255859f,  +0.577f, +0.577f, +0.577f,
	   0.0f,  0.0f, -0.5f,  0.744141f, 0.494141f,  +0.577f, +0.577f, +0.577f,

	   // --- CARA 8 --- (7)
	   0.0f, -0.5f,  0.0f,  0.486328f, 0.248047f,  +0.577f, +0.577f, -0.577f,
	   0.0f,  0.0f,  0.5f,  0.011719f, 0.248047f,  +0.577f, +0.577f, -0.577f,
	  -0.5f,  0.0f,  0.0f,  0.250000f, 0.011719f,  +0.577f, +0.577f, -0.577f,
	};


	Mesh* dado = new Mesh();
	dado->CreateMesh(octaedro_vertices, octaedro_indices, 192, 36);
	meshList.push_back(dado);

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/cubo8caras.png");
	dadoTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/autosincofre.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Lamp_M = Model();
	Lamp_M.LoadModel("Models/lampara.obj");
	Auto_Cofre = Model();
	Auto_Cofre.LoadModel("Models/cofre1.obj");
	Rico_M = Model();
	Rico_M.LoadModel("Models/ricolamp.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//POINT LIGHTS
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//verde
	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f,
		0.7f, 0.7f,
		-6.0f, 1.0f, 15.0f,
		0.3f, 0.3f, 0.3f);
	pointLightCount++;
	//azul
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
		0.7f, 0.7f,
		-3.0f, 1.0f, 18.0f,
		0.3f, 0.3f, 0.3f);
	pointLightCount++;
	//roja
	pointLights[2] = PointLight(1.0f, 0.0f, 1.0f,
		0.7f, 0.7f,
		0.0f, 1.0f, 21.0f,
		0.3f, 0.3f, 0.3f);
	pointLightCount++;
	//amarilla
	pointLights[3] = PointLight(1.0f, 0.8f, 0.4f, //1.0 0.8 0.4
		0.7f, 0.7f,
		0.0f, 2.5f, -8.0f,  //0.0f, 2.5f, -8.0f,
		0.3f, 0.3f, 0.3f);
	pointLightCount++;
	//blanca
	pointLights[4] = PointLight(1.0f, 1.0f, 1.0f, //1.0 0.8 0.4
		0.7f, 0.7f,
		0.0f, 0.0f, 0.0f,  //0.0f, 2.5f, -8.0f,
		0.3f, 0.3f, 0.3f);
	pointLightCount++;
	

	//Segundo arreglo  de luces puntuales: blanca, verde, azul, roja
	pointLights2[0] = pointLights[3];
	//pointLights2[0].SetPos(glm::vec3(-3.0f, 1.0f, 2.0f)); //cambiar posición
	pointLights2[1] = pointLights[0];
	pointLights2[2] = pointLights[1];
	pointLights2[3] = pointLights[2];

	//SPOT LIGHTS
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//luz fija verde
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	// Faro azul del coche
	spotLights[2] = SpotLight(
		0.0f, 0.0f, 1.0f,   // color azul (r,g,b)
		0.3f, 2.0f,         // intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,   // posición inicial (la actualizaremos en el loop)
		0.0f, 0.0f, 1.0f,   // dirección hacia adelante (eje Z)
		1.0f, 0.0f, 0.0f,   // atenuación constante, lineal y cuadrática
		10.0f               // límite del ángulo del cono (en grados)
	);
	spotLightCount++;
	// Faro amarillo del helicoptero
	spotLights[3] = SpotLight(
		1.0f, 1.0f, 0.0f,   // color amarillo (r,g,b)
		0.3f, 1.0f,         // intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,   // posición inicial (la actualizaremos en el loop)
		0.0f, -1.0f, 0.0f,   // dirección hacia abajo (eje -Y)
		1.0f, 0.0f, 0.0f,   // atenuación constante, lineal y cuadrática
		10.0f               // límite del ángulo del cono (en grados)
	);
	spotLightCount++;
	// Spotlight hacia adelante
	spotLights[4] = SpotLight(
		1.0f, 1.0f, 1.0f,   // color blanco
		0.3f, 1.0f,         // intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,   // posición (se actualizará dinámicamente)
		-1.0f, 0.0f, 0.0f,  // dirección hacia adelante (X negativa)
		1.0f, 0.0f, 0.0f,   // atenuación
		15.0f                // ángulo del cono
	);
	spotLightCount++;
	// Spotlight hacia atrás
	spotLights[5] = SpotLight(
		1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,   // dirección hacia atrás (X positiva)
		1.0f, 0.0f, 0.0f,
		15.0f
	);
	spotLightCount++;
	// Luz del cofre del auto (faro)
	spotLights[6] = SpotLight(
		0.627f, 0.125f, 0.941f,    // color blanco cálido (similar a un faro halógeno)
		0.4f, 2.0f,           // intensidades ambiental y difusa
		0.0f, 0.0f, 0.0f,    // posición (ligeramente por delante del auto)
		0.0f, -0.5f, 1.0f,    // dirección: un poco hacia abajo y al frente
		1.0f, 0.09f, 0.032f,  // atenuación (constante, lineal, cuadrática)
		20.0f                 // ángulo del cono (más amplio)
	);
	spotLightCount++;
	
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);

	//VARIABLES
	// Auto
	// dirección del faro (hacia adelante en X)
	glm::vec3 faroDir = glm::vec3(-1.0f, -0.1f, 0.0f);
	glm::vec3 cofreDir = glm::vec3(-1.0f, 1.0f, 0.0f);
	//glm::vec3 autoDir = glm::vec3(-1.0f, 0.0f, 0.0f);
	
	// Helicóptero
	// actualizar posición y dirección de la luz amarilla
	glm::vec3 faroDir2 = glm::vec3(0.0f, -1.0f, 0.0f);

	// posición de la lámpara
	glm::vec3 lampPosition = glm::vec3(0.0f, -0.7f, -8.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		
		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//spotLights[2].SetFlash(faroPos, faroDir);
		//spotLights[3].SetFlash(faroPos2, faroDir2);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		/* --- Enviar las point lights ---
		PointLight activePointLights[MAX_POINT_LIGHTS];
		unsigned int activePointCount = 0;
		for (unsigned int i = 0; i < pointLightCount; i++) {
			// Solo apagar la antorcha si está desactivada
			if (i == 3 && !luzAntorchaEncendida) continue;
			activePointLights[activePointCount++] = pointLights[i];
		}
		// Enviar las point lights activas al shader
		shaderList[0].SetPointLights(activePointLights, activePointCount);
		*/

		/*
		if (mainWindow.getprendida()){
			if (mainWindow.getLucespuntuales() < 0.5f){
				//Se envía al shader el arreglo de luces puntuales, agregar el if para intercambiar arreglos
				shaderList[0].SetPointLights(pointLights, pointLightCount);
				//printf("Primer arreglo de luces puntuales\n");
			}
			else{	//segundo arreglo
				shaderList[0].SetPointLights(pointLights2, pointLightCount);
				//printf("Segundo arreglo de luces puntuales\n");
			}
		}
		else{
			if (mainWindow.getLucespuntuales() < 0.5f){
				//Se envía al shader el arreglo de luces puntuales, agregar el if para intercambiar arreglos
				shaderList[0].SetPointLights(pointLights, pointLightCount-1);
				//printf("Primer arreglo de luces puntuales\n");
			}
			else{	//segundo arreglo
				shaderList[0].SetPointLights(pointLights2, pointLightCount-1);
				//printf("Segundo arreglo de luces puntuales\n");
			}
		}*/

		// Luz puntual de la lámpara
		//if (mainWindow.getprendida()) {
		//	shaderList[0].SetPointLights(pointLights, pointLightCount);
		//}
		//else {
		//	shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		//}

		// Luz puntual de la lámpara
		// if (mainWindow.getprendida()) {
		// 	shaderList[0].SetPointLights(pointLights, pointLightCount);
		// }
		// else {
		// 	shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		// }

		PointLight activePointLights[MAX_POINT_LIGHTS];
		unsigned int activePointCount = 0;

		// Slots 0, 1, 2: Luces fijas (verde, azul, roja). Se asume que siempre están encendidas.
		activePointLights[activePointCount++] = pointLights[0];
		activePointLights[activePointCount++] = pointLights[1];
		activePointLights[activePointCount++] = pointLights[2];

		// Slot 3: Luz del Modelo Rico (amarilla)
		if (mainWindow.getrico()) {
			activePointLights[activePointCount++] = pointLights[3];
		}

		// Slot 4: Luz de la Antorcha/Lámpara (blanca)
		if (mainWindow.getprendida()) {
			activePointLights[activePointCount++] = pointLights[4];
		}

		// Enviar las point lights activas
		shaderList[0].SetPointLights(activePointLights, activePointCount);

		// ... (código para SpotLights)

		//luz cofre
		//if (mainWindow.getarticulacion1()) {
		//	shaderList[0].SetSpotLights(spotLights, spotLightCount);
		//}
		//else {
		//	shaderList[0].SetSpotLights(spotLights, spotLightCount-1);
		//}

		//2. Agregar luz de tipo spotlight para el coche de tal forma que al avanzar (mover con teclado hacia
		//dirección de X negativa ) ilumine con un spotlight hacia adelante y al
		//retroceder((mover con teclado hacia dirección de X positiva) ilumine con
		//un spotlight hacia atrás.Son dos spotlights diferentes que se prenderán y apagarán
		//de acuerdo a alguna bandera asignada por ustedes.

		// Manejo de SpotLights del Coche
		SpotLight activeSpotLights[MAX_SPOT_LIGHTS];
		unsigned int activeSpotCount = 0;

		activeSpotLights[activeSpotCount++] = spotLights[0];
		activeSpotLights[activeSpotCount++] = spotLights[1];
		activeSpotLights[activeSpotCount++] = spotLights[2];
		activeSpotLights[activeSpotCount++] = spotLights[3];

		// 5. Luz del Cofre (spotLights[6]) - Activa sólo si el cofre está abierto (articulacion1 > 0)
		// Nota: Tu lógica en Window.cpp solo permite hasta 0, ajusta el límite si quieres un encendido más fino.
		if (mainWindow.getarticulacion1() < 0.0f) { // Cofre abierto (articulacion1 va de 0 a -45)
			activeSpotLights[activeSpotCount++] = spotLights[6];
		}

		// 6. Luces de movimiento (Punto 2): Adelante (slot 4) y Atrás (slot 5)
		// mainWindow.getAuto() devuelve cocheAdelante: true si avanza (X negativa), false si retrocede (X positiva)
		if (mainWindow.getAuto()) { // Avanzando (tecla 'U' -> X negativa)
		// Enciende Faro Delantero (spotLights[4])
			activeSpotLights[activeSpotCount++] = spotLights[5];
		}
		else { // Retrocediendo (tecla 'Y' -> X positiva)
			// Enciende Faro Trasero (spotLights[5])
			activeSpotLights[activeSpotCount++] = spotLights[4];
		}


		// Enviamos el arreglo de luces activas al shader
		shaderList[0].SetSpotLights(activeSpotLights, activeSpotCount);

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//1.- Agregar su dado de 8 caras y editar sus normales para que las caras del dado sean iluminadas correctamente.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f + mainWindow.getmuevex(), 0.5f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		modelaux = model;
		spotLights[2].SetFlash(glm::vec3(model[3].x-2.0f, model[3].y+0.2f, model[3].z+2.0f),faroDir);
		spotLights[4].SetPos(glm::vec3(model[3].x, model[3].y - 0.2f, model[3].z));
		spotLights[5].SetPos(glm::vec3(model[3].x, model[3].y - 0.2f, model[3].z));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();
		//Cofre
		model = glm::translate(model, glm::vec3(0.0f, 0.58f, 1.79f));
		spotLights[6].SetFlash(glm::vec3(model[3].x, model[3].y + -1.0f, model[3].z + 0.0f),cofreDir);
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_Cofre.RenderModel();
		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.8f, -0.8f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.88f, -0.8f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.8f, -0.8f, -2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.88f, -0.8f, -2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	
		//Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f+mainWindow.getmuevex2(), 5.0f, 6.0));
		spotLights[3].SetFlash(glm::vec3(model[3].x + 1.77f, model[3].y - 0.1f, model[3].z + 0.0f), faroDir2);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Antorcha
		model = glm::mat4(1.0);
		model = glm::translate(model, lampPosition);
		pointLights[4].SetPos(glm::vec3(model[3].x, model[3].y + 3.5f, model[3].z));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp_M.RenderModel();

		//Rico
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -1.0f, 5.0));
		pointLights[3].SetPos(glm::vec3(model[3].x, model[3].y + 3.0f, model[3].z));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rico_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
