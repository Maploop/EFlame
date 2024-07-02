#pragma once

#include "PostProcessManager.h"
#include "SkyBoxManager.h"
#include "ShadowMapHandler.h"

struct GraphicsComponents {
	PostProcessManager* postProcessing = nullptr;
	SkyBoxManager* skyBox = nullptr;
	ShadowMapHandler* shadowMapHandler = nullptr;

	bool directionalLightEnabled = true;
	bool depthVisualizationEnabled = true;
	float fogLevel = 70.0;
	glm::vec3 directinaLightAngle = glm::vec3(1.0, 1.0, 0.0);

	bool phongLightingEnabled = true;
	float gamma = 2.2f;
};