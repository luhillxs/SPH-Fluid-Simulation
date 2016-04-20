#include "stdafx.h"

SPHSystem::SPHSystem() {
	particles.resize(pNumMax);

}

SPHSystem::~SPHSystem() {

}

void SPHSystem::update() {
	for (unsigned int i = 0; i < pNum;i++) {
		particles[i].v = particles[i].v + particles[i].a * timeStep / particles[i].d + gravity * timeStep;
		particles[i].pos = particles[i].pos + particles[i].v * timeStep;
		particles[i].ev = (particles[i].ev + particles[i].v) / 2.0;
	}
}