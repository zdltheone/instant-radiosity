#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "randomNumber.h"
#include "util.h"
#include "geometryObjects.h"

using namespace std;

/*
* 1. What kind of light we are going to use? Point Light? Area Light?
* 2. 
*/

struct PointLight
{
	XMFLOAT3 position;
	XMFLOAT3 direction;
	XMFLOAT4 color;

	PointLight( const XMFLOAT3& pos, const XMFLOAT3& dir, const XMFLOAT4& power ) 
		: position( pos ), direction( dir ), color( power )
	{
	}
};

// For now, the area light is valid only if this area light is a rectangle that parallel either xy plane, yz plane or yz plane.
struct AreaLight
{
	XMFLOAT3 centerPosition;
	XMFLOAT3 direction;
	XMFLOAT4 color;
	double   width;
	double   height;

	AreaLight( const XMFLOAT3& pos, const XMFLOAT3& dir, const XMFLOAT4& power, double& w, double& h ) 
		: centerPosition( pos ), direction( dir ), color( power ), width( w ), height( h)
	{
	}
};

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
	void EmitVPLs( vector<AreaLight>* areaLightContainer, double average_reflectivity );
	double TraceRay( Ray ray );
	bool IntersectScene( Ray ray, double& t );

private:
	RandomNumberGenerator m_rngGenerator;
	unsigned int m_reflectionNum;
	unsigned int m_sampleNum;

	vector<PointLight> m_VPLVec;
};