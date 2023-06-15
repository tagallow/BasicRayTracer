#pragma once

#include "Point3D.h"
#include "Vector3D.h"

class Ray
{
public:
        Point3D s;
        Vector3D v;
public:

	Ray(void)
	{
	}

        Ray(Point3D s,Vector3D v)
	{
                this->s = s;
                this->v = v;
	}

	~Ray(void)
	{
	}
};
