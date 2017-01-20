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
	void update(glm::mat4 parentMat);
	DOF * dof;
};

#endif
