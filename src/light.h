#ifndef _light_h_
#define _light_h_

#include "util.h"
#include "Transformable.h"
#include "primitive.h"


class Light : public Transformable{
public:
    Light() {};
    virtual ~Light() {};

    XMFLOAT4 emission;
};

/*
* 1. What kind of light we are going to use? Point Light? Area Light?
* 2. 
*/
struct PointLight : public Light
{
	XMFLOAT3 direction;
	XMFLOAT4 color;

    PointLight() {};

	PointLight(const XMFLOAT3& pos, const XMFLOAT3& dir, const XMFLOAT4& power ) 
		: direction( dir ), color( power )
	{
        setPosition(pos);
	}
};

// For now, the area light is valid only if this area light is a rectangle that parallel either xy plane, yz plane or yz plane.
struct AreaLight : public Light
{
	XMFLOAT3 direction;
	XMFLOAT4 color;
	double   width;
	double   height;

    AreaLight() {};
	AreaLight( const XMFLOAT3& dir, const XMFLOAT4& power, double& w, double& h ) 
		: direction( dir ), color( power ), width( w ), height( h)
	{
	}
};

#endif