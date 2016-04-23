#pragma once

#include "Particle.h"

#include "CMU462.h"
#include "misc.h"
#include "vector3D.h"

#include <vector>

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
	void init(); // init_system

	// animation, system run
	void run(); 
	void setRunning() {
		sysRunning = true;
	}
	void stopRunning() {
		sysRunning = false;
	}
	inline bool getRunningState() {
		return sysRunning;
	}

	// get partical status for drawing
	Particle* particles;
	inline unsigned int getpNum() {
		return pNum;
	}



private:
	void update(); // advection, updates every particles' velocity & position 
	void buildTable(); // build_table, build the hash table of cells

	unsigned int cellHash(int cx, int cy, int cz); // calc_cell_hash


	unsigned int pNum; // num_particle
	unsigned int pNumMax; // max_particle

	Vector3D worldSize; // world_size
	double cellSize; //cell_size
	uint3 gridSize; //grid_size
	unsigned int cellNum; //tot_cell
	Particle** cell;

	double h; // radius of kernel
	double h2; // kernel_2: kernel * kernel
	double mass;
	double kDens; // self_dens
	double kColorLapl;

	double timeStep; //time_step
	Vector3D gravity;

	
	double wallDamping; // wall_damping
	double restDens; //rest_density
	double R; // gas_constant
	double viscosity;
	double surfNorm; // surf_norm
	double surfCoe; // surf_coe

	double poly6; // poly6_value
	double spiky; // spiky_value
	double visco; // visco_value

	double poly6Grad; // gradient grad_poly6
	double poly6Lapl; // laplace lplc_poly6

	bool sysRunning;

};


