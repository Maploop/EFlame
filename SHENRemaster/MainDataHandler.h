#pragma once

#include "GraphicsComponents.h"
#include "ArchiveHandler.h"
#include "logger.hpp"

class MainDataHandle {
public:
	MainDataHandle(const MainDataHandle&) = default;
	MainDataHandle& operator=(const MainDataHandle&) = default;

	void Initiate() {
		ArchiveHandler temp1("./archive/engine_configuration.archive", true);
		engine_config = temp1;
	}

	void LoadEngineData(GraphicsComponents& graphicsComponents) {
		
	}

	void SaveEngineData(GraphicsComponents& graphicsComponents) {
		
		
	}

	ArchiveHandler engine_config;
};