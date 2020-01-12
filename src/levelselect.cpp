#include <SDL_FontCache.h>

#include "levelselect.h"
#include "controls.h"
#include "statemanager.h"
#include "game.h"

static FC_Font* font;

static int boxX;
static int boxY;
static int selectedLevel;

void LevelSelectState::init(SDL_Renderer* renderer)
{
	font = FC_CreateFont();
	FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

	boxX = 40;
	boxY = 80;
	selectedLevel = 0;
}

void LevelSelectState::update()
{
	if(isUserActionPressed(UserAction::RIGHT) && selectedLevel < 9)
	{
		boxX += 17;
		selectedLevel++;
	}
	else if(isUserActionPressed(UserAction::LEFT) && selectedLevel > 0)
	{
		boxX -= 17;
		selectedLevel--;
	}

	if(isUserActionPressed(UserAction::SELECT))
	{
		initControls();
		setRecordingControls(true);
		auto newState = std::make_unique<GameState>();
		changeCurrentState(std::move(newState));

		if(isUserActionDown(UserAction::A))
			selectedLevel += 10;
		setStartLevel(selectedLevel);
	}
}

void LevelSelectState::render(SDL_Renderer* renderer)
{
	SDL_Rect r = {boxX, boxY, 12, 25};
	SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
	SDL_RenderFillRect(renderer, &r);
	FC_Draw(font, renderer, 40, 80, "0 1 2 3 4 5 6 7 8 9");
}

void LevelSelectState::cleanUp()
{
	FC_FreeFont(font);
}
