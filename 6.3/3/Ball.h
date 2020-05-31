#pragma once
#include "stdio.h"
#include <Windows.h>
#include <math.h> 
#include <fstream>

class Vector 
{
public:
	double x, y;

	Vector(double x, double y) : x{ x }, y{ y } 
	{
	}

	double getLength() const
	{
		double l = sqrt(x * x + y * y);
		return l;
	}

	Vector(const Vector& velocity) 
	{
		*this = velocity;
	}

	Vector(Vector&& velocity)
	{
		*this = velocity;
	}


	Vector& operator = (const Vector& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}



	Vector& operator = (Vector&& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}


	Vector& operator +=(const Vector& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector operator +(const Vector& v)
	{
		Vector result(x, y);
		return result += v;
	}

	Vector& operator +=(double num) 
	{
		x += num;
		y += num;
		return *this;
	}

	Vector operator + (double num) 
	{
		Vector copy = Vector(x, y);
		return copy += num;
	}

	Vector operator -=(double n) 
	{
		return (*this + (n * (-1)));
	}

	Vector& operator /=(double num)
	{
		x /= num;
		y /= num;
		return *this;
	}

	Vector operator /(double num)
	{
		Vector result(x, y);
		return result /= num;
	}

	Vector& operator *= (double num)
	{
		x *= num;
		y *= num;
		return *this;
	}

	Vector& operator -=(Vector v) 
	{
		x -= v.x;
		y -= v.y;
		return *this;

	}

	Vector operator -(const Vector& v) 
	{
		Vector result(x, y);
		return result -= v;
	}

	Vector operator * (double num) 
	{
		Vector result(x, y);
		return result *= num;
	}

	Vector normalize() {
		return *this / getLength();
	}

	Vector& operator *= (Vector v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector operator * (const Vector& v)
	{
		Vector r(x, y);
		return r *= v;
	}

	friend std::ostream& operator << (std::ostream& of, Vector& v) 
	{
		of << "{" << v.x << "," << v.y << "}";
		return of;
	}
};

class Ball 
{
private:
	HWND hwnd;
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hb;
	HPEN hp;
public:
	double  radius, mass;
	Vector center;
	Vector velocity;

	Ball(HWND hWnd, double radius, const Vector& v, Vector& center) : hwnd{ hWnd },
		velocity{ v }, center{ center }
	{
		hb = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		this->radius = radius;
		this->mass = radius / 10;
	}

	void draw(HDC hdc) 
	{
		int bias = radius / sqrt(2);
		SelectObject(hdc, hb);
		Ellipse(hdc, center.x - bias, center.y - bias, center.x + bias, center.y + bias);

	}

	void checkBorders(int x, int y, int x_max, int y_max) 
	{
		x /= sqrt(2);
		y /= sqrt(2);
		if (center.x <= radius || center.x >= x_max) 
		{
			velocity.x *= -1;
			if (center.x >= x_max)
			{
				center = Vector(x_max, center.y);
			}

			if (center.x <= radius) {
				center = Vector(radius, center.y);
			}
		}

		if (center.y <= radius || center.y >= y_max) 
		{
			velocity.y *= -1;
			if (center.y >= y_max)
			{
				center = Vector(center.x, y_max - 1);
			}

			if (center.y <= radius) 
			{
				center = Vector(center.x, radius);
			}
		}
	}

	bool isCollision(Ball* ball) 
	{
		double sumOfRaduises = (this->radius + ball->radius) / sqrt(2);
		double distance = sqrt(pow((center.x - ball->center.x), 2) +
			pow(center.y - ball->center.y, 2));
		if (distance <= sumOfRaduises) 
		{
			return true;
		}
		return false;
	}
};
