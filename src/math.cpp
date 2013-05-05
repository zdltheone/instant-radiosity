#include "math.h"

float math_length(const XMFLOAT3& vector) {
    XMVECTOR vec = XMLoadFloat3(&vector);
    XMVECTOR lengthVec = XMVector3Length(vec);

    float length = 0.f;
    XMStoreFloat(&length, lengthVec);
    return length;
}

float math_distance(const XMFLOAT3& point1, const XMFLOAT3& point2) {
    XMVECTOR p1 = XMLoadFloat3(&point1);
    XMVECTOR p2 = XMLoadFloat3(&point2);
    XMVECTOR distanceVec = XMVectorSubtract(p1, p2);
    XMVECTOR length = XMVector3Length(distanceVec);

    float distance = 0.0f;
    XMStoreFloat(&distance,length);
    return distance;
}

float toRadians(float degrees) {
    return degrees * TO_RADIANS;
}

void math_normalize(XMFLOAT3& vec) {
    XMVECTOR v = XMLoadFloat3(&vec);
    v = XMVector3Normalize(v);

    XMStoreFloat3(&vec, v);
}

XMFLOAT3 XMFloat3Sub( const XMFLOAT3& point1, const XMFLOAT3& point2 )
{
	XMVECTOR p1 = XMLoadFloat3( &point1 );
	XMVECTOR p2 = XMLoadFloat3( &point2 );
	XMVECTOR subResult = XMVectorSubtract( p1, p2 );
	XMFLOAT3 result;
	XMStoreFloat3( &result, subResult );

	return result;
}

XMFLOAT3 operator +( const XMFLOAT3& float_one, const XMFLOAT3& float_two )
{
	return XMFLOAT3( float_one.x + float_two.x, float_one.y + float_two.y, float_one.z + float_two.z );
}

XMFLOAT3 operator +( const XMFLOAT3& float_one, const double& eps )
{
	return XMFLOAT3( float_one.x + eps, float_one.y + eps, float_one.z + eps );
}

XMFLOAT3 operator *( const XMFLOAT3& float_one, const double& factor )
{
	return XMFLOAT3( float_one.x * factor, float_one.y * factor, float_one.z * factor );
}

XMFLOAT4 operator *( const XMFLOAT4& float_one, const double& factor )
{
	return XMFLOAT4( float_one.x * factor, float_one.y * factor, float_one.z * factor, float_one.w * factor );
}