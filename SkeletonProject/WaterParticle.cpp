#include "WaterParticle.h"
float PI = 3.1415927f;

WaterParticle::WaterParticle(glm::vec3 initPosition, Sphere * sphere, float supportRadius, float radius)
{
	this->sphere = sphere;
	this->prevPosition = this->position = initPosition;
	this->supportRadius = supportRadius;	
	cout << " The support radius is " << this->supportRadius << endl;
	this->smoothingLength = this->supportRadius / 2.0f;
	cout << " The smoothing length is " << smoothingLength << endl;
	localDensity = pressure = 0.0f;
	viscosity = 0.5f;
	debug = false;
	this->radius = radius;
}
void WaterParticle::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
	sphere->toWorld = glm::translate(glm::mat4(1.0f), position);
	sphere->draw(shaderProgram, view, projection);
}
void WaterParticle::updateForces()
{
	applyGravity();
	//applyPressureForce(); 
	//applyViscosityForce(); 
	applyRepulsion();
}
void WaterParticle::update(float deltaT)
{
	/* Update the position and velocity */
	glm::vec3 acceleration = force / mass;
	//glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = velocity + (deltaT * acceleration);	
	prevPosition = position;
	position = position + (deltaT * velocity);
	if (debug)
	{
		cout << position.x << " " << position.y << " " << position.z << endl;
	}	
}
float WaterParticle::kernel(glm::vec3 nPosition, glm::vec3 locPosition)
{
	float q = getMag(nPosition - locPosition)/ smoothingLength; 
	if (debug)
	{
		cout << " The smoothing length is " << smoothingLength << endl;
	}
	float res = func(q) + (1.0f / pow(smoothingLength, 3.0f));
	if (debug)
		cout << " The kernel is returning " << res << endl;
	return res;
}
float WaterParticle::func(float q)
{
	if (debug)
	{
		cout << " The q passed in is " << q << endl;
	}
	
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
	float res = (3.0f / (2 * PI)) * op2;
	if (debug)
	{
		cout << " The func is returning " << res << endl;
	}
	return res;
}
void WaterParticle::applyGravity()
{
	glm::vec3 gravity = glm::vec3(0.0f, -5.0f, 0.0f);
	applyForce(gravity);
}
void WaterParticle::applyPressureForce()
{
	updatePressure();
	/*glm::vec3 pressureGradient = getPressureGradient();
	if (debug)
	{
		cout << " The pressure gradient ";
		printVector(pressureGradient);
	}
	glm::vec3 pressureForce = -1.0f * (mass / localDensity) * pressureGradient;
	if (debug)
	{
		cout << " The pressure force ";
		printVector(pressureForce);
	} */
	glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
	int size = 0;
	for (int i = 0; i < size; i++)
	{
		float leftOp = (pressure + neighbors[i]->pressure) / (2 * neighbors[i]->localDensity);
		float q = getMag(position - neighbors[i]->position) / smoothingLength;
		tot = tot + (leftOp * getSpikyKernelGradient(q, position - neighbors[i]->position));
	}
	applyForce(-1.0f * mass * tot);
	//applyForce(pressureForce);
}
glm::vec3 WaterParticle::getPressureGradient()
{
	glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
	int size = neighbors.size();
	for (int i = 0; i < size; i++)
	{
		float leftOp = (pressure / pow(localDensity, 2.0f)) + (neighbors[i]->pressure / pow(neighbors[i]->localDensity, 2));
		if (debug)
		{
			cout << " The loop left hop is " << leftOp << endl;
			cout << " The pressure is " << pressure << endl;
		}
		tot = tot + (leftOp * getKernelGradient(neighbors[i]));
		if (debug)
		{
			cout << " The loop tot is ";
			printVector(tot);
		}
	}
	if (debug)
	{
		cout << " The tot is ";
		printVector(tot);
	}
	return localDensity * mass * tot;
}
glm::vec3 WaterParticle::getKernelGradient(WaterParticle * nBor)
{
	float kDiff = kernel(nBor->prevPosition, prevPosition) - kernel(nBor->position, position);
	float xDiff = (abs(nBor->prevPosition.x - prevPosition.x) - abs(nBor->position.x - position.x));
	if (debug)
	{
		cout << " The kDiff is " << kDiff << endl;
		cout << " The xDiff is " << xDiff << endl;
	}

	float yDiff = (abs(nBor->prevPosition.y - prevPosition.y) - abs(nBor->position.y - position.y));
	float zDiff = (abs(nBor->prevPosition.z - prevPosition.z) - abs(nBor->position.z - position.z));

	if (kDiff == 0)
	{
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
	float xVal = kDiff/xDiff, yVal = kDiff/yDiff, zVal = kDiff/zDiff;
	if (xDiff == 0)
	{
		xVal = 0;
	}
	if (yDiff == 0)
	{
		yVal = 0;
	}
	if (zDiff == 0)
	{
		zVal = 0;
	}
	glm::vec3 res = glm::vec3(xVal, yVal, zVal);

	if (debug)
	{
		cout << " The kernel gradient returned is ";
		printVector(res);
	}
	return res;
}
glm::vec3 WaterParticle::getViscosityGradient()
{
	int size = neighbors.size();
	glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < size; i++)
	{
		glm::vec3 leftOp = (velocity - neighbors[i]->velocity) / neighbors[i]->localDensity;	

		glm::vec3 posDiff = (position - neighbors[i]->position);
		glm::vec3 rightOpNum = posDiff * getKernelGradient(neighbors[i]);
		float rightOpDenum = glm::dot(posDiff, posDiff) + (0.01f * pow(smoothingLength, 2));
		glm::vec3 rightOp = rightOpNum / rightOpDenum;	

		tot = tot + (leftOp * rightOp);
	}
	return 2.0f * mass * tot;
}
void WaterParticle::applyRepulsion()
{
	int collisionType = handleCollision();
	if (collisionType != 0)
	{
		if (debug)
		{
			cout << " We collided " << endl;
		}
		handleImpulse(collisionType);
	}
}
int WaterParticle::handleCollision()
{
	float bound = 1.5f;
	bool hit = 0;
	glm::vec3 collisionNormal;
	//The left and right sides 
	if (position.x < (-1.0f * bound) + radius)
	{
		collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
		return 1;
	}
	if (position.x >(bound - radius))
	{
		collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);
		return 1;
	}
	//The top and bottom sides
	if (position.y < (-1.0f * bound) + radius)
	{
		cout << " IN THE RIGHT CASE " << endl;
		collisionNormal = glm::vec3(0.0f, -1.0f, 0.0f);
		return 2;
	}
	if (position.y > (bound - radius))
	{
		collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);
		return 2;
	}
	// The front and back sides 
	if (position.z < (-1.0f * bound) + radius)
	{
		collisionNormal = glm::vec3(0.0f, 0.0f, -1.0f);
		return 3;
	}
	if (position.z >(bound - radius))
	{
		collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		return 3;
	}
	float collisionDepth = glm::dot(position, collisionNormal) - (bound - radius);
	/* First apply repulsion force */
	glm::vec3 resForce = (-1.0f * springConst * collisionDepth * collisionNormal);
	/* Then apply the damping force */
	resForce = resForce + (dampFact * collisionNormal);
	//applyForce(resForce);
	return hit;
}
void WaterParticle::handleImpulse(int collisionType)
{
	cout << " ARE WE BEING CALLED " << collisionType << endl;
	if (collisionType == 1) 
	{
		velocity.x = (velocity.x * -0.5f);
	}
	if (collisionType == 2)
	{
		cout << " WHAAAATTTT !!!" << endl;
		velocity.y = (velocity.y * -35.5f);
	}
	if (collisionType == 3)
	{
		velocity.z = (velocity.z * -0.5f);
	}
	//velocity = velocity * 0.5f;
}
void WaterParticle::applyViscosityForce()
{
	//glm::vec3 viscousForce = mass * viscosity * getViscosityGradient();
	glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
	int size = neighbors.size();
	for (int i = 0; i < size; i++)
	{
		glm::vec3 velDiff = neighbors[i]->velocity - velocity;		
		float q = getMag(position - neighbors[i]->position) / smoothingLength;
		tot = tot + ((velDiff / neighbors[i]->localDensity) * getViscousLagrangian(q));
	}
	applyForce(viscosity * mass * tot);	
}
void WaterParticle::updateLocalDensity()
{
	int size = neighbors.size();
	float tot = 0;
	for (int i = 0; i < size; i++)
	{
		tot += (kernel(neighbors[i]->position, position));
		if(debug)
			cout << " The locDensity loop tot is " << tot << endl;
	}
	localDensity = mass * tot;
	if (debug)
	{
		cout << " The local density is " << localDensity << endl;
	}
}
void WaterParticle::updatePressure()
{
	pressure = stiffConst * (pow(localDensity / restDensity, 7) - 1);
}
glm::vec3 WaterParticle::getSpikyKernelGradient(float q, glm::vec3 posDiff)
{
	float leftOp = -30.0f / (PI * pow(smoothingLength, 4));
	float rightOp = pow(1 - q, 2) / q;
	return leftOp * rightOp * posDiff;
}
float WaterParticle::getViscousLagrangian(float q)
{
	float leftOp = 40.0f / (PI * pow(smoothingLength, 4));
	return leftOp * (1 - q);
}
WaterParticle::~WaterParticle()
{

}

