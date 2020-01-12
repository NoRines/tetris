#ifndef REPLAY_H
#define REPLAY_H

#include "state.h"

class ReplayState : public State
{
public:
	void init(SDL_Renderer* renderer) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	void cleanUp() override;
};

#endif
