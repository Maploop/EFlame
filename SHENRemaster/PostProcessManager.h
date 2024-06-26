#pragma once

#include "Shader.h"

struct GraphicsComponents;
class PostProcessManager {
public:
	PostProcessManager(const char* vertexShader, const char* fragmentShader, int sourceW, int sourceH);
	PostProcessManager() = default;

	GLenum InitializeSelf() noexcept;

	void PreProcess() noexcept;
	void PostProcess(GraphicsComponents renderComponents) noexcept;

	void Free() noexcept;

	// Var
	int antiAliasingSampleCount = 8;
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

