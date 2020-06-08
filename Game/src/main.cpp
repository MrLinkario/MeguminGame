#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
//Shader para las particulas de fuego
Shader shaderParticlesFire;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 10.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;

ShadowBox * shadowBox;

// Model animate instance
// Megumin
Model meguminModelAnimate;
int controlJugador = 1;
int vidas = 3;
int win = 0;
int lose = 0;
int animPerder = 0;
int animExplo = 0;
int animGanar = 0;
int explo = 0;
int roar = 0;

//Shrek
Model shrekModelAnimate;
int shrekF = 0;
float rotShrek=0.0f;
int activShrek = 0;
int puntos = 0;
int swamp = 0;

//Ejercito de Knuckles

//Knuckles1
Model knuckles1ModelAnimate;
int activKnuckles1 = 1;
//Knuckles2
Model knuckles2ModelAnimate;
int activKnuckles2 = 1;
//Knuckles3
Model knuckles3ModelAnimate;
int activKnuckles3 = 1;
//Knuckles4
Model knuckles4ModelAnimate;
int activKnuckles4 = 1;
//Knuckles5
Model knuckles5ModelAnimate;
int activKnuckles5 = 1;
//Knuckles6
Model knuckles6ModelAnimate;
int activKnuckles6 = 1;
//Knuckles7
Model knuckles7ModelAnimate;
int activKnuckles7 = 1;
//Knuckles8
Model knuckles8ModelAnimate;
int activKnuckles8 = 1;
//Knuckles9
Model knuckles9ModelAnimate;
int activKnuckles9 = 1;
//Knuckles0
Model knuckles0ModelAnimate;
int activKnuckles0 = 1;





// Terrain model instance
Terrain terrain(-1, -1, 200, 50, "../Textures/HMPrueba.png");

GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFireID, texId;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/Skybox/hills_ft.tga",
		"../Textures/Skybox/hills_bk.tga",
		"../Textures/Skybox/hills_up.tga",
		"../Textures/Skybox/hills_dn.tga",
		"../Textures/Skybox/hills_rt.tga",
		"../Textures/Skybox/hills_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixMegumin = glm::mat4(1.0f);
glm::mat4 modelMatrixShrek = glm::mat4(1.0f);
//ejercito de knuckles
glm::mat4 modelMatrixKnuckles1 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles2 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles3 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles4 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles5 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles6 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles7 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles8 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles9 = glm::mat4(1.0f);
glm::mat4 modelMatrixKnuckles0 = glm::mat4(1.0f);




int animationIndex = 3;
int livesPlayer = 3;

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"fire", glm::vec3(0.0, 0.0, 7.0)}
};

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 1.0, particleLifetime = 10.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;


double deltaTime;
double currTime, lastTime;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

 // OpenAL Defines
#define NUM_BUFFERS 6
#define NUM_SOURCES 6
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
//source 3
ALfloat source3Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0,0.0, 0.0 };
//source 4
ALfloat source4Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0,0.0, 0.0 };
//source 5
ALfloat source5Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source5Vel[] = { 0.0,0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true, true, true, true };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}


// Implementacion de todas las funciones.

		/********   INIT   *********/

void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr, nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	//Megumin
	meguminModelAnimate.loadModel("../models/Megumin6/Mixamo_Megumin.fbx");
	meguminModelAnimate.setShader(&shaderMulLighting);

	//Shrek
	shrekModelAnimate.loadModel("../models/Shrek/shrek.fbx");
	shrekModelAnimate.setShader(&shaderMulLighting);

	//Ejercito de Knuckles
	//Knuckles1
	knuckles1ModelAnimate.loadModel("../models/Vegetables/col.fbx");
	knuckles1ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles2
	knuckles2ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles2ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles3
	knuckles3ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles3ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles4
	knuckles4ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles4ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles5
	knuckles5ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles5ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles6
	knuckles6ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles6ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles7
	knuckles7ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles7ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles8
	knuckles8ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles8ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles9
	knuckles9ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles9ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles0
	knuckles0ModelAnimate.loadModel("../models/SovietKnuckles/untitled.fbx");
	knuckles0ModelAnimate.setShader(&shaderMulLighting);



	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox

	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}


	// Definiendo la textura a utilizar Background BlendMap

	Texture textureTerrainBackground("../Textures/mud.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);


	// Definiendo la textura a utilizar R Component BlendMap

	Texture textureTerrainR("../Textures/Rock.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);


	// Definiendo la textura a utilizar G Component BlendMap

	Texture textureTerrainG("../Textures/Moss.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);


	// Definiendo la textura a utilizar B Component Blend Map

	Texture textureTerrainB("../Textures/Snow.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);


	// Definiendo la textura a utilizar Carga del BlendMap

	Texture textureTerrainBlendMap("../Textures/blendMapMP.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	//Textura particula de fuego
	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for (int i = 0; i < randData.size(); i++) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	//inicializacion de particulas de fuego
	initParticleBuffersFire();


	//Inicializacion del framebuffer para almacenar el buffer de profunidadad
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/swamp.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fireball.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/wae.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/Explosion.wav");
	buffer[4] = alutCreateBufferFromFile("../sounds/Shrek.wav");
	buffer[5] = alutCreateBufferFromFile("../sounds/oof.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	//alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 3.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	//alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	//alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 0.3f);
	alSourcefv(source[3], AL_POSITION, source2Pos);
	alSourcefv(source[3], AL_VELOCITY, source2Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	//alSourcei(source[3], AL_LOOPING, AL_TRUE);
	alSourcef(source[3], AL_MAX_DISTANCE, 500);

	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 0.3f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	//alSourcei(source[4], AL_LOOPING, AL_TRUE);
	alSourcef(source[4], AL_MAX_DISTANCE, 500);

	alSourcef(source[5], AL_PITCH, 1.0f);
	alSourcef(source[5], AL_GAIN, 3.0f);
	alSourcefv(source[5], AL_POSITION, source4Pos);
	alSourcefv(source[5], AL_VELOCITY, source4Vel);
	alSourcei(source[5], AL_BUFFER, buffer[5]);
	//alSourcei(source[5], AL_LOOPING, AL_TRUE);
	alSourcef(source[5], AL_MAX_DISTANCE, 500);

}

/********   DESTROY   *********/

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderViewDepth.destroy();
	shaderDepth.destroy();
	shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects animate
	meguminModelAnimate.destroy();
	shrekModelAnimate.destroy();
	knuckles1ModelAnimate.destroy();
	knuckles2ModelAnimate.destroy();
	knuckles3ModelAnimate.destroy();
	knuckles4ModelAnimate.destroy();
	knuckles5ModelAnimate.destroy();
	knuckles6ModelAnimate.destroy();
	knuckles7ModelAnimate.destroy();
	knuckles8ModelAnimate.destroy();
	knuckles9ModelAnimate.destroy();
	knuckles0ModelAnimate.destroy();
	


	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}


/********   RESHAPECALLBACK   *********/

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}


/********   KEYCALLBACK   *********/

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}


/********   MOUSECALLBACK   *********/

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}


/********   SCROLLCALLBACK   *********/

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	if (distanceFromTarget >= 7.0 && distanceFromTarget <= 20.0)
		camera->setDistanceFromTarget(distanceFromTarget);
}


/********   MOUSEBUTTONCALLBACK   *********/

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}


