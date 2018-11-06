#pragma once
#include "core/scene.h"
#include <vector>

class Scene05 : public Scene
{
public:
	Scene05(class Engine* engine) : Scene(engine) {}
	virtual ~Scene05() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene05"; }

private:
	glm::vec2 m_uvOffset = glm::vec2(0.0f, 0.0f);

};