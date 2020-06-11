#include "Object.h"

Object::Object(size_t width_, int speed_, Vector2D center_, Vector2D direction_) :
	width(width_), speed(speed_), center(center_), direction(direction_)
{}

void Object::draw(HDC hdc)
{}

void Object::update()
{
	center += direction * speed;
}
