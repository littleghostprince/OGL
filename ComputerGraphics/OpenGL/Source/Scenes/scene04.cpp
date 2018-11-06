#include "Scenes/scene04.h"
#include "Objects/camera.h"
#include "Objects/light.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"

#define MULTI_TEXTURE
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


bool Scene04::Initialize()
{
	//camera
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

	//m_mxProjection = glm::perspective(glm::radians(45.0f), (float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight(), 0.01f, 1000.0f);

	//sphere
	Sphere * model = this->CreateObject<Sphere>();
	model->Initialize(1.0f, 20.0f, 20.0f);
	model->transform.scale = glm::vec3(5.0f); //scaling

	//point light
	PointLight * light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);

	//light

	/*Light* light = this->CreateObject<Light>("pointlight");
	light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->transform.translation = glm::vec3(5.0f, 10.0f, 10.0f);*/

	//shader
	
	/*model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	*/

#ifdef SPECULAR_MAP
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_specular_map.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_specular_map.vs", GL_VERTEX_SHADER);
#elif defined MULTI_TEXTURE
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi.fs", GL_FRAGMENT_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi.vs", GL_VERTEX_SHADER);

#endif 
	
	model->m_shader.Link();
	model->m_shader.Use(); //active

	
	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);

	//m_mxProjection = glm::ortho(0.0f, (float)m_engine->Get<Renderer>()->GetWidth(), (float)m_engine->Get<Renderer>()->GetHeight(),0.0f);
	//^^ 2D

	//vv 3D
	
	
	
//m_transform.translation = glm::vec3(400.0f, 300.0f,0.0f); //position, makes it in the middle of the screen


	model->m_material.ambient = glm::vec3(0.2f); //makes it less bright globally
	model->m_material.diffuse = glm::vec3(1.0f,1.0f,1.0f);
	model->m_material.specular = glm::vec3(0.5f);
	model->m_material.shininess = 1.0f;

#ifdef SPECULAR_MAP
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "metal-diffuse.png", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "metal-specular.png",GL_TEXTURE1);
#elif defined MULTI_TEXTURE
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "metal-diffuse.png", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "moss.png", GL_TEXTURE1);
#endif


	/*model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "flower.png", GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	*/
	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));
	model->m_shader.SetUniform("light.diffuse", glm::vec3(1.0f, 0.0f, 1.0f));
	model->m_shader.SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	model->m_shader.SetUniform("uv_Scale", glm::vec2(1.0f, 1.0f));
	

	return true;
}

void Scene04::Shutdown()
{
}

void Scene04::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Sphere* model = this->GetObject<Sphere>();

	if (m_engine->Get<Input>()->GetActionButton("left") == Input::HELD)
	{
		model->transform.translation.x += 10.0f * dt;
	}
	if (m_engine->Get<Input>()->GetActionButton("right") == Input::HELD)
	{
		model->transform.translation.x -= 10.0f * dt;
	}

	model->transform.rotation = model->transform.rotation * glm::angleAxis(glm::radians(45.0f * dt), glm::vec3(0.0f, 1.0f, 0.0f));

	//model->transform.rotation.y = model->transform.rotation.y + glm::radians(45.0f * dt);

	model->transform.Update();

	Camera * camera = this->GetObject<Camera>();
	camera->Update();
	
	//update light                
	PointLight* light = this->GetObject<PointLight>();
	light->Update();

	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);
	model->m_shader.SetUniform("light.position", lightPosition);
	
	m_uvOffset.y = m_uvOffset.y + (0.1f*dt);
	model->m_shader.SetUniform("uv_offset", m_uvOffset);
	
	/*Light* light = this->GetObjects<Light>("pointlight");
	light->Update();
	m_shader->SetUniform("light.position", m_camera->transform.matrix * light->transform.matrix);

	*/

	//m_mxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	                                            //^^ How close the camera is

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

void Scene04::Render()
{

	m_engine->Get<Renderer>()->ClearBuffer();
	

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();

	for (Renderable* renderable : renderables)
	{
		renderable->Draw(); //draws all our objects
	}
	
	m_engine->Get<Renderer>()->SwapBuffer();

}
