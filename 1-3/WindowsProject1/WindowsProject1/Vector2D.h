#pragma once

#include <cmath>

//Структура вектора
struct Vector2D
{
	double x, y;

	Vector2D();
	Vector2D(double x_, double y_);

	Vector2D& operator+=(const Vector2D& right);

	Vector2D& operator-=(const Vector2D& right);

	Vector2D operator+(const Vector2D& right) const;

	Vector2D operator-(const Vector2D& right) const;

	double operator*(const Vector2D& right) const;

	Vector2D operator*(const double& num) const;

	Vector2D operator/(const double& num) const;

	double length();

	void rotate(double radian);
};

