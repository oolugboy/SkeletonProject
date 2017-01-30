#include "Skeleton.h"
#include "Token.h"
Skeleton::Skeleton(const char * fileName)
{
	/* Reset the id counter*/
	Joint::idCounter = 0;
	//* Assume that the root must always be a ball joint 
	root = new BallJoint(Joint::idCounter++);
	strcpy(this->fileName, fileName);
	parseLoad();
	numJoints = Joint::idCounter;
	currJointId = 0;
}
void Skeleton::bindNextJoint()
{
	currJointId = (currJointId + 1) % numJoints;
}
void Skeleton::adjustPos(glm::vec3 axis, bool incr)
{
	root->adjustPos(axis, incr, currJointId);
}
void Skeleton::update()
{
	root->update(glm::mat4(1.0f));
}
void Skeleton::draw()
{
	root->draw();
}
glm::mat4 Skeleton::getWorldMatrix(Joint * root, int id)
{
	pair<bool, glm::mat4 > res = root->findWorldMatrix(id);
	return res.second;
}
bool Skeleton::parseLoad()
{
	Tokenizer * token = new Tokenizer();
	token->Open(fileName);

	//Parse Tree
	root->load(token);

	//Finish 
	token->Close();
	delete(token);
	return true;
}
Skeleton::~Skeleton()
{
	
}