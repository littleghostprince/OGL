#include "Scenes/scene05.h"
#include "Objects/camera.h"
#include "Objects/light.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"

//#define MULTI_TEXTURE
//#define SPECULAR_MAP

static float cube_vertices[] = {
	// Front
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
	// Right
	 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 // Back
	 -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	  // Left
	  -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	  -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	  -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	  -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	  // Bottom
	  -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	  -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	   1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	   // Top
	   -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
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


bool Scene05::Initialize()
{
	//camera
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

//point light
	PointLight * light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(0.5f, 0.5f, 0.5f);
	light->diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	light->specular = glm::vec3(0.5f);

	//sphere
	Model * model = this->CreateObject<Sphere>();
	((Sphere*)(model))->Initialize(1.0f, 20.0f, 20.0f);
	model->transform.scale = glm::vec3(1.0f); //scaling
	model->transform.translation = glm::vec3(-4.0f, -4.0f, 0.0f);
											//left, down


	//fog shader
	
	//shader
	
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_fog.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_fog.vs", GL_VERTEX_SHADER);
	
	model->m_shader.Link();
	model->m_shader.Use(); //active

	
	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);


	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "flower.png", GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));
	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular",light->specular);

	model->m_shader.SetUniform("uv_Scale", glm::vec2(1.0f, 1.0f));
	
	model->m_shader.SetUniform("fog.distance_min", 10.0f);
	model->m_shader.SetUniform("fog.distance_max", 50.0f);
	model->m_shader.SetUniform("fog.color", glm::vec3(0.5f, 0.5f, 0.5f));


	//MODEL 2

	model = this->CreateObject<Cube>();
	((Cube*)(model))->Initialize(1.0f);
	model->transform.scale = glm::vec3(1.0f); //scaling


	//fog shader

	//shader

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_fog.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_fog.vs", GL_VERTEX_SHADER);
	model->m_shader.Link();
	model->m_shader.Use(); //active


	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "flower.png", GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));
	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);

	model->m_shader.SetUniform("uv_Scale", glm::vec2(1.0f, 1.0f));

	model->m_shader.SetUniform("fog.distance_min", 10.0f);
	model->m_shader.SetUniform("fog.distance_max", 50.0f);
	model->m_shader.SetUniform("fog.color", glm::vec3(0.5f, 0.5f, 0.5f));


	return true;
}

void Scene05::Shutdown()
{
}

void Scene05::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Camera * camera = this->GetObject<Camera>();
	camera->Update();
	
	//update light       
	PointLight* light = this->GetObject<PointLight>();
	light->Update();

	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);

	std::vector<Model*> models = this->GetObjects<Model>();

	for (Model* model : models)
	{
		model->m_shader.Use();

		
		model->m_shader.SetUniform("light.position", lightPosition);


		m_uvOffset.y = m_uvOffset.y + (0.1f*dt);
		model->m_shader.SetUniform("uv_offset", m_uvOffset);

		//update normals
		glm::mat3 mxNormal = glm::mat3(camera->transform.matrix * model->transform.matrix);
		mxNormal = glm::inverse(mxNormal);
		mxNormal = glm::transpose(mxNormal);
		model->m_shader.SetUniform("normal_matrix", mxNormal);

		//update model view
		glm::mat4 mxMV = camera->transform.matrix * model->transform.matrix;
		model->m_shader.SetUniform("model_view_matrix", mxMV);

		//update mvp
		model->transform.Update();
		glm::mat4 mxMP = camera->projection * camera->transform.matrix * model->transform.matrix;

		model->m_shader.SetUniform("model_view_projection_matrix", mxMP);
	}

	
}

void Scene05::Render()
{

	m_engine->Get<Renderer>()->ClearBuffer();
	
	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();

	for (Renderable* renderable : renderables)
	{
		renderable->Draw(); //draws all our objects
	}
	
	m_engine->Get<Renderer>()->SwapBuffer();

}
