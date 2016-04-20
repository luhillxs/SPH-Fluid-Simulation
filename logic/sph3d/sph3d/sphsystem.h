#pragma once

#include "Particle.h"

#include "CMU462.h"
#include "misc.h"
#include "vector3D.h"

#include <vector>

#define BOUNDARY 0.0001f

struct uint3
{
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

class SPHSystem {
public:
	SPHSystem();
	~SPHSystem();

	std::vector<Particle> particles;
	unsigned int pNum;
	unsigned int pNumMax;

	std::vector<Particle*> cell;
	unsigned int cellNum;
	double cellSize;
	uint3 gridSize;

	double h; // radius of kernel
	double mass;

	double timeStep;
	Vector3D gravity;
	double wallDamping;

	Vector3D worldSize;
	

private:
	void update();

};


