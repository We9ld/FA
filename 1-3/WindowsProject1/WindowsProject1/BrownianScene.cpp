#include "BrownianScene.h"

BrownianScene::BrownianScene(size_t left_, size_t top_, size_t right_, size_t bottom_) :
	_left(left_), _top(top_), _right(right_), _bottom(bottom_), is_freeze(false)
{
	//Открываем поток
	_log_fs.open("log.csv", std::ios_base::out | std::ios_base::trunc);

	// Добавляем хэдер для csv файла
	if (_log_fs.is_open())
		_log_fs << "ID1,ID2,x1,y1,x2,y2,velocity1 before,velocity2 before,velocity1 after,velocity2 after" << "\n";
}

BrownianScene::~BrownianScene()
{
	//Закрываем поток
	_log_fs.close();
}

void BrownianScene::update()
{
	if (!is_freeze)
	{
		// check collision for all objects
		for (auto curr1 = objects.begin(); curr1 != objects.end(); ++curr1)
		{
			std::shared_ptr<Ball> b1(curr1->second);

			for (auto curr2 = std::next(curr1); curr2 != objects.end(); ++curr2)
			{
				std::shared_ptr<Ball> b2(curr2->second);

				if (b1->collide(*b2))
				{
					// Длина дегмента между кругами
					double l = b1->radius + b2->radius - hypot(b1->center.x - b2->center.x, b1->center.y - b2->center.y);

					// shift back
					b1->center -= (b1->velocity / b1->velocity.length()) * (l / 2.0);
					b2->center -= (b2->velocity / b2->velocity.length()) * (l / 2.0);

					// Запись в лог
					if (_log_fs.is_open())
						_log_fs << b1->get_id() << ","
						<< b2->get_id() << ","
						<< b1->center.x << ","
						<< b1->center.y << ","
						<< b2->center.x << ","
						<< b2->center.y << ","
						<< b1->velocity.length() << ","
						<< b2->velocity.length() << ",";


					Vector2D p1 = b1->velocity * b1->mass,
						p2 = b2->velocity * b2->mass;

					b1->velocity = (p2 * 2.0 + b1->velocity * (b1->mass - b2->mass)) / (b1->mass + b2->mass);
					b2->velocity = (p1 * 2.0 + b2->velocity * (b2->mass - b1->mass)) / (b1->mass + b2->mass);

					// Запись в лог
					if (_log_fs.is_open())
						_log_fs << b1->velocity.length() << ","
						<< b2->velocity.length() << "\n";
				}
			}
		}

		// Обновление позиции объекта

		for (const auto& curr : objects)
		{
			std::shared_ptr<Ball> obj(curr.second);

			if (obj->center.x + obj->radius > _right)
			{
				// Проверка пересечения объектом правой грани экрана
				obj->center.x = _right - obj->radius;
				obj->velocity.x *= -1;
			}
			else if (obj->center.x - obj->radius < _left)
			{
				// Проверка пересечения объектом левой грани экрана
				obj->center.x = _left + obj->radius;                    //
				obj->velocity.x *= -1;
			}
			else if (obj->center.y + obj->radius > _bottom)
			{
				// Проверка пересечения объектом нижней грани экрана
				obj->center.y = _bottom - obj->radius;
				obj->velocity.y *= -1;
			}
			else if (obj->center.y - obj->radius < _top)
			{
				// Проверка пересечения объектом верхней грани экрана
				obj->center.y = _top + obj->radius;
				obj->velocity.y *= -1;
			}

			obj->update();
		}
	}

}

void BrownianScene::draw(HDC hdc)
{
	Rectangle(hdc, _left, _top, _right, _bottom);

	// Отрисовка всех объектов в сцене
	for (const auto& curr : objects)
	{
		curr.second->draw(hdc);
	}
}

bool BrownianScene::logfile_is_opened()
{
	return _log_fs.is_open();
}
