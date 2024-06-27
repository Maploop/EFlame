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

#include <map>

#include "logger.hpp"
#include "Game.h"
#include "Model.h"
#include "SkyBoxManager.h"
#include "PostProcessManager.h"

const int OGL_MJV = 3;
const int OGL_MNV = 3;
const char* APP_VERSION = "1.0-PRI";

const int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;

void imguiRenderpass();
void window_resize_callback(GLFWwindow* window, int width, int height);

static std::map<const char*, Model*> hierarchy;

Game game = Game();

struct GraphicsComponents {
	PostProcessManager* postProcessing = nullptr;
	SkyBoxManager* skyBox = nullptr;

	bool directionalLightEnabled = true;
	bool depthVisualizationEnabled = true;
	float fogLevel = 70.0;
	glm::vec3 directinaLightAngle = glm::vec3(1.0, 1.0, 0.0);
};

GraphicsComponents graphicsComponents;
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
	glfwWindowHint(GLFW_SAMPLES, 8);

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
	Shader skyboxShader("./sh_res/sh_shader/Skybox.vs", "./sh_res/sh_shader/Skybox.fs");
	skyboxShader.Activate();
	skyboxShader.SetInt("skybox", 0);
	
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	coreProgram.Activate();
	coreProgram.SetVec4("lightColor", lightColor);
	coreProgram.SetVec3("lightPos", lightPos);

	


	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDepthFunc(GL_LESS);

	// Post processing
	SHINFO("PostProcessing > Initializing PostProcessManager...");
	PostProcessManager temp1 = PostProcessManager("./sh_res/sh_shader/Framebuffer.vs", "./sh_res/sh_shader/Framebuffer.fs", SCR_WIDTH, SCR_HEIGHT);
	graphicsComponents.postProcessing = &temp1;
	auto fboStatus = graphicsComponents.postProcessing->InitializeSelf();
	SHDEBUG("nick gfa");
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		SHERROR("PostProcessing > Framebuffer error:: %s", fboStatus);
	
	SHINFO("PostProcessing > PostProcessManager initialized successfully!");
	// End post processing

	SHINFO("UIHandler > Initializing ImGui with OpenGL hook...");
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	Model sword("./sh_res/sh_3d/sword/scene.gltf");
	Model tomb("./sh_res/sh_3d/tomb/scene.gltf");
	Model statue("./sh_res/sh_3d/statue/scene.gltf");
	Model ground("./sh_res/sh_3d/ground/scene.gltf");
	Model grass("./sh_res/sh_3d/grass/scene.gltf");
	hierarchy.insert({"Sword", &sword});
	hierarchy.insert({"Tomb", &tomb});
	hierarchy.insert({"Statue", &statue});
	hierarchy.insert({"Grass", &grass});
	hierarchy.insert({"Ground", &ground});

	SHINFO("PostProcessing > Skybox shader is loading...");
	SkyBoxManager temp = SkyBoxManager("./sh_res/sh_shader/Skybox.vs", "./sh_res/sh_shader/Skybox.fs");
	graphicsComponents.skyBox = &temp;
	graphicsComponents.skyBox->InitializeSelf();
	SHINFO("PostProcessing > Skybox shader is done loading!");
	
	// glfwMaximizeWindow(window);
	while (!glfwWindowShouldClose(window)) {
		// PreP
		graphicsComponents.postProcessing->PreProcess();

		glClearColor(0.85f, 0.85f, 0.90f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.HandleInput(window);
		camera.UpdateMatrix(70.0f, 0.1f, 100.0f);

		glEnable(GL_DEPTH_TEST);

		sword.Render(coreProgram, camera);
		tomb.Render(coreProgram, camera);
		statue.Render(coreProgram, camera);
		//grass.Render(coreProgram, camera);
		//ground.Render(coreProgram, camera);

		coreProgram.Activate();
		coreProgram.SetInt("directionalLightEnabled", graphicsComponents.directionalLightEnabled);
		coreProgram.SetInt("depthVisual", graphicsComponents.depthVisualizationEnabled);
		coreProgram.SetFloat("fogLevel", 100.0f - graphicsComponents.fogLevel);
		coreProgram.SetVec3("dirLightAngle", graphicsComponents.directinaLightAngle);

		// PP
		graphicsComponents.skyBox->Render(camera, SCR_WIDTH, SCR_HEIGHT, 70.0f, 0.1f, 100.0f);

		graphicsComponents.postProcessing->PostProcess();

		imguiRenderpass();

		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handling game cutscenes etc.
		if (game.IsGameRunning()) {
			game.GameSeq_Update(0.1f);
		}
	}

	SHINFO("ProcessHandler > Shutting down SHEN...");
	coreProgram.Free();
	skyboxShader.Free();
	graphicsComponents.skyBox->Free();
	graphicsComponents.postProcessing->Free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

glm::vec3 pos = glm::vec3(1.0, 1.0, 1.0);

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

	ImGui::Begin("Graphics Options");
	ImGui::SliderInt("Anti-Aliasing Samples", &graphicsComponents.postProcessing->antiAliasingSampleCount, 2, 16);
	ImGui::End();

	ImGui::Begin("World Options");
	ImGui::Checkbox("Use Directional Light", &graphicsComponents.directionalLightEnabled);
	ImGui::DragFloat3("DL Angle", glm::value_ptr(graphicsComponents.directinaLightAngle), 0.0, 2.0);
	ImGui::Text("Render");
	ImGui::Checkbox("Placebo Fog", &graphicsComponents.depthVisualizationEnabled);
	ImGui::SliderFloat("Fog Level", &graphicsComponents.fogLevel, 0.0f, 100.0f);
	ImGui::End();

	ImGui::Begin("World Hierarchy");
	std::string ec = "Entity Count: " + std::to_string(hierarchy.size());
	ImGui::Text(ec.c_str());

	for (std::pair<const char*, Model*> entry : hierarchy) { 
		if (ImGui::TreeNode(entry.first)) {
			Model* entity = entry.second;
			std::string dat = "Texture Count: " + std::to_string(entity->loadedTex.size());
			ImGui::Text(dat.c_str());
			ImGui::DragFloat3("Position", glm::value_ptr(pos));
			ImGui::TreePop();
		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SHINFO("StateHandler > Window resize commencing...");
}