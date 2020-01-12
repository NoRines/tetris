#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>

constexpr int boardColumns = 10;
constexpr int boardRows = 25;
constexpr int numVisibleBoardRows = 20;

struct Pos { int x, y; };

void initBoard();
bool isCellFilled(int x, int y);
bool inBoard(int x, int y);
void fillCell(int type, int x, int y);
void drawBoard(SDL_Renderer* renderer);
void removeLines();
void markLinesForRemoval();
int getNumLinesToRemove();
void animateRemove(int step);

#endif
