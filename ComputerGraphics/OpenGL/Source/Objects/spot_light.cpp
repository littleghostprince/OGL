#include "spot_light.h"
#include "core/scene.h"
#include "Objects/cube.h"

Spot_light::Spot_light(Scene* scene, const std::string& name) : Light(scene, name)
{
	m_model = new Cube(scene);
	m_model->Initialize(0.1f);

	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "shaders\\basic_color.vs", GL_VERTEX_SHADER);
	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "shaders\\basic_color.fs", GL_FRAGMENT_SHADER);
	m_model->m_shader.Link();
}

void Spot_light::Update()
{
	transform.Update();
	m_model->transform = transform;
	m_model->Update();
	m_model->m_shader.SetUniform("material.diffuse", diffuse);
}

void Spot_light::Draw()
{
	m_model->Draw();
}

void Spot_light::SetDirection(const glm::vec3 & position, const glm::vec3 & target, const glm::vec3 & up)
{
	transform.matrix = glm::inverse(glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f)));
	transform.rotation = glm::quat_cast(transform.matrix);
	transform.translation = position;
}

glm::vec3 Spot_light::GetDirectionFromView(const glm::mat4 & mxView)
{
	return glm::mat3(mxView * transform.matrix) * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec4 Spot_light::GetPositionFromView(const glm::mat4& mxView)
{
	return mxView * transform.matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
