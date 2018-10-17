#pragma once
#include "engine.h"

class Scene
{
public:
	Scene(class Engine* engine) : m_engine(engine) {}
	virtual ~Scene() {}

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual const char* Name() = 0;

protected:
	class Engine* m_engine;
};