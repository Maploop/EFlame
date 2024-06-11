#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera {
public:
	// Main vectors
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 camMatrix = glm::mat4(1.0f);

	// Width and height of the window for the aspect ratio
	int width;
	int height;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true; 

	// Used to adjust speed and sensetivity of the camera
	// TODO: Add a slider in the future ImGui implementation for this option!!
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Initially required camera values
	Camera(int width, int height, glm::vec3 pos);

	// Sends the camera's (view * projection) matrix to the shader
	void Matrix(Shader& shader, const char* uniform);
	void UpdateMatrix(float FOVdeg, float near, float far);
	// Handle movement, looking around, etc.
	void HandleInput(GLFWwindow* window);

protected:
	// This is used for data customization it does not need to be accessed outside of this class!
	float previousSpeed = speed;
};

