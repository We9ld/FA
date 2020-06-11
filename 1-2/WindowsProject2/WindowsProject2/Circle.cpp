#include "Circle.h"

Circle::Circle(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_) : Figure(width_, speed_, center_, direction_)
{}

void Circle::draw(HDC hdc)
{
	Ellipse(hdc, center.x - (width / 2), center.y - (width / 2), center.x + (width / 2), center.y + (width / 2));
}
