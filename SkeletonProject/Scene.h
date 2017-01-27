#ifndef _SCENE_H_
#define _SCENE_H_


#include <iostream>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <limits>
#include <string>
#include "Skeleton.h"
#include "Skin.h"


class Scene
{
public:
	GLint m_shaderProgram1;
	GLint m_shaderProgram2;
	Scene();

	void drawTest(GLint shaderProgram);
	void drawWasp(GLint shaderProgram);
	void drawDragon(GLint shaderProgram);
	
	void update();
	~Scene();

	Skeleton * dragon;
	Skeleton * wasp;
	Skeleton * test;
	Skeleton *curr;
	Skin * triangleSkin;
	void mouseOrbit(glm::vec3 & lastPosition, glm::vec3 & currPosition, glm::vec3 & cam_pos, int width, int height);
	glm::vec3 trackBallMapping(glm::vec3 point, int width, int height);
};
#endif
