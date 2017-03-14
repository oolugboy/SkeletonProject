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
	void update(bool slerp, float t, bool catMull);
	glm::vec4 slerp(glm::vec4 orientation1, glm::vec4 orientation2, float t);
	glm::vec4 catMull(glm::vec4 ori1, glm::vec4 ori2, glm::vec4 ori3, glm::vec4 ori4, float t);
	void handleCatMull(float t);
	void resetSlerpCube();
	void slerpCube(float t);
	void getSlerpInterval(int & startInd, int & endInd, float t);
	void shortenDistance();
	void draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	~RotationScene();
};

