#ifndef _BALL_JOINT_H_
#define _BALL_JOINT_H_

#include "Joint.h"
#include "DOF.h"
class BallJoint : public Joint
{
public:
	BallJoint();
	~BallJoint();
	bool load(Tokenizer * token);
	void update(glm::vec3 parentPos);
	DOF * dof;
};

#endif
