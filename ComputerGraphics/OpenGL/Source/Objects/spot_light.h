#pragma once
#include "Objects/light.h"

class Spot_light : public Light
{
	public:
		Spot_light(Scene* scene, const std::string& name = "");

		void Update() override;
		void Draw() override;

		void SetDirection(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 GetDirectionFromView(const glm::mat4& mxView);
		glm::vec4 GetPositionFromView(const glm::mat4& mxView);

	protected:
		class Cube* m_model = nullptr;

};

