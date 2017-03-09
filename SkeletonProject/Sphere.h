#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <limits>


using namespace std;

class Sphere 
{
public:
	Sphere(float radius);
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	glm::mat4 toWorld;
	GLuint uProjection, uModelview;
	int latNum;
	int longNum;
	float m_radius;
	GLuint VAO, VBO, NBO, EBO;
	vector< glm::vec3 > vertices;
	vector< glm::vec3 > normals;
	vector< int > indices;
	void genVertices();
	void genIndices();
};