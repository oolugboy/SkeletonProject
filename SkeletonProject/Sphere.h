#pragma once
#include <iostream>
#include "Geode.h"
#include <vector>
#include <cmath>
#include <limits>


using namespace std;

class Sphere : public Geode
{
public:
	Sphere(float radius);
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	glm::mat4 toWorld;
	int latNum;
	int longNum;
	float m_radius;	
	vector< glm::vec3 > vertices;
	vector< glm::vec3 > normals;
	vector< int > indices;
	void genVertices();
	void genIndices();
};