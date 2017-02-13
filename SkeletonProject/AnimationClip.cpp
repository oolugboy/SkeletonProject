#include "AnimationClip.h"

AnimationClip::AnimationClip(const char * fileName, Skeleton * skeleton)
{
	this->skeleton = skeleton;
	this->fileName = fileName;
	token = new Tokenizer();
	timeSet = false;
	elapsedTime = 0;
	load();
}
void AnimationClip::update()
{
	/* Get the channel vals */
	if (timeSet == false)
	{
		origTime = clock();
		timeSet = true;
	}
	else
	{
		elapsedTime = (float)(clock() - origTime) / CLOCKS_PER_SEC;		
	}
	evaluateChannels(elapsedTime);

	/* Then update the skeleton */
	skeleton->update();
}
void AnimationClip::evaluateChannels(float t)
{
	int size = channels.size();
	if (size > 0)
	{
		/* First do the one for the root translation */
		glm::vec3 newOffset;
		newOffset.x = channels[0]->evaluate(t);
		newOffset.y = channels[1]->evaluate(t);
		newOffset.z = channels[2]->evaluate(t);

		int numJoints = (size - 3) / 3;
		for (int i = 0; i < numJoints; i++)
		{
			glm::vec3 newPose;
			int start = 3 * (i + 1);
			newPose.x = channels[start]->evaluate(t);
			newPose.y = channels[start + 1]->evaluate(t);
			newPose.z = channels[start + 2]->evaluate(t);

			skeleton->animatePose(newPose, i);
		}
	}
}
bool AnimationClip::load()
{	
	token->Open(fileName);
	token->FindToken("{"); //By pass the animation flag 
	float rangeBegin = 0.0f, rangeEnd = 0.0f;
	
	while (true)
	{
		char temp[256];
		token->GetToken(temp);

		if (strcmp(temp, "range") == 0)
		{
			rangeBegin = token->GetFloat();
			rangeEnd = token->GetFloat();
		}
		else if (strcmp(temp, "numChannels") == 0)
		{
			numChannels = token->GetInt();
		}
		else if (strcmp(temp, "channel") == 0)
		{
			Channel * newChan = new Channel(rangeBegin, rangeEnd);
			newChan->load(token);	
			newChan->precompute();
			channels.push_back(newChan);
		}	
		else if (strcmp(temp, "}") == 0) // We are done
		{			
			break;
		}
		else //unrecognized token
		{
			token->SkipLine();
		}
	}
	//Finish 
	token->Close();
	delete(token);
	return true;
}
