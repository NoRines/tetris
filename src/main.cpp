#include <iostream>

#include <SDL2/SDL.h>

#include "utils.h"
#include "windefs.h"
#include "menu.h"
#include "game.h"
#include "input.h"
#include "statemanager.h"
#include "controls.h"

static SDL_Window* window;
static SDL_Renderer* renderer;

static int setup();
static void init();
static bool input();
static void update(float frameTime);
static void render();
static void cleanup();

int main(int argc, char** argv)
{
 	if(setup() < 0)
		return -1;

	using timeRes = std::chrono::microseconds;

	init();

	while(!input())
	{
		float frameTime = limitFps<timeRes, 60>() / 1000000.0;
		update(frameTime);
		render();
	}

	cleanup();

	return 0;
}

static int setup()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
		std::cerr << "SDL_Init failed" << std::endl;
		return -1;
	}

	window = SDL_CreateWindow(
			winTitle.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			winWidth,
			winHeight,
			0);
	if(!window)
	{
		std::cerr << "SDL_CreateWindow failed" << std::endl;
		return -1;
	}

	renderer = SDL_CreateRenderer(
			window,
			-1,
			0);
	if(!renderer)
	{
		std::cerr << "SDL_CreateRenderer failed" << std::endl;
		return -1;
	}

	return 0;
}

static void init()
{
	openController();

	auto newState = std::make_unique<MenuState>();
	pushState(std::move(newState));
}

static bool input()
{
	updateOldMaps();
	bool windowShouldClose = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
			windowShouldClose = true;
		else if(e.type == SDL_KEYDOWN)
			setKeyState(e.key.keysym.sym, true);
		else if(e.type == SDL_KEYUP)
			setKeyState(e.key.keysym.sym, false);
		else if(e.type == SDL_CONTROLLERBUTTONDOWN)
			setControllerButtonState(e.cbutton.button, true);
		else if(e.type == SDL_CONTROLLERBUTTONUP)
			setControllerButtonState(e.cbutton.button, false);
	}

	updateControlRecording();

	return windowShouldClose;
}

static void update(float frameTime)
{
	updateState(renderer);
}

static void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderClear(renderer);

	renderState(renderer);

	SDL_RenderPresent(renderer);
}

static void cleanup()
{
	while(popState())
	{
	}

	closeController();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
