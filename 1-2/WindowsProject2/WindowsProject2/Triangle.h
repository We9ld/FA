#pragma once

#include "Figure.h"

class Triangle :
	public Figure
{
	POINT vertices[3];
public:
	Triangle(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_);

	void draw(HDC hdc);
	void update();
};

