#pragma once
#include <windows.h>
#include <string>

#include "Object.h"

class Ball :
	public Object
{
public:
	double radius;
	//Конструктор
	Ball(Vector2D center_, Vector2D velocity_, double mass_, int id_, double radius_);

	bool collide(const Ball& another);

	void update();
	void draw(HDC hdc);
};

