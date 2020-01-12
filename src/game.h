#ifndef GAME_H
#define GAME_H

#include "state.h"

unsigned int getGameFrameNumber();

class GameState : public State
{
public:
	void init(SDL_Renderer* renderer) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	void cleanUp() override;
};

#endif
