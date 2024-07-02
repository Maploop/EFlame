#pragma once

#include "Shader.h"
#include "glm/glm.hpp"
#include <map>
#include "Model.h"

struct GraphicsComponents;
class ShadowMapHandler {
public:
	ShadowMapHandler(const char* vertexShader, const char* fragmentShader);
	ShadowMapHandler() = default;

	void InitializeSelf(glm::vec3 lightPos);

	void PreRender(GraphicsComponents renderOptions) noexcept;
	void Render(std::map<const char*, Model*> hierarchy, Camera camera);

	// Var
	glm::mat4 lightProjection;
	unsigned int shadowMap;
private:
	Shader shadowMapShader;

	unsigned int shadowMapFBO;
	unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
};

