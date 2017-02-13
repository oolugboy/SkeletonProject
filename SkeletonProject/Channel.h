#ifndef CHANNEL_H
#define CHANNEL_H
#include "Token.h"
#include <vector>
#include "KeyFrame.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Channel 
{
public:
	float rangeBegin;
	float rangeEnd;
	int numKeys;
	char extrapIn[256];
	char extrapOut[256];
	glm::mat4 mult;
	const int multVals[16] = {2, -3, 0, 1, -2, 3, 0, 0, 1, -2, 1, 0, 1, -1, 0, 0};	
	float evaluate(float currTime);
	void calcTangents();
	void calcCubicCoeff();
	float getValWithCubicCoeff(int index, float currTime);
	void precompute();
	bool load(Tokenizer * token);
	vector < KeyFrame *> keys;
	Channel(float rangeBegin, float rangeEnd);
	~Channel();
};
#endif
