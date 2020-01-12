#ifndef MENU_H
#define MENU_H

#include "state.h"

class MenuState : public State
{
public:
	void init(SDL_Renderer* renderer) override;
	void update() override;
	void render(SDL_Renderer* renderer) override;
	void cleanUp() override;
};

#endif
