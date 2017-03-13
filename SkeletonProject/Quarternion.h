#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

class Quarternion
{
public:
	Quarternion();
	glm::vec4 orientation;
	glm::mat4 matrixOrientation;
	void update();
	void toMatrix();
	float getMag(glm::vec4 val);
	glm::vec4 multiply(glm::vec4 orientation1, glm::vec4 orientation2);	
	float dot(Quarternion * other);
	void rotate(float deg, glm::vec3 axis);
	~Quarternion();
};

