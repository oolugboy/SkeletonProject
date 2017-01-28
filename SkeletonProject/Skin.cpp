#include "Skin.h"
Skin::Skin(const char * filename, Joint * root)
{
	strcpy(this->filename, filename);
	token = new Tokenizer();
	this->root = root;
	toWorld = glm::mat4(1.0f);
	/* load the file */
	load();
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &NBO);
}

void Skin::loadVertices()
{
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 /* Now to load the normals */
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &(normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// We've sent the vertex and normal data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

bool Skin::load()
{
	token->Open(this->filename);	
	while (true)
	{
		char temp[256];
		token->GetToken(temp);

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

				vertices.push_back(newPos);
			}
			token->FindToken("}");
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
				
				normals.push_back(newNorm);
			}
			token->FindToken("}");
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
					indices.push_back(index);
				}
			}
			token->FindToken("}");	
		}
		else if (strcmp(temp, "bindings") == 0)
		{
			int numBindings = token->GetInt();
			token->FindToken("{");
			for (int i = 0; i < numBindings; i++)
			{
				glm::mat4 newMat = glm::mat4(1.0f);
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
void Skin::update()
{
	/* First update the deformed vertices */
	//getDeformedVertsAndNorms();
	/* Then load the vertices */
	loadVertices();
}
void Skin::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	modelView = view * toWorld;

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &(projection[0][0]));
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelView[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glEnable(GL_DEPTH_TEST);

	/*For the wired */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
void Skin::getDeformedVertsAndNorms()
{
	// Go through all the vertices and normals
	int size = vertices.size();
	for (int i = 0; i < size; i++)
	{
		glm::vec3 newVert = glm::vec3(0, 0, 0);
		glm::vec3 newNorm = glm::vec3(0, 0, 0);
		// Go through all the joints
		int mSize = jointWeightMap[i].size();
		for (int k = 0; k < mSize; k++)
		{
			float weight = jointWeightMap[i][k].second;
			float jointInd = jointWeightMap[i][k].first;

			glm::mat4 world = Skeleton::getWorldMatrix(this->root, jointInd);
			glm::mat4 bindingInv = glm::inverse(bindingMatrices[jointInd]);

			/* Sum of for the vertices */
			glm::vec3 vertAdd = weight * world * bindingInv * glm::vec4(vertices[i], 1.0f);
			newVert = newVert + vertAdd;

			/* Sum up for the normals */
			glm::vec3 normAdd = weight * world * bindingInv * glm::vec4(normals[i], 0.0f);
			newNorm = newNorm + normAdd;
		}
		vertices[i] = newVert;
		normals[i] = glm::normalize(newNorm);
	}
}
Skin::~Skin()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

}

