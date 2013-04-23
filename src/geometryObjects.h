#pragma once

#include "util.h"

// For 1D shape
struct Ray
{
	XMFLOAT3 position;
	XMFLOAT3 direction;

	Ray( XMFLOAT3 pos, XMFLOAT3 dir )
	{
		position = pos;
		direction = dir;
	}

	Ray()
	{
		position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		direction = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}

	inline Ray& operator=( const Ray ray )
	{
		position = ray.position;
		direction = ray.direction;
	}
};

// For 3D shape
class SolidGeometryPrimitive
{
public:
	SolidGeometryPrimitive() : m_surfaceArea( 0.0f ), m_volume( 0.0f ), m_center( XMFLOAT3( 0.0f, 0.0f, 0.0f ) ){}
	SolidGeometryPrimitive( const SolidGeometryPrimitive& otherInstance ) : m_surfaceArea( 0.0f ), m_volume( 0.0f ), m_center( XMFLOAT3( 0.0f, 0.0f, 0.0f ) ){}
	~SolidGeometryPrimitive(){}

public:
	void SetCenter( XMFLOAT3 center );
	XMFLOAT3 GetCenter();

private:
	double m_surfaceArea;
	double m_volume;
	XMFLOAT3 m_center;
};

class Shpere : SolidGeometryPrimitive
{
public:
	Shpere() : m_radius( 0.0f ){}
	Shpere( const Shpere& otherInstance ) : m_radius( 0.0f ){}
	~Shpere(){}

	void SetRadius( double radius );
	double GetRadius();
	
	friend bool RaySphereIntersectTest( Ray& ray, Shpere& shpere, double& t )
	{
		double A, B, C;

		XMFLOAT3 t_pos = ray.position;
		XMFLOAT3 t_dir = ray.direction;

		// TODO Transform from world coordinate system to sphere's local coordinate system


		A = 1;
		B = ( t_dir.x * t_pos.x + t_dir.y * t_pos.y + t_dir.z * t_pos.z ) * 2;
		C = pow( t_pos.x, 2 ) + pow( t_pos.y, 2 ) + pow( t_pos.z, 2 ) - pow( shpere.m_radius, 2 );

		double delta = pow( B, 2 ) - 4 * C;

		if( delta < 0 )
		{
			return false;
		}

		t = ( -B - sqrt( delta ) ) / 2.0f;

		if( t < 0 )
		{
			t = ( -B + sqrt( delta ) ) / 2.0f;
		}

		return true;
	}

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

	friend bool RayCubeIntersectTest( Ray& ray, Cube& cube, double& t, XMFLOAT3& normal )
	{
		XMFLOAT3 t_pos = ray.position;
		XMFLOAT3 t_dir = ray.direction;

		// TODO Transform from world coordinate system to sphere's local coordinate system

		double edgeLen = cube.m_edgeLen;

		double box_min[ 3 ] = { -edgeLen / 2.0, -edgeLen / 2.0f, -edgeLen / 2.0f };
		double box_max[ 3 ] = { edgeLen / 2.0, edgeLen / 2.0f, edgeLen / 2.0f };
		double t_arrayDir[ 3 ] = { t_dir.x, t_dir.y, t_dir.z };
		double t_arrayPos[ 3 ] = { t_pos.x, t_dir.y, t_dir.z };
		double T_near = -999999.0f, T_far = 999999.0f;

		int sideRecorder = -1;
		for( int i = 0; i < 3; i++ )
		{
			double T_1 = 0.0f, T_2 = 0.0f;

			T_1 = ( box_min[ i ] - t_arrayPos[ i ] ) / t_arrayDir[ i ];
			T_2 = ( box_max[ i ] - t_arrayPos[ i ] ) / t_arrayDir[ i ];

			if( T_1 > T_2 )
			{
				double pivot = T_1;
				T_1 = T_2;
				T_2 = pivot;
			}

			if( T_1 > T_near ) { T_near = T_1; sideRecorder = i; }
			if( T_2 < T_far )  T_far = T_2;

			if( T_near > T_far ) return false;
			if( T_far < 0.0f )   return false;
		}

		t = T_near;

		XMVECTOR t_vDir = XMLoadFloat3( &XMFLOAT3( t_dir.x * -1, t_dir.y * -1, t_dir.z * -1 ) );
		XMFLOAT3 result;
		switch( sideRecorder )
		{
		case 0:
			{
				// The dot product between v1 and v2 is replicated into each component
				XMStoreFloat3( &result, XMVector3Dot( t_vDir, XMLoadFloat3( &XMFLOAT3( 1.0f, 0.0f, 0.0f ) ) ) );
				if( result.x > 0 )
				{
					normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );
				}
				else
				{
					normal = XMFLOAT3( -1.0f, 0.0f, 0.0f );
				}
				break;
			}
		case 1:
			{
				XMStoreFloat3( &result, XMVector3Dot( t_vDir, XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ) ) );
				if( result.x > 0 )
				{
					normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
				}
				else
				{
					normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
				}
				break;
			}
		case 2:
			{
				XMStoreFloat3( &result, XMVector3Dot( t_vDir, XMLoadFloat3( &XMFLOAT3( 0.0f, 0.0f, 1.0f ) ) ) );
				if( result.x > 0 )
				{
					normal = XMFLOAT3( 0.0f, 0.0f, 1.0f );
				}
				else
				{
					normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );
				}
				break;
			}
		}

		return true;
	}

private:
	double m_edgeLen;
};


// For 2D shape
class GeometryPrimitive
{
public:
	GeometryPrimitive() : m_area( 0.0f ){};
	GeometryPrimitive( const GeometryPrimitive& otherInstance ) : m_area( 0.0f ){};
	~GeometryPrimitive(){};

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

	friend bool RaySquareIntersectTest( Ray& ray, Square& square, double& t )
	{
		XMFLOAT3 t_pos = ray.position;
		XMFLOAT3 t_dir = ray.direction;

		// TODO Transform from world coordinate system to sphere's local coordinate system

		t = -1.0f * ( t_pos.y / t_dir.y );

		double x = t_pos.x + t_dir.x * t;
		double z = t_pos.z + t_dir.z * t;
		double square_len = square.m_edgeLen / 2.0f;

		if( ( x >= -square_len && x <= square_len ) && ( z >= -square_len && z <= square_len ) )
		{
			return true;
		}

		t = 0.0f;

		return false;
	}

private:
	double m_edgeLen;
};

