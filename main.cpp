#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

using std::cout, std::endl;

int main()
{
	//initializes the library (GLFW)
	glfwInit();


	//--- context to create window ---
	//tell GLFW using opengl version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//modern opengl functions "package"
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//---intializing the window---
	//data type window named window (width, height, name, is it fullscreen, share resources?)
	GLFWwindow* window = glfwCreateWindow(800, 800, "SampleWindow", NULL, NULL);
	if (window == NULL) 
	{
		cout << "Failed to create SampleWindow" << endl;
		glfwTerminate();
		return -1;
	}

	//---updating opengl states to match context of window---
	//should have single context per thread if multithreaded (to avoid messing up state of another contexts)
	glfwMakeContextCurrent(window);

	//loads configurations for opengl
	gladLoadGL();

	//viewport refers to dimension of rendering window 
	//x, y, height, width
	glViewport(0, 0, 800, 800);

	//color value to reset color buffer with (state setting)
	//x, y, z, alpha
	glClearColor(0.4f, 255, 0.43, 1.0f);
	//clears current frame buffer and replaces it with color designated in glClearColor()
	//(state doing)
	glClear(GL_COLOR_BUFFER_BIT);
	//GLFW windows have double buffering (front and back)
	//only the front buffer gets displayed, so need to swap to update frame
	glfwSwapBuffers(window);

	//executing the window life cycle
	while (!glfwWindowShouldClose(window))
	{
		//checks to retrieve action/input events in buffer that the OS signals
		glfwPollEvents();
	}

	//delete window once no longer in use
	glfwDestroyWindow(window);
	//terminates the library
	glfwTerminate();

	return 0;
}