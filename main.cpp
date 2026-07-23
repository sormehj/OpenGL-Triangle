#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

using std::cout, std::endl;


//---glsl source shader code---
//in practice should be a seperate  file
//sets 3D position of each vertex
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//setting the color of each pixel within rendered shape (pixels between the vertices)
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";



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

	//equilateral triangle
	//every 3 floats is one point
	GLfloat vertices[] =
	{
		//left corner
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 
		//right corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		//top corner
		0.0f, 0.5f * float(sqrt(3)) / 2, 0.0f,
	};

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

	//create vertex shader object stored into unsigned int
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//shader object (source code gets replaced), # of strings, reference to source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//real time compiles the vertexShader source code
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//--- Wrapping, attaching, & linking shader obj into shader program ---
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//del shader obj dont them after linking into program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//---create VAO (vertex array obj used point to and organize VBOs (vertex buffer obj)---
	GLuint VAO, VBO; //ID
	//always bind vertex array obj before configuring VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	//binds vertex buffer obj to GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//copies triangle vertice data in gpu through array buffer
	//target buffer slot, size of dataset, vertex data, use case/hint
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//index, components per vertex, data type, noramlized?,
	//stride (space between one vertex to the next), starting byte position (the offset) right at beginning
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//index of vertex attribute to be enabled
	glEnableVertexAttribArray(0);

	//unbinding VAO and VBO (by binding to 0)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//color value to reset color buffer with (state setting)
	//x, y, z, alpha
	glClearColor(0.4f, 255, 0.43f, 1.0f);
	//clears current frame buffer and replaces it with color designated in glClearColor()
	//(state doing)
	glClear(GL_COLOR_BUFFER_BIT);
	//GLFW windows have double buffering (front and back)
	//only the front buffer gets displayed, so need to swap to update frame
	glfwSwapBuffers(window);

	//executing the window life cycle
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.4f, 255, 0.43f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);


		//checks to retrieve action/input events in buffer that the OS signals
		glfwPollEvents();
	}

	//object clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	
	
	//delete window once no longer in use
	glfwDestroyWindow(window);
	//terminates the library
	glfwTerminate();

	return 0;
}