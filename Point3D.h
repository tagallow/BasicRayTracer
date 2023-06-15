#pragma once

#include <math.h>

class Point3D
{
public:
	float x,y,z;
public:

	Point3D(void)
	{
		x = y = z = 0.0f;
	}

	Point3D(float x,float y,float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Point3D(const Point3D& P)
	{
		x = P.x;
		y = P.y;
		z = P.z;
	}

	void init(float x,float y,float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float dist(Point3D p)
	{
		float dist = 0;

		dist = (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) + (z-p.z)*(z-p.z);
		return sqrtf(dist);
	}

	~Point3D(void)
	{
	}
};
