#pragma once

#include "util.h"
#include "primitive.h"
#include "ray.h"


// For 3D shape
class SolidGeometryPrimitive : public Primitive
{
public:
	SolidGeometryPrimitive() : m_surfaceArea( 0.0f ), m_volume( 0.0f ), m_center( XMFLOAT3( 0.0f, 0.0f, 0.0f ) ){}
	SolidGeometryPrimitive( const SolidGeometryPrimitive& otherInstance ) : m_surfaceArea( 0.0f ), m_volume( 0.0f ), m_center( XMFLOAT3( 0.0f, 0.0f, 0.0f ) ){}
	~SolidGeometryPrimitive(){}

public:
	void SetCenter( XMFLOAT3 center );
	XMFLOAT3 GetCenter();

    virtual bool intersect(const Ray& ray, double& tOut) const;

private:
	double m_surfaceArea;
	double m_volume;
	XMFLOAT3 m_center;
};

class Sphere : SolidGeometryPrimitive
{
public:
	Sphere() : m_radius( 0.0f ){}
	Sphere( const Sphere& otherInstance ) : m_radius( 0.0f ){}
	~Sphere(){}

	void SetRadius( double radius );
	double GetRadius();

    bool intersect(const Ray& ray, double& tOut) const;
	
private:
	double m_radius;
};

class Cube : SolidGeometryPrimitive
{
public:
	Cube() : m_edgeLen( 0.0f ){}
	Cube( const Cube& otherInstance ) : m_edgeLen( 0.0f ){}
	~Cube(){}

	void SetEdgeLen( double length );
	double GetEdgeLen();

    bool intersect(const Ray& ray, double& tOut) const;

private:
	double m_edgeLen;
};


// For 2D shape
class GeometryPrimitive : public Primitive
{
public:
	GeometryPrimitive() : m_area( 0.0f ){};
	GeometryPrimitive( const GeometryPrimitive& otherInstance ) : m_area( 0.0f ){};
	~GeometryPrimitive(){};

    virtual bool intersect(const Ray& ray, double& tOut) const;

private:
	double m_area;
};

class Square : GeometryPrimitive
{
public:
	Square() : m_edgeLen( 0.0f ){}
	Square( const Square& otherInstance ) : m_edgeLen( 0.0f ){}
	~Square();

	void SetEdgeLen( double length );
	double GetEdgeLen();

    bool intersect(const Ray& ray, double& tOut) const;

private:
	double m_edgeLen;
};

