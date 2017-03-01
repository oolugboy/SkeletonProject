#pragma once
#include "Particle.h"
#include <GL/glew.h>
#include <vector>
#include <cmath>

using namespace std;

class SpringDamper
{
public:
	Particle * a;
	Particle * b;
	SpringDamper(Particle * a, Particle * b, float restLength);
	float springConst;
	float dampFact;
	float restLength;	
	void update(float deltaT);
	float getLength(glm::vec3 e, float deltaT);

};

