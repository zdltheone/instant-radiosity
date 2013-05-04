#include "instantRadiosity.h"
#include "math.h"

const double eps = 0.001f;

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

	double    lightAttenuationFactor = 0.8 / acos( -1 );
	for( int i = 1; i <= m_sampleNum; i++ )
	{
		// Sample light position at the start point
		double pos_x = m_rngGenerator.PhiBDirected( 2, i ) - 0.5f;
		double pos_z = m_rngGenerator.PhiBDirected( 3, i ) - 0.5f;

		pos_x *= areaLight->width;
		pos_z *= areaLight->height;

		// Suppose the radiance is equal everywhere on the rectangle area light source
		XMFLOAT4 rad( 1.0f, 1.0f, 1.0f, 1.0f );

		XMFLOAT3 lightStartPoint( pos_x, 25.0f, pos_z );

		for( int reflectionIter = 0; reflectionIter < m_reflectionNum; reflectionIter++ )
		{
			double pdf_refl = pow( average_reflectivity, reflectionIter );
			double now_flux = 1.0f / pdf_refl;

			// Store virtual point light, the second parameter is not use yet.
			//m_VPLVec.push_back( PointLight( lightStartPoint, XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT4( rad.x * lightAttenuationFactor, rad.y * lightAttenuationFactor, rad.z * lightAttenuationFactor, rad.w * lightAttenuationFactor ) ) );
			m_VPLVec.push_back( PointLight( lightStartPoint, XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT4( rad.x * now_flux, rad.y * now_flux, rad.z * now_flux, rad.w * now_flux  ) ) );

			// Sample direction, use sphere polar coordinates
			double phi = asin( sqrt( m_rngGenerator.PhiBDirected( m_rngGenerator.GetithPrimeNumber( 2 * reflectionIter + 2 ), i ) ) );
			double theta = 2 * acos( -1 ) * m_rngGenerator.PhiBDirected( m_rngGenerator.GetithPrimeNumber( 2 * reflectionIter + 3 ), i );

			XMFLOAT3 pointAtSampleSphere( sin( phi ) * cos( theta ), sin( phi ) * sin( theta ), cos( phi ) );
			// Translate this point to sphere's location
			XMVECTOR translatedVec = XMLoadFloat3( &pointAtSampleSphere );
			XMMATRIX transMatrix = XMMatrixTranslation( lightStartPoint.x, lightStartPoint.y, lightStartPoint.z );
			translatedVec = XMVector3Transform( translatedVec, transMatrix );
			XMStoreFloat3( &pointAtSampleSphere, translatedVec );
			
			// Calculate the reflection direction
			XMFLOAT3 lightDirection( pointAtSampleSphere.x - lightStartPoint.x, pointAtSampleSphere.y - lightStartPoint.y, pointAtSampleSphere.z - lightStartPoint.z );

			//cout << pointAtSampleSphere.x << " " << pointAtSampleSphere.y << " " << pointAtSampleSphere.z << endl;
			//cout << "Radius is " << pow( lightDirection.x, 2 ) + pow( lightDirection.y, 2 ) + pow( lightDirection.z, 2 ) << endl;

			// The original equation is rad *= Kd(x) / pi. I assume that Kd(x) is a constant for every single triangle's material
			rad.x *= lightAttenuationFactor; /*0.5 / acos( -1 );*/
			rad.y *= lightAttenuationFactor;
			rad.z *= lightAttenuationFactor;
			rad.w *= lightAttenuationFactor;

			float t = -1.0f;
			// Normalized the light direction
			XMVECTOR tmp = XMLoadFloat3( &lightDirection );
			tmp = XMVector3Normalize( tmp );
			XMStoreFloat3( &lightDirection, tmp );
            
            XMFLOAT3 normal;
			const Primitive *primitive = NULL;
			if( ( primitive = scene->intersectScene( Ray( lightStartPoint + lightDirection * eps, lightDirection ), normal, t ) ) == NULL || t < 0 )
			{
				break;
			}

			//cout << t << endl;
			if( primitive->getType() == 0 )
			{
				cout << "Hit a Sauare!" << endl;
			}
			else if( primitive->getType() == 1 )
			{
				cout << "Hit a Sphere!" << endl;
			}
			else
			{
				cout << "Hit a Cube!" << endl; 
			}
			// Calculate the hit point;
			lightStartPoint.x += lightDirection.x * t;
			lightStartPoint.y += lightDirection.y * t;
			lightStartPoint.z += lightDirection.z * t;
		}
	}
}

XMFLOAT3 InstantRadiosity::GetRadiance( const XMFLOAT3 intersectionPoint, const XMFLOAT3 intersectionNormal, const  Scene* scene )
{
	XMFLOAT4 accumulateContribution( 0.0f, 0.0f, 0.0f, 0.0f );
	static unsigned int num = 0;
	for( int i = 0; i < m_VPLVec.size(); i++ )
	{
		float t;
		XMFLOAT3 pointToVPL = XMFloat3Sub( m_VPLVec[ i ].getPosition(), intersectionPoint );
		// Get the normal of hitpoint
		XMFLOAT3 hitPointSurfaceNormal = intersectionNormal;

		// Evaluate BRDF
		XMVECTOR pointToVPLVec = XMLoadFloat3( &pointToVPL );
		XMVECTOR hitPointSurfaceNormalVec = XMLoadFloat3( &hitPointSurfaceNormal );
		pointToVPLVec = XMVector3Normalize( pointToVPLVec );
		hitPointSurfaceNormalVec = XMVector3Normalize( hitPointSurfaceNormalVec );

		XMVECTOR dotResult = XMVector3Dot( pointToVPLVec, hitPointSurfaceNormalVec );
		XMFLOAT3 dotResultFloat3;
		XMStoreFloat3( &dotResultFloat3, dotResult );
		float diffuse = dotResultFloat3.x;

		if( diffuse < 0 )
		{
			continue;
		}

		// Shoot shadow ray
        XMFLOAT3 normal;
		const Primitive *primitive = NULL;
		if( ( primitive = scene->intersectScene( Ray( intersectionPoint + pointToVPL * eps, pointToVPL ), normal, t ) ) == NULL || ( t < 0 && fabs( t ) > 0.0001 ) )
		{
			continue;
		}

		XMFLOAT3 testRayHitPoint( intersectionPoint.x * t, intersectionPoint.y * t, intersectionPoint.z * t );

		float dis1 = math_distance( intersectionPoint, m_VPLVec[ i ].getPosition() );
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
			num++;
		}
 	}

	//cout << "The total vpls is " << m_VPLVec.size() << " , and number of vpls that make contribution to this point is " << num << endl;
	//if( num != 0 )
	//{
	//	cout << "Hit something" << endl;
	//}
	return XMFLOAT3( accumulateContribution.x, accumulateContribution.y, accumulateContribution.z );
}