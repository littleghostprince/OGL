#pragma once
#include "core/scene.h"
#include <vector>

class Scene12 : public Scene
{
public:
	Scene12(class Engine* engine) : Scene(engine) {}
	virtual ~Scene12() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Shadow "; }
private: 
	int m_lightIndex = 0;
private:
	glm::vec2 m_uvOffset = glm::vec2(0.0f, 0.0f);
	GLuint m_framebuffer;
	float m_time = 0.0f;
	GLuint m_depthBuffer = 0;
	Shader* m_depthShader = nullptr;
};