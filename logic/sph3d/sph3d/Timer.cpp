#include "stdafx.h"
#include "Timer.h"

bool Timer::update() {
	this->frames++;
	int currentTime = GetTickCount();
	int delta = abs(currentTime - this->lastTime);
	//std::cout << int(GetTickCount()) << "this:" << this->lastTime<< std::endl;
	if (delta > this->updateTime) {
		this->fps = double(this->frames) / double(delta) * 1000.0;
		this->frames = 0;
		this->lastTime = currentTime;
		return true;
	}
	return false;
}

double Timer::getFPS() {
	return this->fps;
}