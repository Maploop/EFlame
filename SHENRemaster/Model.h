#pragma once

#include <json/json.hpp>
#include "Mesh.h"

using json = nlohmann::json;

class Model {
public:
	Model(const char* file);

	void Render(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	void LoadMesh(unsigned int indMesh);

	void TraverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Get binary data from file
	std::vector<unsigned char> GetData();
	// Interpret data
	std::vector<float> GetFloats(json accessor);
	std::vector<GLuint> GetIndices(json accessor);
	std::vector<Texture> GetTextures();

	// Assemble all floats into Vertex structures
	std::vector<Vertex> AssembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats (helper functions)
	std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec);

	std::string GetFileContents(const char* pathName) const noexcept;
};

