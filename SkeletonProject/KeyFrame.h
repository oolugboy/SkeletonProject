#ifndef KEY_FRAME_H
#define KEY_FRAME_H
#include "Token.h"

class KeyFrame
{
public:
	KeyFrame();
	char inTangRule[256];
	char outTangRule[256];
	float t;
	float val;
	bool load(Tokenizer * token);
	float inTang;
	float outTang;
	float A, B, C, D;
	~KeyFrame();
};

#endif

