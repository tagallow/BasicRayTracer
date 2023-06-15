#pragma once

#include "Point3D.h"

class Vector3D
{
public:
	float x,y,z;
public:
	Vector3D(void)
	{
		x = y = z = 0.0f;
	}

	Vector3D(float x,float y,float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D(const Vector3D& V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
	}

	Vector3D(Point3D From,Point3D To)
	{
		x = To.x - From.x;
		y = To.y - From.y;
		z = To.z - From.z;
	}

	void init(float x,float y,float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void init(Point3D From,Point3D To)
	{
		x = To.x - From.x;
		y = To.y - From.y;
		z = To.z - From.z;
	}


	float dotProduct(Vector3D& V)
	{
		return x*V.x + y*V.y + z*V.z;
	}

	void normalize()
	{
		float mag;

		mag = sqrtf(x*x + y*y + z*z);
		if (mag!=0)
		{
			x/=mag;
			y/=mag;
			z/=mag;
		}
	}

	float mag()
	{
		return sqrtf(x*x + y*y + z*z);
	}

	Vector3D operator+(Vector3D& V)
	{
		Vector3D R;

		R.x = x+V.x;
		R.y = y+V.y;
		R.z = z+V.z;
		return R;
	}

	Vector3D operator-(Vector3D& V)
	{
		Vector3D R;

		R.x = x-V.x;
		R.y = y-V.y;
		R.z = z-V.z;
		return R;
	}



	Vector3D crossProduct(Vector3D& V)
	{
		Vector3D R;

		R.x = y*V.z - V.y*z;
		R.y = -(x*V.z - z*V.x);
		R.z = x*V.y - y*V.x;
		return R;
	}


	~Vector3D(void)
	{
	}
};
