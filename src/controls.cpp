#include <vector>
#include <fstream>
#include <ctime>
#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>

#include "controls.h"

#include "input.h"
#include "game.h"

static bool recording = false;
static bool replaying = false;
static uint64_t seed;
static int startLevel = 0;

struct ActionStamp
{
	unsigned int frame;
	bool state;
};

static std::vector<ActionStamp> leftHistory;
static std::vector<ActionStamp> rightHistory;
static std::vector<ActionStamp> upHistory;
static std::vector<ActionStamp> downHistory;
static std::vector<ActionStamp> aHistory;
static std::vector<ActionStamp> bHistory;

struct ReplayControlState
{
	bool left;
	bool right;
	bool up;
	bool down;
	bool a;
	bool b;
};

static ReplayControlState rControlState;
static ReplayControlState oldRControlState;

static void recordAction(UserAction ua, bool state)
{
	auto frame = getGameFrameNumber();
	switch(ua)
	{
		case UserAction::LEFT:
			leftHistory.push_back({frame, state});
			break;
		case UserAction::RIGHT:
			rightHistory.push_back({frame, state});
			break;
		case UserAction::UP:
			upHistory.push_back({frame, state});
			break;
		case UserAction::DOWN:
			downHistory.push_back({frame, state});
			break;
		case UserAction::A:
			aHistory.push_back({frame, state});
			break;
		case UserAction::B:
			bHistory.push_back({frame, state});
			break;
		default: break;
	}
}

