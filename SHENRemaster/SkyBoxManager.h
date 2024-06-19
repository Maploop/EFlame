#pragma once

#include "Shader.h"
#include "Camera.h"

class SkyBoxManager {
public:
	SkyBoxManager(const char* vertexShader, const char* fragmentShader);

	void InitializeSelf() noexcept;
	void Render(Camera& camera, int width, int height, float fov, float near, float far) noexcept;
	void Free() noexcept;

private:
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;

	Shader skyboxShader;
};

