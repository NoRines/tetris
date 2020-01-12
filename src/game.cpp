#include <iostream>
#include <ctime>
#include <array>
#include <algorithm>

#include <SDL_FontCache.h>

#include "game.h"

#include "block.h"
#include "shape.h"
#include "board.h"
#include "controls.h"
#include "statemanager.h"
#include "gameover.h"
#include "pause.h"

enum class PlayState
{
	DROP_STATE,
	CLEAR_STATE,
	ENTRY_STATE
};

static Shape shape;
static Shape nextShape;
static std::mt19937 gen;
static unsigned int frameTicker = 0;
static PlayState playState;
static unsigned int dasCounter = 0;
static int startLevel = 0;
static int level = 0;
static int lines = 0;
static int linesForNextLevel = 0;
static int score = 0;
static bool flashBackground = false;

static FC_Font* hFont;
static FC_Font* pFont;


static constexpr int levelTimes[] = {
//	 0	 1	 2	 3	 4	 5	 6	 7	8  9 
	48, 43, 38, 33, 28, 23,	18, 13, 8, 6,
//	10 11 12 13 14 15 16 17 18 19 20 21 22
	5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2,
//	23 24 25 26 27 28 29
	2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1};


static void updateDrop();
static void updateClear();
static void updateEntry();

unsigned int getGameFrameNumber()
{
	return frameTicker;
}

