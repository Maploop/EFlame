#pragma once

#include <json/json.hpp>
#include "Mesh.h"
#include "PhysicsHandler.h"

using json = nlohmann::json;

class Model {
public:
	Model(const char* file);
	Model(const char* file, Texture* overrideDiff, Texture* overrideMet);

	void updateTransform();
	void Render(Shader& shader, Camera& camera);

	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void Scale(glm::vec3 scale);
	void Scale(float scale);

	// Level design function, should not be used in debug/testing scenarios
	void SetStatic(bool cached);

	void SetOverrideTextureDiffuse(Texture* tex);
	void SetOverrideTextureMetallic(Texture* tex);
	
	void SetUseOverrideTextures(bool flag) { useOverrideTextures = flag;  };

	// void setPhysicsActor(physx::PxRigidActor* actor) { physicsActor = actor; };

	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 rotation;
	glm::vec3 rotationDegrees;
	glm::vec3 scale = glm::vec3(1.0);

	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	// physx::PxRigidActor* physicsActor;
private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	bool useOverrideTextures = false;
	Texture* overrideDiffuseTex;
	Texture* overrideMetallicTex;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

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

	// Cache related
	bool constantlyUpdated = true;
	bool isStatic = false;
};

