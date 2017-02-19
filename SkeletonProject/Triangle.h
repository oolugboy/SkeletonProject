#pragma once
#include "Particle.h"
#include <GL/glew.h>
#include <vector>

using namespace std;

class Triangle
{
public:
	Particle * a;
	Particle * b;
	Particle * c;
	Triangle(Particle * a, Particle * b, Particle * c);
	~Triangle();
	void loadVertices();
	GLuint VAO, VBO, NBO;
	void update();
	void fillVertsAndNorms();
	GLuint uProjection, uModelview;
	glm::mat4 modelView;
	glm::mat4 toWorld;
	void render();
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	void calcNormal();
	vector< glm::vec3 > vertices;
	vector< glm::vec3 > normals;
	glm::vec3 normal;
};
