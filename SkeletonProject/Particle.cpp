#include "Particle.h"

Particle::Particle(glm::vec3 initPosition)
{
	position = initPosition;
	velocity = force = glm::vec3(0, 0, 0);
	fixed = false;
}
void Particle::update()
{

}