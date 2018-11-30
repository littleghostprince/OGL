#pragma once

#include "core/object.h"
#include "core/engine.h"

class Camera : public Object
{
public:
	Camera(Scene* scene, const std::string& name = "") : Object(scene, name) {}

	void Initialize();
	void Update();

	void SetView(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
	void SetProjection(float fov, float nearClip, float farClip);
	void SetProjection( float fov, float aspectRatio, float nearClip, float farClip);

	//void Edit() override;

	enum eType
	{
		EDITOR,
		LOOK_AT
	};

	void UpdateLookAt(glm::vec3& translate, glm::vec3& rotate);
	void UpdateEditor(glm::vec3& translate, glm::vec3& rotate);
public:
	glm::mat4 projection;
	float m_rate = 5.0f;
	eType type = eType::EDITOR;
};
