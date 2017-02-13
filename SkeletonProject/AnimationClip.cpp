#include "AnimationClip.h"

AnimationClip::AnimationClip(const char * fileName, Skeleton * skeleton)
{
	this->skeleton = skeleton;
	this->fileName = fileName;
	token = new Tokenizer();
	load();
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
