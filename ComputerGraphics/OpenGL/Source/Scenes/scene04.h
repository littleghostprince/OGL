#pragma once
#include "core/scene.h"
#include <vector>

class Scene04 : public Scene
{
public:
	Scene04(class Engine* engine) : Scene(engine) {}
	virtual ~Scene04() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene03"; }

private:
	glm::vec2 m_uvOffset = glm::vec2(0.0f, 0.0f);

};