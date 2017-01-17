#ifndef SKELETON_HPP
#define SKELETON_HPP
#include "BallJoint.h"
class Skeleton
{
public:
	Skeleton(const char * fileName);
	~Skeleton();
	bool parseLoad();
	Joint * root;
	char fileName[BUFSIZ];
};

#endif