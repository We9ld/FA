#include "Vector2D.h"

Vector2D::Vector2D() : x(0), y(0)
{}

Vector2D::Vector2D(double x_, double y_) : x(x_), y(y_)
{}

Vector2D& Vector2D::operator+=(const Vector2D& right)
{
	x = x + right.x;
	y = y + right.y;

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& right)
{
	x = x - right.x;
	y = y - right.y;

	return *this;
}

Vector2D Vector2D::operator+(const Vector2D& right) const
{
	Vector2D sum = *this;

	return sum += right;
}

Vector2D Vector2D::operator-(const Vector2D& right) const
{
	Vector2D sub = *this;

	return sub -= right;
}

double Vector2D::operator*(const Vector2D& right) const
{
	// scalar product

	return x * right.x + y * right.y;
}

Vector2D Vector2D::operator*(const double& num) const
{
	return Vector2D(x * num, y * num);
}

Vector2D Vector2D::operator/(const double& num) const
{
	return Vector2D(x / num, y / num);
}

double Vector2D::length()
{
	return hypot(x, y);
}

void Vector2D::rotate(double radian)
{
	// Поворот вектора на угол в радианах

	// Используем матрицу поворота
	x = x * cos(radian) - y * sin(radian);
	y = x * sin(radian) + y * cos(radian);
}
