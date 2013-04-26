#include "geometryObjects.h"

void SolidGeometryPrimitive::SetCenter( XMFLOAT3 center )
{
	m_center = center;
}

XMFLOAT3 SolidGeometryPrimitive::GetCenter()
{
	return m_center;
}

bool SolidGeometryPrimitive::intersect(const Ray& ray, double& tOut) const {
    return false;
}

/*
* Sphere member functions definitions
*/
void Sphere::SetRadius( double radius )
{
	m_radius = radius;
}

double Sphere::GetRadius()
{
	return m_radius;
}

bool Sphere::intersect(const Ray& ray, double& tOut) const {
    double A, B, C;

	XMFLOAT3 t_pos = ray.position;
	XMFLOAT3 t_dir = ray.direction;

	// TODO Transform from world coordinate system to sphere's local coordinate system

	A = 1;
	B = ( t_dir.x * t_pos.x + t_dir.y * t_pos.y + t_dir.z * t_pos.z ) * 2;
	C = pow( t_pos.x, 2 ) + pow( t_pos.y, 2 ) + pow( t_pos.z, 2 ) - pow( m_radius, 2 );

	double delta = pow( B, 2 ) - 4 * C;

	if( delta < 0 )
	{
		return false;
	}

	tOut = ( -B - sqrt( delta ) ) / 2.0f;

	if( tOut < 0 )
	{
		tOut = ( -B + sqrt( delta ) ) / 2.0f;
	}

	return true;
}

/*
* Cube member functions definitions
*/
void Cube::SetEdgeLen( double length )
{
	m_edgeLen = length;
}

double Cube::GetEdgeLen()
{
	return m_edgeLen;
}



bool Cube::intersect(const Ray& ray, double& tOut) const {
    XMFLOAT3 t_pos = ray.position;
	XMFLOAT3 t_dir = ray.direction;

	// TODO Transform from world coordinate system to sphere's local coordinate system

	double edgeLen = m_edgeLen;

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

	tOut = T_near;

    //move this to a different function if we need it
    /*
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
    */
	return true;
}


bool GeometryPrimitive::intersect(const Ray& ray, double& tOut) const {
    return false;
}

/*
* Square member functions definitions
*/
void Square::SetEdgeLen( double length )
{
	m_edgeLen = length;
}

double Square::GetEdgeLen()
{
	return m_edgeLen;
}

bool Square::intersect(const Ray& ray, double& tOut) const {
    return false;
}