#pragma once
#include "engine.h"

class Input : public System
{
public:
	enum eButtonState
	{
		IDLE,
		PRESSED,
		HELD,
		RELEASED
	};

	enum eDevice
	{
		KEYBOARD,
		MOUSE,
		CONTROLLER
	};

	enum eAxis
	{
		X,
		Y
	};
	
	struct InputInfo
	{
		eDevice device;
		int id;
		int index;
	};

	struct ControllerInfo
	{
		SDL_GameController* controller; 
		
		Uint8 buttonState[SDL_CONTROLLER_BUTTON_MAX];
		Uint8 prevButtonState[SDL_CONTROLLER_BUTTON_MAX];

		float axis[SDL_CONTROLLER_AXIS_MAX];
		float prevAxis[SDL_CONTROLLER_AXIS_MAX];

	};

public:
	Input(Engine* engine) : System(engine) {};
	virtual ~Input() {};

	bool Initialize();
	void Shutdown();
	void Update();

	const char* Name() { return "Input"; }

	void AddAction(const std::string& action, int id, eDevice device, int index=0);
	eButtonState GetActionButton(const std::string& action);
	float GetActionAxisAbsolute(const std::string& action);
	float GetActionAxisRelative(const std::string& action);

	//eButtonState GetButtonAction(SDL_Scancode scancode);
	//eButtonState GetMouseButtonAction(int button);

	eButtonState GetButtonState(int id, eDevice device = eDevice::KEYBOARD, int index = 0);
	
	float GetAxisAbsolute(int id, eDevice device = eDevice::MOUSE, int index = 0);
	float GetAxisRelative(int id, eDevice device = eDevice::MOUSE, int index = 0);


protected:
	bool GetButtonDown(int id, eDevice device, int index = 0);
	bool GetPreviousButtonDown(int id, eDevice device, int index = 0);


private:

	//Keyboard
	Uint8* m_prevKeystate = 0;
	Uint8* m_keystate = 0;
	int m_numKeys = 0 ;
	
	//Mouse 
	Uint32 m_mouseButtonState;
	Uint32 m_prevMouseButtonState;
	glm::vec2 m_mousePosition;
	glm::vec2 m_prevMousePosition;
	
	//Controller
	std::vector<ControllerInfo> m_controllers;
	std::map<std::string, InputInfo> m_actions;
};

