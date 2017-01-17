#include "Skeleton.h"
#include "Token.h"
Skeleton::Skeleton(const char * fileName)
{
	//* Assume that the root must always be a ball joint 
	root = new BallJoint();
	strcpy(this->fileName, fileName);
	parseLoad();
}
bool Skeleton::parseLoad()
{
	Tokenizer * token = new Tokenizer();
	token->Open(fileName);

	//Parse Tree
	root->load(token);

	//Finish 
	token->Close();
	delete(token);
	return true;
}
Skeleton::~Skeleton()
{
	
}