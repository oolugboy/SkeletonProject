#include "Window.h"

using namespace std;
const char* window_title = "GLFW Starter Project";

GLint shaderProgram;

bool leftButton = false, rightButton = false;
float scrollOffset = 0.0f;
glm::vec3 currPos(0.0f, 0.0f, 0.0f);
glm::vec3 lastPos(0.0f, 0.0f, 0.0f);
bool accelerate = false;
bool turnRight = false;
bool turnLeft = false;

Scene * scene;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define VERTEX_SHADER2_PATH "../shader2.vert"
#define FRAGMENT_SHADER2_PATH "../shader2.frag"
#define AGENT_VERTEX_SHADER_PATH "shaders/agentShader.vert"
#define AGENT_FRAGMENT_SHADER_PATH "shaders/agentShader.frag"


Camera * Window::camera;
// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 5.0f, 15.0f);		// e  | Position of camera
glm::vec3 Window::cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 Window::cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
int Window::isTexture = 1;
bool Window::debug = true;

glm::mat4 Window::P;
glm::mat4 Window::V;
GLint Window::shaderProgram2;
GLint Window::agentShaderProgram;

int camera_mode = 0;


void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top

	/* Initialize the scene */
	scene = new Scene();
	/*Set the initial projection matrix */
	P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.

void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		cout << " The projection is being set " << endl;
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		//V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{

}

void Window::display_callback(GLFWwindow* window)
{
	//Make the scene initially red 
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}
/* For the keyboard user input */
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if escape was pressed
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
	{
		// Close the window. This causes the program to also terminate.
		glfwSetWindowShouldClose(window, GL_TRUE);
	}	
}
void Window::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	/** Update the last position on any mouse position event */
	lastPos = currPos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		rightButton = true;
	}
	else
	{
		rightButton = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		leftButton = true;
	}
	else
	{
		leftButton = false;
	}
}
void Window::cursor_pos_callback(GLFWwindow * window, double xPos, double yPos)
{
	currPos.x = xPos;
	currPos.y = yPos;
}
void Window::scroll_callback(GLFWwindow * window, double xOffset, double yOffset)
{
	scrollOffset = yOffset;
	//scene->zoom(scrollOffset, camera->cam_pos);
}