static std::pair<uint32_t, uint8_t> checkUserAction(UserAction ua)
{
	switch(ua)
	{
		case UserAction::LEFT:
			return std::make_pair(SDLK_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		case UserAction::RIGHT:
			return std::make_pair(SDLK_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		case UserAction::UP:
			return std::make_pair(SDLK_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
		case UserAction::DOWN:
			return std::make_pair(SDLK_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		case UserAction::A:
			return std::make_pair(SDLK_z, SDL_CONTROLLER_BUTTON_A);
		case UserAction::B:
			return std::make_pair(SDLK_x, SDL_CONTROLLER_BUTTON_B);
		case UserAction::START:
			return std::make_pair(SDLK_ESCAPE, SDL_CONTROLLER_BUTTON_START);
		case UserAction::SELECT:
			return std::make_pair(SDLK_RETURN, SDL_CONTROLLER_BUTTON_BACK);
		default: break;
	}
	return std::make_pair(0, 0);
}

static void clearHistory()
{
	leftHistory.clear();
	rightHistory.clear();
	upHistory.clear();
	downHistory.clear();
	aHistory.clear();
	bHistory.clear();
}

static bool isRecordedUserActionDown(UserAction ua, const ReplayControlState& rc)
{
	switch(ua)
	{
		case UserAction::LEFT:
			return rc.left;
		case UserAction::RIGHT:
			return rc.right;
		case UserAction::UP:
			return rc.up;
		case UserAction::DOWN:
			return rc.down;
		case UserAction::A:
			return rc.a;
		case UserAction::B:
			return rc.b;
		default: break;
	}
	return false;
}

static bool isRecordedUserActionPressed(UserAction ua)
{
	return isRecordedUserActionDown(ua, rControlState) && !isRecordedUserActionDown(ua, oldRControlState);
}

void updateControlRecording()
{
	if(recording)
	{
		if(isUserActionPressed(UserAction::LEFT) || isUserActionReleased(UserAction::LEFT))
		{
			recordAction(UserAction::LEFT, !isUserActionPressed(UserAction::LEFT));
		}
		if(isUserActionPressed(UserAction::RIGHT) || isUserActionReleased(UserAction::RIGHT))
		{
			recordAction(UserAction::RIGHT, !isUserActionPressed(UserAction::RIGHT));
		}
		if(isUserActionPressed(UserAction::UP) || isUserActionReleased(UserAction::UP))
		{
			recordAction(UserAction::UP, !isUserActionPressed(UserAction::UP));
		}
		if(isUserActionPressed(UserAction::DOWN) || isUserActionReleased(UserAction::DOWN))
		{
			recordAction(UserAction::DOWN, !isUserActionPressed(UserAction::DOWN));
		}
		if(isUserActionPressed(UserAction::A) || isUserActionReleased(UserAction::A))
		{
			recordAction(UserAction::A, !isUserActionPressed(UserAction::A));
		}
		if(isUserActionPressed(UserAction::B) || isUserActionReleased(UserAction::B))
		{
			recordAction(UserAction::B, !isUserActionPressed(UserAction::B));
		}
	}
}

void updateReplay()
{
	if(replaying)
	{
		oldRControlState = rControlState;

		auto frame = getGameFrameNumber();

		auto updateAction = [frame](std::vector<ActionStamp>& his, bool& action)
		{
			if(his.empty())
				return;
			while(frame == his.back().frame)
			{
				action = his.back().state;
				his.pop_back();
			}
		};

		updateAction(leftHistory, rControlState.left);
		updateAction(rightHistory, rControlState.right);
		updateAction(upHistory, rControlState.up);
		updateAction(downHistory, rControlState.down);
		updateAction(aHistory, rControlState.a);
		updateAction(bHistory, rControlState.b);
	}
}

void initControls()
{
	seed = std::time(nullptr);

	rControlState.left = false;
	rControlState.right = false;
	rControlState.up = false;
	rControlState.down = false;
	rControlState.a = false;
	rControlState.b = false;

	oldRControlState.left = false;
	oldRControlState.right = false;
	oldRControlState.up = false;
	oldRControlState.down = false;
	oldRControlState.a = false;
	oldRControlState.b = false;

	recording = false;
	replaying = false;

	seed = std::time(nullptr);
	clearHistory();
}

void setRecordingControls(bool rec)
{
	recording = rec;
}

void setReplayingControls(bool rep)
{
	replaying = rep;
}

void setStartLevel(int lvl)
{
	startLevel = lvl;
}

int getStartLevel()
{
	return startLevel;
}

uint64_t getControlSeed()
{
	return seed;
}

bool isRecording()
{
	return recording;
}

bool isReplaying()
{
	return replaying;
}

void loadHistory(const std::string& fileName)
{
	clearHistory();
	recording = false;
	std::ifstream file(fileName, std::ios::binary);

	auto readHistory = [&file](std::vector<ActionStamp>& his, int size)
	{
		his.resize(size);
		for(auto& i : his)
		{
			file.read(reinterpret_cast<char*>(&i.state), sizeof(bool));
			file.read(reinterpret_cast<char*>(&i.frame), sizeof(unsigned int));
		}
		if(his.size() != 0)
		{
			std::string tmp;
			file >> tmp;
		}
	};

	if(!file.is_open())
		return;

	std::string readData;

	file >> readData;
	if(readData == "SEED")
		file >> seed;
	file >> readData;
	if(readData == "LEVEL")
		file >> startLevel;

	file >> readData;
	int size = 0;
	if(readData == "LEFT")
	{
		file >> size;
		readHistory(leftHistory, size);
	}
	file >> readData;
	if(readData == "RIGHT")
	{
		file >> size;
		readHistory(rightHistory, size);
	}
	file >> readData;
	if(readData == "UP")
	{
		file >> size;
		readHistory(upHistory, size);
	}
	file >> readData;
	if(readData == "DOWN")
	{
		file >> size;
		readHistory(downHistory, size);
	}
	file >> readData;
	if(readData == "A")
	{
		file >> size;
		readHistory(aHistory, size);
	}
	file >> readData;
	if(readData == "B")
	{
		file >> size;
		readHistory(bHistory, size);
	}

	file.close();
}

void saveHistory(const std::string& fileName)
{
	std::reverse(leftHistory.begin(), leftHistory.end());
	std::reverse(rightHistory.begin(), rightHistory.end());
	std::reverse(upHistory.begin(), upHistory.end());
	std::reverse(downHistory.begin(), downHistory.end());
	std::reverse(aHistory.begin(), aHistory.end());
	std::reverse(bHistory.begin(), bHistory.end());


	std::ofstream file(fileName, std::ios::binary);

	auto writeHistory = [&file](const std::string& name, std::vector<ActionStamp>& his)
	{
		file << name << " " << his.size() << std::endl;
		for(auto& i : his)
		{
			file.write(reinterpret_cast<char*>(&i.frame), sizeof(unsigned int));
			file.write(reinterpret_cast<char*>(&i.state), sizeof(bool));
		}
		file << std::endl;
	};

	if(!file.is_open())
		return;
	file << "SEED " << seed << std::endl;
	file << "LEVEL " << startLevel << std::endl;

	writeHistory("LEFT", leftHistory);
	writeHistory("RIGHT", rightHistory);
	writeHistory("UP", upHistory);
	writeHistory("DOWN", downHistory);
	writeHistory("A", aHistory);
	writeHistory("B", bHistory);

	file.close();

	recording = false;
	clearHistory();
}

bool isUserActionDown(UserAction ua)
{
	if(replaying)
		return isRecordedUserActionDown(ua, rControlState);

	auto kb = checkUserAction(ua);
	return isKeyDown(kb.first) || isControllerButtonDown(kb.second);
}

bool isUserActionPressed(UserAction ua)
{
	if(replaying)
		return isRecordedUserActionPressed(ua);

	auto kb = checkUserAction(ua);
	return isKeyPressed(kb.first) || isControllerButtonPressed(kb.second);
}

bool isUserActionReleased(UserAction ua)
{
	auto kb = checkUserAction(ua);
	return isKeyReleased(kb.first) || isControllerButtonReleased(kb.second);
}
