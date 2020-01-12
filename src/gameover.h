#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "state.h"

void setGameOverResult(int l, int s);

class GameOverState : public State
{
public:
	void init(SDL_Renderer* renderer) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	void cleanUp() override;
};

#endif
