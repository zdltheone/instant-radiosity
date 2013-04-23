#include "geometryObjects.h"

void SolidGeometryPrimitive::SetCenter( XMFLOAT3 center )
{
	m_center = center;
}

XMFLOAT3 SolidGeometryPrimitive::GetCenter()
{
	return m_center;
}

/*
* Shpere member functions definitions
*/
void Shpere::SetRadius( double radius )
{
	m_radius = radius;
}

double Shpere::GetRadius()
{
	return m_radius;
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