#include "Figure.h"

Figure::Figure(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_) : width(width_), speed(speed_), center(center_), direction(direction_)
{}

void Figure::draw(HDC hdc)
{}

void Figure::update()
{
	center += direction * speed;
}
