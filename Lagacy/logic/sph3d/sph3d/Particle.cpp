#include "stdafx.h"
#include "Particle.h"

void Particle::printParticle(){
	std::cout << "=========" << id << "=========" << std::endl;
	std::cout << " pos: " << pos << " vel: " << vel << std::endl;
	std::cout << " dens: " << dens << " press: " << press << std::endl;
	std::cout << " acc: " << acc << " ev: " << ev << std::endl;
}
