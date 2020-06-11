#include "Triangle.h"

Triangle::Triangle(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_) : Figure(width_, speed_, center_, direction_)
{
	// init vertices
	vertices[0] = { (LONG)(center.x - width / 2), (LONG)(center.y + width / 2) };
	vertices[1] = { (LONG)(center.x + width / 2), (LONG)(center.y + width / 2) };
	vertices[2] = { (LONG)(center.x), (LONG)(center.y - width / 2) };
}

void Triangle::draw(HDC hdc)
{
	Polygon(hdc, vertices, 3);
}

void Triangle::update()
{
	vertices[0] = { (LONG)(center.x - width / 2), (LONG)(center.y + width / 2) };
	vertices[1] = { (LONG)(center.x + width / 2), (LONG)(center.y + width / 2) };
	vertices[2] = { (LONG)(center.x), (LONG)(center.y - width / 2) };

	Figure::update();
}
