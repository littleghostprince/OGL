#include "Scenes/scene06.h"
#include "Objects/camera.h"
#include "Objects/light.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"

bool Scene06::Initialize()
{
	m_engine->Get<Input>()->AddAction("point", SDL_SCANCODE_1, Input::eDevice::KEYBOARD);
	m_engine->Get<Input>()->AddAction("directional", SDL_SCANCODE_2, Input::eDevice::KEYBOARD);

	//camera
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

//point light
	PointLight * light = this->CreateObject<PointLight>("point_light");
	light->transform.translation = glm::vec3(0.5f, 0.5f, 0.5f);
	light->diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	light->specular = glm::vec3(0.5f);

	//Directional light
	DirectionalLight* directional_light = this->CreateObject<DirectionalLight>("directional_light");
	directional_light->SetDirection(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f));
	directional_light->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	directional_light->specular = glm::vec3(0.5f);
	//sphere
	Model * model = this->CreateObject<Sphere>("model1");
	((Sphere*)(model))->Initialize(1.0f, 20.0f, 20.0f);
	model->transform.scale = glm::vec3(1.0f); //scaling
	model->transform.translation = glm::vec3(-4.0f, -4.0f, 0.0f);
											//left, down


	//fog shader
	
	//shader
	
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi.vs", GL_VERTEX_SHADER);
	
	model->m_shader.Link();
	model->m_shader.Use(); //active

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "flower.png", GL_TEXTURE0);
	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	//model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));

	//MODEL 2

	model = this->CreateObject<Plane>("model2");
	((Plane*)(model))->Initialize(10.0f,10.0f,10,10,5.0f,5.0f);
	model->transform.translation = glm::vec3(0.0f,-5.0f,0.0f); //scaling

	//shader

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi.vs", GL_VERTEX_SHADER);
	model->m_shader.Link();
	model->m_shader.Use(); //active



	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "cobble-diffuse.tga", GL_TEXTURE1);

	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));

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

	light = this->GetObject<DirectionalLight>();
	light->transform.rotation = light->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(-45.0) * dt, 0.0f));
	light->transform.translation = light->transform.rotation * glm::vec3(3.0f, 3.0f, 3.0f);

	if (m_engine->Get<Input>()->GetActionButton("point") == Input::eButtonState::HELD)
	{
		m_lightIndex = 0;
	}
	if (m_engine->Get<Input>()->GetActionButton("directional") == Input::eButtonState::HELD)
	{
		m_lightIndex = 1;
	
	}

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
	}
	if (m_lightIndex == 1)
	{
		light = this->GetObject<DirectionalLight>();
		lightPosition = ((DirectionalLight*)light)->GetDirectionFromView(camera->transform.matrix);
	}
	std::vector<Model*> models = this->GetObjects<Model>();

	for (Model* model : models)
	{
		
		model->m_shader.Use();

		//update light
		model->m_shader.SetUniform("light.position", lightPosition);
		model->m_shader.SetUniform("light.diffuse", light->diffuse);
		model->m_shader.SetUniform("light.specular", light->specular);
	 

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
