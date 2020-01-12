#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "state.h"

void saveResult(int level, int score);

class HighScoreState : public State
{
public:
	void init(SDL_Renderer* renderer) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	void cleanUp() override;
};

#endif
