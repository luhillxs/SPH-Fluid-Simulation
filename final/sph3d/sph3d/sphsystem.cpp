#include "stdafx.h"

#define BOUNDARY 0.0001

SPHSystem::SPHSystem() {
	//std::cout << "construct sph" << std::endl;
	pNum = 0; // num_particle
	pNumMax = 30000; // max_particle
	particles = new Particle[pNumMax];	
	
	h = 0.04; // radius of kernel
	h2 = h * h; // kernel_2: kernel * kernel
	mass = 0.02;

	worldSize.x = 0.64; worldSize.y = 0.64; worldSize.z = 0.64; // world_size
	cellSize = h; //cell_size
	gridSize.x = (unsigned int)ceil(worldSize.x / h);	gridSize.y = (unsigned int)ceil(worldSize.y / h);	gridSize.z = (unsigned int)ceil(worldSize.z / h);//grid_size, 16*16*16
	cellNum = gridSize.x * gridSize.y * gridSize.z; //tot_cell, 4096
	cell = new Particle*[cellNum];
	//std::cout << "gridSize: (" << gridSize.x << ", " << gridSize.y << ", " << gridSize.z << " ), cellNum: " << cellNum << std::endl;

	

	timeStep = 0.003; //time_step
	gravity.x = 0.0; gravity.y = - 6.8; gravity.z = 0.0;

	wallDamping = -0.5f; // wall_damping
	restDens = 1000.0; //rest_density
	R = 1.0; // gas_constant
	viscosity = 6.5;
	surfNorm = 6.0; // surf_norm, surface tension tipping point
	surfCoe = 0.1; // surf_coe

	poly6 = 315.0 / (64.0 * PI * pow(h,9)); // poly6_value
	spiky = -45.0 / ( PI * pow(h,6)); // spiky_value
	visco = 45.0 / ( PI * pow(h,6)); // visco_value

	poly6Grad = -945 / ( 32 * PI * pow(h, 9)); // gradient grad_poly6
	poly6Lapl = -945 / ( 8 * PI * pow(h, 9)); // laplace lplc_poly6

	kDens = mass * poly6 * pow(h,6); // self_dens
	kColorLapl = mass * poly6Lapl * h2 * (0 - 3 / 4 * h2); // self_lplc_color

}

SPHSystem::~SPHSystem() {
	//std::cout << "deconstruct sph" << std::endl;
	delete[]particles;
	delete[]cell;
}

void SPHSystem::init() {
	//std::cout << " - initial particles" << std::endl;

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
	//std::cout << " - sph try run" << std::endl;
	if (sysRunning) {
		buildTable();
		calcDensPress();
		calcForceAdv();
		update();
	}
}

void SPHSystem::buildTable() {
	//std::cout << " -- built cell hash table" << std::endl;
	Particle *p;
	unsigned int hash;

	for (unsigned int i = 0; i < cellNum; i++) {
		cell[i] = NULL;
	}

	for (unsigned int i = 0; i < pNum; i++) {
		p = &(particles[i]);

		// get position in grid(cell) of particle
		p->cellPos.x = int(floor(p->pos.x / cellSize));
		p->cellPos.y = int(floor(p->pos.y / cellSize));
		p->cellPos.z = int(floor(p->pos.z / cellSize));

		hash = calCellHash(p->cellPos);
		//std::cout << " --- cell hash: " << hash << std::endl;
		
		if (cell[hash] == NULL) {
			p->next = NULL;
		}
		else {
			p->next = cell[hash];
		}
		cell[hash] = p;

	}

	
}

void SPHSystem::calcDensPress() {
	Particle *p;
	Particle *np; //neighbour

	// p->cellPos cell_pos
	int3 nPos; // near_pos
	unsigned int hash;

	Vector3D deltaPos; // rel_pos, p->pos - np->pos

	double r2; // r^2

	// traverse every particle
	for (unsigned int i = 0; i < pNum; i++) {
		p = &(particles[i]);

		p->dens = 0.0;
		p->press = 0.0;

		// traverse all neighbour cells
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = -1; z <= 1; z++) {
					// get all particles in a neighbour cell
					nPos.x = p->cellPos.x + x;
					nPos.y = p->cellPos.y + y;
					nPos.z = p->cellPos.z + z;
					hash = calCellHash(nPos);
					if (hash == 0xffffffff)	continue;
					np = cell[hash];

					// traverse all particles in a cell
					while (np != NULL) {
						deltaPos = p->pos - np->pos;
						r2 = deltaPos.norm2();

						if ( r2<h2 && r2>EPS_D ) {
							p->dens += mass * poly6 * pow(h2 - r2, 3);
						}
						np = np->next;
					} // end while: traverse all particles in a cell

				}
			}
		} // end for(for(for())): traverse all neighbour cells

		p->dens = p->dens + kDens;
		p->press = (pow(p->dens / restDens, 7) - 1) * R; // Tait equation
	} // end for: traverse every particle
}


