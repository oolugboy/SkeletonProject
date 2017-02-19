#include "Triangle.h"
#include <iostream>

Triangle::Triangle(Particle * a, Particle * b, Particle * c)
{
	this->a = a;
	this->b = b;
	this->c = c;

	toWorld = glm::mat4(1.0f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
}
void Triangle::calcNormal()
{	
	normal = glm::normalize(cross(b->position - a->position, c->position - a->position));
}
void Triangle::loadVertices()
{	
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//cout << " The size of the vertices is " << vertices.size() << endl;
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


	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &(normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}
void Triangle::update()
{
	fillVertsAndNorms();
	loadVertices();
}
void Triangle::fillVertsAndNorms()
{
	vertices.push_back(a->position);
	vertices.push_back(b->position);
	vertices.push_back(c->position);

	/* Now for the normals */
	normals.push_back(a->normal);
	normals.push_back(b->normal);
	normals.push_back(c->normal);
}
void Triangle::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{	
	cout << " drawing " << endl;
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); */

	glDrawArrays(GL_TRIANGLES, 0, 3); // # for 3 sides of a triangle 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
Triangle::~Triangle()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteVertexArrays(1, &VAO);
}
glm::vec3 cross(glm::vec3 a, glm::vec3 b)
{
	float xVal = (a.y * b.z) - (a.z * b.y);
	float yVal = ((a.x * b.z) - (a.z * b.x)) * -1.0f;
	float zVal = (a.x * b.y) - (a.y * b.x);
	return glm::vec3(xVal, yVal, zVal);
}
