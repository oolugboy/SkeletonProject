#include "Scene.h"

using namespace std;


Scene::Scene()
{
	test = new Skeleton("test.skel");
	dragon = new Skeleton("dragon.skel");

	//currSkel = wasp = new Skeleton("wasp.skel");
//	currSkin = waspSkin = new Skin("wasp.skin", wasp->root);
	//waspAnim = new AnimationClip("wasp_walk.anim", wasp);

	currSkel = tube = new Skeleton("tube.skel");
	currSkin = tubeSkin = new Skin("tube.skin", tube->root);

	cloth = new Cloth(2, 0.0f, 0.0f);

	water = new Fluid();
	//sampleAnim = new AnimationClip("sample.anim", tube);
}

void Scene::drawTest(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
	glUseProgram(shaderProgram);
	//test->draw();
	//tube->draw();
	//tubeSkin->draw(shaderProgram, view, projection);
	//currSkin->draw(shaderProgram, view, projection);
	//cloth->draw(shaderProgram, view, projection);
	water->draw(shaderProgram, view, projection);
}
void Scene::drawDragon(GLint shaderProgram)
{
	glUseProgram(shaderProgram);
	dragon->draw();
}
void Scene::drawWasp(GLint shaderProgram)
{
	glUseProgram(shaderProgram);
	wasp->draw();
}
void Scene::bindNextJoint()
{
	currSkel->bindNextJoint();
}
void Scene::adjustPos(glm::vec3 axis, bool incr)
{
	currSkel->adjustPos(axis, incr);
}
void Scene::adjustWindSpeed(bool incr)
{
	cloth->adjustWindSpeed(incr);
}
void Scene::moveCloth(glm::vec3 diff)
{
	cloth->moveFixedParticles(diff);
}
void Scene::update()
{
/*	waspAnim->update();
	currSkin->update(); */
	float currTime = ((float)clock() / CLOCKS_PER_SEC);
	cloth->update(currTime - prevTime);
	water->update(currTime - prevTime);
	prevTime = currTime;
}

void Scene::mouseOrbit(glm::vec3 & lastPosition, glm::vec3 & currPosition, glm::vec3 & cam_pos, int width, int height)
{
	glm::vec3 mappedLastPos = trackBallMapping(lastPosition, width, height);
	glm::vec3 mappedCurrPos = trackBallMapping(currPosition, width, height);

	float mLPMag = glm::length(mappedLastPos);
	float mCPMag = glm::length(mappedCurrPos);

	glm::vec3 direction = mappedCurrPos - mappedLastPos;
	float velocity = glm::length(direction);
	if (velocity > 0.001)
	{
		glm::vec3 rotAxis = glm::cross(mappedLastPos, mappedCurrPos);
		float rotAngle = acos(glm::dot(mappedLastPos, mappedCurrPos) / (mLPMag * mCPMag));

		glm::vec4 temp = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * glm::vec4(cam_pos, 1.0f);
		cam_pos = glm::vec3(temp.x, temp.y, temp.z);
		lastPosition = currPosition;
	}
}

glm::vec3 Scene::trackBallMapping(glm::vec3 point, int width, int height)
{
	glm::vec3 res;
	res.x = ((2.0f * point.x) - width) / width;
	res.y = (height - (2.0f * point.y)) / height;
	res.z = 0.0f;
	float depth = fmin(glm::length(res), 1.0f);
	res.z = sqrt(1.001f - pow(depth, 2));
	res = glm::normalize(res);
	return res;
}

Scene::~Scene()
{
	cout << "Scene deleted" << endl;
}
