#pragma once
#include "object.h"


//represents a sphere of unit radius centered at the origin
class Sphere :public Object
{
public:
    Sphere(string name="");
    bool intersect(Ray r,HitRecord& h);
	~Sphere(void);
};
