#ifndef SKELETON_HPP
#define SKELETON_HPP
#include "BallJoint.h"
class Skeleton
{
public:
	Skeleton(const char * fileName);
	~Skeleton();
	void update();
	void draw();
	bool parseLoad();
	static glm::mat4 getWorldMatrix(Joint * root, int id);
	Joint * root;
	char fileName[BUFSIZ];
};
#endif