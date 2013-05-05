#include "geometryObjects.h"

void SolidGeometryPrimitive::SetCenter( XMFLOAT3 center )
{
	m_center = center;
}

XMFLOAT3 SolidGeometryPrimitive::GetCenter()
{
	return m_center;
}

bool SolidGeometryPrimitive::intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const {
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


XMFLOAT3 Sphere::getNormal(const XMFLOAT3& point) const {
    XMFLOAT4 worldOrigin;
    XMStoreFloat4(&worldOrigin, XMVector3Transform(XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)), 
                                XMLoadFloat4x4(&this->getTransformation())));

    XMVECTOR vec1 = XMLoadFloat3(&point);
    XMVECTOR vec2 = XMLoadFloat3(&XMFLOAT3(worldOrigin.x, worldOrigin.y, worldOrigin.z));
    XMFLOAT3 normal;
    XMStoreFloat3(&normal, XMVector3Normalize(vec1 - vec2));
    
    return normal;
}

bool Sphere::intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const {
    double A, B, C;

	XMFLOAT3 t_pos = ray.position;
	XMFLOAT3 t_dir = ray.direction;

	//transform sphere to world coordinates
    XMFLOAT4 worldOrigin;
    XMStoreFloat4(&worldOrigin, XMVector3Transform(XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)), 
                                XMLoadFloat4x4(&this->getTransformation())));

	A = 1;
	B = 2 * ( (t_dir.x * (t_pos.x - worldOrigin.x)) + (t_dir.y * (t_pos.y - worldOrigin.y)) + (t_dir.z * (t_pos.z - worldOrigin.z)) );
	C = pow( (t_pos.x - worldOrigin.x), 2 ) + pow( (t_pos.y - worldOrigin.y), 2 ) + pow( (t_pos.z - worldOrigin.z), 2 ) - pow( m_radius, 2 );

	double delta = pow( B, 2 ) - (4 * A * C);

	if( delta < 0 )
	{
		return false;
	}

	tOut = ( -B - sqrt( delta ) ) / 2.0f;

	if( tOut < 0 )
	{
		tOut = ( -B + sqrt( delta ) ) / 2.0f;
	}

    normalOut = getNormal(ray.getPointAlongRay(tOut));

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



bool Cube::intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const {
    XMFLOAT3 t_pos = ray.position;
	XMFLOAT3 t_dir = ray.direction;

	//transform ray into local coordinates
    XMMATRIX world = XMLoadFloat4x4(&this->getTransformation());
    XMMATRIX inverse = XMMatrixInverse(&XMMatrixDeterminant(world), world);
    XMStoreFloat3(&t_pos, XMVector3Transform(XMLoadFloat3(&t_pos), inverse));
    XMStoreFloat3(&t_dir, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&t_dir), inverse)));

	double edgeLen = m_edgeLen;

	double box_min[ 3 ] = { -edgeLen / 2.0, -edgeLen / 2.0f, -edgeLen / 2.0f };
	double box_max[ 3 ] = { edgeLen / 2.0, edgeLen / 2.0f, edgeLen / 2.0f };
	double t_arrayDir[ 3 ] = { t_dir.x, t_dir.y, t_dir.z };
	double t_arrayPos[ 3 ] = { t_pos.x, t_pos.y, t_pos.z };
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
				normalOut = XMFLOAT3( 1.0f, 0.0f, 0.0f );
			}
			else
			{
				normalOut = XMFLOAT3( -1.0f, 0.0f, 0.0f );
			}
			break;
		}
	case 1:
		{
			XMStoreFloat3( &result, XMVector3Dot( t_vDir, XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ) ) );
			if( result.x > 0 )
			{
				normalOut = XMFLOAT3( 0.0f, 1.0f, 0.0f );
			}
			else
			{
				normalOut = XMFLOAT3( 0.0f, -1.0f, 0.0f );
			}
			break;
		}
	case 2:
		{
			XMStoreFloat3( &result, XMVector3Dot( t_vDir, XMLoadFloat3( &XMFLOAT3( 0.0f, 0.0f, 1.0f ) ) ) );
			if( result.x > 0 )
			{
				normalOut = XMFLOAT3( 0.0f, 0.0f, 1.0f );
			}
			else
			{
				normalOut = XMFLOAT3( 0.0f, 0.0f, -1.0f );
			}
			break;
		}
	}

    //transform normal to world coordinates
    XMVECTOR vec = XMVector3TransformNormal( XMLoadFloat3(&normalOut), world);
    XMStoreFloat3(&normalOut, XMVector3Normalize(vec));

	return true;
}


bool GeometryPrimitive::intersect(const Ray& ray, XMFLOAT3& normalOut, float& tOut) const {
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

bool Square::intersect( const Ray& ray, XMFLOAT3& normalOut, float& tOut ) const 
{
	XMFLOAT3 t_pos = ray.position;
	XMFLOAT3 t_dir = ray.direction;

    //transform ray into local coordinates
	XMMATRIX world = XMLoadFloat4x4( &this->getTransformation() );
    XMMATRIX inverse = XMMatrixInverse( &XMMatrixDeterminant(world), world );
    XMMATRIX transpose = XMMatrixTranspose(world);
    XMMATRIX inverseTranspose = XMMatrixInverse( &XMMatrixDeterminant(transpose), transpose );
	XMStoreFloat3( &t_pos, XMVector3Transform( XMLoadFloat3(&t_pos), inverse ) );
	XMStoreFloat3( &t_dir, XMVector3Normalize(XMVector3TransformNormal( XMLoadFloat3(&t_dir), inverse ) ));

    float edgelen = m_edgeLen;
    XMFLOAT3 tl(-edgelen, 0.f,  edgelen);
    XMFLOAT3 br( edgelen, 0.f, -edgelen);

  //  XMStoreFloat3(&tl, XMVector3TransformNormal(XMLoadFloat3(&tl), world));
  //  XMStoreFloat3(&br, XMVector3TransformNormal(XMLoadFloat3(&br), world));

    if(t_dir.y == 0.f) {
        return false;
    }
	
	double t = -( t_pos.y ) / ( t_dir.y );

    if(t < 0 ) {
        return false;
    }

	XMFLOAT3 intersectionPoint = XMFLOAT3( t_pos.x + t_dir.x * t, t_pos.y + t_dir.y * t, t_pos.z + t_dir.z * t );
	if( intersectionPoint.x < tl.x || intersectionPoint.x > br.x )
	{
		return false;
	}

	if( intersectionPoint.z < br.z || intersectionPoint.z > tl.z )
	{
		return false;
	}

	tOut = t;

    //transform normal from local(xz) to world coordinates
    XMVECTOR vec = XMVector3Transform( XMLoadFloat3(&XMFLOAT3(0.f, 1.f, 0.f)), inverseTranspose);
    XMStoreFloat3(&normalOut, XMVector3Normalize(vec));

    return true;
}