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
	camera->SetView(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f));

	//sphere
	Model * model = this->CreateObject<Model>("model1");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\cube.obj");
	model->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	model->transform.scale = glm::vec3(30.0f);

	//shader
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\cubemap.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\cubemap.vs", GL_VERTEX_SHADER);

	model->m_shader.Link();
	model->m_shader.Use(); //active

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.diffuse = glm::vec3(0.8f);
	model->m_material.specular = glm::vec3(0.8f);
	model->m_material.shininess = 50.0f;

	std::vector<std::string> suffixes = { "_posx","_negx","_posy","_negy","_posz","_negz" };
	model->m_material.AddTextureCube(m_engine->Get<FileSystem>()->GetPathname() + "lancellotti", suffixes, "jpg", GL_TEXTURE0);
	model->m_shader.AddSubroutine(GL_VERTEX_SHADER, "skybox");

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	//model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));
	model = this->CreateObject<Model>("model2");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\sphere.obj");
	model->transform.scale = glm::vec3(1.0f);
	model->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\cubemap.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\cubemap.vs", GL_VERTEX_SHADER);
	model->m_shader.Link();
	model->m_shader.Use(); //active

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.diffuse = glm::vec3(0.8f);
	model->m_material.specular = glm::vec3(0.8f);
	model->m_material.shininess = 1.0f;

	model->m_material.AddTextureCube(m_engine->Get<FileSystem>()->GetPathname() + "lancellotti", suffixes, "jpg", GL_TEXTURE0);
	model->m_shader.AddSubroutine(GL_VERTEX_SHADER, "reflection");

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	//model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));
	return true;
}

void Scene08::Shutdown()
{
}

void Scene08::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();
	//update model
	Model* model = this->GetObject<Model>("model2");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(45.0) * dt, 0.0f));

	Camera * camera = this->GetObject<Camera>();

	//update objects
	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
		model->m_shader.SetUniform("model_matrix", model->transform.matrix);
		model->m_shader.SetUniform("camera_position", camera->transform.translation);
	}
}

void Scene08::Render()
{
	glDisable(GL_CULL_FACE);
	m_engine->Get<Renderer>()->ClearBuffer();
	
	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();

	for (Renderable* renderable : renderables)
	{
		renderable->Draw(); //draws all our objects
	}

	m_engine->Get<Renderer>()->SwapBuffer();

}
