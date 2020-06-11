#pragma once
#include "Figure.h"

class Circle :
	public Figure
{
public:
	Circle(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_);

	void draw(HDC hdc);
};

