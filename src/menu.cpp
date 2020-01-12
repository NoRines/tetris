#include <SDL_FontCache.h>

#include "menu.h"
#include "controls.h"
#include "statemanager.h"
#include "levelselect.h"
#include "game.h"
#include "highscore.h"
#include "replay.h"

static FC_Font* menuFont;

static int boxX;
static int boxY;

enum class SelectState
{
	SELECT_LEVEL,
	HIGHSCORE_LIST,
	WATCH_REPLAY
};

static SelectState currentState;

void MenuState::init(SDL_Renderer* renderer)
{
	menuFont = FC_CreateFont();
	FC_LoadFont(menuFont, renderer, "fonts/FreeSans.ttf", 30, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

	boxX = 20;
	boxY = 54;

	currentState = SelectState::SELECT_LEVEL;
}

void MenuState::update()
{
	if(isUserActionPressed(UserAction::DOWN))
	{
		boxY += 40;
		if(currentState == SelectState::SELECT_LEVEL)
			currentState = SelectState::HIGHSCORE_LIST;
		else if(currentState == SelectState::HIGHSCORE_LIST)
			currentState = SelectState::WATCH_REPLAY;
		else if(currentState == SelectState::WATCH_REPLAY)
		{
			currentState = SelectState::SELECT_LEVEL;
			boxY = 54;
		}
	}
	if(isUserActionPressed(UserAction::UP))
	{
		boxY -= 40;
		if(currentState == SelectState::SELECT_LEVEL)
		{
			currentState = SelectState::WATCH_REPLAY;
			boxY = 54 + 80;
		}
		else if(currentState == SelectState::HIGHSCORE_LIST)
			currentState = SelectState::SELECT_LEVEL;
		else if(currentState == SelectState::WATCH_REPLAY)
			currentState = SelectState::HIGHSCORE_LIST;
	}

	if(isUserActionPressed(UserAction::SELECT))
	{
		if(currentState == SelectState::SELECT_LEVEL)
		{
			auto newState = std::make_unique<LevelSelectState>();
			changeCurrentState(std::move(newState));
		}
		else if(currentState == SelectState::HIGHSCORE_LIST)
		{
			auto newState = std::make_unique<HighScoreState>();
			changeCurrentState(std::move(newState));
		}
		else if(currentState == SelectState::WATCH_REPLAY)
		{
			//initControls();
			//setReplayingControls(true);
			//loadHistory("replay.mf");
			//auto newState = std::make_unique<GameState>();
			//changeCurrentState(std::move(newState));
			auto newState = std::make_unique<ReplayState>();
			changeCurrentState(std::move(newState));
		}
	}
}

void MenuState::render(SDL_Renderer* renderer)
{
	SDL_Rect r = { boxX, boxY, 10, 10 };
	SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
	SDL_RenderFillRect(renderer, &r);
	FC_Draw(menuFont, renderer, 40, 40, "Select Level");
	FC_Draw(menuFont, renderer, 40, 80, "Highscore List");
	FC_Draw(menuFont, renderer, 40, 120, "Watch Replay");
}

void MenuState::cleanUp()
{
	FC_FreeFont(menuFont);
}
