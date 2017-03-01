#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.h"
#include "SpringDamper.h"
#include <vector>
#include <unordered_set>

using namespace std;

class Cloth
{
public:
	vector< unordered_set < Triangle *> > vertTriMap;
	vector< Triangle *> triangles;
	vector< Particle *> particles;
	vector< SpringDamper *> springDampers;
	float div;
	int width;
	int height;
	float springConstant;
	float dampingFactor;
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	Cloth(int width, float springFactor, float dampingFactor);
	void update(float deltaT);
	void updateNormals(float deltaT);
	void moveFixedParticles(glm::vec3 diff);
	~Cloth();
	void initParticles();
	void initTriangles();
	void initSpringDampers();
	void applyGravity();	
	void applyAeroDynForce();
	void zeroOutForces();
	glm::vec3 windVelocity;
	glm::vec3 gravity;
	void updateSpringDampers(float deltaT);
	void updateParticles(float deltaT);
	void adjustWindSpeed(bool incr);	
	const float windMax = 90.0f;
	const float windMin = 0.0f;
	const float windOff = 5.0f;
	float getMag(glm::vec3 val);
};