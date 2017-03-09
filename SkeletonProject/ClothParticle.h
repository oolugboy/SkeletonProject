#pragma once
#include "Particle.h"
class ClothParticle : public Particle
{
public:
	ClothParticle(glm::vec3 initPosition);
	void update(float deltaT);
	~ClothParticle();	
};

