#pragma once
#include "Object.h"

//represents a plane of side 1 lying on the X-Z plane centered at the origin
class Plane : public Object
{
public:
    Plane(string name="");
    bool intersect(Ray r,HitRecord& h);
    ~Plane(void);
};
