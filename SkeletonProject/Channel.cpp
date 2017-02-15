#include "Channel.h"
#include <iostream>

Channel::Channel(float rangeBegin, float rangeEnd)
{
	this->rangeBegin = rangeBegin;
	this->rangeEnd = rangeEnd;
	debug = false;
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
//	cout << " ***** About to do the coefficients *****" << endl;
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
		keyParams[2] = keys[i]->outTang * (keys[i + 1]->t - keys[i]->t);
		keyParams[3] = keys[i + 1]->outTang * (keys[i + 1]->t - keys[i]->t);	
		
		glm::vec4 res = mult * keyParams;
		keys[i]->A = res[0];
		keys[i]->B = res[1];
		keys[i]->C = res[2];
		keys[i]->D = res[3];
		
		if (debug)
		{
			cout << " ****************** WHATTTT ?? **************" << endl;

			cout << " The key params " << keyParams[0] << " " << keyParams[1] << " " << keyParams[2] << " " << keyParams[3] << endl;
			cout << " The coeffs " << res[0] << " " << res[1] << " " << res[2] << " " << res[3] << endl;
		}
	}
}
float Channel::getExtrapVal(float currTime, bool inExtrap)
{
	int size = keys.size();	
	float range = keys[size - 1]->t - keys[0]->t;
	float valRange = keys[size - 1]->val - keys[0]->val;	
	
	if (inExtrap)
	{
		float tDiff = keys[0]->t - currTime;
		if (strcmp(extrapIn, "constant") == 0)
		{
			return keys[0]->val;
		}
		if (strcmp(extrapIn, "linear") == 0)
		{	
			return keys[0]->val - (keys[0]->outTang * tDiff);
		}
		if (strcmp(extrapIn, "cycle") == 0)
		{			
			float offset = tDiff - (float)(range * (int)(tDiff / range));
			return evaluate(keys[size - 1]->t - offset);
		} 
		if (strcmp(extrapIn, "cycle_offset") == 0)
		{
			int mult = (tDiff / range);
			float offset = tDiff - (float)(range * mult);
			return ((mult + 1) * valRange * -1.0f) + evaluate(keys[size - 1]->t - offset);
		}
	}
	else
	{
		float tDiff = currTime - keys[size - 1]->t;
		if (strcmp(extrapOut, "constant") == 0)
		{
			return keys[size - 1]->val;
		}
		if (strcmp(extrapOut, "linear") == 0)
		{
			return (keys[size - 1]->inTang * tDiff) + keys[size - 1]->val;
		}
		if (strcmp(extrapOut, "cycle") == 0)
		{
			float offset = tDiff - (float)(range * (int)(tDiff / range));
			return evaluate(keys[0]->t + offset);
		}
		if (strcmp(extrapOut, "cycle_offset") == 0)
		{
			int mult = (tDiff / range);
			float offset = tDiff - (float)(range * mult);
			return ((mult + 1) * valRange) + evaluate(keys[0]->t + offset);
		}
	}
}
float Channel::evaluate(float currTime)
{
	if (debug)
		cout << " The current elapsed time is " << currTime << endl;
	/* Do a brute force linear search first */
	int size = keys.size();

	if (size == 1)
		return keys[0]->val;

	for (int i = 0; i < size; i++)
	{
		if (keys[i]->t == currTime) // The exact match case 
		{
			return keys[i]->val;
		}
		if (i == 0 && currTime < keys[i]->t)// The extrapIn case
		{
			if (debug)
				cout << " In the extrap in case " << endl;
			return getExtrapVal(currTime, true);
		}
		if (i == (size - 1) && currTime > keys[i]->t) // The extrapOut case 
		{
			if (debug)
				cout << " In the extrap out case " << endl;
			return getExtrapVal(currTime, false);
		}//The in span case 
		if (i < (size - 1) && keys[i]->t < currTime && keys[i + 1]->t > currTime)
		{
			if (debug)
				cout << " In the span case " << endl;
			return getValWithCubicCoeff(i, currTime);
		}
	}
	return 0; // TODO The extrapolate 
}
float Channel::getValWithCubicCoeff(int index, float currTime)
{
	float u = (currTime - keys[index]->t) / (keys[index + 1]->t - keys[index]->t);
	return keys[index]->D + (u * (keys[index]->C + (u * (keys[index]->B + (u * keys[index]->A)))));
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
			if (i == 0)
			{
				keys[i]->inTang = (float)(keys[i]->val - keys[i + 1]->val) / (float)(keys[i]->t - keys[i + 1]->t);
			}
		}
		if (strcmp(keys[i]->outTangRule, "linear") == 0)
		{
			if (i < (size - 1))
			{
				keys[i]->outTang = (float)(keys[i]->val - keys[i + 1]->val) / (float)(keys[i]->t - keys[i + 1]->t);
			}
			if (i == size - 1)
			{
				keys[i]->outTang = (float)(keys[i]->val - keys[i - 1]->val) / (float)(keys[i]->t - keys[i - 1]->t);
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
				if (size > 1)// NEED TO FIX THIS !!!!!
				{
					if (i == 0)  /* Linear for the last case */
						keys[i]->outTang = keys[i]->inTang = (float)(keys[i]->val - keys[i + 1]->val) / (float)(keys[i]->t - keys[i + 1]->t);
					else  /* Linear for the last case */
						keys[i]->inTang = keys[i]->outTang = (float)(keys[i]->val - keys[i - 1]->val) / (float)(keys[i]->t - keys[i - 1]->t);
				}
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