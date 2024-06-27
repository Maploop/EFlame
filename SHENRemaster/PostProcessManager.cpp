#include "PostProcessManager.h"

#include "logger.hpp"
#include "GraphicsComponents.h"

float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

PostProcessManager::PostProcessManager(const char* vertexShader, const char* fragmentShader, int sourceW, int sourceH)
	: framebufferShader(vertexShader, fragmentShader) {
	framebufferShader.Activate();
	framebufferShader.SetInt("screenTexture", 0);

	PostProcessManager::sourceH = sourceH;
	PostProcessManager::sourceW = sourceW;
}

GLenum PostProcessManager::InitializeSelf() noexcept {
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// This framebuffer will be left alone for Anti Aliasing unfortunately.
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, antiAliasingSampleCount, GL_RGB16F, sourceW, sourceH, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, antiAliasingSampleCount, GL_DEPTH24_STENCIL8, sourceW, sourceH);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	
	// This next one will be used for Post Processing :D
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sourceW, sourceH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	return fboStatus;
}

/*
	Self explainatory, must be run before everything even frame clearing
*/
void PostProcessManager::PreProcess() noexcept {
	// This is for the Multisampling FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

/*
	Keep in mind the method PostProcess() must be called after the frame render is done
*/
void PostProcessManager::PostProcess(GraphicsComponents renderComponents) noexcept {
	// Bind the default framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
	glBlitFramebuffer(0, 0, sourceW, sourceH, 0, 0, sourceW, sourceH, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw the framebuffer rectangle
	framebufferShader.Activate();
	framebufferShader.SetFloat("gamma", renderComponents.gamma);
	glBindVertexArray(rectVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessManager::Free() noexcept {
	framebufferShader.Free();
}
