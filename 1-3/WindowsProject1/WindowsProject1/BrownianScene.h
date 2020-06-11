#pragma once


#include <map>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>

#include "Ball.h"

class BrownianScene
{
	//Файл лога
	std::ofstream _log_fs;
	size_t _left, _top, _right, _bottom;
public:
	//Контейнер
	std::map<int, std::shared_ptr<Ball>> objects;
	bool is_freeze;
	//Конструктор с деструктором
	BrownianScene(size_t left_, size_t top_, size_t right_, size_t bottom_);
	~BrownianScene();

	void update();
	void draw(HDC);

	bool logfile_is_opened();
};

