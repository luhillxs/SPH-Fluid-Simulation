#pragma once

#include "vector2D.h"
#include "color.h"

using namespace CMU462;

class Particle {
public:
	Vector2D pos; // position
	Vector2D v;	// velocity
	double m; // mass
	double d; // density
	double p; // pressure
	Color c; // color
	Vector2D f; // force

	Vector2D a; // acceleration

	Particle* next;
};

class Cell {
public:
	Particle *head;
};
