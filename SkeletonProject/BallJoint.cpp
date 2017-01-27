#include "BallJoint.h"

BallJoint::BallJoint()
{
	dof = new DOF();
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
}
bool BallJoint::load(Tokenizer * token)
{
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
			child->load(token);
			children.push_back(child);
		}
		else if (strcmp(temp, "}") == 0)
		{
			//cout << " In the close condition " << endl;
			// Done parsing file, now to load the vertices
			genVertices();
			loadVertices();
			return true;
		}
		else //unrecognized token
		{
			token->SkipLine();
		}
	}
	return false;
}
void BallJoint::update(glm::mat4 parentMat)
{	
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), offset);
	
	//The DOF angles are in degrees
	float deg = dof->pose.x;
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(1.0f, 0.0f, 0.0f));

	deg = dof->pose.y;
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 1.0f, 0.0f));

	deg = dof->pose.z;
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 0.0f, 1.0f));		

	glm::mat4 locMatrix = trans * rotZ * rotY * rotX;
	toWorld = parentMat * locMatrix;

	int size = children.size();
	for (int i = 0; i < size; i++)
	{
		children[i]->update(toWorld);
	}
}
BallJoint::~BallJoint()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);	
	delete(dof);
}
