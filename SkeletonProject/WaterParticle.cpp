#include "WaterParticle.h"
float PI = 3.1415927f;

WaterParticle::WaterParticle(glm::vec3 initPosition, Sphere * sphere, float supportRadius, float radius)
{
	this->sphere = sphere;
	this->prevPosition = this->position = initPosition;
	this->supportRadius = supportRadius;	
	cout << " The support radius is " << this->supportRadius << endl;
	this->smoothingLength = this->supportRadius;
	cout << " The smoothing length is " << smoothingLength << endl;
	localDensity = pressure = 0.0f;
	viscosity = 0.0005f;
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
	if (debug)
	{
		cout << " The force experienced in this frame is";
		printVector(force);
	}
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

void WaterParticle::applyGravity()
{
	glm::vec3 gravity = glm::vec3(0.0f, -5.0f, 0.0f);
	applyForce(gravity);
}
void WaterParticle::updatePressure()
{
	pressure = stiffConst * (localDensity - restDensity);
	if (debug)
	{
		cout << " The update pressure is " << pressure << endl;		
	}

}
glm::vec3 WaterParticle::getSpikyKernelGradient(float q, glm::vec3 posDiff)
{
	if (q >= 0 && q <= 1)
	{
		glm::vec3 leftOp = (-24.0f  * posDiff)/ (PI * pow(smoothingLength, 8));
		if (debug)
		{
			cout << " The presGrad leftOp";
			printVector(leftOp);
		}
		float rightOp = pow(pow(smoothingLength, 2) - pow(getMag(posDiff), 2), 2);
		if(debug)
			cout << " The pressGrad rightOp " << rightOp << endl;
		if (debug)
		{
			cout << " The pressGrad posDiff ";
			printVector(posDiff);
		}
		glm::vec3 res = (leftOp * rightOp);	
		
		if (getMag(res) > 0.001f)
		{
			if (debug)
			{
				cout << " WHAT ARE YOU NOT REDUCING " << endl;
			}
			res = glm::normalize(res) * 0.001f;
		}
		if (debug)
		{
			cout << " The pressure gradient returned is ";
			printVector(res);
		}
		return res;
	}
	if (debug)
	{
		cout << " I guess the pressure gradient is zero then " << endl;
	}
	return glm::vec3(0);
}
void WaterParticle::applyPressureForce()
{
	updatePressure();	

	glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
	int size = neighbors.size();
	for (int i = 0; i < size; i++)
	{
		float leftOp = (pressure + neighbors[i]->pressure) / (2 * neighbors[i]->localDensity);
		if(debug)
			cout << " The pressure leftOp is " << leftOp << endl;
		float q = getMag(position - neighbors[i]->position) / smoothingLength;
		tot = tot + (leftOp * getSpikyKernelGradient(q, position - neighbors[i]->position));
	}
	glm::vec3 res = (-1.0f * mass * tot);
	if (debug)
	{
		cout << " The pressure applied would be ";
		printVector(res);
	}
	applyForce(res);	
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
	if (collisionType == 1) 
	{
		velocity.x = (velocity.x * -1.0f);
	}
	if (collisionType == 2)
	{
		velocity.y = (velocity.y * -1.0f);
	}
	if (collisionType == 3)
	{
		velocity.z = (velocity.z * -1.0f);
	}	
}
float WaterParticle::getViscousLagrangian(float q)
{
	if (q >= 0 && q <= 1)
	{
		float leftOp = 40.0f / (PI * pow(smoothingLength, 4));
		float res = leftOp * (1 - q);
		if (debug)
		{
			cout << " The non zero lagrangian is " << res << endl;
		}
		return res;
	}
	if (debug)
		cout << " I guess returning zero " << endl;
	return 0.0f;
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
		/*if (debug)
		{
			cout << " The neigbors local des"
		}*/
	}
	glm::vec3 res = viscosity * mass * tot;
	if (debug)
	{
		cout << " The viscocity applied would now be ";
		printVector(res);
	}
	applyForce(viscosity * mass * tot);	
}
float WaterParticle::kernel(glm::vec3 nPosition, glm::vec3 locPosition)
{
	float q = getMag(nPosition - locPosition) / smoothingLength;
	if (q >= 0 && q <= 1)
	{
		float r = getMag(nPosition - locPosition);
		float leftOp = 4 / (PI * pow(smoothingLength, 8));
		float rightOp = pow(pow(smoothingLength, 2) - pow(r, 2), 3);
		float res = leftOp * rightOp;
		if (debug)
		{
			cout << " The kernel gradient is " << res << endl;
		}
		return res;
	}
	else
	{
		if (debug)
		{
			cout << " I guess the kernel gradient is zero " << endl;
		}
		return 0;
	}
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
	if (debug)
	{
		cout << " The local density is " << localDensity << endl;
	}
}

WaterParticle::~WaterParticle()
{

}

