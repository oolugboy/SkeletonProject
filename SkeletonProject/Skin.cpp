#include "Skin.h"
Skin::Skin(const char * filename)
{
	strcpy(this->filename, filename);
	token = new Tokenizer();
	/* load the file */
	load();
}

bool Skin::load()
{
	token->Open(this->filename);
	cout << " In the skin load " << endl;	
	while (true)
	{
		char temp[256];
		token->GetToken(temp);
		//cout << " The temp is " << temp << endl;
		if (strcmp(temp, "positions") == 0)
		{
			int numPos = token->GetInt();
			token->FindToken("{");			
			for (int i = 0; i < numPos; i++)
			{
				glm::vec3 newPos;
				newPos.x = token->GetFloat();				
				newPos.y = token->GetFloat();
				newPos.z = token->GetFloat();
				cout << newPos.x << " " << newPos.y << " " << newPos.z << endl;
				vertices.push_back(newPos);
			}
			token->FindToken("}");
			cout << " Done loading the position " << endl;
		}
		else if (strcmp(temp, "normals") == 0)
		{
			int numNorms = token->GetInt();
			token->FindToken("{");
			for (int i = 0; i < numNorms; i++)
			{
				glm::vec3 newNorm;
				newNorm.x = token->GetFloat();
				newNorm.y = token->GetFloat();
				newNorm.z = token->GetFloat();
				cout << newNorm.x << " " << newNorm.y << " " << newNorm.z << endl;
				normals.push_back(newNorm);
			}
			token->FindToken("}");
			cout << " Done loading the normals " << endl;
		}
		else if (strcmp(temp, "skinweights") == 0)
		{
			int numVerts = token->GetInt();
			token->FindToken("{");
			for (int i = 0; i < numVerts; i++)
			{
				int numAttachs = token->GetInt();
				vector < pair< int, float > >wMap;
				for (int j = 0; j < numAttachs; j++)
				{
					int jointInd = token->GetInt();
					float weight = token->GetFloat();					
					wMap.push_back(make_pair(jointInd, weight));
				}
				jointWeightMap.push_back(wMap);
			}
			token->FindToken("}");
			cout << " Done loading the skin weights " << endl;
		}
		else if (strcmp(temp, "triangles") == 0)
		{
			int numTriangles = token->GetInt();
			token->FindToken("{");
			for (int i = 0; i < numTriangles; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					int index = token->GetInt();
					cout << " The index is " << index << endl;
					indices.push_back(index);
				}
			}
			token->FindToken("}");
			cout << " Done loading the triangles " << endl;
		}
		else if (strcmp(temp, "bindings") == 0)
		{
			int numBindings = token->GetInt();
			token->FindToken("{");
			for (int i = 0; i < numBindings; i++)
			{
				glm::mat4 newMat;
				/* Start reading in the matrix */
				token->FindToken("{");
				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						float val = token->GetFloat();
						newMat[j][k] = val;						
					}
				}
				token->FindToken("}");
				newMat[3][3] = 1;
				bindingMatrices.push_back(newMat);
			}
			token->FindToken("}");
			cout << " Done loading the bindings " << endl;
			/* Break assuing "Binding" is the last token */
			break;		
		}
		else //unrecognized token
		{
			token->SkipLine();
		}
	}
	return false;
}