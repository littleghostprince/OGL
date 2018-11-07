#include "Scenes/scene06.h"
#include "Objects/camera.h"
#include "Objects/light.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"
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


bool Scene06::Initialize()
{
	m_engine->Get<Input>()->AddAction("point", SDL_SCANCODE_1, Input::eDevice::KEYBOARD);
	m_engine->Get<Input>()->AddAction("directional", SDL_SCANCODE_2, Input::eDevice::KEYBOARD);

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
	Model * model = this->CreateObject<Sphere>("model1");
	((Sphere*)(model))->Initialize(1.0f, 20.0f, 20.0f);
	model->transform.scale = glm::vec3(1.0f); //scaling
	model->transform.translation = glm::vec3(-4.0f, -4.0f, 0.0f);
											//left, down


	//fog shader
	
	//shader
	
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	
	model->m_shader.Link();
	model->m_shader.Use(); //active

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "flower.png", GL_TEXTURE0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

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

	model = this->CreateObject<Plane>("model2");
	((Plane*)(model))->Initialize(10.0f,10.0f,10,10,5.0f,5.0f);
	model->transform.translation = glm::vec3(0.0f,-5.0f,0.0f); //scaling

	//shader

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.Link();
	model->m_shader.Use(); //active



	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "cobble-diffuse.tga", GL_TEXTURE0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));
	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);

	//model->m_shader.SetUniform("uv_Scale", glm::vec2(1.0f, 1.0f));

	return true;
}

void Scene06::Shutdown()
{
}

void Scene06::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();
	//update model
	Model* model = this->GetObject<Model>("model1");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(45.0) * dt, 0.0f));

	Light* light = this->GetObject<PointLight>();
	light->transform.rotation = light->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(-45.0) * dt, 0.0f));
	light->transform.translation = light->transform.rotation * glm::vec3(3.0f, 3.0f, 3.0f);

	if (m_engine->Get<Input>()->GetActionButton("point") == Input::eButtonState::HELD) m_lightIndex = 0;
	if (m_engine->Get<Input>()->GetActionButton("directional") == Input::eButtonState::HELD) m_lightIndex = 1;


	

	//update objects
	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}
	Camera * camera = this->GetObject<Camera>();

	//update lights
	glm::vec4 lightPosition;
	if (m_lightIndex == 0)
	{
		light = this->GetObject<PointLight>();
		lightPosition = ((PointLight*)light)->GetPositionFromView(camera->transform.matrix);
		//code
	}
	if (m_lightIndex == 1)
	{
		light = this->GetObject<PointLight>();
		lightPosition = ((DirectionalLight*)light)->GetDirectionFromView(camera->transform.matrix);
		//code
	}
	std::vector<Model*> models = this->GetObjects<Model>();

	for (Model* model : models)
	{
		
		model->m_shader.Use();

		//update light
		model->m_shader.SetUniform("light.position", lightPosition);
	   
		//m_uvOffset.y = m_uvOffset.y + (0.1f*dt);
		//model->m_shader.SetUniform("uv_offset", m_uvOffset);

	}

	
}

void Scene06::Render()
{

	m_engine->Get<Renderer>()->ClearBuffer();
	
	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();

	for (Renderable* renderable : renderables)
	{
		renderable->Draw(); //draws all our objects
	}
	
	m_engine->Get<Renderer>()->SwapBuffer();

}
