#pragma once
#include <windows.h>

#include "Vector2D.h"

class Object
{
protected:
	//У объекта должен быть идентефикатор
	int _id;
public:
	double mass;
	Vector2D center, velocity;

	Object(Vector2D center_, Vector2D velocity_, double mass_, int id_);

	virtual void update() = 0;
	virtual void draw(HDC hdc) = 0;
	//Геттер id
	int get_id();
};

