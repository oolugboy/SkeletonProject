#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Geode.h"

class Cube: public Geode
{
public:
	Cube(bool wired, glm::vec3 position);
	void loadVertices();
	GLuint vertNum = 36;
	const glm::vec3 vertices[36] = {
		//Front
		glm::vec3(-1.5f, -1.5f, -1.5f),
		glm::vec3(-1.5f,  1.5f, -1.5f),
		glm::vec3(1.5f,  1.5f, -1.5f),
		glm::vec3(1.5f,  1.5f, -1.5f),
		glm::vec3(1.5f, -1.5f, -1.5f),
		glm::vec3(-1.5f, -1.5f, -1.5f),

		//Back
		glm::vec3(1.5f, -1.5f,  1.5f),
		glm::vec3(1.5f, 1.5f,  1.5f),
		glm::vec3(-1.5f,  1.5f,  1.5f),
		glm::vec3(-1.5f,  1.5f,  1.5f),
		glm::vec3(-1.5f,  -1.5f,  1.5f),
		glm::vec3(1.5f, -1.5f,  1.5f),

		//Left
		glm::vec3(-1.5f, -1.5f, 1.5f),
		glm::vec3(-1.5f, 1.5f,  1.5f),
		glm::vec3(-1.5f,  1.5f,  -1.5f),
		glm::vec3(-1.5f,  1.5f,  -1.5f),
		glm::vec3(-1.5f,  -1.5f, -1.5f),
		glm::vec3(-1.5f, -1.5f, 1.5f),

		//Right
		glm::vec3(1.5f, -1.5f, -1.5f),
		glm::vec3(1.5f,  1.5f, -1.5f),
		glm::vec3(1.5f,  1.5f,  1.5f),
		glm::vec3(1.5f,  1.5f,  1.5f),
		glm::vec3(1.5f, -1.5f,  1.5f),
		glm::vec3(1.5f, -1.5f, -1.5f),

		//Bottom
		glm::vec3(-1.5f, -1.5f, -1.5f),
		glm::vec3(1.5f, -1.5f, -1.5f),
		glm::vec3(1.5f, -1.5f,  1.5f),
		glm::vec3(1.5f, -1.5f,  1.5f),
		glm::vec3(-1.5f, -1.5f,  1.5f),
		glm::vec3(-1.5f, -1.5f, -1.5f),

		//Top
		glm::vec3(1.5f,  1.5f,  1.5f),
		glm::vec3(1.5f,  1.5f, -1.5f),
		glm::vec3(-1.5f,  1.5f, -1.5f),
		glm::vec3(-1.5f,  1.5f, -1.5f),
		glm::vec3(-1.5f,  1.5f,  1.5f),
		glm::vec3(1.5f,  1.5f,  1.5f)
	};
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	~Cube();
};

