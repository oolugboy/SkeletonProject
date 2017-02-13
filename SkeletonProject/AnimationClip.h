#ifndef ANIMATION_CLIP_H_
#define ANIMATION_CLIP_H_
#include <vector>
#include "Token.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Skeleton.h"
#include "Channel.h"
#include <time.h>
#define PI 3.14159265358979f

class AnimationClip
{
public:
	AnimationClip(const char * fileName, Skeleton * skeleton);
	vector< Channel * > channels;
	Tokenizer * token;
	const char * fileName;
	Skeleton * skeleton;
	bool load();
	void update();
	int numChannels;
	clock_t origTime;
	float elapsedTime;
	void evaluateChannels(float t);
	bool timeSet;
	~AnimationClip();
};
#endif