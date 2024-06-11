#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "logger.hpp"
#include "Texture.h"
#include "Camera.h"
#include "Game.h"
#include "VAO.h"
#include "EBO.h"

const int OGL_MJV = 3;
const int OGL_MNV = 3;
const char* APP_VERSION = "1.0-PRI";

const int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;

void imguiRenderpass();
void window_resize_callback(GLFWwindow* window, int width, int height);

Game game = Game();

Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
GLFWwindow* window;

int main() {
	SHEMPTY("");
	SHEMPTY("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	SHEMPTY("");
	SHEMPTY("	EFLAME ENGINE VER. %s", APP_VERSION);
	SHEMPTY("");
	SHEMPTY("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	SHEMPTY("");
	SHEMPTY("Internal Logging Provided By EF Corp.");
	SHEMPTY("If you are seeing this message it means you're looking at internal logs it illegally. Please don't.");
	SHEMPTY("");

	SHINFO("StartupHandler > EFlame Engine version %s internal \"E\" starting...", APP_VERSION);
	if (!glfwInit()) {
		SHFATAL("Failed to initialize GLFW!");
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OGL_MJV);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OGL_MNV);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
		-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
	};

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};



	// TODO -- Fix [window] being NULL apparently?
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "EFlame v1.0", NULL, NULL);
	if (window == NULL) {
		SHFATAL("Failed to create GLFWwindow*");
		glfwTerminate();
		return -1;
	}

	// loading window TB icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("./sh_res/sh_ico/SHEN.jpg", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, window_resize_callback);

	gladLoadGL();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	SHINFO("(CALLBACK) StartupHandler > \"gladLoadGL\" -> OpenGL v%i.%i has been initialized.", OGL_MJV, OGL_MNV);

	Shader coreProgram("./sh_res/sh_shader/Core.vs", "./sh_res/sh_shader/Core.fs");
	Shader lightShader("./sh_res/sh_shader/Light.vs", "./sh_res/sh_shader/Light.fs");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	VAO vao1;
	vao1.Bind();
	VBO vbo1(vertices, sizeof(vertices));
	EBO ebo1(indices, sizeof(indices));

	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*) 0);
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*) (3 * sizeof(float)));
	vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*) (6 * sizeof(float)));
	vao1.LinkAttrib(vbo1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*) (8 * sizeof(float)));
	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 2.0f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	lightShader.SetMat4("model", lightModel);
	lightShader.SetVec4("lightColor", lightColor);

	coreProgram.Activate();
	coreProgram.SetMat4("model", pyramidModel);
	coreProgram.SetVec4("lightColor", lightColor);
	coreProgram.SetVec3("lightPos", lightPos);
	coreProgram.SetVec3("camPos", camera.position);

	// Texture container = Texture("./sh_res/sh_tex/container.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	Texture planks = Texture("./sh_res/sh_tex/planks.png", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture planksSpec = Texture("./sh_res/sh_tex/planksSpec.png", GL_TEXTURE_2D, 2, GL_RED, GL_UNSIGNED_BYTE);

	planks.AssignUnit(coreProgram, "tex00", 1);
	planksSpec.AssignUnit(coreProgram, "tex01", 2);

	glEnable(GL_DEPTH_TEST);

	SHINFO("UIHandler > Initializing ImGui with OpenGL hook...");
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.HandleInput(window);
		camera.UpdateMatrix(70.0f, 0.1f, 100.0f);

		coreProgram.Activate();
		coreProgram.SetVec3("camPos", camera.position);

		camera.Matrix(coreProgram, "camMatrix");

		planks.Bind();
		planksSpec.Bind();
		vao1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		imguiRenderpass();

		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handling game cutscenes etc.
		if (game.IsGameRunning()) {
			game.GameSeq_Update(0.1f);
		}
	}

	SHINFO("ProcessHandler > Shutting down SHEN...");
	vao1.Free();
	vbo1.Free();
	ebo1.Free();
	//container.free();
	planks.Free();
	planksSpec.Free();
	coreProgram.Free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void imguiRenderpass() {
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("General Options");
	ImGui::SliderFloat("Camera Speed", &camera.speed, 0.01f, 1.0f);
	if (ImGui::Button("Start Game Sequence")) {
		SHWARN("(CALLBACK) GameHandler > Tried to call 'startGameSequence' but there is none?! (Check GAME.h)");
		game.lowLevelStartGameSequence();
	}
	ImGui::SameLine();
	if (ImGui::Button("Exit")) {
		glfwSetWindowShouldClose(window, true);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SHINFO("StateHandler > Window resize commencing...");
}