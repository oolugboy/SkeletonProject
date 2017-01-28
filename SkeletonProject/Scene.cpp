#include "Scene.h"

using namespace std;


Scene::Scene()
{
	cout << " About to create the skeleton " << endl;
	test = new Skeleton("test.skel");
	dragon = new Skeleton("dragon.skel");
	wasp = new Skeleton("wasp.skel");		
	tube = new Skeleton("tube.skel");
	tubeSkin = new Skin("tube.skin", tube->root);
}

void Scene::drawTest(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
	glUseProgram(shaderProgram);
	//test->draw();
	//tube->draw();
	tubeSkin->draw(shaderProgram, view, projection);
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

void Scene::update()
{
	test->update();
	wasp->update();
	dragon->update();
	tube->update();
	tubeSkin->update();
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
