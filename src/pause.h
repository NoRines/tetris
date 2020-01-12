#ifndef PAUSE_H
#define PAUSE_H

#include "state.h"


void setGameStatePointer(State* gs);

class PauseState : public State
{
public:
	void init(SDL_Renderer* renderer) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	void cleanUp() override;
};

#endif
