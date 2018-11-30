#pragma once
#include "core/scene.h"
#include <vector>

class Scene09 : public Scene
{
public:
	Scene09(class Engine* engine) : Scene(engine) {}
	virtual ~Scene09() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene09"; }
private: 
	int m_lightIndex = 0;
private:
	glm::vec2 m_uvOffset = glm::vec2(0.0f, 0.0f);

};