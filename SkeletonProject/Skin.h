#ifndef SKIN_H_
#define SKIN_H_
#include <vector>
#include "Token.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define PI 3.14159265358979f

using namespace std;

class Skin
{
public:
	Skin(const char * filename);
	~Skin();
	Tokenizer * token;
	bool load();
	void update(glm::mat4 parentMat);
	void draw();
	void loadVertices();
	void genVertices();	
	vector< glm::vec3 > vertices;
	vector < glm::vec3 > normals;
	vector< vector < pair<int , float> > >jointWeightMap;
	vector < int > indices;
	vector< glm::mat4 > bindingMatrices;
	GLuint VBO, VAO, TBO, EBO, NBO;
	GLuint uProjection, uModelview;
	glm::mat4 modelView;
	glm::mat4 toWorld;	
	char filename[BUFSIZ];
};
#endif