void GameState::init(SDL_Renderer* renderer)
{
	hFont = FC_CreateFont();
	FC_LoadFont(hFont, renderer, "fonts/FreeSans.ttf", 30, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
	pFont = FC_CreateFont();
	FC_LoadFont(pFont, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

	startLevel = getStartLevel();
	level = startLevel;
	lines = 0;
	score = 0;
	flashBackground = false;
	linesForNextLevel = std::min(startLevel * 10 + 10, std::max(100, startLevel * 10 - 50));

	frameTicker = 0;
	playState = PlayState::DROP_STATE;
	dasCounter = 0;

	gen.seed(getControlSeed());
	shape = createShape(gen, 5, 6);
	nextShape = createShape(gen, 13, 11);

	initBoard();
}

void GameState::update()
{
	switch(playState)
	{
		case PlayState::DROP_STATE: updateDrop(); break;
		case PlayState::CLEAR_STATE: updateClear(); break;
		case PlayState::ENTRY_STATE: updateEntry(); break;
	}

	if(isUserActionPressed(UserAction::START))
	{
		auto newState = std::make_unique<PauseState>();
		setGameStatePointer(getTopState());
		pushState(std::move(newState));
	}

	frameTicker++;
}

void GameState::render(SDL_Renderer* renderer)
{
	switch(playState)
	{
		case PlayState::DROP_STATE:
		{
			drawShape(shape, renderer);
		} break;
		case PlayState::CLEAR_STATE:
		{
			if(flashBackground)
			{
				SDL_Rect r = {0, 0, playAreaWidth, playAreaHeight};
				SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
				SDL_RenderFillRect(renderer, &r);
			}
		} break;
		case PlayState::ENTRY_STATE:
		{
			drawShape(shape, renderer);
		} break;
	}
	drawBoard(renderer);

	SDL_Rect r = {playAreaWidth, 0, winWidth, winHeight};
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, &r);

	drawShape(nextShape, renderer);

	FC_Draw(hFont, renderer, playAreaWidth + 5, winHeight / 6, "Next Shape");
	FC_Draw(hFont, renderer, playAreaWidth + 20, winHeight / 2, "Score: ");
	FC_Draw(pFont, renderer, playAreaWidth + 20, (winHeight / 2) + 40, std::to_string(score).c_str());
	FC_Draw(hFont, renderer, playAreaWidth + 20, (winHeight / 2) + 80, "Lines: ");
	FC_Draw(pFont, renderer, playAreaWidth + 20, (winHeight / 2) + 120, std::to_string(lines).c_str());
	FC_Draw(hFont, renderer, playAreaWidth + 20, (winHeight / 2) + 160, "Level: ");
	FC_Draw(pFont, renderer, playAreaWidth + 20, (winHeight / 2) + 200, std::to_string(level).c_str());
}

void GameState::cleanUp()
{
	FC_FreeFont(hFont);
	FC_FreeFont(pFont);
}

static int getScoreForLines(int lvl, int numLines)
{
	switch(numLines)
	{
		case 1: return 40 * (lvl + 1);
		case 2: return 100 * (lvl + 1);
		case 3: return 300 * (lvl + 1);
		case 4: return 1200 * (lvl + 1);
	}
	return 0;
}

static void updateDrop()
{
	static unsigned int dropTicker = 0;

	int dropTicks = levelTimes[level];
	int scoreForDrop = 0;

	if(isUserActionDown(UserAction::DOWN))
	{
		dropTicks = 2;
		scoreForDrop = 1;
	}

	if(dropTicker++ >= dropTicks)
	{
		dropTicker = 0;
		if(!moveShapeDown(shape))
		{
			placeShape(shape);
			markLinesForRemoval();
			if(getNumLinesToRemove() > 0)
				playState = PlayState::CLEAR_STATE;
			else
				playState = PlayState::ENTRY_STATE;
		}
		else
			score += scoreForDrop;
	}

	bool leftDown = isUserActionDown(UserAction::LEFT);
	bool leftPress = isUserActionPressed(UserAction::LEFT);
	bool rightDown = isUserActionDown(UserAction::RIGHT);
	bool rightPress = isUserActionPressed(UserAction::RIGHT);
	bool shouldMove = false;

	if(leftDown || rightDown)
	{
		if(leftPress || rightPress)
		{
			dasCounter = 0;
			shouldMove = true;
		}
		else
		{
			if(dasCounter == 16)
			{
				dasCounter -= 6;
				shouldMove = true;
			}
			else
				dasCounter++;
		}
	}

	if(shouldMove)
	{
		bool canMove = true;
		if(leftDown)
			canMove = moveShapeLeft(shape);
		else if(rightDown)
			canMove = moveShapeRight(shape);
		if(!canMove)
			dasCounter = 16;
	}


	if(isUserActionPressed(UserAction::B))
		rotateShape(shape, -1);
	if(isUserActionPressed(UserAction::A))
		rotateShape(shape, 1);
}

static void updateClear()
{
	static unsigned int animStep = 0;

	if(frameTicker % 4 == 0)
	{
		if(getNumLinesToRemove() == 4 && !flashBackground)
			flashBackground = true;
		else if(getNumLinesToRemove() == 4 && flashBackground)
			flashBackground = false;

		animateRemove(++animStep);
	}

	if(animStep >= 5)
	{
		flashBackground = false;
		removeLines();
		int linesRemoved = getNumLinesToRemove();
		lines += linesRemoved;
		score += getScoreForLines(level, linesRemoved);

		if(lines >= linesForNextLevel)
		{
			linesForNextLevel += 10;
			level++;
		}

		animStep = 0;
		playState = PlayState::ENTRY_STATE;
	}
}

static void updateEntry()
{
	static unsigned int entryTicker = 1;

	int delay = 0;

	if(shape.pos.y >= boardRows - 2)
		delay = 10;
	else if(shape.pos.y >= boardRows - 6)
		delay = 12;
	else if(shape.pos.y >= boardRows - 10)
		delay = 14;
	else if(shape.pos.y >= boardRows - 14)
		delay = 16;
	else if(shape.pos.y >= boardRows - 18)
		delay = 18;

	if(entryTicker == 1)
	{
		shape = createShape(nextShape.type, 5, 6);
		nextShape = createShape(gen, 13, 11, nextShape.type);

		bool gameOver = false;
		for(auto& b : shape.blocks)
		{
			if(isCellFilled(b.p.x, b.p.y))
			{
				gameOver = true;
				break;
			}
		}

		if(gameOver)
		{
			auto newState = std::make_unique<GameOverState>();
			changeCurrentState(std::move(newState));
			setGameOverResult(level, score);
		}
	}

	if(entryTicker++ >= delay)
	{
		entryTicker = 1;
		playState = PlayState::DROP_STATE;
	}
}
