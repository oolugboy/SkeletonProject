#ifndef JOINT_H_
#define JOINT_H_
#include <vector>
#include "Token.h"
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
		virtual void update(glm::vec3 parentPos) = 0;
		void draw();
		void loadVertices();
		glm::mat4 toWorld;
		glm::vec3 offset;
		glm::vec3 currPos;
		vector< glm::vec3 > vertices;		
};
#endif

