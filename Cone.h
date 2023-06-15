/*
 * Cone.h
 *
 *  Created on: Sep 13, 2009
 *      Author: ashesh
 */

#ifndef CONE_H_
#define CONE_H_

#include "Object.h"

//represents a cone of unit radius and height 1 standing upright with its circular face on the X-Z plane and axis along +Y axis

class Cone: public Object {
public:
    Cone(string name="");
    virtual ~Cone();
};

#endif /* CONE_H_ */
