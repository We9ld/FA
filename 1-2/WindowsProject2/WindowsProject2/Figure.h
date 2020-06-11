#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <windows.h>

#include "Vector2D.h"

class Figure {
public:
	unsigned int width;
	int speed;
	Vector2D center, direction;

	Figure(unsigned int width, int speed, Vector2D center_, Vector2D direction_);

	virtual void draw(HDC hdc);
	virtual void update();
};

