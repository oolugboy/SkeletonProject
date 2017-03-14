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
	for (int i = 0; i < numCubes - 2; i++)
	{
		cout << " The dot is " << orientations[i]->dot(orientations[i + 1]) << endl;
		if (orientations[i]->dot(orientations[i + 1]) < 0)
		{
			cout << " WE HAVE A LONG WAY FOUND " << endl;
			orientations[i + 1]->orientation = (-1.0f * orientations[i + 1]->orientation);
		} 
	}
}
void RotationScene::initCubes()
{
	float xOffset = -1.5f;
	float yOffset = 0.0f;
	/* Instantiate the cubes */
	for (int i = 0; i < numCubes; i++)
	{
		Cube * newCube = new Cube();
		if (i == (numCubes - 1)) // Do the offset for the interp cube 
			yOffset = -1.5f;
		/* Position the cube */
		newCube->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(xOffset, yOffset, 0.0f));
		/* Scale the cube */
		newCube->toWorld = newCube->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
		cubes.push_back(newCube);
		xOffset += 0.8f;

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
	float rad = acos(glm::dot(orientation1, orientation2));	

	glm::vec4 leftOp = (sin((1 - t) * rad) / sin(rad)) * orientation1;

	glm::vec4 rightOp = (sin(t * rad) / sin(rad)) * orientation2;

	return leftOp + rightOp;
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
		glm::vec3 position = glm::vec3(cubes[i]->toWorld[3]);
		cubes[i]->toWorld = glm::translate(glm::mat4(1.0f), position);
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
