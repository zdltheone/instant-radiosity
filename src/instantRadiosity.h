#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "randomNumber.h"
#include "light.h"
#include "util.h"
#include "geometryObjects.h"
#include "raytracer.h"

using namespace std;

class InstantRadiosity
{
public:
	InstantRadiosity();
	InstantRadiosity( const InstantRadiosity& other );
	~InstantRadiosity();

	void SetReflectionNum( unsigned int number );
	unsigned int GetReflectionNum();
	void SetSampleNum( unsigned int number );
	unsigned int GetSampleNum();
	void EmitVPLs( vector<AreaLight>* areaLightContainer, double average_reflectivity, RayTracer* raytracer );

private:
	RandomNumberGenerator m_rngGenerator;
	unsigned int m_reflectionNum;
	unsigned int m_sampleNum;
	vector<PointLight> m_VPLVec;
};