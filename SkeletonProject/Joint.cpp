#include "Joint.h"
#include "Window.h"

int Joint::idCounter;
Joint::Joint()
{
	
}
void Joint::loadVertices()
{
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

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

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}
void Joint::render()
{
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(Window::shaderProgram, "projection");
	uModelview = glGetUniformLocation(Window::shaderProgram, "modelview");

	modelView = Window::V * toWorld;

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelView[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glEnable(GL_DEPTH_TEST);

	/*For the wired */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
void Joint::draw()
{
	//First draw self
	render();

	//Then draw children
	int size = children.size();
	for (int i = 0; i < size; i++)
	{
		children[i]->draw();
	}
}
pair<bool, glm::mat4> Joint::findWorldMatrix(int id)
{
	if (this->id == id)
	{
		return make_pair(true, toWorld);
	}
	int size = children.size();
	for (int i = 0; i < size; i++)
	{
		pair<bool, glm::mat4> res = children[i]->findWorldMatrix(id);
		if (res.first)
			return res;
	}
	return make_pair(false, glm::mat4(1.0f));
}
void Joint::genVertices()
{
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
Joint::~Joint()
{

}
