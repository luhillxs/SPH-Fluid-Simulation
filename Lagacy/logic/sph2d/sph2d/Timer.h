#pragma once

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

class Timer {
public:
	Timer() {
		frames = 0;
		updateTime = 1000;
		lastTime = 0;
		fps = 0.0;
	}
	bool update();
	double getFPS();

private:
	int frames;
	int updateTime; 
	int lastTime; // last update system time
	double fps;
};