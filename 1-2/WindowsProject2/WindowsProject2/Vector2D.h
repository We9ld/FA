#pragma once


struct Vector2D
{
	int x, y;

	Vector2D();
	Vector2D(int x_, int y_);

	Vector2D& operator+=(const Vector2D& right);

	Vector2D& operator-=(const Vector2D& right);

	Vector2D operator+(const Vector2D& right) const;

	Vector2D operator-(const Vector2D& right) const;

	int operator*(const Vector2D& right) const;

	Vector2D operator*(const int& num) const;

	double length();

	void rotate(double radian);
};

