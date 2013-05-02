#pragma once

#include "util.h"
#include "primitive.h"
#include "ray.h"
#include "math.h"


// For 3D shape
class SolidGeometryPrimitive : public Primitive
{
public:
	SolidGeometryPrimitive(Primitive::PrimitiveType type) : Primitive(type), m_surfaceArea( 0.0f ), m_volume( 0.0f ), m_center( XMFLOAT3( 0.0f, 0.0f, 0.0f ) ){}
	~SolidGeometryPrimitive(){}

public:
	void SetCenter( XMFLOAT3 center );
	XMFLOAT3 GetCenter();

    virtual bool intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const;

private:
	double m_surfaceArea;
	double m_volume;
	XMFLOAT3 m_center;
};

class Sphere : public SolidGeometryPrimitive
{
public:
    Sphere() : SolidGeometryPrimitive(PrimitiveType::Sphere), m_radius( 0.0f ){};
	~Sphere(){}

	void SetRadius( double radius );
	double GetRadius();

    XMFLOAT3 getNormal(const XMFLOAT3& point) const;
    bool intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const;
	
private:
	double m_radius;
};

class Cube : public SolidGeometryPrimitive
{
public:
    Cube() : SolidGeometryPrimitive(PrimitiveType::Cube), m_edgeLen( 0.0f ){}
	~Cube(){}

	void SetEdgeLen( double length );
	double GetEdgeLen();
    
    XMFLOAT3 getNormal(const XMFLOAT3& point) const;
    bool intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const;

private:
	double m_edgeLen;
};


// For 2D shape
class GeometryPrimitive : public Primitive
{
public:
    GeometryPrimitive(Primitive::PrimitiveType type) : Primitive(type), m_area( 0.0f ){};
	~GeometryPrimitive(){};

    virtual bool intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const;

private:
	double m_area;
};

class Square : public GeometryPrimitive
{
public:
    Square() : GeometryPrimitive(PrimitiveType::Square), m_edgeLen( 0.0f ){}
	~Square();

	void SetEdgeLen( double length );
	double GetEdgeLen();

    bool intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const;

private:
	double m_edgeLen;
};

