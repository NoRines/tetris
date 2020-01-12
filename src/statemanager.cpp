#include <stack>
#include <iostream>

#include "statemanager.h"


#include "menu.h"
#include "game.h"
#include "levelselect.h"


static std::stack<std::unique_ptr<State>> stateStack;
static bool shouldInit = false;

static bool stateChanged = false;
static std::unique_ptr<State> tmpHolder;

static bool popStateRequested = false;

static void initState(SDL_Renderer* renderer);
static void cleanState();

State* getTopState()
{
	return stateStack.top().get();
}

void requestPopState()
{
	popStateRequested = true;
}

bool popState()
{
	cleanState();
	std::cout << "popState()" << std::endl;
	if(!stateStack.empty())
		stateStack.pop();

	return !stateStack.empty();
}

void pushState(std::unique_ptr<State> newState)
{
	std::cout << "pushState()" << std::endl;
	stateStack.push(std::move(newState));
	shouldInit = true;
}

void changeCurrentState(std::unique_ptr<State> newState)
{
	stateChanged = true;
	tmpHolder = std::move(newState);
}

static void initState(SDL_Renderer* renderer)
{
	std::cout << "initState()" << std::endl;
	shouldInit = false;
	if(!stateStack.empty())
		stateStack.top()->init(renderer);
}

static void cleanState()
{
	std::cout << "cleanState()" << std::endl;
	if(!stateStack.empty())
		stateStack.top()->cleanUp();
}

void updateState(SDL_Renderer* renderer)
{
	if(stateChanged && !popStateRequested)
	{
		stateChanged = false;
		popState();
		pushState(std::move(tmpHolder));
	}

	if(popStateRequested && !stateChanged)
	{
		popStateRequested = false;
		popState();
	}

	if(shouldInit)
		initState(renderer);

	if(!stateStack.empty())
		stateStack.top()->update();

	if(shouldInit)
		initState(renderer);
}

void renderState(SDL_Renderer* renderer)
{
	if(!stateStack.empty())
		stateStack.top()->render(renderer);
}

