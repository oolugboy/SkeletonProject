#include "RotationScene.h"
#include <iostream>
RotationScene::RotationScene()
{
	/* Initialize the cubes */
	initCubes();
	currCubeInd = 0;
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
void RotationScene::slerpCube(float t)
{
	int startInd = 0, endInd = 0;
	if (t >= 0 && t <= 1)
	{
		startInd = 0;
		endInd = 1;
	}
	if (t >= 1 && t <= 2)
	{
		startInd = 1;
		endInd = 2;
	}
	if (t >= 2 && t <= 3)
	{
		startInd = 2;
		endInd = 3;
	}
	if (t >= 3 && t <= 4)
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
}
void RotationScene::rotateBoundCube(float deg, glm::vec3 axis)
{
	cout << " The quarternion before ";
	printVector(orientations[currCubeInd]->orientation);
	orientations[currCubeInd]->rotate(deg, axis);
	cout << " The quarternion after ";
	printVector(orientations[currCubeInd]->orientation);
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
	for (int i = 0; i < (numCubes - 1); i++)
	{
		orientations[i]->update();
	}
}
void RotationScene::updateWorldMatrices()
{
	for (int i = 0; i < (numCubes - 1); i++)
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
void RotationScene::update()
{
	updateOrientations();
	updateWorldMatrices();
}
RotationScene::~RotationScene()
{
}
