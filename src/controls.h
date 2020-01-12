#ifndef CONTROLS_H
#define CONTROLS_H

#include <string>
#include <cstdint>

enum class UserAction
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	A,
	B,
	START,
	SELECT
};

void updateControlRecording();
void updateReplay();
void initControls();
void setRecordingControls(bool rec);
void setReplayingControls(bool rep);
void setStartLevel(int lvl);
int getStartLevel();
uint64_t getControlSeed();
bool isRecording();
bool isReplaying();
void loadHistory(const std::string& fileName);
void saveHistory(const std::string& fileName);

bool isUserActionDown(UserAction ua);
bool isUserActionPressed(UserAction ua);
bool isUserActionReleased(UserAction ua);

#endif
