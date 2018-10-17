#pragma once
#include "core/engine.h"


struct Transform
{
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 matrix = glm::mat4(1.0f);

	void Update()
	{
		glm::mat4 mxt  = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mxrx = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 mxry = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mxrz = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 mxs  = glm::scale(glm::mat4(1.0f), scale);

		matrix = mxt * (mxrz * mxry * mxrx) * mxs;
	}
};