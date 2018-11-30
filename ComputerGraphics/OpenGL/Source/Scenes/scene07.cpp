#include "Scenes/scene07.h"
#include "Objects/camera.h"
#include "Objects/light.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"
#include "Objects/spot_light.h"
#include "Math/transform.h"

bool Scene07::Initialize()
{
	//m_engine->Get<Input>()->AddAction("point", SDL_SCANCODE_1, Input::eDevice::KEYBOARD);
	//m_engine->Get<Input>()->AddAction("directional", SDL_SCANCODE_2, Input::eDevice::KEYBOARD);

	//camera
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

	//Spot light
	Spot_light* spot_light = this->CreateObject<Spot_light>("spot_light");
	spot_light->diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
	spot_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spot_light->cutoff = glm::radians(45.0f);
	spot_light->exponent = 10.0f;

	spot_light->transform.translation = glm::vec3(5.0f , 5.0f , 5.0f );
	spot_light->SetDirection(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//sphere
	Model * model = this->CreateObject<Sphere>("model1");
	((Sphere*)(model))->Initialize(1.0f, 20, 20);
	model->transform.scale = glm::vec3(1.0f); //scaling
	model->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
											//left, down

	//fog shader
	
	//shader
	
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_spotlight.vs", GL_VERTEX_SHADER);
	
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
	model->m_shader.SetUniform("light.cutoff", spot_light->cutoff);
	model->m_shader.SetUniform("light.exponent", spot_light->exponent);


	//MODEL 2

	model = this->CreateObject<Plane>("model2");
	((Plane*)(model))->Initialize(10.0f,10.0f,10,10,5.0f,5.0f);
	model->transform.translation = glm::vec3(0.0f,-2.0f,0.0f); //scaling

	//shader

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_spotlight.vs", GL_VERTEX_SHADER);
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
	model->m_shader.SetUniform("light.cutoff", spot_light->cutoff);
	model->m_shader.SetUniform("light.exponent", spot_light->exponent);

	return true;
}

void Scene07::Shutdown()
{
}

void Scene07::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();
	//update model
	Model* model = this->GetObject<Model>("model1");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(45.0) * dt, 0.0f));

	Camera * camera = this->GetObject<Camera>();

	Spot_light* light = this->GetObject<Spot_light>();
//	light->transform.rotation = light->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(-45.0) * dt, 0.0f));
	//light->transform.translation = light->transform.rotation * glm::vec3(1.0f, 1.0f, 1.0f);
	
	glm::vec4 lightPosition = ((Spot_light*)light)->GetPositionFromView(camera->transform.matrix);
	glm::vec3 lightDirection = ((Spot_light*)light)->GetDirectionFromView(camera->transform.matrix);
	//update objects
	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	//update lights
	
	std::vector<Model*> models = this->GetObjects<Model>();

	for (Model* model : models)
	{
		
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", lightPosition);
		model->m_shader.SetUniform("light.direction", lightDirection);
		model->m_shader.SetUniform("light.diffuse", light->diffuse);
		model->m_shader.SetUniform("light.specular", light->specular);
		model->m_shader.SetUniform("light.cutoff", light->cutoff);
		model->m_shader.SetUniform("light.exponent", light->exponent);
	}

	m_engine->Get<UI>()->Start();
	ImGui::Begin("Window");
	ImGui::Text("Hello World");
	ImGui::ColorEdit3("Diffuse", (float*)&light->diffuse);
	ImGui::ColorEdit3("Specular", (float*)&light->specular);
	ImGui::End();

	ImGui::Begin("Transform");
	model = this->GetObject<Model>("model1");
	Transform::Edit(&model->transform);
	ImGui::End();
}

void Scene07::Render()
{

	m_engine->Get<Renderer>()->ClearBuffer();
	
	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();

	for (Renderable* renderable : renderables)
	{
		renderable->Draw(); //draws all our objects
	}
	m_engine->Get<UI>()->Draw();

	m_engine->Get<Renderer>()->SwapBuffer();

}
