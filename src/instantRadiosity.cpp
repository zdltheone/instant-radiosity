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

void InstantRadiosity::EmitVPLs( vector<AreaLight>* areaLightContainer, double average_reflectivity, RayTracer* raytracer )
{
	AreaLight areaLight = (*areaLightContainer)[ 0 ];
	double    lightAttenuationFactor = 0.8;
	for( int i = 0; i < m_sampleNum; i++ )
	{
		// Sample light position at the start point
		double pos_x = m_rngGenerator.PhiBDirected( 2, i );
		double pos_z = m_rngGenerator.PhiBDirected( 3, i );

		//XMFLOAT4 rad = areaLight.color * ;
		// Suppose the radiance is equal everywhere on the rectangle area light source
		XMFLOAT4 rad( 1.0f, 1.0f, 1.0f, 1.0f );

		XMFLOAT3 lightStartPoint( pos_x, 5.0f, pos_z );

		for( int reflectionIter = 0; reflectionIter < m_reflectionNum; reflectionIter++ )
		{
			// TODO pdf reflective this term is used in the paper but we don't need this for now
			double pdf_refl = pow( average_reflectivity, reflectionIter );

			// Store virtual point light, the second parameter is not use yet.
			m_VPLVec.push_back( PointLight( lightStartPoint, XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT4( rad.x * lightAttenuationFactor, rad.y * lightAttenuationFactor, rad.z * lightAttenuationFactor, rad.w * lightAttenuationFactor ) ) );

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

			raytracer->traceRayOnce( Ray( lightStartPoint, lightDirection ), t );

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