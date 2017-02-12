#include "KeyFrame.h"
#include <iostream>

using namespace std;

KeyFrame::KeyFrame()
{	
}
bool KeyFrame::load(Tokenizer * token)
{
	t = token->GetFloat();
	val = token->GetFloat();

	char temp[256];
	token->GetToken(temp);

	if (temp[0] >= 48 && temp[0] <= 57) // If it is a digit
	{
		strcpy(inTangRule, "fixed");
		strcpy(outTangRule, "fixed");
		inTang = atof(temp);
		outTang = token->GetFloat();
	}
	else
	{
		strcpy(inTangRule, temp);
		token->GetToken(outTangRule);
	}
	cout << " The inTangRule is " << inTangRule << endl;
	cout << " The outTangRule is " << outTangRule << endl;
	return true;
}