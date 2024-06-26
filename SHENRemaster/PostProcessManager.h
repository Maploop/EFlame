#pragma once

#include "Shader.h"

class PostProcessManager {
public:
	PostProcessManager(const char* vertexShader, const char* fragmentShader, int sourceW, int sourceH);

	GLenum InitializeSelf() noexcept;

	void PreProcess() noexcept;
	void PostProcess() noexcept;

	void Free() noexcept;
private:
	unsigned int rectVAO, rectVBO;
	unsigned int FBO;
	unsigned int framebufferTexture;
	unsigned int postProcessingFBO;
	unsigned int postProcessingTexture;
	unsigned int RBO;

	int sourceW, sourceH;

	Shader framebufferShader;
};

