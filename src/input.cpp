#include "input.h"
#include <unordered_map>

#include <SDL2/SDL.h>

using KeyMap = std::unordered_map<uint32_t, bool>;
using ButtonMap = std::unordered_map<uint8_t, bool>;

static SDL_GameController* controller = nullptr;

static KeyMap keymap;
static KeyMap oldKeyMap;

static ButtonMap buttonMap;
static ButtonMap oldButtonMap;

template<typename Map, typename T>
static bool checkMap(Map& k, T key)
{
	auto itt = k.find(key);
	if(itt == k.end())
		return false;

	return itt->second;
}

void openController()
{
	for(int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if(SDL_IsGameController(i))
		{
			controller = SDL_GameControllerOpen(i);
			break;
		}
	}
}

void closeController()
{
	if(controller)
		SDL_GameControllerClose(controller);
}

void updateOldMaps()
{
	oldKeyMap = keymap;
	oldButtonMap = buttonMap;
}

void setKeyState(uint32_t key, bool state)
{
	keymap[key] = state;
}

bool isKeyDown(uint32_t key)
{
	return checkMap(keymap, key);
}

bool isKeyPressed(uint32_t key)
{
	return checkMap(keymap, key) && !checkMap(oldKeyMap, key);
}

bool isKeyReleased(uint32_t key)
{
	return !checkMap(keymap, key) && checkMap(oldKeyMap, key);
}

void setControllerButtonState(uint8_t button, bool state)
{
	buttonMap[button] = state;
}

bool isControllerButtonDown(uint8_t button)
{
	return checkMap(buttonMap, button);
}

bool isControllerButtonPressed(uint8_t button)
{
	return checkMap(buttonMap, button) && !checkMap(oldButtonMap, button);
}

bool isControllerButtonReleased(uint8_t button)
{
	return !checkMap(buttonMap, button) && checkMap(oldButtonMap, button);
}
