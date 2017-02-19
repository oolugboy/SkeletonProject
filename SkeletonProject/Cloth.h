#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.h"
#include <vector>
#include <unordered_set>

using namespace std;

class Cloth
{
public:
	vector< unordered_set < Triangle *> > vertTriMap;
	vector< Triangle *> triangles;
	vector< Particle *> particles;
	int width;
	int height;
	float springConstant;
	float dampingFactor;
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	Cloth(int width, float springFactor, float dampingFactor);
	void update();
	void updateNormals();
	~Cloth();
	void initParticles();
	void initTriangles();
};