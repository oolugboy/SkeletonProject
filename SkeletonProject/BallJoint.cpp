#include "BallJoint.h"

BallJoint::BallJoint()
{
	dof = new DOF();
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	vertices.resize(36);
	vertices[0] = glm::vec3(boxMin.x, boxMin.y, boxMin.z);// triangle 1 : begin
	vertices[1] = glm::vec3(boxMin.x, boxMin.y, boxMax.z);
	vertices[2] = glm::vec3(boxMin.x, boxMax.y, boxMax.z); // triangle 1 : end
	vertices[3] = glm::vec3(boxMax.x, boxMax.y, boxMin.z); // triangle 2 : begin
	vertices[4] = glm::vec3(boxMin.x, boxMin.y, boxMin.z);
	vertices[5] = glm::vec3(boxMin.x, boxMax.y, boxMin.z); // triangle 2 : end
	vertices[6] = glm::vec3(boxMax.x, boxMin.y, boxMax.z);
	vertices[7] = glm::vec3(boxMin.x, boxMin.y, boxMin.z);
	vertices[8] = glm::vec3(boxMax.x, boxMin.y, boxMin.z);
	vertices[9] = glm::vec3(boxMax.x, boxMax.y, boxMin.z);
	vertices[10] = glm::vec3(boxMax.x, boxMin.y, boxMin.z);
	vertices[11] = glm::vec3(boxMin.x, boxMin.y, boxMin.z);
	vertices[12] = glm::vec3(boxMin.x, boxMin.y, boxMin.z);
	vertices[13] = glm::vec3(boxMin.x, boxMax.y, boxMax.z);
	vertices[14] = glm::vec3(boxMin.x, boxMax.y, boxMin.z);
	vertices[15] = glm::vec3(boxMax.x, boxMin.y, boxMax.z);
	vertices[16] = glm::vec3(boxMin.x, boxMin.y, boxMax.z);
	vertices[17] = glm::vec3(boxMin.x, boxMin.y, boxMin.z);
	vertices[18] = glm::vec3(boxMin.x, boxMax.y, boxMax.z);
	vertices[19] = glm::vec3(boxMin.x, boxMin.y, boxMax.z);
	vertices[20] = glm::vec3(boxMax.x, boxMin.y, boxMax.z);
	vertices[21] = glm::vec3(boxMax.x, boxMax.y, boxMax.z);
	vertices[22] = glm::vec3(boxMax.x, boxMin.y, boxMin.z);
	vertices[23] = glm::vec3(boxMax.x, boxMax.y, boxMin.z);
	vertices[24] = glm::vec3(boxMax.x, boxMin.y, boxMin.z);
	vertices[25] = glm::vec3(boxMax.x, boxMax.y, boxMax.z);
	vertices[26] = glm::vec3(boxMax.x, boxMin.y, boxMax.z);
	vertices[27] = glm::vec3(boxMax.x, boxMax.y, boxMax.z);
	vertices[28] = glm::vec3(boxMax.x, boxMax.y, boxMin.z);
	vertices[29] = glm::vec3(boxMin.x, boxMax.y, boxMin.z);
	vertices[30] = glm::vec3(boxMax.x, boxMax.y, boxMax.z);
	vertices[31] = glm::vec3(boxMin.x, boxMax.y, boxMin.z);
	vertices[32] = glm::vec3(boxMin.x, boxMax.y, boxMax.z);
	vertices[33] = glm::vec3(boxMax.x, boxMax.y, boxMax.z);
	vertices[34] = glm::vec3(boxMin.x, boxMax.y, boxMax.z);
	vertices[35] = glm::vec3(boxMax.x, boxMin.y, boxMax.z);
}
bool BallJoint::load(Tokenizer * token)
{
	cout << " In the join load " << endl;
	token->FindToken("{");
	while(true)
	{
		char temp[256];
		token->GetToken(temp);
		//cout << " The temp is " << temp << endl;
		if (strcmp(temp, "offset") == 0)
		{
			offset.x = token->GetFloat();
			offset.y = token->GetFloat();
			offset.z = token->GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0)
		{
			boxMin.x = token->GetFloat();
			boxMin.y = token->GetFloat();
			boxMin.z = token->GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0)
		{
			boxMax.x = token->GetFloat();
			boxMax.y = token->GetFloat();
			boxMax.z = token->GetFloat();
		}
		else if (strcmp(temp, "rotxlimit") == 0)
		{
			dof->rotMin.x = token->GetFloat();
			dof->rotMax.x = token->GetFloat();			
		}
		else if (strcmp(temp, "rotylimit") == 0)
		{
			dof->rotMin.y = token->GetFloat();
			dof->rotMax.y = token->GetFloat();		
		}
		else if (strcmp(temp, "rotzlimit") == 0)
		{
			dof->rotMin.z = token->GetFloat();
			dof->rotMax.z = token->GetFloat();	
		}
		else if (strcmp(temp, "pose") == 0)
		{
			dof->pose.x = token->GetFloat();
			dof->pose.y = token->GetFloat();
			dof->pose.z = token->GetFloat();
			dof->clamp(dof->pose);
		}
		else if (strcmp(temp, "balljoint") == 0)
		{
			Joint * child = new BallJoint();
			cout << " About to load a child " << endl;
			child->load(token);
			cout << " Done loading the child " << endl;
			children.push_back(child);
		}
		else if (strcmp(temp, "}") == 0)
		{
			//cout << " In the close condition " << endl;
			return true;
		}
		else //unrecognized token
		{
			token->SkipLine();
		}
	}
	return false;
}
void BallJoint::update(glm::vec3 parentPos)
{	
	currPos = parentPos + offset;
	toWorld = glm::translate(glm::mat4(1.0f), currPos);
	//The DOF angles are in degrees
	float deg = dof->pose.x * (180.0f / PI);
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deg, glm::vec3(1.0f, 0.0f, 0.0f));

	deg = dof->pose.y * (180.0f / PI);
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 1.0f, 0.0f));

	deg = dof->pose.z * (180.0f / PI);
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 0.0f, 1.0f));	

	int size = children.size();
	for (int i = 0; i < size; i++)
	{
		children[i]->update(currPos);
	}
}
BallJoint::~BallJoint()
{

}
