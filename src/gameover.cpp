#include <SDL_FontCache.h>

#include "windefs.h"
#include "gameover.h"
#include "controls.h"
#include "statemanager.h"
#include "menu.h"
#include "highscore.h"

static FC_Font* font;
static FC_Font* smallFont;
static bool recording;
static int level;
static int score;

void setGameOverResult(int l, int s)
{
	level = l;
	score = s;
}

void GameOverState::init(SDL_Renderer* renderer)
{
	font = FC_CreateFont();
	FC_LoadFont(font, renderer, "fonts/FreeSansBold.ttf", 50, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	smallFont = FC_CreateFont();
	FC_LoadFont(smallFont, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

	if(!isReplaying())
		saveResult(level, score);

	recording = isRecording();
	setRecordingControls(false);
	setReplayingControls(false);
}

void GameOverState::update()
{
	if(isUserActionPressed(UserAction::SELECT))
	{
		auto newState = std::make_unique<MenuState>();
		changeCurrentState(std::move(newState));
	}
	if(recording)
	{
		if(isUserActionPressed(UserAction::START))
		{
			saveHistory("replay.mf");
			recording = false;
		}
	}
}

void GameOverState::render(SDL_Renderer* renderer)
{
	FC_Draw(font, renderer, (winWidth / 2) - 130, (winHeight / 2) - 100, "Game Over");
	FC_Draw(font, renderer, (winWidth / 2) - 230, (winHeight / 2), "Score: ");
	FC_Draw(font, renderer, (winWidth / 2) - 50, (winHeight / 2), std::to_string(score).c_str());
	if(recording)
		FC_Draw(smallFont, renderer, (winWidth / 2) - 50, (winHeight / 2) + 150, "Press START to save replay");
}

void GameOverState::cleanUp()
{
	FC_FreeFont(font);
	FC_FreeFont(smallFont);
}
