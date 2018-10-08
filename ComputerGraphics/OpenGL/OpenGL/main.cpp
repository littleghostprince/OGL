#include <glad\glad.h>
#include "sdl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char** argv)
{
	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); //which version of GL is using
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetSwapInterval(1);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!gladLoadGL()) {
		exit(-1);
	}

	//shader
	GLint success;
	GLchar infoLog[512];

	const char* vertexShader = "#version 430 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 color;\n"
		"out vec3 fragment_color;\n"
		"uniform mat4 model;\n"
		"void main()\n"
		"{\n"
		"fragment_color = color;\n"
		"gl_Position = model * vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";

	const char* fragmentShader = "#version 430 core\n"
		"out vec4 color;\n"
		"in vec3 fragment_color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(fragment_color, 1.0);\n"    //you can change the color here 
		"}\n\0";

	//creating shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShader, NULL); //gets stream of text that defines vertex shader
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success); //check if vertex shader is successful
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, sizeof(infoLog), NULL, infoLog);
		SDL_Log("Shader Compile Error: %s\n", infoLog);
	}

	//fragment shader
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, sizeof(infoLog), NULL, infoLog);
		SDL_Log("Shader Compile Error: %s\n", infoLog);
	}

	//all shaders need a shader program that contains all shaders
	GLuint shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, sizeof(infoLog), NULL, infoLog);
		SDL_Log("Shader Link Error: %s\n", infoLog);
	}

	//clean up after the shader. 
	//Use Program tells you what shader youll like to use 
	glUseProgram(shaderProgramID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	// vao - vertex array object
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// vertex buffer data
	const GLfloat vertexBuffer[] = //x,y,z , r , g , b
	{
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};


	// vbo - vertex buffer object
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	//GLuint position = glGetAttribLocation(shaderProgramID, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	//3 is how many points, FL_FLOAT is what datatype 

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	///////

	GLint uniform = glGetUniformLocation(shaderProgramID, "model");
	glm::mat4 mxModel = glm::mat4(1.0f);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();

		mxModel = glm::rotate(mxModel, 0.0001f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniform,1,FALSE,&mxModel[0][0]);

		glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		/*
		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5f, -0.5f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 0.5f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		*/
		SDL_GL_SwapWindow(window);
	}

	return 0;
}

