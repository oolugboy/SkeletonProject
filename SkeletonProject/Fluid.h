#pragma once
#include <vector>
#include "WaterParticle.h"
#include "Cube.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
class Fluid
{
public:
	vector< WaterParticle *> particles;
	void updateNeighbors();
	void updateParticles(float deltaT);
	void update(float deltaT);
	int numParticles;
	void initParticles();
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	Sphere * genSphere;
	Cube * boundCube;
	float supportRadius;
	float smoothingRadius;
	float radius;
	float getMag(glm::vec3 val);
	void zeroOutForces();
	void clearNeighbors();
	void updateLocalDensities();
	void updateParticleForces();
	Fluid();
	~Fluid();
};

