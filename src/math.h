#ifndef _math_h_
#define _math_h_

#include "util.h"

const float INF = 1e20f;
const float EPSILON = 1e-6f;
const float TO_RADIANS = XM_PI/180.f;

float math_length(const XMFLOAT3& vector);
float math_distance(const XMFLOAT3& point1, const XMFLOAT3& point2);
float toRadians(float degrees);
void math_normalize(XMFLOAT3& vec);


#endif