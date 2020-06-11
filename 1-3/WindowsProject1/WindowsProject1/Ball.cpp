#include "Ball.h"
//�������� �������
Ball::Ball(Vector2D center_, Vector2D velocity_, double mass_, int id_, double radius_) :
	Object(center_, velocity_, mass_, id_), radius(radius_)
{}
//������� ����������
void Ball::update()
{
	center += velocity;
}
//������� ���������
void Ball::draw(HDC hdc)
{

	Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);

	// draw ball id
	std::wstring wstr_id = std::to_wstring(_id);

	TextOut(hdc, center.x, center.y, wstr_id.c_str(), wstr_id.size());
}
//������� ����������� "�����������"
bool Ball::collide(const Ball& another)
{
	return radius + another.radius > hypot(center.x - another.center.x, center.y - another.center.y);
}