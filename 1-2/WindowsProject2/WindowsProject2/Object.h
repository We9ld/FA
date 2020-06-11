#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <windows.h>

#include "Vector2D.h"

class Object {
public:
	int speed;
	size_t width;
	Vector2D center, direction;

	Object(size_t width_, int speed_, Vector2D center_, Vector2D direction_);

	virtual void draw(HDC hdc);
	virtual void update();
};

