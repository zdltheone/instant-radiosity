#include "instantRadiosity.h"
#include "math.h"

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

void InstantRadiosity::EmitVPLs( double average_reflectivity, const Scene* scene )
{
	vector<const Light*> lightVec = ( scene->getLights( Light::AreaLight ) );
	AreaLight* areaLight = (AreaLight*)lightVec[ 0 ];

	//AreaLight areaLight = scene->getLights( Light::AreaLight )[ 0 ];

	double    lightAttenuationFactor = 0.8;
	for( int i = 0; i < m_sampleNum; i++ )
	{
		// Sample light position at the start point
		double pos_x = m_rngGenerator.PhiBDirected( 2, i );
		double pos_z = m_rngGenerator.PhiBDirected( 3, i );

		pos_x *= areaLight->width;
		pos_z *= areaLight->height;

		//cout << pos_x * areaLight.width << " " << pos_z * areaLight.height << endl;

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

			float t = -1.0f;
			// Normalized the light direction
			XMVECTOR tmp = XMLoadFloat3( &lightDirection );
			tmp = XMVector3Normalize( tmp );
			XMStoreFloat3( &lightDirection, tmp );
            
            XMFLOAT3 normal;
			if( scene->intersectScene( Ray( lightStartPoint, lightDirection ), normal, t ) == NULL )
			{
				break;
			}

			cout << t << endl;

			// Ray hit nothing so we terminate this path
			if( t < 0 )
			{
				break;
			}

			// Calculate the hit point;
			lightStartPoint.x *= t;
			lightStartPoint.y *= t;
			lightStartPoint.z *= t;

			//cout << lightStartPoint.x << " " << lightStartPoint.y << " " << lightStartPoint.z << endl;
		}
	}
}

XMFLOAT4 InstantRadiosity::GetRadiance( XMFLOAT3 intersectionPoint, XMFLOAT3 intersectionNormal, Scene* scene )
{
	XMFLOAT4 accumulateContribution;
	for( int i = 0; i < m_VPLVec.size(); i++ )
	{
		float t;
		XMFLOAT3 pointToVPL = XMFloat3Sub( m_VPLVec[ i ].getPosition(), intersectionPoint );
		// Get the normal of hitpoint
		XMFLOAT3 hitPointSurfaceNormal = intersectionNormal;

		// Evaluate BRDF
		XMVECTOR pointToVPLVec = XMLoadFloat3( &pointToVPL );
		XMVECTOR hitPointSurfaceNormalVec = XMLoadFloat3( &hitPointSurfaceNormal );
		XMVECTOR dotResult = XMVector3Dot( pointToVPLVec, hitPointSurfaceNormalVec );
		XMFLOAT3 dotResultFloat3;
		XMStoreFloat3( &dotResultFloat3, dotResult );
		float diffuse = dotResultFloat3.x;

		// Check whether any object obscured the light to this hitpoint
        XMFLOAT3 normal;
		scene->intersectScene( Ray( intersectionPoint, pointToVPL ), normal, t );
		XMFLOAT3 testRayHitPoint( intersectionPoint.x * t, intersectionPoint.y * t, intersectionPoint.z * t );

		float dis1 = math_length( pointToVPL );
		float dis2 = math_distance( intersectionPoint, testRayHitPoint );

		// Accumulate light contribution if the virtual point light's radiance can reach this point
		if( fabs( dis1 - dis2 ) < 0.001f )
		{
			XMFLOAT4 plColor = m_VPLVec[ i ].power;
			float factor = 1.0f / m_VPLVec.size() * diffuse;
			plColor.x *= factor;
			plColor.y *= factor;
			plColor.z *= factor;
			plColor.w *= factor;
			accumulateContribution.x += plColor.x;
			accumulateContribution.y += plColor.y;
			accumulateContribution.z += plColor.z;
			accumulateContribution.w += plColor.w;
		}
 	}

	return accumulateContribution;
}