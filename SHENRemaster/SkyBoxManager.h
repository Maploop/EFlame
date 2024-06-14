#pragma once

#include "Shader.h"
#include "Camera.h"

class SkyBoxManager {
public:
	SkyBoxManager(const char* vertexShader, const char* fragmentShader, int width, int height, float fov, float near, float far, Camera camera);

	void InitializeSelf() noexcept;
	void Render() noexcept;
	void Free() noexcept;

private:
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;

	int widthS;
	int heightS;
	float fov;
	float near;
	float far;
	Camera camera;

	Shader skyboxShader;
};

