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
#include "AnimationClip.h"
#include "Cloth.h"
#include <time.h>


class Scene
{
public:
	GLint m_shaderProgram1;
	GLint m_shaderProgram2;
	Scene();

	void drawTest(GLint shaderProgram, glm::mat4 view, glm::mat4 projection);
	void drawWasp(GLint shaderProgram);
	void drawDragon(GLint shaderProgram);
	
	void update();
	~Scene();

	/* The currently bound skeleton */
	Skeleton * currSkel;
	Skin * currSkin;

	/* Skeletons*/
	Skeleton * dragon;
	Skeleton * wasp;
	Skeleton * test;
	Skeleton * tube;
	
	/* Skins */
	Skin * triangleSkin;
	Skin * waspSkin;
	Skin * tubeSkin;

	/* Animation clips */
	AnimationClip * waspAnim;
	AnimationClip * sampleAnim;

	/* Animating cloth */
	Cloth * cloth;

	float prevTime;

	void bindNextJoint();
	void adjustPos(glm::vec3 axis, bool incr);
	void mouseOrbit(glm::vec3 & lastPosition, glm::vec3 & currPosition, glm::vec3 & cam_pos, int width, int height);
	void moveCloth(glm::vec3 diff);
	void adjustWindSpeed(bool incr);
	glm::vec3 trackBallMapping(glm::vec3 point, int width, int height);

};
#endif
