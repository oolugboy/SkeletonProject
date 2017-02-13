#include "Channel.h"
#include <iostream>

Channel::Channel(float rangeBegin, float rangeEnd)
{
	this->rangeBegin = rangeBegin;
	this->rangeEnd = rangeEnd;
	
	int index = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mult[i][j] = multVals[index];			
			index++;
		}
		cout << endl;
	}		
}
void Channel::precompute()
{
	calcTangents();
	cout << " ***** About to do the coefficients *****" << endl;
	calcCubicCoeff();
}
void Channel::calcCubicCoeff()
{
	int size = keys.size();
	for (int i = 0; i < (size - 1); i++)
	{
		glm::vec4 keyParams;
		keyParams[0] = keys[i]->val;		
		keyParams[1] = keys[i + 1]->val;
		keyParams[2] = keys[i]->outTang;
		keyParams[3] = keys[i + 1]->outTang;	
		
		glm::vec4 res = mult * keyParams;
		keys[i]->A = res[0];
		keys[i]->B = res[1];
		keys[i]->C = res[2];
		keys[i]->D = res[3];
	}
}
void Channel::calcTangents()
{
	int size = keys.size();
	for (int i = 0; i < size; i++)
	{
		if (strcmp(keys[i]->inTangRule, "flat") == 0)
		{
			keys[i]->inTang = 0.0f;
		}
		if (strcmp(keys[i]->outTangRule, "flat") == 0)
		{
			keys[i]->outTang = 0.0f;
		}
		if (strcmp(keys[i]->inTangRule, "linear") == 0)
		{
			if (i > 0)
			{
				keys[i]->inTang = (float)(keys[i]->val - keys[i - 1]->val) / (float)(keys[i]->t - keys[i - 1]->t);
			}
		}
		if (strcmp(keys[i]->outTangRule, "linear") == 0)
		{
			if (i < (size - 1))
			{
				keys[i]->outTang = (float)(keys[i]->val - keys[i + 1]->val) / (float)(keys[i]->t - keys[i + 1]->t);
			}
		}
		if (strcmp(keys[i]->inTangRule, "smooth") == 0 || strcmp(keys[i]->outTangRule, "smooth") == 0)
		{
			if (i > 0 && i < (size - 1))
			{
				if(strcmp(keys[i]->inTangRule, "smooth") == 0)
					keys[i]->inTang = (float)(keys[i - 1]->val - keys[i + 1]->val) / (float)(keys[i - 1]->t - keys[i + 1]->t);
				if(strcmp(keys[i]->outTangRule, "smooth") == 0)
					keys[i]->outTang = (float)(keys[i - 1]->val - keys[i + 1]->val) / (float)(keys[i - 1]->t - keys[i + 1]->t);
			}
			else
			{	
				if(i == 0)  /* Linear for the last case */
					keys[i]->outTang = (float)(keys[i]->val - keys[i + 1]->val) / (float)(keys[i]->t - keys[i + 1]->t);
				else  /* Linear for the last case */
					keys[i]->inTang = (float)(keys[i]->val - keys[i - 1]->val) / (float)(keys[i]->t - keys[i - 1]->t);
			}
		}
	}
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
			token->GetToken(extrapOut);
		}
		else if (strcmp(temp, "keys") == 0)
		{
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