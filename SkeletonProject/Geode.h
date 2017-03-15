#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
class Geode
{
public:
	Geode();
	~Geode();
	bool wired;
	glm::mat4 modelView;
	glm::mat4 toWorld;
	glm::vec3 position;
	virtual void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection) = 0;
	GLuint VBO, VAO, TBO, EBO, NBO;
	GLuint uProjection, uModelview;
};

