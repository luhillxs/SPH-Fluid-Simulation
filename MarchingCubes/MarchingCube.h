#pragma once
#ifndef __MARCHINGCUBE_H__
#define __MARCHINGCUBE_H__

#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

struct float3
{
	float x;
	float y;
	float z;
};

class MarchingCube
{
private:

	uint row_vox;
	uint col_vox;
	uint len_vox;
	uint tot_vox;
	float step;

	float *scalar;
	float3 *normal;
	float3 *pos;
	float3 origin;

	float isovalue;

public:

	MarchingCube(uint _row_vox, uint _col_vox, uint _len_vox, float *_scalar, float3 *_pos, float3 _origin, float _step, float _isovalue);
	~MarchingCube();
	void run();
};

#endif