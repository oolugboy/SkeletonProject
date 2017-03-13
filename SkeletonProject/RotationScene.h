#pragma once
#include "Cube.h"
#include "Quarternion.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;

class RotationScene
{
public:
	RotationScene();
	vector <Cube *> cubes;
	vector <Quarternion * > orientations;
	void updateOrientations();
	void updateWorldMatrices();
	void initCubes();
	const int numCubes = 6;
	int currCubeInd;
	void nextCubeInd();
	void rotateBoundCube(float deg, glm::vec3 axis);
	void printVector(glm::vec4 val);
	void update();
	void resetSlerpCube();
	void slerpCube(float t);
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	~RotationScene();
};

