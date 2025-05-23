#include "ArchiveHandler.h"

#include <filesystem>

ArchiveHandler::ArchiveHandler(std::string archiveFilePath, bool createIfNotExists) {

	this->archiveFilePath = archiveFilePath;

	std::ifstream stream(archiveFilePath);
	if (!std::filesystem::exists(archiveFilePath)) {
		data = json({});
		std::ofstream outFile(archiveFilePath);
		outFile << "{}";
		outFile.close();

		SHINFO("ARCHIVE File %s was created now as it was non-existent.", archiveFilePath);
		this->didExist = false;
	}
	else {
		data = json::parse(stream);
		SHINFO("ARCHIVE File %s has been parsed.", archiveFilePath);
		this->didExist = true;
	}
}

void ArchiveHandler::SetBranch(std::string branchName) {
	this->currentBranch = branchName;
}

bool ArchiveHandler::FetchBool(std::string key) {
	if (currentBranch == "N0N3")
		return data[key];
	return data[currentBranch][key];
}

std::string ArchiveHandler::FetchStr(std::string key) {
	if (currentBranch == "N0N3")
		return data[key];
	return data[currentBranch][key];
}

int ArchiveHandler::FetchInt(std::string key) {
	if (currentBranch == "N0N3")
		return data[key];
	return data[currentBranch][key];
}

float ArchiveHandler::FetchFloat(std::string key) {
	if (currentBranch == "N0N3")
		return data[key];
	return data[currentBranch][key];
}

long ArchiveHandler::FetchLong(std::string key) {
	if (currentBranch == "N0N3")
		return data[key];
	return data[currentBranch][key];
}

glm::vec2 ArchiveHandler::GetVec2(std::string key) {
	if (currentBranch == "N0N3") {
		json nonBranchObj = data[key];
		glm::vec2 ourVec(nonBranchObj["x"], nonBranchObj["y"]);
		return ourVec;
	}
	json branchObj = data[currentBranch][key];
	glm::vec2 ourVec(branchObj["x"], branchObj["y"]);
	return ourVec;
}

glm::vec3 ArchiveHandler::GetVec3(std::string key) {
	if (currentBranch == "N0N3") {
		json nonBranchObj = data[key];
		glm::vec3 ourVec(nonBranchObj["x"], nonBranchObj["y"], nonBranchObj["z"]);
		return ourVec;
	}
	json branchObj = data[currentBranch][key];
	glm::vec3 ourVec(branchObj["x"], branchObj["y"], branchObj["z"]);
	return ourVec;
}

glm::vec4 ArchiveHandler::GetVec4(std::string key) {
	if (currentBranch == "N0N3") {
		json nonBranchObj = data[key];
		glm::vec4 ourVec(nonBranchObj["w"], nonBranchObj["x"], nonBranchObj["y"], nonBranchObj["z"]);
		return ourVec;
	}
	json branchObj = data[currentBranch][key];
	glm::vec4 ourVec(branchObj["w"], branchObj["x"], branchObj["y"], branchObj["z"]);
	return ourVec;
}

void ArchiveHandler::SetStr(std::string key, std::string value) {
	if (currentBranch == "N0N3") {
		data[key] = value;
		return;
	}
	data[currentBranch][key] = value;
}

void ArchiveHandler::SetInt(std::string key, int value) {
	if (currentBranch == "N0N3") {
		data[key] = value;
		return;
	}
	data[currentBranch][key] = value;
}

void ArchiveHandler::SetFloat(std::string key, float value) {
	if (currentBranch == "N0N3") {
		data[key] = value;
		return;
	}
	data[currentBranch][key] = value;
}

void ArchiveHandler::SetLong(std::string key, long value) {
	if (currentBranch == "N0N3") {
		data[key] = value;
		return;
	}
	data[currentBranch][key] = value;
}

void ArchiveHandler::SetBool(std::string key, bool value) {
	if (currentBranch == "N0N3") {
		data[key] = value;
		return;
	}
	data[currentBranch][key] = value;
}

void ArchiveHandler::SetVec2(std::string key, glm::vec2 value) {
	if (currentBranch == "N0N3") {
		data[key]["x"] = value.x;
		data[key]["y"] = value.y;
		return;
	}
	data[currentBranch][key]["x"] = value.x;
	data[currentBranch][key]["y"] = value.y;
}

void ArchiveHandler::SetVec3(std::string key, glm::vec3 value) {
	if (currentBranch == "N0N3") {
		data[key]["x"] = value.x;
		data[key]["y"] = value.y;
		data[key]["z"] = value.z;
		return;
	}
	data[currentBranch][key]["x"] = value.x;
	data[currentBranch][key]["y"] = value.y;
	data[currentBranch][key]["z"] = value.z;
}

void ArchiveHandler::SetVec4(std::string key, glm::vec4 value) {
	if (currentBranch == "N0N3") {
		data[key]["w"] = value.w;
		data[key]["x"] = value.x;
		data[key]["y"] = value.y;
		data[key]["z"] = value.z;
		return;
	}
	data[currentBranch][key]["w"] = value.w;
	data[currentBranch][key]["x"] = value.x;
	data[currentBranch][key]["y"] = value.y;
	data[currentBranch][key]["z"] = value.z;
}

void ArchiveHandler::UpdateLocalFile() {
	std::ofstream f(archiveFilePath);
	f << std::setw(8) << data << std::endl;
	f.close();
}