/********   PROCESSINPUT   *********/

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && controlJugador == 1) {
		modelMatrixMegumin = glm::rotate(modelMatrixMegumin, glm::radians(1.0f), glm::vec3(0, 1, 0));
		animationIndex = 5;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && controlJugador == 1) {
		modelMatrixMegumin = glm::rotate(modelMatrixMegumin, glm::radians(-1.0f), glm::vec3(0, 1, 0));
		animationIndex = 5;
	}if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && controlJugador == 1) {
		modelMatrixMegumin = glm::translate(modelMatrixMegumin, glm::vec3(0, 0, 0.15));
		animationIndex = 5;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && controlJugador == 1) {
		modelMatrixMegumin = glm::translate(modelMatrixMegumin, glm::vec3(0, 0, -0.15));
		animationIndex = 5;
	}
	else if (controlJugador == 1) {
		animationIndex = 3;
	}

	modelMatrixKnuckles1 = glm::translate(modelMatrixMegumin, glm::vec3(-1.1f, 2.9f, -0.0f));

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS  && controlJugador==1) {
		modelMatrixKnuckles1 = glm::translate(modelMatrixMegumin, glm::vec3(0.0f, 0.0f, 5.0f));
		animationIndex = 1;
	}

	if (puntos > 4) {
		activShrek = 1;
		//swamp = 1;
	}

	/*if (swamp == 1) {
		alSourcePlay(source[0]);
		swamp = 2;
	}
*/
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//alSourcePlay(source[5]);
	}
	
	

	//Animacion final: inicio
	/*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		controlJugador = 0;
		win = 1;
	}*/

	if (win == 1 && rotShrek < 19000) {
		//alSourcePlay(source[3]);
		animationIndex = 0;
		animExplo = animExplo + 100; 
	}

	if (animExplo >= 2400) {
		shrekF = 1;
	}

	if (shrekF == 1) {
		//alSourcePlay(source[4]);
		if (roar == 0) {
			roar = 1;
		}
		rotShrek = rotShrek+100;
		modelMatrixShrek= glm::rotate(modelMatrixShrek, glm::radians(10.0f), glm::vec3(0, 1, 0));
	}

	if (rotShrek >= 19000) {
		activShrek = 0;
	}

	if (win == 1 && rotShrek >= 19000) {
		animationIndex = 6;
		animGanar = animGanar + 100;
	}

	if (animGanar >= 6000) {
		exitApp = true;
	}
	//Animacion final: end

	//Fin de juego: inicio

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		vidas = vidas - 1;
	}

	if (vidas < 1) {
		controlJugador = 0;
		lose = 1;
	}

	if (lose == 1) {
		//alSourcePlay(source[5]);
		animationIndex = 4;
		animPerder = animPerder + 100;
	}

	if (animPerder >= 6000) {
		exitApp = true;
	}
	//Fin de juego:fin


	glfwPollEvents();
	return continueApplication;
}


