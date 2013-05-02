#ifndef _light_h_
#define _light_h_

#include "util.h"
#include "Transformable.h"
#include "primitive.h"


class Light : public Transformable{
public:
    enum LightType {
        PointLight,
        AreaLight
    };
public:
    Light(LightType type) : _type(type) {};
    virtual ~Light() {};

    LightType getType() const {
        return _type;
    };
   
    XMFLOAT4 power;
private:
    LightType _type;
};

/*
* 1. What kind of light we are going to use? Point Light? Area Light?
* 2. 
*/
struct PointLight : public Light
{
	XMFLOAT3 direction;
	

    PointLight() : Light(LightType::PointLight) {};

	PointLight(const XMFLOAT3& pos, const XMFLOAT3& dir, const XMFLOAT4& power ) 
		: Light(LightType::PointLight), direction( dir )
	{
        this->power = power;
        setPosition(pos);
	}
};

// For now, the area light is valid only if this area light is a rectangle that parallel either xy plane, yz plane or yz plane.
struct AreaLight : public Light
{
	XMFLOAT3 direction;
	double   width;
	double   height;

    AreaLight() : Light(LightType::AreaLight) {};
	AreaLight( const XMFLOAT3& dir, const XMFLOAT4& power, double& w, double& h ) 
		: Light(LightType::AreaLight), direction( dir ), width( w ), height( h)
	{
         this->power = power;
	}
};

#endif