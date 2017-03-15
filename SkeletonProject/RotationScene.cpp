#include "RotationScene.h"
#include <iostream>
#define PI 3.14159265
RotationScene::RotationScene()
{
	/* Initialize the cubes */
	initCubes();
	currCubeInd = 0;
}

void RotationScene::shortenDistance()
{
	/*for (int i = 0; i < numCubes - 2; i++)
	{
		cout << " The dot is " << orientations[i]->dot(orientations[i + 1]) << endl;
		if (orientations[i]->dot(orientations[i + 1]) < 0)
		{
			cout << " WE HAVE A LONG WAY FOUND " << endl;
			orientations[i + 1]->orientation = (-1.0f * orientations[i + 1]->orientation);
		} 
	}*/
}
void RotationScene::initCubes()
{
	float xOffset = -1.5f;
	float yOffset = 0.0f;
	/* Instantiate the cubes */
	for (int i = 0; i < numCubes; i++)
	{
		bool  wired = true;
		if (i == (numCubes - 1))
			wired = false;
		Cube * newCube = new Cube(wired, positions[i]);		
		/* Scale the cube */
		newCube->toWorld = newCube->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
		cubes.push_back(newCube);
		/* Also store its orientation */
		orientations.push_back(new Quarternion());
	}	
}
glm::vec4 RotationScene::catMull(glm::vec4 ori1, glm::vec4 ori2, glm::vec4 ori3, glm::vec4 ori4, float t)
{
	glm::vec4 ori12 = slerp(ori1, ori2, t + 1.0f);
	glm::vec4 ori23 = slerp(ori2, ori3, t);
	glm::vec4 ori34 = slerp(ori3, ori4, t - 1.0f);

	glm::vec4 ori123 = slerp(ori12, ori23, (t + 1.0f) / 2.0f);
	glm::vec4 ori234 = slerp(ori23, ori34, t / 2.0f);
	
	return slerp(ori123, ori234, t);
}
void RotationScene::handleCatMull(float t)
{
	int startInd = 0, endInd = 0;
	getSlerpInterval(startInd, endInd, t);

	cout << " The interval " << startInd << " : " << endInd << " : " << t << endl;

	glm::vec4 ori1;	
	glm::vec4 ori2 = orientations[startInd]->orientation;
	glm::vec4 ori3 = orientations[endInd]->orientation;
	glm::vec4 ori4;

	if (startInd != 0)
	{
		ori1 = orientations[startInd - 1]->orientation;
	}
	else
	{
		ori1 = ori2;
	}
	if (endInd != 4)
	{
		ori4 = orientations[endInd + 1]->orientation;
	}
	else
	{
		ori4 = ori3;
	}
	/* Clamp between 0 and 1 */
	t = t - (float)startInd;
	orientations[5]->orientation = catMull(ori1, ori2, ori3, ori4, t);
}
void RotationScene::slerpCube(float t)
{
	int startInd = 0, endInd = 0;
	getSlerpInterval(startInd, endInd, t);

	glm::vec4 orientation1 = orientations[startInd]->orientation;
	glm::vec4 orientation2 = orientations[endInd]->orientation;
	
	/* Clamp between 0 and 1 */
	t = t - (float)startInd;
	orientations[5]->orientation = slerp(orientation1, orientation2, t); 
}
glm::vec4 RotationScene::slerp(glm::vec4 orientation1, glm::vec4 orientation2, float t)
{
	/* First check the clamp cases */
	/*if (t < 0.0f)
	{
		return orientation1;
	} */
	if ((float)glm::dot(orientation1, orientation2) >= 1.0f) // Same vector 
	{
		cout << " Caught a 1!!" << endl;
		return orientation1;
	} 
	cout << " The orientation 1 is ";
	printVector(orientation1);
	cout << " The orientation 2 is ";
	printVector(orientation2);
	cout << " The dot is " << (float)glm::dot(orientation1, orientation2) << endl;
	float rad = acos((float)glm::dot(orientation1, orientation2));	
	cout << " The rad is " << rad << endl;
	glm::vec4 leftOp = (sin((1.0f - t) * rad) / sin(rad)) * orientation1;
	cout << " The leftOp is ";
	printVector(leftOp);

	glm::vec4 rightOp = (sin(t * rad) / sin(rad)) * orientation2;
	cout << " The rightOp is ";
	printVector(rightOp);
	
	return leftOp + rightOp;
}
void RotationScene::lerpPos(float t)
{
	int startInd = 0, endInd = 0;
	getSlerpInterval(startInd, endInd, t);
	t = t - (float)startInd;
	cubes[5]->position =  ((1.0f - t) * cubes[startInd]->position) + ((t) * cubes[endInd]->position);	
}
void RotationScene::getSlerpInterval(int & startInd, int & endInd, float t)
{
	if (t >= 0 && t <= 1)
	{
		startInd = 0;
		endInd = 1;
	}
	if (t > 1 && t <= 2)
	{
		startInd = 1;
		endInd = 2;
	}
	if (t > 2 && t <= 3)
	{
		startInd = 2;
		endInd = 3;
	}
	if (t > 3 && t <= 4)
	{
		startInd = 3;
		endInd = 4;
	}
}
float RotationScene::getMag(glm::vec4 val)
{
	return sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2) + pow(val.a, 2));
}
void RotationScene::resetSlerpCube()
{
	glm::vec3 position = glm::vec3(cubes[5]->toWorld[3]);
	cubes[5]->toWorld = glm::translate(glm::mat4(1.0f), position);
	cubes[5]->toWorld = cubes[5]->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	orientations[5]->orientation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
void RotationScene::rotateBoundCube(float deg, glm::vec3 axis)
{
	orientations[currCubeInd]->rotate(deg, axis);
	shortenDistance();
}
void RotationScene::nextCubeInd()
{
	currCubeInd = (currCubeInd + 1) % 5;
}
void RotationScene::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
	for (int i = 0; i < numCubes; i++)
	{
		cubes[i]->draw(shaderProgram, view, projection);
	}
}
void RotationScene::updateOrientations()
{
	for (int i = 0; i < numCubes; i++)
	{
		orientations[i]->update();
	}
}
void RotationScene::updateWorldMatrices()
{
	for (int i = 0; i < numCubes; i++)
	{		
		cubes[i]->toWorld = glm::translate(glm::mat4(1.0f), cubes[i]->position);
		cubes[i]->toWorld = cubes[i]->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
		cubes[i]->toWorld = cubes[i]->toWorld * orientations[i]->matrixOrientation;
	}
}
void RotationScene::printVector(glm::vec4 val)
{
	cout << val.x << " " << val.y << " " << val.z << " " << val.a << endl;
}
void RotationScene::update(bool interp, float t, bool catMull)
{
	if (interp)
	{
		lerpPos(t);
		if (catMull)
		{
			handleCatMull(t);
		}
		else
		{
			slerpCube(t);
		}
	}
	updateOrientations();
	updateWorldMatrices();
}
RotationScene::~RotationScene()
{
}
