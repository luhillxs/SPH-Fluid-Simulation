#pragma once

#include "vector3D.h"
//#include "color.h"

using namespace CMU462;

class Particle {
public:
	unsigned int id;
	Vector3D pos; // position
	Vector3D v;	// velocity
	double m; // mass
	double d; // density
	double p; // pressure
	//Color c; // color
	Vector3D f; // force

	Vector3D a; // acceleration
	Vector3D ev;

	Particle* next;
};
