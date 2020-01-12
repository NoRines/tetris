#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>

class State
{
public:
	virtual void init(SDL_Renderer* renderer) = 0;
	virtual void update() = 0;
	virtual void render(SDL_Renderer* renderer) = 0;
	virtual void cleanUp() = 0;

	virtual ~State() = default;
};

#endif
