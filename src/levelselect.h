#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include "state.h"

class LevelSelectState : public State
{
public:
	void init(SDL_Renderer* renderer) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	void cleanUp() override;
};

#endif
