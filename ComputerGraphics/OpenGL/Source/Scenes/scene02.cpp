#include "Scenes/scene02.h"
#include "Objects/camera.h"

static float cube_vertices[] = {
	// Front
	-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	// Right
	 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	 // Back
	 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f,  1.0f, -1.0f, 1.0f, 0.0f,
	  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	  1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
	  // Left
	  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	  -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
	  -1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	  -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
	  // Bottom
	  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	  -1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	   1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	   1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
	   // Top
	   -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f, -1.0f, 0.0f, 1.0f
};

static GLushort indices[] =
{
	 0,  1,  2,  0,  2,  3,
	 4,  5,  6,  4,  6,  7,
	 8,  9, 10,  8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23
};

bool Scene02::Initialize()
{

	//shader
	m_shader = new Shader();
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname()+"shaders\\basic_uv.vs", GL_VERTEX_SHADER);
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname()+"shaders\\basic_uv.fs", GL_FRAGMENT_SHADER);
	m_shader->Link();
	m_shader->Use(); //active


	m_vertexArray = new VertexArray(m_engine);
	m_vertexArray->CreateBuffer(5 * sizeof(GLfloat), sizeof(cube_vertices)/(5 * sizeof(GLfloat)),(void*)cube_vertices);
	m_vertexArray->CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(indices)/sizeof(GLushort), (void*)indices);

	m_vertexArray->SetAttribute(0, 3, (5 * sizeof(GLfloat)), 0);
	m_vertexArray->SetAttribute(1, 2, (5 * sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));


	//position
	//GLuint position = glGetAttribLocation(shaderProgramID, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	//3 is how many points, FL_FLOAT is what datatype 
	//8 is the stride. how many is it to the next

	//color
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//UV
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)( 5 * sizeof(GLfloat)));

	///////

	//Texture
	GLuint textureID = Material::LoadTexture(m_engine->Get<FileSystem>()->GetPathname() + "face.png");
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);

	//m_mxProjection = glm::ortho(0.0f, (float)m_engine->Get<Renderer>()->GetWidth(), (float)m_engine->Get<Renderer>()->GetHeight(),0.0f);
	//^^ 2D

	//vv 3D
	m_mxProjection = glm::perspective(glm::radians(45.0f), (float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight(), 0.01f, 1000.0f);
	
	m_mxModel = glm::mat4(1.0f);
	m_transform.scale = glm::vec3(10.0f); //scaling
	
	m_camera = new Camera(this);
//m_transform.translation = glm::vec3(400.0f, 300.0f,0.0f); //position, makes it in the middle of the screen

	return true;
}

void Scene02::Shutdown()
{
}

void Scene02::Update()
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

	m_camera->SetView(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//m_mxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	                                            //^^ How close the camera is

	m_transform.Update();

	glm::mat4 mxMP = m_mxProjection * m_camera->transform.matrix * m_mxModel;

	m_shader->SetUniform("model", mxMP);
}

void Scene02::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();
	
	m_vertexArray->Draw(GL_TRIANGLES);

	
	m_engine->Get<Renderer>()->SwapBuffer();

}
