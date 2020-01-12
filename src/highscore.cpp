#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include <SDL_FontCache.h>

#include "highscore.h"
#include "controls.h"
#include "menu.h"
#include "statemanager.h"

static FC_Font* font;
static FC_Font* scoreFont;
static constexpr int numRecords = 5;

struct ScoreRecord
{
	int level;
	int score;
};

static std::vector<ScoreRecord> scoreRecords;

static void readScoreFile(std::vector<ScoreRecord>& data)
{
	data.clear();

	std::ifstream inFile("score.txt");

	if(inFile.is_open())
	{
		ScoreRecord record;
		while(inFile >> record.level)
		{
			inFile >> record.score;
			data.push_back(record);
		}

		inFile.close();
	}
}

void saveResult(int level, int score)
{
	std::ifstream inFile("score.txt");
	std::vector<ScoreRecord> data;

	readScoreFile(data);

	data.push_back({level, score});

	std::sort(data.begin(), data.end(),
			[](const ScoreRecord& a, const ScoreRecord& b)
			{
				return a.score > b.score;
			});

	std::ofstream outFile("score.txt");

	if(outFile.is_open())
	{
		int i = 0;
		for(auto& r : data)
		{
			if(i++ > numRecords)
				break;
			outFile << r.level << " " << r.score << std::endl;
		}

		outFile.close();
	}
}

void HighScoreState::init(SDL_Renderer* renderer)
{
	font = FC_CreateFont();
	FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 30, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	scoreFont = FC_CreateFont();
	FC_LoadFont(scoreFont, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

	readScoreFile(scoreRecords);
}

void HighScoreState::update()
{
	if(isUserActionPressed(UserAction::SELECT))
	{
		auto newState = std::make_unique<MenuState>();
		changeCurrentState(std::move(newState));
	}
}

void HighScoreState::render(SDL_Renderer* renderer)
{
	FC_Draw(font, renderer, 40, 40, "Highscore List");
	FC_Draw(scoreFont, renderer, 40, 80, "Level");
	FC_Draw(scoreFont, renderer, 100, 80, "Score");

	int yOffset = 30;

	for(auto& i : scoreRecords)
	{
		FC_Draw(scoreFont, renderer, 40, 80 + yOffset, std::to_string(i.level).c_str());
		FC_Draw(scoreFont, renderer, 100, 80 + yOffset, std::to_string(i.score).c_str());
		yOffset += 30;
	}
}

void HighScoreState::cleanUp()
{
	FC_FreeFont(scoreFont);
	FC_FreeFont(font);
}
