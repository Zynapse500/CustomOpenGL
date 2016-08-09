#include <stdio.h>


//OpenGL includes
//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//Soil
#include <SOIL.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



//My includes
#include "Shader.h"
#include "Camera.h"


//INPUT
//Keyboard
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool keys[1024];
void updateMovement();

//Mouse
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
bool firstMouse = true;
GLfloat lastMouseX;
GLfloat lastMouseY;


//window
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
int screen_width, screen_height;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

//DeltaTime
GLfloat deltaTime = 0.0f;
GLfloat lastFrameTime = 0.0f;


//temp
GLfloat rotSpeed = 0.0f;



int main()
{
	printf("Hello World!\n");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	glfwWindowHint(GLFW_SAMPLES, 4);


	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My OpenGL window!", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		getchar();
		return -1;
	}

	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);


	glEnable(GL_MULTISAMPLE);


	GLfloat vertices[] = {
     0.0f,  0.0f, 0.0f, //top
     //front triangle
    -0.5f, -1.0f, 0.5f, 
     0.5f, -1.0f, 0.5f,
     //right
     0.5f, -1.0f,-0.5f,
     //back
    -0.5f, -1.0f,-0.5f,
    //left
    -0.5f, -1.0f, 0.5f, 
};  

	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(GLfloat), (GLvoid*)(0));
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	Shader shader("shaders/vertex.vs", "shaders/fragment.frag");

	projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 100.0f);


	GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
	GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");

	while(!glfwWindowShouldClose(window))
	{
		GLfloat currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		glfwPollEvents();
		updateMovement();

		model = glm::mat4();
		model = glm::rotate(model, (GLfloat)glm::radians(glfwGetTime() * rotSpeed), glm::vec3(1.0f,0.0f,0.0f));
		view = camera.GetViewMatrix();


		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
			shader.Use();

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(action == GLFW_PRESS)
		keys[key] = true;
	else if(action == GLFW_RELEASE)
		keys[key] = false;
}

void updateMovement()
{
	if(keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if(keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if(keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if(keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if(keys[GLFW_KEY_SPACE])
		camera.ProcessKeyboard(UP, deltaTime);
	if(keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard(DOWN, deltaTime);


}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if(firstMouse)
	{
		lastMouseX = xPos;
		lastMouseY = yPos;
		firstMouse = false;
	}

	GLfloat xoffset = xPos - lastMouseX;
	GLfloat yoffset = lastMouseY - yPos;

	lastMouseY = yPos;
	lastMouseX = xPos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}