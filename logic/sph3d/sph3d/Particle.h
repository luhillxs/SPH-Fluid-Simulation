#pragma once

#include "vector3D.h"
//#include "color.h"

#include <ostream>

using namespace CMU462;

class Particle {
public:
	unsigned int id;
	Vector3D pos; // position
	Vector3D vel;	// velocity
	//double mass; // mass
	double dens; // density
	double press; // pressure
	//Color c; // color
	//Vector3D force; // force

	Vector3D acc; // acceleration
	Vector3D ev;

	Particle* next;

	void printParticle();
};
