#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"
#include "Particle.h"

class WaterParticle : public Particle
{
public:
	WaterParticle(glm::vec3 initPosition, Sphere * sphere, float supportRadius);
	~WaterParticle();
	int index;
	glm::vec3 prevPosition;
	glm::vec3 prevPrevPosition;
	bool visited;
	void update(float deltaT);
	void applyGravity();
	void applyViscocityForce();
	void applyPressureForce();
	float kernel(glm::vec3 nPosition, glm::vec3 locPosition);
	float func(float q);
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	float localDensity;
	float pressure;
	float viscocity;
	const float restDensity = 10.0f;
	float supportRadius;
	float smoothingLength;
	void updateLocalDensity();
	void updatePressure();
	glm::vec3 getPressureGradient();
	glm::vec3 getKernelGradient(WaterParticle * nBor);
	const float stiffConst = 3.0f;
	vector< WaterParticle *> neighbors;
	Sphere * sphere;
};

