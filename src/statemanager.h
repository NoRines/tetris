#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <SDL2/SDL.h>
#include <memory>

#include "state.h"

State* getTopState();

void requestPopState();
bool popState();
void pushState(std::unique_ptr<State> newState);
void changeCurrentState(std::unique_ptr<State> newState);

void updateState(SDL_Renderer* renderer);
void renderState(SDL_Renderer* renderer);

#endif
