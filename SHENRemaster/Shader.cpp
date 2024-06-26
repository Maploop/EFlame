#include "Shader.h"
#include "logger.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	SHINFO("ShaderHandler > Loading in Shader '%s'...", vertexPath);

	std::string vert = GetFileContents(vertexPath);
	std::string frag = GetFileContents(fragmentPath);

	int success;
	char infoLog[512];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto c = vert.c_str();
	glShaderSource(vertexShader, 1, &c, NULL);
	glCompileShader(vertexShader);

	// Check if VERTEX shader is compiled correctly
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
		SHERROR("ShaderHandler > Internal error while compiling vertex shader '%s' :: %s", vertexPath, infoLog);
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	auto c1 = frag.c_str();
	glShaderSource(fragmentShader, 1, &c1, NULL);
	glCompileShader(fragmentShader);
	// Check if VERTEX shader is compiled correctly
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
		SHERROR("ShaderHandler > Internal error while compiling fragment shader '%s' :: %s", fragmentPath, infoLog);
		return;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// Check if VERTEX shader is compiled correctly
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, 0, infoLog);
		SHERROR("ShaderHandler > Program LINK error :: %s", infoLog);
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Free() {
	glDeleteProgram(ID);
	SHINFO("MemoryHandler > Freed Shader with program ID '%i'", ID);
}

void Shader::SetInt(const char* location, int value) {
	glUniform1i(glGetUniformLocation(ID, location), value);
}

void Shader::SetFloat(const char* location, int value) {
	glUniform1f(glGetUniformLocation(ID, location), value);
}

void Shader::SetMat4(const char* location, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, location), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec4(const char* location, glm::vec4 value) {
	glUniform4fv(glGetUniformLocation(ID, location), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const char* location, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(ID, location), 1, glm::value_ptr(value));
}

std::string Shader::GetFileContents(const char* pathName) const noexcept {
	std::ifstream in(pathName, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}