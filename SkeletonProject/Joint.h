#ifndef JOINT_H_
#define JOINT_H_
#include <vector>
#include "Token.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define PI 3.14159265358979f

using namespace std;

class Joint
{
	public:
		Joint();
		virtual ~Joint();
		vector < Joint * > children;
		glm::vec3 boxMin;
		glm::vec3 boxMax;
		virtual bool load(Tokenizer * token) = 0;
		virtual void update(glm::mat4 parentMat) = 0;
		void draw();
		void loadVertices();
		void genVertices();
		void render();
		GLuint VBO, VAO, TBO, EBO, NBO;
		GLuint uProjection, uModelview;
		glm::mat4 modelView;
		glm::mat4 toWorld;
		glm::vec3 offset;
		glm::vec3 currPos;
		vector< glm::vec3 > vertices;		
};
#endif

