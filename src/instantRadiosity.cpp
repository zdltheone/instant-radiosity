#include "instantRadiosity.h"

InstantRadiosity::InstantRadiosity()
{
	m_rngGenerator.GeneratePrimeList( 10000 );
}

InstantRadiosity::InstantRadiosity( const InstantRadiosity& otherInstant )
{
	*this = otherInstant;
}

InstantRadiosity::~InstantRadiosity()
{
}

void InstantRadiosity::SetReflectionNum( unsigned int number )
{
	m_reflectionNum = number;
}

unsigned int InstantRadiosity::GetReflectionNum()
{
	return m_reflectionNum;
}

void InstantRadiosity::SetSampleNum( unsigned int number )
{
	m_sampleNum = number;
}

unsigned int InstantRadiosity::GetSampleNum()
{
	return m_sampleNum;
}

void InstantRadiosity::EmitVPLs( vector<AreaLight>* areaLightContainer, double average_reflectivity )
{
	AreaLight areaLight = (*areaLightContainer)[ 0 ];
	for( int i = 0; i < m_sampleNum; i++ )
	{
		// Sample light position at the start point
		double pos_x = m_rngGenerator.PhiBDirected( 2, i );
		double pos_z = m_rngGenerator.PhiBDirected( 3, i );

		// Calculate the light power at this point
		//XMFLOAT4 rad = areaLight.color * ;
		XMFLOAT4 rad( 1.0f, 1.0f, 1.0f, 1.0f );

		XMFLOAT3 lightStartPoint( pos_x, 5.0f, pos_z );
		//m_VPLVec.push_back( PointLight( XMFLOAT3( pos_x, 5.0f, pos_z ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), radianceAtPoint ) );

		for( int reflectionIter = 0; reflectionIter < m_reflectionNum; reflectionIter++ )
		{
			// pdf reflective
			double pdf_refl = pow( average_reflectivity, reflectionIter );

			// Store virtual point light, the second parameter is not use yet.
			m_VPLVec.push_back( PointLight( lightStartPoint, XMFLOAT3( 0.0f, -1.0f, 0.0f ), rad / pdf_refl ) );

			// Sample direction, use sphere polar coordinates
			double phi = asin( m_rngGenerator.PhiBDirected( m_rngGenerator.GetithPrimeNumber( 2 * reflectionIter + 2 ), i ) );
			double theta = 2 * acos( -1 ) * m_rngGenerator.PhiBDirected( m_rngGenerator.GetithPrimeNumber( 2 * reflectionIter + 3 ), i );
			
			XMFLOAT3 pointAtSampleSphere( sin( phi ) * cos( theta ), sin( phi ) * sin( theta ), cos( phi ) );
			// Calculate the reflection direction
			XMFLOAT3 lightDirection( pointAtSampleSphere.x - lightStartPoint.x, pointAtSampleSphere.y - lightStartPoint.y, pointAtSampleSphere.z - lightStartPoint.z );

			// The original equation is rad *= Kd(x) / pi. I assume that Kd(x) is a constant for every single triangle's material
			rad.x *= 0.5 / acos( -1 );
			rad.y *= 0.5 / acos( -1 );
			rad.z *= 0.5 / acos( -1 );
			rad.w *= 0.5 / acos( -1 );

			double t;
			// Normalized the light direction
			XMVECTOR tmp = XMLoadFloat3( &lightDirection );
			tmp = XMVector3Normalize( tmp );
			XMStoreFloat3( &lightDirection, tmp );

			t = TraceRay( Ray( lightStartPoint, lightDirection ) );

			// Ray hit nothing so we terminate this path
			if( t < 0 )
			{
				break;
			}

			// Calculate the hit point;
			lightStartPoint.x *= t;
			lightStartPoint.y *= t;
			lightStartPoint.z *= t;
		}
	}
}

double InstantRadiosity::TraceRay( Ray ray )
{
	double t = 0.0f;
	if( IntersectScene( ray, t ) )
	{
		return t;
	}

	return -1.0f;
}

bool InstantRadiosity::IntersectScene( Ray ray, double& t )
{
	double t = -100000.0f;

	// TODO we may need a scene's manager and store all the object in its instance
	// TODO need to delete
	vector<Shpere> sphereVec;
	vector<Square> squareVec;
	for( int i = 0; i < sphereVec.size(); i++ )
	{
		double tmpT = -10000000.0f;
		if( RaySphereIntersectTest( ray, sphereVec[ i ], tmpT ) )
		{
			if( tmpT < t )
			{
				t = tmpT;
			}
		}
	}

	for( int i = 0; i < squareVec.size(); i++ )
	{
		double tmpT = -10000000.0f;
		if( RaySquareIntersectTest( ray, squareVec[ i ], tmpT ) )
		{
			if( tmpT < t )
			{
				t = tmpT;
			}
		}
	}

	if( t != -100000.0f )
	{ 
		return true;
	}
	else
	{
		return false;
	}
}