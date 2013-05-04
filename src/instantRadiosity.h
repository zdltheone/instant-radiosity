#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "randomNumber.h"
#include "light.h"
#include "util.h"
#include "geometryObjects.h"
#include "primitive.h"
#include "scene.h"

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
	void EmitVPLs( double average_reflectivity, Scene* scene );
	XMFLOAT3 GetRadiance( const XMFLOAT3 intersectionPoint, const XMFLOAT3 intersectionNormal, const Scene* scene );

private:
	RandomNumberGenerator m_rngGenerator;
	unsigned int m_reflectionNum;
	unsigned int m_sampleNum;
	vector<PointLight> m_VPLVec;
};