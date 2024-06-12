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
#include "Game.h"
#include "Mesh.h"

const int OGL_MJV = 3;
const int OGL_MNV = 3;
const char* APP_VERSION = "1.0-PRI";

const int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;

void imguiRenderpass();
void window_resize_callback(GLFWwindow* window, int width, int height);

Game game = Game();
bool gameUseDirLight = true;
glm::vec3 dirLightAngle = glm::vec3(1.0, 1.0, 0.0);

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
	Vertex vertices[] =
	{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
		Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Vertex lightVertices[] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
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
	SHINFO("StartupHandler > Loading textures...");
	Texture textures[]{
		Texture("./sh_res/sh_tex/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./sh_res/sh_tex/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	SHINFO("(CALLBACK) StartupHandler > \"gladLoadGL\" -> OpenGL v%i.%i has been initialized.", OGL_MJV, OGL_MNV);

	Shader coreProgram("./sh_res/sh_shader/Core.vs", "./sh_res/sh_shader/Core.fs");
	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex);

	Shader lightShader("./sh_res/sh_shader/Light.vs", "./sh_res/sh_shader/Light.fs");
	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);

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
		coreProgram.SetInt("directionalLightEnabled", gameUseDirLight);
		coreProgram.SetVec3("dirLightAngle", dirLightAngle);

		// camera.Matrix(coreProgram, "camMatrix");

		floor.Render(coreProgram, camera);
		light.Render(lightShader, camera);

		imguiRenderpass();

		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handling game cutscenes etc.
		if (game.IsGameRunning()) {
			game.GameSeq_Update(0.1f);
		}
	}

	SHINFO("ProcessHandler > Shutting down SHEN...");
	//container.free();
	coreProgram.Free();
	lightShader.Free();

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

	ImGui::Begin("World Options");
	ImGui::Checkbox("Use Directional Light", &gameUseDirLight);
	ImGui::DragFloat3("DL Angle", glm::value_ptr(dirLightAngle), 0.0, 2.0);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SHINFO("StateHandler > Window resize commencing...");
}