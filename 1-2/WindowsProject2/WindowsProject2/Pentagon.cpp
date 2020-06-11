#include "Pentagon.h"

Pentagon::Pentagon(unsigned int width_, int speed_, Vector2D center_, Vector2D direction_) : Figure(width_, speed_, center_, direction_)
{
	// init vertices
	vertices[0] = { (LONG)(center.x - (width / 2) * cos(18 * (M_PI / 180.0))), (LONG)(center.y - (width / 2) * sin(18 * (M_PI / 180.0))) };
	vertices[1] = { (LONG)(center.x - (width / 2) * cos(54 * (M_PI / 180.0))), (LONG)(center.y + (width / 2) * sin(54 * (M_PI / 180.0))) };
	vertices[2] = { (LONG)(center.x + (width / 2) * cos(54 * (M_PI / 180.0))), (LONG)(center.y + (width / 2) * sin(54 * (M_PI / 180.0))) };
	vertices[3] = { (LONG)(center.x + (width / 2) * cos(18 * (M_PI / 180.0))), (LONG)(center.y - (width / 2) * sin(18 * (M_PI / 180.0))) };
	vertices[4] = { (LONG)(center.x), (LONG)(center.y - (width / 2)) };
}

void Pentagon::draw(HDC hdc)
{
	Polygon(hdc, vertices, 5);
}

void Pentagon::update()
{
	Figure::update();

	vertices[0] = { (LONG)(center.x - (width / 2) * cos(18 * (M_PI / 180.0))), (LONG)(center.y - (width / 2) * sin(18 * (M_PI / 180.0))) };
	vertices[1] = { (LONG)(center.x - (width / 2) * cos(54 * (M_PI / 180.0))), (LONG)(center.y + (width / 2) * sin(54 * (M_PI / 180.0))) };
	vertices[2] = { (LONG)(center.x + (width / 2) * cos(54 * (M_PI / 180.0))), (LONG)(center.y + (width / 2) * sin(54 * (M_PI / 180.0))) };
	vertices[3] = { (LONG)(center.x + (width / 2) * cos(18 * (M_PI / 180.0))), (LONG)(center.y - (width / 2) * sin(18 * (M_PI / 180.0))) };
	vertices[4] = { (LONG)(center.x), (LONG)(center.y - (width / 2)) };
}
