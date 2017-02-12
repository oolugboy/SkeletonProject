#ifndef CHANNEL_H
#define CHANNEL_H
#include "Token.h"
#include <vector>
#include "KeyFrame.h"

using namespace std;

class Channel 
{
public:
	float rangeBegin;
	float rangeEnd;
	int numKeys;
	char extrapIn[256];
	char extrapOut[256];
	bool load(Tokenizer * token);
	vector < KeyFrame *> keys;
	Channel(float rangeBegin, float rangeEnd);
	~Channel();
};
#endif
