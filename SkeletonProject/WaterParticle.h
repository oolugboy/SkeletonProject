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
	WaterParticle(glm::vec3 initPosition, Sphere * sphere, float supportRadius, float radius);
	~WaterParticle();
	int index;
	float radius;
	glm::vec3 prevPosition;	
	void update(float deltaT);
	void applyGravity();
	void applyViscosityForce();
	void applyPressureForce();
	void applyRepulsion();
	int handleCollision();
	float kernel(glm::vec3 nPosition, glm::vec3 locPosition);
	float func(float q);
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	float localDensity;
	float pressure;
	float viscosity;
	const float restDensity = 2000.0f;
	float supportRadius;
	float smoothingLength;
	void updateLocalDensity();
	void updatePressure();
	void updateForces();
	void handleImpulse(int collisionType);
	glm::vec3 getPressureGradient();
	glm::vec3 getViscosityGradient();
	glm::vec3 getSpikyKernelGradient(float q, glm::vec3 posDiff);
	float getViscousLagrangian(float q);
	glm::vec3 getKernelGradient(WaterParticle * nBor);
	const float stiffConst = 3.0f;
	const float springConst = 20.0f;	
	const float dampFact = 2.0f;
	vector< WaterParticle *> neighbors;
	Sphere * sphere;
};

