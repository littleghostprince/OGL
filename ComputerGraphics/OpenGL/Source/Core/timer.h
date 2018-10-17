#pragma once 
#include "engine.h"

class Timer : public System
{
public:
	Timer(Engine* engine) : System(engine) {}
	virtual ~Timer() {}

	bool Initialize();
	void Shutdown();
	void Update();

	const char* Name() { return "Timer"; }

	float DeltaTime() const { return m_dt * m_timeScale; }
	float UnscaledDeltaTime() const { return m_dt; }

	void SetTimeScale(float timeScale) { m_timeScale = timeScale; }
	float GetTimeScale() const { return m_timeScale; }
	void Pause() { m_paused = true; }
	void UnPause() { m_paused = false; }
	bool IsPaused() const{ return m_paused; }

	void Reset();

private:
	float m_dt = 0.0f;
	float m_timeScale = 0.0f;
	Uint32 m_prevTicks = 0;
	bool m_paused;
};