void SPHSystem::calcForceAdv() {
	Particle *p;
	Particle *np; //neighbour

	// p->cellPos cell_pos
	int3 nPos; // near_pos
	unsigned int hash;

	Vector3D deltaPos; // rel_pos, p->pos - np->pos
	Vector3D deltaVel; // rel_vel, np

	double r; // radius between 2 particles
	double r2; // r^2
	double h_r; // kernel_r, kernel radius(h) - r
	double volume; // V

	double kPress; // pres_kernel
	double kVisco; // visc_kernel

	double tempForce; // temp_force

	// surface tension
	Vector3D colorGrad; // grad_color
	double colorLapl; // lplc_color

	for (unsigned int i = 0; i < pNum; i++) {
		p = &(particles[i]);

		// init accelerate and color gradient and laplace
		for (int j = 0; j < 3; j++) { 
			p->acc[j] = 0.0;
			colorGrad[j] = 0.0;
		}
		colorLapl = 0.0;

		// traverse all neighbour cells
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = -1; z <= 1; z++) {
					// get all particles in a neighbour cell
					nPos.x = p->cellPos.x + x;
					nPos.y = p->cellPos.y + y;
					nPos.z = p->cellPos.z + z;
					hash = calCellHash(nPos);
					if (hash == 0xffffffff)	continue;
					np = cell[hash];

					// traverse all particles in a cell
					while (np != NULL) {
						deltaPos = p->pos - np->pos;
						r2 = deltaPos.norm2();

						if ( r2<h2 && r2>EPS_D ){
							r = deltaPos.norm();
							volume = mass / np->dens / 2;
							h_r = h - r;

							kPress = spiky * h_r * h_r;
							tempForce = volume * (p->press + np->press) * kPress;
							p->acc -= deltaPos * tempForce / r;

							deltaVel = np->ev - p->ev;
							kVisco = visco * (h_r);
							tempForce = volume * viscosity * kVisco;
							p->acc += deltaVel * tempForce;

							double temp = (-1) * poly6Grad * volume * pow(h2 - r2, 2);
							colorGrad += temp * deltaPos;
							colorLapl += poly6Lapl * volume * (h2 - r2) * (r2 - 3 / 4 * (h2 - r2));
						}
						np = np->next;
					} // end whild: traverse all particles in a cell

				}
			}
		} // end for(for(for())): get all particles in a neighbour cell

		colorLapl += kColorLapl / p->dens;
		p->surfNorm = colorGrad.norm();

		if ( p->surfNorm > surfNorm) {
			p->acc += surfCoe * colorLapl * colorGrad / p->surfNorm;
		}
	} // end for: traverse every particle

}

void SPHSystem::update() {
	//std::cout << " -- update velocity and position" << std::endl;
	Particle *p;
	for (unsigned int i=0; i < pNum;i++) {
		p = &(particles[i]);

		p->vel += p->acc * timeStep / p->dens + gravity * timeStep;

		p->pos += p->vel * timeStep;

		// Bounding from the boundary
		for (int j = 0; j < 3;j++) {
			if (p->pos[j] >= worldSize[j] - BOUNDARY) {
				p->vel[j] *= wallDamping;
				p->pos[j] = worldSize[j] - BOUNDARY;
			}
			if (p->pos[j] < 0.0) {
				p->vel[j] *= wallDamping;
				p->pos[j] = 0.0 + BOUNDARY;
			}
		}

		p->ev = (p->ev + p->vel) / 2.0; // TODO

	}
}

unsigned int SPHSystem::calCellHash(int3 pos) {
	if (pos.x < 0 || pos.x >= int(gridSize.x) || pos.y < 0 || pos.x >= int(gridSize.y) || pos.z < 0 || pos.z >= int(gridSize.z)) {
		return (unsigned int)0xffffffff;
	}

	pos.x = pos.x & (gridSize.x - 1);
	pos.y = pos.y & (gridSize.y - 1);
	pos.z = pos.z & (gridSize.z - 1);

	return ((unsigned int)(pos.x)) + ((unsigned int)(pos.y)) * gridSize.x + ((unsigned int)(pos.z)) * gridSize.x * gridSize.y;
}