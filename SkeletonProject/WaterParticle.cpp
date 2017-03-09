#include "WaterParticle.h"

WaterParticle::WaterParticle(glm::vec3 initPosition, Sphere * sphere, float supportRadius)
{
	this->sphere = sphere;
	this->prevPosition = this->position = initPosition;
	this->supportRadius = supportRadius;
	this->visited = false;
	this->smoothingLength = this->supportRadius / 2.0f;
	localDensity = pressure = viscocity = 0.0f;
}
void WaterParticle::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
	sphere->toWorld = glm::translate(glm::mat4(1.0f), position);
	sphere->draw(shaderProgram, view, projection);
}
void WaterParticle::update(float deltaT)
{
	/* apply the forces */
	applyGravity();


	/* Update the position and velocity */
	glm::vec3 acceleration = force / mass;
	//glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = velocity + (deltaT * acceleration);
	prevPrevPosition = prevPosition;
	prevPosition = position;
	position = position + (deltaT * velocity);
	visited = true;
}
float WaterParticle::kernel(glm::vec3 nPosition, glm::vec3 locPosition)
{
	float q = getMag(nPosition - locPosition)/ smoothingLength; 
	return func(q) + (1.0f / pow(smoothingLength, 3.0f));
}
float WaterParticle::func(float q)
{
	float pi = 3.1415927f;	
	float op2 = 0.0f;
	if (q >= 0 && q < 1)
	{
		op2 = (2.0f / 3.0f) - pow(q, 2) + (0.5f * (pow(q, 3)));
	}
	else if (q >= 1 && q <= 2)
	{
		op2 = (1.0f / 6.0f) * (pow(2 - q, 3));
	}
	else
	{
		op2 = 0;
	}
	return (3.0f / (2 * pi)) * op2;
}
void WaterParticle::applyGravity()
{
	glm::vec3 gravity = glm::vec3(0.0f, -0.5f, 0.0f);
	applyForce(gravity);
}
void WaterParticle::applyPressureForce()
{
	updatePressure();
	glm::vec3 pressureForce = -1.0f * (mass / localDensity) * getPressureGradient();
	applyForce(pressureForce);
}
glm::vec3 WaterParticle::getPressureGradient()
{
	glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
	int size = neighbors.size();
	for (int i = 0; i < size; i++)
	{
		float leftOp = (pressure / pow(localDensity, 2.0f)) + (neighbors[i]->pressure / pow(neighbors[i]->localDensity, 2));
		tot = tot + (leftOp * getKernelGradient(neighbors[i]));
	}
	return localDensity * mass * tot;
}
glm::vec3 WaterParticle::getKernelGradient(WaterParticle * nBor)
{
	glm::vec3 currNPosition;
	glm::vec3 prevNPosition;

	if (nBor->visited) // A phase ahead 
	{
		currNPosition = nBor->prevPosition;
		prevNPosition = nBor->prevPrevPosition;
	}
	else
	{
		currNPosition = nBor->position;
		prevPosition = nBor->prevPosition;
	}
	float kDiff = kernel(prevNPosition, prevPosition) - kernel(currNPosition, position);
	float xDiff = (abs(prevNPosition.x - prevPosition.x) - abs(currNPosition.x - position.x));
	float yDiff = (abs(prevNPosition.y - prevPosition.y) - abs(currNPosition.y - position.y));
	float zDiff = (abs(prevNPosition.z - prevPosition.z) - abs(currNPosition.z - position.z));

	return glm::vec3(kDiff / xDiff, kDiff / yDiff, kDiff / zDiff);
}
glm::vec3 WaterParticle::getViscocityGradient()
{

}
void WaterParticle::applyViscocityForce()
{

}
void WaterParticle::updateLocalDensity()
{
	int size = neighbors.size();
	float tot = 0;
	for (int i = 0; i < size; i++)
	{
		tot += (kernel(neighbors[i]->position, position));
	}
	localDensity = mass * tot;
}
void WaterParticle::updatePressure()
{
	pressure = stiffConst * (pow(localDensity / restDensity, 7) - 1);
}
WaterParticle::~WaterParticle()
{

}

