#pragma once
#include "Figure.h"

class Pentagon :
	public Figure
{
	POINT vertices[5];
public:
	Pentagon(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_);

	void draw(HDC hdc);
	void update();
};

