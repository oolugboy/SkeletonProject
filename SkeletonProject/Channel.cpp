#include "Channel.h"
#include <iostream>

Channel::Channel(float rangeBegin, float rangeEnd)
{
	this->rangeBegin = rangeBegin;
	this->rangeEnd = rangeEnd;
}
bool Channel::load(Tokenizer * token)
{
	token->FindToken("{");
	while (true)
	{
		char temp[256];
		token->GetToken(temp);
		if (strcmp(temp, "extrapolate") == 0)
		{
			token->GetToken(extrapIn);
			cout << " The extrapIn is " << extrapIn << endl;
			token->GetToken(extrapOut);
			cout << " The exrapOut is " << extrapOut << endl;
		}
		else if (strcmp(temp, "keys") == 0)
		{
			cout << " Get the keys too " << endl;
			numKeys = token->GetInt();
			token->FindToken("{");
			for (int i = 0; i < numKeys; i++)
			{
				KeyFrame * newKey = new KeyFrame();
				newKey->load(token);
				keys.push_back(newKey);
			}
			token->FindToken("}");
		}	
		else if (strcmp(temp, "}") == 0)
		{
			return true;
		}
	}
	return false;
}