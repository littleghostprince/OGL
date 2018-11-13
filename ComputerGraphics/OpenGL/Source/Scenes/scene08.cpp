#include "Scenes/scene08.h"
#include "Objects/camera.h"
#include "Objects/light.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"
#include "Objects/spot_light.h"
#include "Math/transform.h"
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>

#define NUM_LIGHTS 5
bool Scene08::Initialize()
{
	m_engine->Get<Input>()->AddAction("point", SDL_SCANCODE_1, Input::eDevice::KEYBOARD);
	m_engine->Get<Input>()->AddAction("directional", SDL_SCANCODE_2, Input::eDevice::KEYBOARD);

	//camera
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

	//Spot light
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		Spot_light* spot_light = this->CreateObject<Spot_light>("spot_light");
		
		glm::vec3 position = glm::sphericalRand(4.0f);
		glm::vec3 color = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 1.0f));

		spot_light->diffuse = color;
		spot_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spot_light->cutoff = glm::radians(45.0f);
		spot_light->exponent = 10.0f;


	spot_light->transform.translation = glm::vec3(5.0f , 5.0f , 5.0f );
	spot_light->SetDirection(glm::vec3(1.0f, 1.0f, 1.0f), position);
	}

	auto lights = GetObjects<PointLight>();

	//sphere
	Model * model = this->CreateObject<Sphere>("model1");
	((Sphere*)(model))->Initialize(1.0f, 20, 20);
	model->transform.scale = glm::vec3(1.0f); 
	model->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);

	//shader
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	
	model->m_shader.Link();
	model->m_shader.Use(); //active

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.diffuse = glm::vec3(0.8f);
	model->m_material.specular = glm::vec3(0.8f);
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "uvgrid.jpg", GL_TEXTURE0);
	model->m_material.SetTextures();

	for (size_t i = 0; i < lights.size(); i++)
	{
		char uniformName[32];
		sprintf_s(uniformName, "lights[%d].diffuse", i);
		model->m_shader.SetUniform(uniformName, lights[i]->diffuse);
		sprintf_s(uniformName, "lights[%d].specular", i);
		model->m_shader.SetUniform(uniformName, lights[i]->specular);
		sprintf_s(uniformName, "lights[%d].cutoff", i);
		model->m_shader.SetUniform(uniformName, lights[i]->cutoff);
		sprintf_s(uniformName, "lights[%d].exponent", i);
		model->m_shader.SetUniform(uniformName, lights[i]->exponent);

		
	}

 model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));

	


	//MODEL 2

	model = this->CreateObject<Plane>("model2");
	((Plane*)(model))->Initialize(10.0f,10.0f,10,10,5.0f,5.0f);
	model->transform.translation = glm::vec3(0.0f,-2.0f,0.0f); //scaling

	//shader

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.Link();
	model->m_shader.Use(); //active
	

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.diffuse = glm::vec3(0.8f);
	model->m_material.specular = glm::vec3(0.8f);
	model->m_material.shininess = 1.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "uvgrid.jpg", GL_TEXTURE0);

	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));

	return true;
}

void Scene08::Shutdown()
{
}

void Scene08::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();
	//update model
	Model* model = this->GetObject<Model>("model1");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(45.0) * dt, 0.0f));

	Camera * camera = this->GetObject<Camera>();

	auto lights = GetObjects<Light>();
	for (size_t i = 0; i < lights.size(); i++)
	{
		//calculate light position 
	glm::vec4 lightPosition = ((Spot_light*)lights[i])->GetPositionFromView(camera->transform.matrix);
	glm::vec3 lightDirection = ((Spot_light*)lights[i])->GetDirectionFromView(camera->transform.matrix);
	
		auto models = GetObjects<Model>();
		for (auto model : models)
		{
			//calculate light position 
			char uniformName[32];
			model->m_shader.Use();
			sprintf_s(uniformName, "lights[%d].position", i);
			model->m_shader.SetUniform(uniformName, lightPosition);
			sprintf_s(uniformName, "lights[%d].direction", i);
			model->m_shader.SetUniform(uniformName, lightDirection);
			sprintf_s(uniformName, "lights[%d].diffuse", i);
			model->m_shader.SetUniform(uniformName, lights[i]->diffuse);
			sprintf_s(uniformName, "lights[%d].specular", i);
			model->m_shader.SetUniform(uniformName, lights[i]->specular);
			sprintf_s(uniformName, "lights[%d].exponent", i);
			model->m_shader.SetUniform(uniformName, lights[i]->exponent);
		
		}

	}

	//update objects
	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

}

void Scene08::Render()
{

	m_engine->Get<Renderer>()->ClearBuffer();
	
	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();

	for (Renderable* renderable : renderables)
	{
		renderable->Draw(); //draws all our objects
	}

	m_engine->Get<Renderer>()->SwapBuffer();

}
