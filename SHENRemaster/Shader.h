#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	GLuint ID;

public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void Activate();
	void Free();

	/*
		NOTE -- Utility Methods Beyond Here
	*/

	void SetInt(const char* location, int value);
	void SetFloat(const char* location, int value);
	void SetMat4(const char* location, glm::mat4 value);
	void SetVec4(const char* location, glm::vec4 value);
	void SetVec3(const char* location, glm::vec3 value);

	std::string GetFileContents(const char* pathName) const noexcept;
};

