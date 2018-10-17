#include "input.h"
#include <string.h>


bool Input::Initialize()
{

	SDL_GetKeyboardState(&m_numKeys);
	m_keystate = new Uint8[m_numKeys];
	m_prevKeystate = new Uint8[m_numKeys];

	const Uint8* keystate = SDL_GetKeyboardState(nullptr);
	memcpy(m_keystate, keystate, m_numKeys);
	memcpy(m_prevKeystate, m_keystate, m_numKeys);

	m_mouseButtonState = SDL_GetMouseState(nullptr, nullptr);
	m_prevMouseButton = m_mouseButtonState;

		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i))
			{
				ControllerInfo controllerInfo;
				controllerInfo.controller = SDL_GameControllerOpen(i);
				memset(controllerInfo.buttonState, 0, SDL_CONTROLLER_BUTTON_MAX);
				memset(controllerInfo.prevButtonState, 0, SDL_CONTROLLER_BUTTON_MAX);
				memset(controllerInfo.axis, 0, SDL_CONTROLLER_AXIS_MAX);
				memset(controllerInfo.prevAxis, 0, SDL_CONTROLLER_AXIS_MAX);

				m_controllers.push_back(controllerInfo);
			}
		}
	return true;
}

void Input::Shutdown()
{
	delete m_keystate;
	delete m_prevKeystate;
}

void Input::Update()
{
	//keyboard
	memcpy(m_prevKeystate, m_keystate, m_numKeys);
	const Uint8* keystate = SDL_GetKeyboardState(nullptr);
	memcpy(m_keystate, keystate, m_numKeys);

	//mouse
	m_prevMouseButton = m_mouseButtonState;
	m_mouseButtonState = SDL_GetMouseState(nullptr, nullptr);
	m_prevMousePosition = m_mousePosition;
	SDL_Point axis;
	SDL_GetMouseState(&axis.x,&axis.y);
	m_mousePosition = glm::vec2(axis.x,axis.y);

	//controller
	for (ControllerInfo& controllerInfo : m_controllers)
	{
		memcpy(controllerInfo.prevButtonState, controllerInfo.buttonState, SDL_CONTROLLER_BUTTON_MAX);
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
		{
			controllerInfo.buttonState[i] = SDL_GameControllerGetButton(controllerInfo.controller, (SDL_GameControllerButton) i);
		}

		memcpy(controllerInfo.prevAxis, controllerInfo.axis, SDL_CONTROLLER_AXIS_MAX);
		for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
		{
			Sint16 value = SDL_GameControllerGetAxis(controllerInfo.controller, (SDL_GameControllerAxis)i);
			controllerInfo.axis[i] = value / float (SDL_MAX_SINT16);
		}
	}
}

void Input::AddAction(const std::string & action, int id, eDevice device, int index)
{
	auto iter = m_actions.find(action);
	if (iter == m_actions.end())
	{
		InputInfo inputInfo = { device, id, index };
		m_actions[action] = inputInfo;
	}
}

Input::eButtonState Input::GetActionButton(const std::string & action)
{
	eButtonState state = eButtonState::IDLE;

	auto iter = m_actions.find(action);
	if (iter != m_actions.end())
	{
		InputInfo inputInfo = iter->second;
		state = GetButtonState(inputInfo.id, inputInfo.device, inputInfo.index);
	}

	return state; 
}

float Input::GetActionAxisAbsolute(const std::string & action)
{
	float axis = 0.0;

	auto iter = m_actions.find(action);
	if (iter != m_actions.end())
	{
		InputInfo inputInfo = iter->second;
		axis = GetAxisAbsolute(inputInfo.id, inputInfo.device, inputInfo.index);
	}
	return axis;
}

float Input::GetActionAxisRelative(const std::string & action)
{
	float axis = 0.0;

	auto iter = m_actions.find(action);
	if (iter != m_actions.end())
	{
		InputInfo inputInfo = iter->second;
		axis = GetAxisRelative(inputInfo.id, inputInfo.device, inputInfo.index);
	}
	return axis;
	
}

/*
InputManager::eButtonState InputManager::GetButtonAction(SDL_Scancode scancode)
{
	eButtonState action = eButtonState::IDLE;
	
	if (m_keystate[scancode])
	{
		action = (m_prevKeystate[scancode]) ? eButtonState::HELD : eButtonState::PRESSED;
	}
	else
	{
		action = (m_prevKeystate[scancode]) ? eButtonState::RELEASED : eButtonState::IDLE;
	}
	return action;
}
*/
/*
Input::eButtonState Input::GetMouseButtonAction(int button)
{
	eButtonState action = eButtonState::IDLE;

	return action;
}

*/


Input::eButtonState Input::GetButtonState(int id, eDevice device, int index)
{
	eButtonState state = eButtonState::IDLE;

	bool buttonDown = GetButtonDown(id, device, index);
	bool prevButtonDown = GetPreviousButtonDown(id, device, index);

	if (buttonDown)
	{
		state = (prevButtonDown) ? eButtonState::HELD : eButtonState::PRESSED;
	}
	else
	{
		state = (prevButtonDown) ? eButtonState::RELEASED : eButtonState::IDLE;
	}

	return state;
}

float Input::GetAxisAbsolute(int id, eDevice device, int index)
{
	float axis = 0.0f;

	switch (device)
	{
	case eDevice::KEYBOARD:
		assert(0);
		break;
	case eDevice::MOUSE:
		axis = m_mousePosition[id];
		break;
	case eDevice::CONTROLLER:
		assert(index < (int)m_controllers.size());
		axis = m_controllers[index].axis[id];
		break;
	}

	return axis;
}

float Input::GetAxisRelative(int id, eDevice device, int index)
{

	float axis = 0.0f;

	switch (device)
	{
	case eDevice::KEYBOARD:
		assert(0);
		break;
	case eDevice::MOUSE:
		axis = m_mousePosition[id] - m_prevMousePosition[id];
		break;
	case eDevice::CONTROLLER:
		assert(index < (int) m_controllers.size());
		axis = m_controllers[index].axis[id] - m_controllers[index].prevAxis[id];
		break;
	}

	return axis;
}

bool Input::GetButtonDown(int id, eDevice device, int index)
{
	bool buttonDown = false; 

	switch (device)
	{
	case eDevice::KEYBOARD:
		buttonDown = m_keystate[id];
		break;
	case eDevice::MOUSE:
		buttonDown = m_mouseButtonState & SDL_BUTTON(id);
		break;
	case eDevice::CONTROLLER:
		assert(index < (int)m_controllers.size());
		buttonDown = m_controllers[index].buttonState[id];
		break;
	}

	return buttonDown;
}

bool Input::GetPreviousButtonDown(int id, eDevice device, int index)
{

	bool buttonDown = false;

	switch (device)
	{
	case eDevice::KEYBOARD:
		buttonDown = m_prevKeystate[id];
		break;
	case eDevice::MOUSE:
		buttonDown = m_prevMouseButton & SDL_BUTTON(id);
		break;
	case eDevice::CONTROLLER:
		assert(index < (int)m_controllers.size());
		buttonDown = m_controllers[index].prevButtonState[id];
		break;
	}

	return buttonDown;
}
