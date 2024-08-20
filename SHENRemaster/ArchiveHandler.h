#pragma once

#include <json/json.hpp>
#include <string>
#include <iostream>
#include <fstream>

#include "logger.hpp"
#include <glm/glm.hpp>

using json = nlohmann::json;

class ArchiveHandler {
public:
	ArchiveHandler(std::string archiveFilePath, bool createIfNotExists);

	void SetBranch(std::string branchName);

	bool FetchBool(std::string key);
	std::string FetchStr(std::string key);
	int FetchInt(std::string key);
	float FetchFloat(std::string key);
	long FetchLong(std::string key);
	glm::vec2 GetVec2(std::string key);
	glm::vec3 GetVec3(std::string key);
	glm::vec4 GetVec4(std::string key);

	void SetStr(std::string key, std::string value);
	void SetInt(std::string key, int value);
	void SetFloat(std::string key, float value);
	void SetLong(std::string key, long value);
	void SetBool(std::string key, bool value);
	void SetVec2(std::string key, glm::vec2 value);
	void SetVec3(std::string key, glm::vec3 value);
	void SetVec4(std::string key, glm::vec4 value);

	void UpdateLocalFile();

	bool ShouldLoadDefault() { return !didExist; };

private:
	std::string archiveFilePath;
	std::string currentBranch = "N0N3";
	bool didExist;
	json data;
};

