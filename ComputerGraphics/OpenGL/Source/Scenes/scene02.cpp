#include "Scenes/scene01.h"

bool Scene01::Initialize()
{

	//shader
	m_shader = new Shader(m_engine);
	m_shader->CompileShader("shaders\\basic.vs", GL_VERTEX_SHADER);
	m_shader->CompileShader("shaders\\basic.fs", GL_FRAGMENT_SHADER);
	m_shader->Link();
	m_shader->Use(); //active

	/*// vertex buffer data
	const GLfloat vertexBuffer[] = //x,y,z , r , g , b
	{
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	*/
	const GLfloat vertexBuffer[] =
	{
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};
	const GLushort indexBuffer[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_vertexArray = new VertexArray(m_engine);
	m_vertexArray->CreateBuffer(8 * sizeof(GLfloat), sizeof(vertexBuffer)/(8 * sizeof(GLfloat)),(void*)vertexBuffer);
	m_vertexArray->CreateIndexBuffer(GL_UNSIGNED_SHORT, 6, (void*)indexBuffer);

	m_vertexArray->SetAttribute(0, 3, (8 * sizeof(GLfloat)), 0);
	m_vertexArray->SetAttribute(1, 3, (8 * sizeof(GLfloat)), (void*)(3*sizeof(GLfloat)));
	m_vertexArray->SetAttribute(2, 2, (8 * sizeof(GLfloat)), (void*)(6 * sizeof(GLfloat)));

	// vbo - vertex buffer object
	GLuint vertexBufferID;


	//position
	//GLuint position = glGetAttribLocation(shaderProgramID, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	//3 is how many points, FL_FLOAT is what datatype 
	//8 is the stride. how many is it to the next

	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//UV
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	///////

	//Index Buffer
	GLuint indexBufferID;
	

	//Texture
	GLuint textureID = SOIL_load_OGL_texture(
		"resources\\face.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, textureID);

	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);

	//m_mxProjection = glm::ortho(0.0f, (float)m_engine->Get<Renderer>()->GetWidth(), (float)m_engine->Get<Renderer>()->GetHeight(),0.0f);
	//^^ 2D

	//vv 3D
	m_mxProjection = glm::perspective(glm::radians(45.0f), (float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight(), 0.01f, 1000.0f);
	
	m_mxModel = glm::mat4(1.0f);
	m_transform.scale = glm::vec3(20.0f); //scaling
	//m_transform.translation = glm::vec3(400.0f, 300.0f,0.0f); //position, makes it in the middle of the screen

	return true;
}

void Scene01::Shutdown()
{
}

void Scene01::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	if (m_engine->Get<Input>()->GetActionButton("left") == Input::HELD)
	{
		m_transform.translation.x += 10.0f * dt;
	}
	if (m_engine->Get<Input>()->GetActionButton("right") == Input::HELD)
	{
		m_transform.translation.x -= 10.0f * dt;
	}

	m_mxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	                                            //^^ How close the camera is

	m_transform.Update();
	m_mxModel = m_transform.matrix;
	glm::mat4 mxMP = m_mxProjection * m_mxView * m_mxModel;

	m_shader->SetUniform("model", mxMP);
}

void Scene01::Render()
{
	glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_vertexArray->Draw(GL_TRIANGLES);

	
	m_engine->Get<Renderer>()->SwapBuffer();

}
