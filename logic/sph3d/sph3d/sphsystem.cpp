#include "stdafx.h"

#define BOUNDARY 0.0001

SPHSystem::SPHSystem() {
	std::cout << "construct sph" << std::endl;
	pNum = 0; // num_particle
	pNumMax = 30000; // max_particle
	particles = new Particle[pNumMax];	
	
	h = 0.04; // radius of kernel
	//double h2; // kernel_2: kernel * kernel
	//double mass;
	//double kDens; // self_dens
	//double kColorLapl;

	worldSize.x = 0.64; worldSize.y = 0.64; worldSize.z = 0.64; // world_size
	cellSize = h; //cell_size
	gridSize.x = (unsigned int)ceil(worldSize.x / h);	gridSize.y = (unsigned int)ceil(worldSize.y / h);	gridSize.z = (unsigned int)ceil(worldSize.z / h);//grid_size, 16*16*16
	cellNum = gridSize.x * gridSize.y * gridSize.z; //tot_cell, 4096
	cell = new Particle*[cellNum];
	//std::cout << "gridSize: (" << gridSize.x << ", " << gridSize.y << ", " << gridSize.z << " ), cellNum: " << cellNum << std::endl;

	

	//double timeStep; //time_step
	//Vector3D gravity;

	//
	wallDamping = -0.5f; // wall_damping
	restDens = 1000.0; //rest_density
	//double R; // gas_constant
	//double viscosity;
	//double surfNorm; // surf_norm
	//double surfCoe; // surf_coe

	//double poly6; // poly6_value
	//double spiky; // spiky_value
	//double visco; // visco_value

	//double poly6Grad; // gradient grad_poly6
	//double poly6Lapl; // laplace lplc_poly6

}

SPHSystem::~SPHSystem() {
	std::cout << "deconstruct sph" << std::endl;
	delete[]particles;
	delete[]cell;
}

void SPHSystem::init() {
	std::cout << "initial particles" << std::endl;

	double initRange = 0.6;
	double delta = h * 0.5;

	Particle *p;

	for (double x = 0.0; x < worldSize.x * initRange; x += delta) {
		if (pNum >= pNumMax)	break;
		for (double y = 0.0; y < worldSize.y * initRange; y += delta) {
			if (pNum >= pNumMax)	break;
			for (double z = 0.0; z < worldSize.z * initRange; z += delta) {
				if (pNum >= pNumMax)	break;
				//std::cout << "x: " << x << "y: " << y << "z: " << z << std::endl;
				// add particle in particle list
				p = &(particles[pNum]);

				p->id = pNum;

				p->pos.x = x;	p->pos.y = y;	p->pos.z = z;
				p->vel.x = 0.0; p->vel.y = 0.0; p->vel.z = 0.0;
				p->dens = restDens;
				p->press = 0.0;

				p->acc.x = 0.0; p->acc.y = 0.0; p->acc.z = 0.0;
				p->ev.x = 0.0;	p->ev.y = 0.0;	p->ev.z = 0.0;

				p->next = NULL;

				++pNum;
			}
		}
	}

	if (pNum < pNumMax) pNumMax = pNum;

}

void SPHSystem::run() {
	if (sysRunning) {
		buildTable();
		update();
	}
}

void SPHSystem::buildTable() {
	std::cout << "built cell hash table" << std::endl;
	Particle *p;
	unsigned int hash;

	for (unsigned int i = 0; i < cellNum; i++) {
		cell[i] = NULL;
	}

	for (unsigned int i = 0; i < pNum; i++) {
		p = &(particles[i]);

		// get position in grid(cell) of particle
		int cx = int(floor(p->pos.x / cellSize));
		int cy = int(floor(p->pos.y / cellSize));
		int cz = int(floor(p->pos.z / cellSize));

		hash = cellHash(cx, cy, cz);

		if (cell[hash] == NULL) {
			p->next = NULL;
		}
		else {
			p->next = cell[hash];
		}
		cell[hash] = p;

	}

	
}

void SPHSystem::update() {
	Particle *p;
	for (unsigned int i=0; i < pNum;i++) {
		p = &(particles[i]);

		p->vel += p->acc * timeStep / p->dens + gravity * timeStep;

		p->pos += p->vel * timeStep;

		// Bounding from the boundary
		for (int i = 0; i < 3;i++) {
			if (p->pos[i] >= worldSize[i] - BOUNDARY) {
				p->vel[i] *= wallDamping;
				p->pos[i] = worldSize[i] - BOUNDARY;
			}
			if (p->pos[i] < 0.0) {
				p->vel[i] *= wallDamping;
				p->pos[i] = 0.0 + BOUNDARY;
			}
		}

		p->ev = (p->ev + p->vel) / 2.0; // TODO

	}
}

unsigned int SPHSystem::cellHash(int cx, int cy, int cz) {
	if (cx < 0 || cx >= int(gridSize.x) || cy < 0 || cx >= int(gridSize.y) || cz < 0 || cz >= int(gridSize.z)) {
		return (unsigned int)0xffffffff;
	}

	cx = cx & (gridSize.x - 1);
	cy = cy & (gridSize.y - 1);
	cz = cz & (gridSize.z - 1);

	return ((unsigned int)(cx)) + ((unsigned int)(cy)) * gridSize.x + ((unsigned int)(cz)) * gridSize.x * gridSize.y;
}