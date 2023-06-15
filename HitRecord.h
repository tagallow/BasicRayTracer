#pragma once

#include "Ray.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Texture.h"

class HitRecord
{
public:
        Point3D p;
        Vector3D n;
        Material material;
        Texture *texture;
	float t;
	float tex_s,tex_t;
public:
	HitRecord(void)
	{
		t = 0;
		tex_s = tex_t = -1;
                texture = NULL;
	}
	~HitRecord(void)
	{
	}
};
