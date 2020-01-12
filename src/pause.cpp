#include <SDL_FontCache.h>

#include "pause.h"
#include "game.h"
#include "controls.h"
#include "statemanager.h"


static FC_Font* font;
static State* gameState = nullptr;

void setGameStatePointer(State* gs)
{
	gameState = gs;
}

void PauseState::init(SDL_Renderer* renderer)
{
	font = FC_CreateFont();
	FC_LoadFont(font, renderer, "fonts/FreeSansBold.ttf", 50, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
}

void PauseState::update()
{
	bool shouldReplay = isReplaying();
	setReplayingControls(false);
	if(isUserActionPressed(UserAction::START))
	{
		gameState = nullptr;
		requestPopState();
	}
	setReplayingControls(shouldReplay);
}

void PauseState::render(SDL_Renderer* renderer)
{
	if(gameState)
		gameState->render(renderer);
	FC_Draw(font, renderer, 40, 80, "PAUSED");
}

void PauseState::cleanUp()
{
	FC_FreeFont(font);
}
