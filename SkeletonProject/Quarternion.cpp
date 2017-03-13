#include "Quarternion.h"
#include <iostream>
#include <cmath>
#define PI 3.14159265
Quarternion::Quarternion()
{
	orientation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	matrixOrientation = glm::mat4(1.0f);
}
void Quarternion::rotate(float deg, glm::vec3 axis)
{
	cout << " rotating " << endl;
	float rad = deg * (PI / 180.0f);
	glm::vec4 rotOrientation = glm::vec4(cos(rad / 2.0f), axis.x * sin(rad / 2.0f), axis.y * sin(rad / 2.0f), axis.z * sin(rad / 2.0f));
	orientation = multiply(rotOrientation, orientation);

}
glm::vec4 Quarternion::multiply(glm::vec4 orientation1, glm::vec4 orientation2)
{
	if (getMag(orientation2) == 0)
	{
		return orientation1;
	}
	float s1 = orientation1.x;
	float s2 = orientation2.x;

	glm::vec3 v1 = glm::vec3(orientation1.y, orientation1.z, orientation1.a);
	glm::vec3 v2 = glm::vec3(orientation2.y, orientation2.z, orientation2.a);

	glm::vec3 resLeft = (s1 * v2) + (s2 * v1) + glm::cross(v1, v2);	

	return glm::vec4((s1 * s2) - glm::dot(v1, v2), resLeft);	
}
float Quarternion::dot(Quarternion * other)
{

}
float Quarternion::getMag(glm::vec4 val)
{
	return sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2) + pow(val.a, 2));
}
void Quarternion::update()
{
	toMatrix();
}
void Quarternion::toMatrix()
{
	float q[4] = {orientation.x, orientation.y, orientation.z, orientation.a };
	matrixOrientation[0][0] = 1 - (2.0f * pow(q[2], 2)) - (2.0f * pow(q[3], 2));
	matrixOrientation[0][1] = (2.0f * q[1] * q[2]) + (2.0f * q[0] * q[3]);
	matrixOrientation[0][2] = (2.0f * q[1] * q[3]) - (2.0f * q[0] * q[2]);
	matrixOrientation[1][0] = (2.0f * q[1] * q[2]) - (2.0f * q[0] * q[3]);
	matrixOrientation[1][1] = 1 - (2.0f * pow(q[1], 2)) - (2.0f * pow(q[3], 2));
	matrixOrientation[1][2] = (2.0f * q[2] * q[3]) + (2.0f * q[0] * q[1]);
	matrixOrientation[2][0] = (2.0f * q[1] * q[3]) + (2.0f * q[0] * q[2]);
	matrixOrientation[2][1] = (2.0f * q[2] * q[3]) - (2.0f * q[0] * q[1]);
	matrixOrientation[2][2] = 1 - (2 * pow(q[1], 2)) - (2 * pow(q[2], 2));

	/* TODO remove */
	//matrixOrientation = glm::mat4(1.0f);
}
Quarternion::~Quarternion()
{
}
