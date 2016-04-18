// sph2d.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Timer.h"
#include "vector2D.h"

using namespace CMU462;

int main()
{
	Timer *timer = new Timer();
	Vector2D vc;
	int sec = 0;
	while(sec<10) {
		if (timer->update()) {
			std::cout << "FPS: " << timer->getFPS() << std::endl;
			++sec;
		}
	}

    return 0;
}

