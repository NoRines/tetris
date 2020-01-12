#include <SDL_FontCache.h>

#include "replay.h"

#include "game.h"
#include "controls.h"
#include "windefs.h"
#include "pause.h"
#include "statemanager.h"

static FC_Font* font;

static GameState gameState;

static int speedMultiplyer;

void ReplayState::init(SDL_Renderer* renderer)
{
	font = FC_CreateFont();
	FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

	initControls();
	setReplayingControls(true);
	loadHistory("replay.mf");

	gameState.init(renderer);

	speedMultiplyer = 1;
}

void ReplayState::update()
{
	setReplayingControls(false);

	if(isUserActionPressed(UserAction::LEFT))
	{
		speedMultiplyer--;
		if(speedMultiplyer < 1)
			speedMultiplyer = 1;
	}
	else if(isUserActionPressed(UserAction::RIGHT))
	{
		speedMultiplyer++;
	}
	else if(isUserActionPressed(UserAction::START))
	{
		auto newState = std::make_unique<PauseState>();
		setGameStatePointer(getTopState());
		pushState(std::move(newState));
	}

	setReplayingControls(true);


	int numUpdatesLeft = speedMultiplyer;
	while(numUpdatesLeft-- > 0)
	{
		updateReplay();
		gameState.update();
	}
}

void ReplayState::render(SDL_Renderer* renderer)
{
	gameState.render(renderer);

	FC_Draw(font, renderer, playAreaWidth + 100, 20, ("x" + std::to_string(speedMultiplyer)).c_str());
}

void ReplayState::cleanUp()
{
	gameState.cleanUp();
	FC_FreeFont(font);
}
