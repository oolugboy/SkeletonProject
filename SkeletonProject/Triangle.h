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
	void update(float deltaT);
	void fillVertsAndNorms();
	GLuint uProjection, uModelview;
	glm::mat4 modelView;
	glm::mat4 toWorld;
	void render();
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	void calcNormal();
	float getMag(glm::vec3 val);
	void calcExposedArea(glm::vec3 windVelocity);
	void applyAeroDynForce(glm::vec3 relVelocity);
	vector< glm::vec3 > vertices;
	vector< glm::vec3 > normals;
	float airDensity;
	float dragCoeff;
	float area;
	glm::vec3 normal;
	float prevTime;
};
