#include "Scenes/scene10.h"
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
bool Scene10::Initialize()
{
	//camera
	Camera * camera = this->CreateObject<Camera>("Camera");
	camera->Initialize();
	camera->type = Camera::eType::EDITOR;
	camera->SetView(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f));

	//RTT Camera
	camera = this->CreateObject<Camera>("camera_rtt");
	camera->Initialize(); 
	camera->SetView(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetProjection(45.0f, 1.0f, 0.01, 1000.0f);
	camera->type = Camera::eType::LOOK_AT;

	//point light
		PointLight* point_light = this->CreateObject<PointLight>();
		
		glm::vec3 position = glm::vec3(1.0f);
		glm::vec3 color = glm::vec3(1.0f);

		point_light->diffuse = color;
		point_light->specular = glm::vec3(1.0f);
		point_light->transform.translation = position;

	//MODEL

	//sphere
	Model * model = this->CreateObject<Sphere>("model1");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\ogre.obj");
	model->m_cameraName = "camera_rtt";

	//shader
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.vs", GL_VERTEX_SHADER);
	
	model->m_shader.Link();
	model->m_shader.Use(); //active

	model->m_material.ambient = glm::vec3(0.5f); //makes it less bright globally
	model->m_material.diffuse = glm::vec3(0.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "ogre_diffuse.bmp", GL_TEXTURE0);
	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", point_light->diffuse);
	model->m_shader.SetUniform("light.specular", point_light->specular);

	//SECOND
	model = this->CreateObject<Cube>("model2");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\cube.obj");
	model->transform.translation = glm::vec3(0.0f, 0.0f, -5.0f);
	model->transform.scale = glm::vec3(2.0f);
	model->m_cameraName = "Camera";

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.vs", GL_VERTEX_SHADER);

	model->m_shader.Link();
	model->m_shader.Use(); //active

	GLuint textureID = Material::CreateTexture(512, 512); 
	model->m_material.AddTexture(textureID, GL_TEXTURE0);
	model->m_material.SetTextures();

	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", point_light->diffuse);
	model->m_shader.SetUniform("light.specular", point_light->specular);

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	//Depth Buffer
	GLuint depthBufferID; glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 }; 
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
	assert(result == GL_FRAMEBUFFER_COMPLETE);

	return true;
}

void Scene10::Shutdown()
{
}

void Scene10::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	PointLight* light = this->CreateObject<PointLight>();

	//update objects
	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}
	//update model
	//Camera for first model
	Model* model = this->GetObject<Model>("model1");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(25.0f * dt), 0.0f));
	Camera* camera = GetObject<Camera>("camera_rtt");
	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);
    model->m_shader.Use();
	model->m_shader.SetUniform("light.position", lightPosition);

	//camera for second model 
	model = this->GetObject<Model>("model2");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(glm::radians(15.0f * dt), glm::radians(15.0f * dt), 0.0f));
	camera = GetObject<Camera>("Camera"); 
	lightPosition = light->GetPositionFromView(camera->transform.matrix);
	model->m_shader.Use(); 
	model->m_shader.SetUniform("light.position", lightPosition);

}

void Scene10::Render()
{
	glViewport(0, 0, 512, 512);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Model* model = this->GetObject<Model>("model1");
	model->Draw();

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->GetWidth(), m_engine->Get<Renderer>()->GetHeight());

	m_engine->Get<Renderer>()->ClearBuffer();
	model = this->GetObject<Model>("model2");
	model->Draw();

	PointLight* light = this->GetObject<PointLight>();
	light->Draw();

	m_engine->Get<Renderer>()->SwapBuffer();

}
