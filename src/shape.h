#ifndef SHAPE_H
#define SHAPE_H

#include <array>
#include <random>
#include "block.h"

extern const Color shapeColors[];

struct ShapeRotation
{
	int numRotStates;
	int startIndex;
	int startYOffset;
	
	std::array<Pos, 4> preRotOps;
};

struct Shape
{
	int type;
	Pos pos;
	std::array<Block, 4> blocks;

	int rotIndex;
};

Shape createShape(std::mt19937& generator, int x, int y, int lastType = 7);
Shape createShape(int blockType, int x, int y);

void drawShape(const Shape& s, SDL_Renderer* renderer);

bool moveShapeLeft(Shape& s);
bool moveShapeRight(Shape& s);
bool moveShapeDown(Shape& s);

void placeShape(const Shape& s);
bool rotateShape(Shape& s, int dir);

#endif
