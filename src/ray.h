#ifndef _ray_h_
#define _ray_h_

#include "util.h"

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

    void getPointAlongRay(double t, XMFLOAT3& pointOut) const {
         XMVECTOR originVec = XMLoadFloat3(&position);
         XMVECTOR directionVec = XMLoadFloat3(&direction);
         XMStoreFloat3(&pointOut, originVec + directionVec * t);
    };

     XMFLOAT3 getPointAlongRay(double t) const {
         XMFLOAT3 pointOut;
         XMVECTOR originVec = XMLoadFloat3(&position);
         XMVECTOR directionVec = XMLoadFloat3(&direction);
         XMStoreFloat3(&pointOut, originVec + directionVec * t);

         return pointOut;
    };

	inline Ray& operator=( const Ray& ray )
	{
		position = ray.position;
		direction = ray.direction;
	}
};

#endif