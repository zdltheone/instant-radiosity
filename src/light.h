#ifndef _light_h_
#define _light_h_

#include "util.h"

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

class Light {
public:
	Light();
	~Light();
private:
};

#endif