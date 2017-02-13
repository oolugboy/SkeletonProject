#ifndef JOINT_H_
#define JOINT_H_
#include <vector>
#include "Token.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "DOF.h"
#define PI 3.14159265358979f

using namespace std;

class Joint
{
	public:
		int id;
		static int idCounter;
		Joint();
		virtual ~Joint();
		vector < Joint * > children;
		glm::vec3 boxMin;
		glm::vec3 boxMax;
		void printMatrix(glm::mat4 matrix);
		void printVector(glm::vec3 vec);
		virtual bool load(Tokenizer * token) = 0;
		virtual void update(glm::mat4 parentMat) = 0;
		pair< bool , glm::mat4> findWorldMatrix(int id);
		void animateOffset(glm::vec3 newOffset);
		void animatePose(glm::vec3 newPose, int jointId);
		void adjustPos(glm::vec3 axis, bool incr, int jointId);
		void draw();
		void loadVertices();
		void genVertices();
		void render();
		DOF * dof;
		GLuint VBO, VAO;
		GLuint uProjection, uModelview;
		glm::mat4 modelView;
		glm::mat4 toWorld;
		glm::vec3 offset;
		glm::vec3 currPos;
		vector< glm::vec3 > vertices;		
};
#endif

