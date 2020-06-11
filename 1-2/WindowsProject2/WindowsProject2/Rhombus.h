#pragma once

#include "Figure.h"

class Rhombus :
	public Figure
{
	POINT vertices[4];
public:
	Rhombus(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_);

	void draw(HDC hdc);
	void update();
};

