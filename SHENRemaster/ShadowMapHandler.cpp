#include "ShadowMapHandler.h"

#include "GraphicsComponents.h"

ShadowMapHandler::ShadowMapHandler(const char* vertexShader, const char* fragmentShader) 
	: shadowMapShader(vertexShader, fragmentShader) {

}

void ShadowMapHandler::InitializeSelf(glm::vec3 lightPos) {
	// Framebuffer for Shadow Map
	glGenFramebuffers(1, &shadowMapFBO);

	// Texture for Shadow Map FBO
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
	glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightProjection = orthgonalProjection * lightView;

	shadowMapShader.Activate();
	shadowMapShader.SetMat4("lightProjection", lightProjection);
}

void ShadowMapHandler::PreRender(GraphicsComponents renderOptions) noexcept {
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapHandler::Render(std::map<const char*, Model*> hierarchy, Camera camera) {
	for (std::pair<const char*, Model*> entry : hierarchy) {
		entry.second->Render(this->shadowMapShader, camera);
	}
}