/********   APPLICATIONLOOP   *********/

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	//Colocación de modelos
	//Megumin
	modelMatrixMegumin = glm::translate(modelMatrixMegumin, glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMegumin = glm::rotate(modelMatrixMegumin, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	//Shrek
	modelMatrixShrek= glm::translate(modelMatrixShrek, glm::vec3(-40.0f, 0.05f, -84.0f));
	modelMatrixShrek= glm::rotate(modelMatrixShrek, glm::radians(0.0f), glm::vec3(0, 1, 0));

	//EJercito de Knuckles
	//Knuckles1
	modelMatrixKnuckles1 = glm::translate(modelMatrixMegumin, glm::vec3(1.0f, 1.05f, 1.0f));
	modelMatrixKnuckles1 = glm::rotate(modelMatrixKnuckles1, glm::radians(90.0f), glm::vec3(0, 1, 0));
	//Knuckles2
	modelMatrixKnuckles2 = glm::translate(modelMatrixKnuckles2, glm::vec3(-80.0f, 0.05f, -65.0f));
	modelMatrixKnuckles2 = glm::rotate(modelMatrixKnuckles2, glm::radians(0.0f), glm::vec3(0, 1, 0));
	//Knuckles3
	modelMatrixKnuckles3 = glm::translate(modelMatrixKnuckles3, glm::vec3(8.0f, 0.05f, -55.0f));
	modelMatrixKnuckles3 = glm::rotate(modelMatrixKnuckles3, glm::radians(0.0f), glm::vec3(0, 1, 0));
	//Knuckles4
	modelMatrixKnuckles4 = glm::translate(modelMatrixKnuckles4, glm::vec3(60.0f, 0.05f, -90.0f));
	modelMatrixKnuckles4 = glm::rotate(modelMatrixKnuckles4, glm::radians(0.0f), glm::vec3(0, 1, 0));
	//Knuckles5
	modelMatrixKnuckles5 = glm::translate(modelMatrixKnuckles5, glm::vec3(-70.0f, 0.05f, 75.0f));
	modelMatrixKnuckles5 = glm::rotate(modelMatrixKnuckles5, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//Knuckles6
	modelMatrixKnuckles6 = glm::translate(modelMatrixKnuckles6, glm::vec3(5.0f, 0.05f, 85.0f));
	modelMatrixKnuckles6 = glm::rotate(modelMatrixKnuckles6, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//Knuckles7
	modelMatrixKnuckles7 = glm::translate(modelMatrixKnuckles7, glm::vec3(70.0f, 0.05f, 70.0f));
	modelMatrixKnuckles7 = glm::rotate(modelMatrixKnuckles7, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	//Knuckles8
	modelMatrixKnuckles8 = glm::translate(modelMatrixKnuckles8, glm::vec3(-25.0f, 0.05f, 5.0f));
	modelMatrixKnuckles8 = glm::rotate(modelMatrixKnuckles8, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//Knuckles9
	modelMatrixKnuckles9 = glm::translate(modelMatrixKnuckles9, glm::vec3(-12.0f, 0.05f, -65.0f));
	modelMatrixKnuckles9 = glm::rotate(modelMatrixKnuckles9, glm::radians(0.0f), glm::vec3(0, 1, 0));
	//Knuckles0
	modelMatrixKnuckles0 = glm::translate(modelMatrixKnuckles0, glm::vec3(-12.0f, 0.05f, -87.0f));
	modelMatrixKnuckles0 = glm::rotate(modelMatrixKnuckles0, glm::radians(0.0f), glm::vec3(0, 1, 0));

	

	lastTime = TimeManager::Instance().GetTime();

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	//Posición light, apuntador a la cámara, distancia máxima de alcance, plano cercano y el FOV (field of view)
	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 300.0f);

		axis = glm::axis(glm::quat_cast(modelMatrixMegumin));
		angleTarget = glm::angle(glm::quat_cast(modelMatrixMegumin));
		target = glm::vec3(modelMatrixMegumin[3].x, modelMatrixMegumin[3].y + 2.0f, modelMatrixMegumin[3].z);

		if (std::isnan(angleTarget))
			angleTarget = 0.0;
		if (axis.y < 0)
			angleTarget = -angleTarget;
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		// Se actualiza la caja usando las dimensiones del viewport
		shadowBox->update(screenWidth, screenHeight);
		shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0 / shadowBox->getWidth();
		lightProjection[1][1] = 2.0 / shadowBox->getHeight();
		lightProjection[2][2] = -2.0 / shadowBox->getLength();
		lightProjection[3][3] = 1.0;
		lightView = glm::lookAt(shadowBox->getCenter(), shadowBox->getCenter() + glm::normalize(-lightPos), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(glm::mat3(view))));

		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con multiples luces *****************************
		shaderTerrain.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));


		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.8)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.8)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		 // reset viewport
		 /*glViewport(0, 0, screenWidth, screenHeight);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 // render Depth map to quad for visual debugging
		 shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, depthMap);
		 boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		 boxViewDepth.render();*/

		 /*******************************************
		  * 2.- We render the normal objects
		  *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();
		/*******************************************
		 * Debug to box light box
		 *******************************************/
		 /*glm::vec3 front = glm::normalize(-lightPos);
		 glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		 glm::vec3 up = glm::normalize(glm::cross(front, right));
		 glDisable(GL_CULL_FACE);
		 glm::mat4 boxViewTransform = glm::mat4(1.0f);
		 boxViewTransform = glm::translate(boxViewTransform, centerBox);
		 boxViewTransform[0] = glm::vec4(right, 0.0);
		 boxViewTransform[1] = glm::vec4(up, 0.0);
		 boxViewTransform[2] = glm::vec4(front, 0.0);
		 boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		 boxLightViewBox.enableWireMode();
		 boxLightViewBox.render(boxViewTransform);
		 glEnable(GL_CULL_FACE);*/

		 /*******************************************
		  * Creacion de colliders
		  * IMPORTANT do this before interpolations
		  *******************************************/

		  // Collider de Megumin
		AbstractModel::OBB meguminCollider;
		glm::mat4 modelmatrixColliderMegumin = glm::mat4(modelMatrixMegumin);
		modelmatrixColliderMegumin = glm::rotate(modelmatrixColliderMegumin, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		meguminCollider.u = glm::quat_cast(modelmatrixColliderMegumin);
		modelmatrixColliderMegumin = glm::scale(modelmatrixColliderMegumin, glm::vec3(0.021, 0.021, 0.021));
		modelmatrixColliderMegumin = glm::translate(modelmatrixColliderMegumin,
			glm::vec3(meguminModelAnimate.getObb().c.x,
				meguminModelAnimate.getObb().c.y,
				meguminModelAnimate.getObb().c.z));
		meguminCollider.e = meguminModelAnimate.getObb().e * glm::vec3(1.0, 1.0, 6.0);
		meguminCollider.c = glm::vec3(modelmatrixColliderMegumin[3]);
		addOrUpdateColliders(collidersOBB, "megumin", meguminCollider, modelMatrixMegumin);


		// Collider de Shrek
		AbstractModel::OBB shrekCollider;
		if (activShrek == 1) {
			glm::mat4 modelmatrixColliderShrek = glm::mat4(modelMatrixShrek);
			modelmatrixColliderShrek = glm::rotate(modelmatrixColliderShrek, glm::radians(90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			shrekCollider.u = glm::quat_cast(modelmatrixColliderShrek);
			modelmatrixColliderShrek = glm::scale(modelmatrixColliderShrek, glm::vec3(0.000000021, 0.000000021, 0.000000021));
			modelmatrixColliderShrek = glm::translate(modelmatrixColliderShrek,
				glm::vec3(shrekModelAnimate.getObb().c.x,
					shrekModelAnimate.getObb().c.y,
					shrekModelAnimate.getObb().c.z));
			shrekCollider.e = shrekModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 1.0);
			shrekCollider.c = glm::vec3(modelmatrixColliderShrek[3]);
			addOrUpdateColliders(collidersOBB, "shrek", shrekCollider, modelMatrixShrek);
		}
		else {
			glm::mat4 modelmatrixColliderShrek = glm::mat4(modelMatrixShrek);
			modelmatrixColliderShrek = glm::rotate(modelmatrixColliderShrek, glm::radians(90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			shrekCollider.u = glm::quat_cast(modelmatrixColliderShrek);
			modelmatrixColliderShrek = glm::scale(modelmatrixColliderShrek, glm::vec3(0.000000021, 0.000000021, 0.000000021));
			modelmatrixColliderShrek = glm::translate(modelmatrixColliderShrek, glm::vec3(0.0f, 0.0f, -400.0f));
			shrekCollider.e = shrekModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 1.0);
			shrekCollider.c = glm::vec3(modelmatrixColliderShrek[3]);
			addOrUpdateColliders(collidersOBB, "shrek", shrekCollider, modelMatrixShrek);
		}

		//collider ejercito de Knuckles
		//knuckles1
		AbstractModel::OBB knuckles1Collider;
		if (activKnuckles1 == 1) {
			glm::mat4 modelmatrixColliderKnuckles1 = glm::mat4(modelMatrixKnuckles1);
			modelmatrixColliderKnuckles1 = glm::rotate(modelmatrixColliderKnuckles1,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles1Collider.u = glm::quat_cast(modelmatrixColliderKnuckles1);
			modelmatrixColliderKnuckles1 = glm::scale(modelmatrixColliderKnuckles1, glm::vec3(0.21, 0.21, 0.21));
			modelmatrixColliderKnuckles1 = glm::translate(modelmatrixColliderKnuckles1,
				glm::vec3(knuckles1ModelAnimate.getObb().c.x,
					knuckles1ModelAnimate.getObb().c.y,
					knuckles1ModelAnimate.getObb().c.z));
			knuckles1Collider.e = knuckles1ModelAnimate.getObb().e * glm::vec3(3.0, 3.0, 1.0);
			knuckles1Collider.c = glm::vec3(modelmatrixColliderKnuckles1[3]);
			addOrUpdateColliders(collidersOBB, "knuckles1", knuckles1Collider, modelMatrixKnuckles1);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles1 = glm::mat4(modelMatrixKnuckles1);
			modelmatrixColliderKnuckles1 = glm::rotate(modelmatrixColliderKnuckles1,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles1Collider.u = glm::quat_cast(modelmatrixColliderKnuckles1);
			modelmatrixColliderKnuckles1 = glm::scale(modelmatrixColliderKnuckles1, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles1 = glm::translate(modelmatrixColliderKnuckles1,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles1Collider.e = knuckles1ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles1Collider.c = glm::vec3(modelmatrixColliderKnuckles1[3]);
			addOrUpdateColliders(collidersOBB, "knuckles1", knuckles1Collider, modelMatrixKnuckles1);
		}
		//knuckles2
		AbstractModel::OBB knuckles2Collider;
		if (activKnuckles2 == 1) {
			glm::mat4 modelmatrixColliderKnuckles2 = glm::mat4(modelMatrixKnuckles2);
			modelmatrixColliderKnuckles2 = glm::rotate(modelmatrixColliderKnuckles2,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles2Collider.u = glm::quat_cast(modelmatrixColliderKnuckles2);
			modelmatrixColliderKnuckles2 = glm::scale(modelmatrixColliderKnuckles2, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles2 = glm::translate(modelmatrixColliderKnuckles2,
				glm::vec3(knuckles2ModelAnimate.getObb().c.x,
					knuckles2ModelAnimate.getObb().c.y,
					knuckles2ModelAnimate.getObb().c.z));
			knuckles2Collider.e = knuckles2ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles2Collider.c = glm::vec3(modelmatrixColliderKnuckles2[3]);
			addOrUpdateColliders(collidersOBB, "knuckles2", knuckles2Collider, modelMatrixKnuckles2);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles2 = glm::mat4(modelMatrixKnuckles2);
			modelmatrixColliderKnuckles2 = glm::rotate(modelmatrixColliderKnuckles2,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles2Collider.u = glm::quat_cast(modelmatrixColliderKnuckles2);
			modelmatrixColliderKnuckles2 = glm::scale(modelmatrixColliderKnuckles2, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles2 = glm::translate(modelmatrixColliderKnuckles2,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles2Collider.e = knuckles2ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles2Collider.c = glm::vec3(modelmatrixColliderKnuckles2[3]);
			addOrUpdateColliders(collidersOBB, "knuckles2", knuckles2Collider, modelMatrixKnuckles2);
		}
		//knuckles3
		AbstractModel::OBB knuckles3Collider;
		if (activKnuckles3 == 1) {
			glm::mat4 modelmatrixColliderKnuckles3 = glm::mat4(modelMatrixKnuckles3);
			modelmatrixColliderKnuckles3 = glm::rotate(modelmatrixColliderKnuckles3,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles3Collider.u = glm::quat_cast(modelmatrixColliderKnuckles3);
			modelmatrixColliderKnuckles3 = glm::scale(modelmatrixColliderKnuckles3, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles3 = glm::translate(modelmatrixColliderKnuckles3,
				glm::vec3(knuckles3ModelAnimate.getObb().c.x,
					knuckles3ModelAnimate.getObb().c.y,
					knuckles3ModelAnimate.getObb().c.z));
			knuckles3Collider.e = knuckles3ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles3Collider.c = glm::vec3(modelmatrixColliderKnuckles3[3]);
			addOrUpdateColliders(collidersOBB, "knuckles3", knuckles3Collider, modelMatrixKnuckles3);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles3 = glm::mat4(modelMatrixKnuckles3);
			modelmatrixColliderKnuckles3 = glm::rotate(modelmatrixColliderKnuckles3,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles3Collider.u = glm::quat_cast(modelmatrixColliderKnuckles3);
			modelmatrixColliderKnuckles3 = glm::scale(modelmatrixColliderKnuckles3, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles3 = glm::translate(modelmatrixColliderKnuckles3,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles3Collider.e = knuckles3ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles3Collider.c = glm::vec3(modelmatrixColliderKnuckles3[3]);
			addOrUpdateColliders(collidersOBB, "knuckles3", knuckles3Collider, modelMatrixKnuckles3);
		}
		//knuckles4
		AbstractModel::OBB knuckles4Collider;
		if (activKnuckles4 == 1) {
			glm::mat4 modelmatrixColliderKnuckles4 = glm::mat4(modelMatrixKnuckles4);
			modelmatrixColliderKnuckles4 = glm::rotate(modelmatrixColliderKnuckles4,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles4Collider.u = glm::quat_cast(modelmatrixColliderKnuckles4);
			modelmatrixColliderKnuckles4 = glm::scale(modelmatrixColliderKnuckles4, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles4 = glm::translate(modelmatrixColliderKnuckles4,
				glm::vec3(knuckles4ModelAnimate.getObb().c.x,
					knuckles4ModelAnimate.getObb().c.y,
					knuckles4ModelAnimate.getObb().c.z));
			knuckles4Collider.e = knuckles4ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles4Collider.c = glm::vec3(modelmatrixColliderKnuckles4[3]);
			addOrUpdateColliders(collidersOBB, "knuckles4", knuckles4Collider, modelMatrixKnuckles4);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles4 = glm::mat4(modelMatrixKnuckles4);
			modelmatrixColliderKnuckles4 = glm::rotate(modelmatrixColliderKnuckles4,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles4Collider.u = glm::quat_cast(modelmatrixColliderKnuckles4);
			modelmatrixColliderKnuckles4 = glm::scale(modelmatrixColliderKnuckles4, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles4 = glm::translate(modelmatrixColliderKnuckles4,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles4Collider.e = knuckles4ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles4Collider.c = glm::vec3(modelmatrixColliderKnuckles4[3]);
			addOrUpdateColliders(collidersOBB, "knuckles4", knuckles4Collider, modelMatrixKnuckles4);
		}
		//knuckles5
		AbstractModel::OBB knuckles5Collider;
		if (activKnuckles5 == 1) {
			glm::mat4 modelmatrixColliderKnuckles5 = glm::mat4(modelMatrixKnuckles5);
			modelmatrixColliderKnuckles5 = glm::rotate(modelmatrixColliderKnuckles5,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles5Collider.u = glm::quat_cast(modelmatrixColliderKnuckles5);
			modelmatrixColliderKnuckles5 = glm::scale(modelmatrixColliderKnuckles5, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles5 = glm::translate(modelmatrixColliderKnuckles5,
				glm::vec3(knuckles5ModelAnimate.getObb().c.x,
					knuckles5ModelAnimate.getObb().c.y,
					knuckles5ModelAnimate.getObb().c.z));
			knuckles5Collider.e = knuckles5ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles5Collider.c = glm::vec3(modelmatrixColliderKnuckles5[3]);
			addOrUpdateColliders(collidersOBB, "knuckles5", knuckles5Collider, modelMatrixKnuckles5);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles5 = glm::mat4(modelMatrixKnuckles5);
			modelmatrixColliderKnuckles5 = glm::rotate(modelmatrixColliderKnuckles5,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles5Collider.u = glm::quat_cast(modelmatrixColliderKnuckles5);
			modelmatrixColliderKnuckles5 = glm::scale(modelmatrixColliderKnuckles5, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles5 = glm::translate(modelmatrixColliderKnuckles5,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles5Collider.e = knuckles5ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles5Collider.c = glm::vec3(modelmatrixColliderKnuckles5[3]);
			addOrUpdateColliders(collidersOBB, "knuckles5", knuckles5Collider, modelMatrixKnuckles5);
		}
		//knuckles6
		AbstractModel::OBB knuckles6Collider;
		if (activKnuckles6 == 1) {
			glm::mat4 modelmatrixColliderKnuckles6 = glm::mat4(modelMatrixKnuckles6);
			modelmatrixColliderKnuckles6 = glm::rotate(modelmatrixColliderKnuckles6,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles6Collider.u = glm::quat_cast(modelmatrixColliderKnuckles6);
			modelmatrixColliderKnuckles6 = glm::scale(modelmatrixColliderKnuckles6, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles6 = glm::translate(modelmatrixColliderKnuckles6,
				glm::vec3(knuckles6ModelAnimate.getObb().c.x,
					knuckles6ModelAnimate.getObb().c.y,
					knuckles6ModelAnimate.getObb().c.z));
			knuckles6Collider.e = knuckles6ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles6Collider.c = glm::vec3(modelmatrixColliderKnuckles6[3]);
			addOrUpdateColliders(collidersOBB, "knuckles6", knuckles6Collider, modelMatrixKnuckles6);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles6 = glm::mat4(modelMatrixKnuckles6);
			modelmatrixColliderKnuckles6 = glm::rotate(modelmatrixColliderKnuckles6,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles6Collider.u = glm::quat_cast(modelmatrixColliderKnuckles6);
			modelmatrixColliderKnuckles6 = glm::scale(modelmatrixColliderKnuckles6, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles6 = glm::translate(modelmatrixColliderKnuckles6,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles6Collider.e = knuckles6ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles6Collider.c = glm::vec3(modelmatrixColliderKnuckles6[3]);
			addOrUpdateColliders(collidersOBB, "knuckles6", knuckles6Collider, modelMatrixKnuckles6);
		}
		//knuckles7
		AbstractModel::OBB knuckles7Collider;
		if (activKnuckles7 == 1) {
			glm::mat4 modelmatrixColliderKnuckles7 = glm::mat4(modelMatrixKnuckles7);
			modelmatrixColliderKnuckles7 = glm::rotate(modelmatrixColliderKnuckles7,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles7Collider.u = glm::quat_cast(modelmatrixColliderKnuckles7);
			modelmatrixColliderKnuckles7 = glm::scale(modelmatrixColliderKnuckles7, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles7 = glm::translate(modelmatrixColliderKnuckles7,
				glm::vec3(knuckles7ModelAnimate.getObb().c.x,
					knuckles7ModelAnimate.getObb().c.y,
					knuckles7ModelAnimate.getObb().c.z));
			knuckles7Collider.e = knuckles7ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles7Collider.c = glm::vec3(modelmatrixColliderKnuckles7[3]);
			addOrUpdateColliders(collidersOBB, "knuckles7", knuckles7Collider, modelMatrixKnuckles7);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles7 = glm::mat4(modelMatrixKnuckles7);
			modelmatrixColliderKnuckles7 = glm::rotate(modelmatrixColliderKnuckles7,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles7Collider.u = glm::quat_cast(modelmatrixColliderKnuckles7);
			modelmatrixColliderKnuckles7 = glm::scale(modelmatrixColliderKnuckles7, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles7 = glm::translate(modelmatrixColliderKnuckles7,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles7Collider.e = knuckles7ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles7Collider.c = glm::vec3(modelmatrixColliderKnuckles7[3]);
			addOrUpdateColliders(collidersOBB, "knuckles7", knuckles7Collider, modelMatrixKnuckles7);
		}
		//knuckles8
		AbstractModel::OBB knuckles8Collider;
		if (activKnuckles8 == 1) {
			glm::mat4 modelmatrixColliderKnuckles8 = glm::mat4(modelMatrixKnuckles8);
			modelmatrixColliderKnuckles8 = glm::rotate(modelmatrixColliderKnuckles8,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles8Collider.u = glm::quat_cast(modelmatrixColliderKnuckles8);
			modelmatrixColliderKnuckles8 = glm::scale(modelmatrixColliderKnuckles8, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles8 = glm::translate(modelmatrixColliderKnuckles8,
				glm::vec3(knuckles8ModelAnimate.getObb().c.x,
					knuckles8ModelAnimate.getObb().c.y,
					knuckles8ModelAnimate.getObb().c.z));
			knuckles8Collider.e = knuckles8ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles8Collider.c = glm::vec3(modelmatrixColliderKnuckles8[3]);
			addOrUpdateColliders(collidersOBB, "knuckles8", knuckles8Collider, modelMatrixKnuckles8);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles8 = glm::mat4(modelMatrixKnuckles8);
			modelmatrixColliderKnuckles8 = glm::rotate(modelmatrixColliderKnuckles8,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles8Collider.u = glm::quat_cast(modelmatrixColliderKnuckles8);
			modelmatrixColliderKnuckles8 = glm::scale(modelmatrixColliderKnuckles8, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles8 = glm::translate(modelmatrixColliderKnuckles8,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles8Collider.e = knuckles8ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles8Collider.c = glm::vec3(modelmatrixColliderKnuckles8[3]);
			addOrUpdateColliders(collidersOBB, "knuckles8", knuckles8Collider, modelMatrixKnuckles8);
		}
		//knuckles9
		AbstractModel::OBB knuckles9Collider;
		if (activKnuckles9 == 1) {
			glm::mat4 modelmatrixColliderKnuckles9 = glm::mat4(modelMatrixKnuckles9);
			modelmatrixColliderKnuckles9 = glm::rotate(modelmatrixColliderKnuckles9,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles9Collider.u = glm::quat_cast(modelmatrixColliderKnuckles9);
			modelmatrixColliderKnuckles9 = glm::scale(modelmatrixColliderKnuckles9, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles9 = glm::translate(modelmatrixColliderKnuckles9,
				glm::vec3(knuckles1ModelAnimate.getObb().c.x,
					knuckles9ModelAnimate.getObb().c.y,
					knuckles9ModelAnimate.getObb().c.z));
			knuckles9Collider.e = knuckles9ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles9Collider.c = glm::vec3(modelmatrixColliderKnuckles9[3]);
			addOrUpdateColliders(collidersOBB, "knuckles9", knuckles9Collider, modelMatrixKnuckles9);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles9 = glm::mat4(modelMatrixKnuckles9);
			modelmatrixColliderKnuckles9 = glm::rotate(modelmatrixColliderKnuckles9,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles9Collider.u = glm::quat_cast(modelmatrixColliderKnuckles9);
			modelmatrixColliderKnuckles9 = glm::scale(modelmatrixColliderKnuckles9, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles9 = glm::translate(modelmatrixColliderKnuckles9,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles9Collider.e = knuckles9ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles9Collider.c = glm::vec3(modelmatrixColliderKnuckles9[3]);
			addOrUpdateColliders(collidersOBB, "knuckles9", knuckles9Collider, modelMatrixKnuckles9);
		}
		//knuckles0
		AbstractModel::OBB knuckles0Collider;
		if (activKnuckles0 == 1) {
			glm::mat4 modelmatrixColliderKnuckles0 = glm::mat4(modelMatrixKnuckles0);
			modelmatrixColliderKnuckles0 = glm::rotate(modelmatrixColliderKnuckles0,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles0Collider.u = glm::quat_cast(modelmatrixColliderKnuckles0);
			modelmatrixColliderKnuckles0 = glm::scale(modelmatrixColliderKnuckles0, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles0 = glm::translate(modelmatrixColliderKnuckles0,
				glm::vec3(knuckles0ModelAnimate.getObb().c.x,
					knuckles0ModelAnimate.getObb().c.y,
					knuckles0ModelAnimate.getObb().c.z));
			knuckles0Collider.e = knuckles0ModelAnimate.getObb().e * glm::vec3(0.21, 0.21, 0.21) * glm::vec3(1.787401574, 1.787401574, 1.787401574);
			knuckles0Collider.c = glm::vec3(modelmatrixColliderKnuckles0[3]);
			addOrUpdateColliders(collidersOBB, "knuckles0", knuckles0Collider, modelMatrixKnuckles0);
		}
		else {
			glm::mat4 modelmatrixColliderKnuckles0 = glm::mat4(modelMatrixKnuckles0);
			modelmatrixColliderKnuckles0 = glm::rotate(modelmatrixColliderKnuckles0,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			knuckles0Collider.u = glm::quat_cast(modelmatrixColliderKnuckles0);
			modelmatrixColliderKnuckles0 = glm::scale(modelmatrixColliderKnuckles0, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderKnuckles0 = glm::translate(modelmatrixColliderKnuckles0,
				glm::vec3(0.0f, 0.0f, -400.0f));
			knuckles0Collider.e = knuckles0ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			knuckles0Collider.c = glm::vec3(modelmatrixColliderKnuckles0[3]);
			addOrUpdateColliders(collidersOBB, "knuckles0", knuckles0Collider, modelMatrixKnuckles0);
		}


		



		//acciones d elos colliders
		//colosion shrek y muegumin
		if (testOBBOBB(shrekCollider, meguminCollider)) {
			if (explo == 0) {
				alSourcePlay(source[3]);
				explo = 1;
			}
			std::cout << "Colision de shrek y megumin" << std::endl;
			controlJugador = 0;
			if(roar==1){
			alSourcePlay(source[4]);
			roar = 2;
			}
			win=1;
		}

		////colision Knuckles1 y megumin
		//if (testOBBOBB(knuckles1Collider, meguminCollider)) {
		//	std::cout << "Colision de knuckles1 y megumin" << std::endl;
		//	activKnuckles1 = 0;
		//	vidas=vidas-1;
		//}
		//colision Knuckles2 y megumin
		if (testOBBOBB(knuckles2Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles2 y megumin" << std::endl;
			activKnuckles2 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles3 y megumin
		if (testOBBOBB(knuckles3Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles3 y megumin" << std::endl;
			activKnuckles3 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles4 y megumin
		if (testOBBOBB(knuckles4Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles4 y megumin" << std::endl;
			activKnuckles4 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles5 y megumin
		if (testOBBOBB(knuckles5Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles5 y megumin" << std::endl;
			activKnuckles5 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles6 y megumin
		if (testOBBOBB(knuckles6Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles6 y megumin" << std::endl;
			activKnuckles6 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles7 y megumin
		if (testOBBOBB(knuckles7Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles7 y megumin" << std::endl;
			activKnuckles7 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles8 y megumin
		if (testOBBOBB(knuckles8Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles8 y megumin" << std::endl;
			activKnuckles8 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles9 y megumin
		if (testOBBOBB(knuckles9Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles9 y megumin" << std::endl;
			activKnuckles9 = 0;
			vidas = vidas - 1;
		}
		//colision Knuckles0 y megumin
		if (testOBBOBB(knuckles0Collider, meguminCollider)) {
			alSourcePlay(source[1]);
			std::cout << "Colision de knuckles0 y megumin" << std::endl;
			activKnuckles0 = 0;
			vidas = vidas - 1;
		}

		//colosiones con la col
		//colision con Knuckles2
		if (testOBBOBB(knuckles1Collider, knuckles2Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles2" << std::endl;
			activKnuckles2 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles3
		if (testOBBOBB(knuckles1Collider, knuckles3Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles3" << std::endl;
			activKnuckles3 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles4
		if (testOBBOBB(knuckles1Collider, knuckles4Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles4" << std::endl;
			activKnuckles4 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles5
		if (testOBBOBB(knuckles1Collider, knuckles5Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles5" << std::endl;
			activKnuckles5 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles6
		if (testOBBOBB(knuckles1Collider, knuckles6Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles6" << std::endl;
			activKnuckles6 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles7
		if (testOBBOBB(knuckles1Collider, knuckles7Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles7" << std::endl;
			activKnuckles7 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles8
		if (testOBBOBB(knuckles1Collider, knuckles8Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles8" << std::endl;
			activKnuckles8 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles9
		if (testOBBOBB(knuckles1Collider, knuckles9Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles9" << std::endl;
			activKnuckles9 = 0;
			puntos = puntos + 1;
		}
		//colision con Knuckles0
		if (testOBBOBB(knuckles1Collider, knuckles0Collider)) {
			alSourcePlay(source[5]);
			std::cout << "Colision de col y Knuckles0" << std::endl;
			activKnuckles0 = 0;
			puntos = puntos + 1;
		}

		

		


		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			//boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			//sphereCollider.render(matrixCollider);
		}

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(mayowCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(mayowCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(mayowCollider.u) * glm::vec4(mayowCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, mayowCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
					&& testOBBOBB(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
					&& testSphereSphereIntersection(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
					std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
			colIt++) {
			std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.find(colIt->first);
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("megumin") == 0)
						modelMatrixMegumin = std::get<1>(jt->second);
				}
			}
		}



		/*******************************************
		 * State machines
		 *******************************************/

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		/*source0Pos[0] = modelMatrixKnuckles2[3].x;
		source0Pos[1] = modelMatrixKnuckles2[3].y;
		source0Pos[2] = modelMatrixKnuckles2[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source2Pos[0] = modelMatrixKnuckles3[3].x;
		source2Pos[1] = modelMatrixKnuckles3[3].y;
		source2Pos[2] = modelMatrixKnuckles3[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		source3Pos[0] = modelMatrixKnuckles4[3].x;
		source3Pos[1] = modelMatrixKnuckles4[3].y;
		source3Pos[2] = modelMatrixKnuckles4[3].z;
		alSourcefv(source[3], AL_POSITION, source3Pos);

		source4Pos[0] = modelMatrixShrek[3].x;
		source4Pos[1] = modelMatrixShrek[3].y;
		source4Pos[2] = modelMatrixShrek[3].z;
		alSourcefv(source[3], AL_POSITION, source4Pos);
*/
		/*source5Pos[0] = modelMatrixShrek[3].x;
		source5Pos[1] = modelMatrixShrek[3].y;
		source5Pos[2] = modelMatrixShrek[3].z;
		alSourcefv(source[3], AL_POSITION, source5Pos);
*/

		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixMegumin[3].x;
		listenerPos[1] = modelMatrixMegumin[3].y;
		listenerPos[2] = modelMatrixMegumin[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixMegumin[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixMegumin[2]);


		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		alListenerfv(AL_ORIENTATION, listenerOri);

		/*for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}*/



	}
}


/********   PREPARESCENE   *********/

void prepareScene() {

	skyboxSphere.setShader(&shaderSkybox);

	terrain.setShader(&shaderTerrain);

	//Megumin
	meguminModelAnimate.setShader(&shaderMulLighting);

	//Shrek
	shrekModelAnimate.setShader(&shaderMulLighting);

	//Ejercito de knuckles
	//Knuckles1
	knuckles1ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles2
	knuckles2ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles3
	knuckles3ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles4
	knuckles4ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles5
	knuckles5ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles6
	knuckles6ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles7
	knuckles7ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles8
	knuckles8ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles9
	knuckles9ModelAnimate.setShader(&shaderMulLighting);
	//Knuckles0
	knuckles0ModelAnimate.setShader(&shaderMulLighting);

	


}


/********   PREPAREDEPTHSCENE   *********/

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	//Megumin
	meguminModelAnimate.setShader(&shaderDepth);

	//Shrek
	shrekModelAnimate.setShader(&shaderDepth);

	//ejercito de knuckles
	//knuckles1
	knuckles1ModelAnimate.setShader(&shaderDepth);
	//knuckles2
	knuckles2ModelAnimate.setShader(&shaderDepth);
	//knuckles3
	knuckles3ModelAnimate.setShader(&shaderDepth);
	//knuckles4
	knuckles4ModelAnimate.setShader(&shaderDepth);
	//knuckles5
	knuckles5ModelAnimate.setShader(&shaderDepth);
	//knuckles6
	knuckles6ModelAnimate.setShader(&shaderDepth);
	//knuckles7
	knuckles7ModelAnimate.setShader(&shaderDepth);
	//knuckles8
	knuckles8ModelAnimate.setShader(&shaderDepth);
	//knuckles9
	knuckles9ModelAnimate.setShader(&shaderDepth);
	//knuckles0
	knuckles0ModelAnimate.setShader(&shaderDepth);

	


}


/********   RENDERSCENE   *********/

void renderScene(bool renderParticles) {
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	 // Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
	modelMatrixMegumin[3][1] = terrain.getHeightTerrain(modelMatrixMegumin[3][0], modelMatrixMegumin[3][2]);
	glm::mat4 modelMatrixMeguminBody = glm::mat4(modelMatrixMegumin);
	modelMatrixMeguminBody = glm::scale(modelMatrixMeguminBody, glm::vec3(0.021, 0.021, 0.021));
	meguminModelAnimate.setAnimationIndex(animationIndex);
	meguminModelAnimate.render(modelMatrixMeguminBody);

	modelMatrixShrek[3][1] = terrain.getHeightTerrain(modelMatrixShrek[3][0], modelMatrixShrek[3][2]);
	glm::mat4 modelMatrixShrekBody = glm::mat4(modelMatrixShrek);
	modelMatrixShrekBody = glm::scale(modelMatrixShrekBody, glm::vec3(0.4, 0.4, 0.4));
	//shrekModelAnimate.setAnimationIndex(animationIndex);
	if (activShrek == 1) {
		shrekModelAnimate.render(modelMatrixShrekBody);
	}

	//knuckles1
	//modelMatrixKnuckles1[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles1[3][0], modelMatrixKnuckles1[3][2]);
	glm::mat4 modelMatrixKnuckles1Body = glm::mat4(modelMatrixKnuckles1);
	modelMatrixKnuckles1Body = glm::scale(modelMatrixKnuckles1Body, glm::vec3(5.0, 5.0, 5.0));
	//Knuckles1ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles1 == 1) {
		knuckles1ModelAnimate.render(modelMatrixKnuckles1Body);
	}
	//knuckles2
	modelMatrixKnuckles2[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles2[3][0], modelMatrixKnuckles2[3][2]);
	glm::mat4 modelMatrixKnuckles2Body = glm::mat4(modelMatrixKnuckles2);
	modelMatrixKnuckles2Body = glm::scale(modelMatrixKnuckles2Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles2ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles2 == 1) {
		knuckles2ModelAnimate.render(modelMatrixKnuckles2Body);
	}
	//knuckles3
	modelMatrixKnuckles3[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles3[3][0], modelMatrixKnuckles3[3][2]);
	glm::mat4 modelMatrixKnuckles3Body = glm::mat4(modelMatrixKnuckles3);
	modelMatrixKnuckles3Body = glm::scale(modelMatrixKnuckles3Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles3ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles3 == 1) {
		knuckles3ModelAnimate.render(modelMatrixKnuckles3Body);
	}
	//knuckles4
	modelMatrixKnuckles4[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles4[3][0], modelMatrixKnuckles4[3][2]);
	glm::mat4 modelMatrixKnuckles4Body = glm::mat4(modelMatrixKnuckles4);
	modelMatrixKnuckles4Body = glm::scale(modelMatrixKnuckles4Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles4ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles4 == 1) {
		knuckles4ModelAnimate.render(modelMatrixKnuckles4Body);
	}
	//knuckles5
	modelMatrixKnuckles5[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles5[3][0], modelMatrixKnuckles5[3][2]);
	glm::mat4 modelMatrixKnuckles5Body = glm::mat4(modelMatrixKnuckles5);
	modelMatrixKnuckles5Body = glm::scale(modelMatrixKnuckles5Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles5ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles5 == 1) {
		knuckles5ModelAnimate.render(modelMatrixKnuckles5Body);
	}
	//knuckles6
	modelMatrixKnuckles6[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles6[3][0], modelMatrixKnuckles6[3][2]);
	glm::mat4 modelMatrixKnuckles6Body = glm::mat4(modelMatrixKnuckles6);
	modelMatrixKnuckles6Body = glm::scale(modelMatrixKnuckles6Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles6ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles6 == 1) {
		knuckles6ModelAnimate.render(modelMatrixKnuckles6Body);
	}
	//knuckles7
	modelMatrixKnuckles7[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles7[3][0], modelMatrixKnuckles7[3][2]);
	glm::mat4 modelMatrixKnuckles7Body = glm::mat4(modelMatrixKnuckles7);
	modelMatrixKnuckles7Body = glm::scale(modelMatrixKnuckles7Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles7ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles7 == 1) {
		knuckles7ModelAnimate.render(modelMatrixKnuckles7Body);
	}
	//knuckles8
	modelMatrixKnuckles8[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles8[3][0], modelMatrixKnuckles8[3][2]);
	glm::mat4 modelMatrixKnuckles8Body = glm::mat4(modelMatrixKnuckles8);
	modelMatrixKnuckles8Body = glm::scale(modelMatrixKnuckles8Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles8ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles8 == 1) {
		knuckles8ModelAnimate.render(modelMatrixKnuckles8Body);
	}
	//knuckles9
	modelMatrixKnuckles9[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles9[3][0], modelMatrixKnuckles9[3][2]);
	glm::mat4 modelMatrixKnuckles9Body = glm::mat4(modelMatrixKnuckles9);
	modelMatrixKnuckles9Body = glm::scale(modelMatrixKnuckles9Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles9ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles9 == 1) {
		knuckles9ModelAnimate.render(modelMatrixKnuckles9Body);
	}
	//knuckles0
	modelMatrixKnuckles0[3][1] = terrain.getHeightTerrain(modelMatrixKnuckles0[3][0], modelMatrixKnuckles0[3][2]);
	glm::mat4 modelMatrixKnuckles0Body = glm::mat4(modelMatrixKnuckles0);
	modelMatrixKnuckles0Body = glm::scale(modelMatrixKnuckles0Body, glm::vec3(0.7, 0.7, 0.7));
	//Knuckles0ModelAnimate.setAnimationIndex(animationIndex);
	if (activKnuckles0 == 1) {
		knuckles0ModelAnimate.render(modelMatrixKnuckles0Body);
	}

	



	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
		  if (renderParticles && it->second.first.compare("fire") == 0) {
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0);
			//modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			if (shrekF == 1) {
				modelFireParticles = glm::translate(modelFireParticles, glm::vec3(-40.0f, 0.05f, -84.0f));
				modelFireParticles = glm::rotate(modelFireParticles, glm::radians(90.0f), glm::vec3(0, 1, 0));
			}
			else {
				modelFireParticles = glm::translate(modelFireParticles, glm::vec3(13.0f, 55.05f, -500.0f));
			}
			modelFireParticles = glm::rotate(modelFireParticles, glm::radians(90.0f), glm::vec3(0, 1, 0));
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

			/****************************+
			 * Open AL sound data
			 */
			/*source1Pos[0] = modelFireParticles[3].x;
			source1Pos[1] = modelFireParticles[3].y;
			source1Pos[2] = modelFireParticles[3].z;
			alSourcefv(source[1], AL_POSITION, source1Pos);*/

			/**********
			 * End Render particles systems
			 */
		}

	}


	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
