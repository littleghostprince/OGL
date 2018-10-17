#pragma once
#include <glad\glad.h>
#include "sdl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL.h"

#include <vector>
#include <map>

#include "system.h"
#include "input.h"
#include "timer.h"
#include "file_system.h"
#include "Renderer/renderer.h"
#include "Renderer/shader.h"
#include "Renderer/vertex_array.h"
#include "Math/transform.h"

class Engine
{
public:
	Engine() {}
	~Engine() {}

	bool Initialize();
	void Shutdown();
	void Update();
	
	// get engine system
	template <typename T>
	T* Get()
	{
		for (System* system : m_systems)
		{
			if (dynamic_cast<T*>(system) != nullptr)
			{
				return static_cast<T*>(system);
			}
		}
		return nullptr;
	}

	bool ForceQuit() { return m_quit; }

private:
	std::vector<class System*> m_systems;
	bool m_quit = false;
};