#pragma once

#include "Particle.h"

#include "CMU462.h"
#include "misc.h"
#include "vector3D.h"

#include <vector>


class SPHSystem {
public:
	//  1. new SPHSystem;
	//	2. call init();
	//	3. call run();
	//	4. call setRunning() if want it to move; call stopRunning to stop;
	//	5. get *particles and use i = getpNum(), particles[i]->pos to traverse all particles' position;   
	//	6. call delete sph to deconstruct.
	SPHSystem();
	~SPHSystem();

	void init(); // init_system

	// animation, system run
	void run();  // call setRunning() to let it run
	inline void setRunning() { sysRunning = true; }
	inline void stopRunning() { sysRunning = false; }
	inline bool getRunningState() { return sysRunning; }

	// get partical status for drawing
	Particle* particles;
	inline unsigned int getpNum() { return pNum; }

	// get other variable might needed
	inline Vector3D getWorldSize() { return worldSize; }




private:
	void update(); // advection, updates every particles' velocity & position 
	void buildTable(); // build_table, build the hash table of cells
	void calcDensPress(); // calculate density and pressure
	void calcForceAdv(); 

	unsigned int calCellHash(int3 pos); 


	unsigned int pNum; // number of particles
	unsigned int pNumMax; // maximum

	Vector3D worldSize;
	double cellSize;
	uint3 gridSize;
	unsigned int cellNum; // total cell
	Particle** cell;

	double h; // radius of kernel
	double h2; // kernel_2: kernel * kernel
	double mass;
	double kDens; 
	double kColorLapl;

	double timeStep;
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


