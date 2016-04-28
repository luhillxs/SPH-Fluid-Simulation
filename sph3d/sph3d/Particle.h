#pragma once

#include "vector3D.h"
//#include "color.h"

#include <ostream>

using namespace CMU462;

struct uint3
{
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

struct int3
{
	int x;
	int y;
	int z;
};

class Particle {
public:
	unsigned int id;
	Vector3D pos; // position
	Vector3D vel;	// velocity
	double dens; // density
	double press; // pressure

	int3 cellPos; // particle in which cell

	Vector3D acc; // acceleration

	double surfNorm; // for surface tension, colorGrad.norm()

	Particle* next;

	void printParticle();
};
