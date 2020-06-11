#include "Object.h"

Object::Object(Vector2D center_, Vector2D velocity_, double mass_, int id_) :
	center(center_), velocity(velocity_), mass(mass_), _id(id_)
{}

int Object::get_id()
{
	return _id;
}
