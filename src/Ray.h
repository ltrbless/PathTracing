#ifndef _RAY_H_
#define _RAY_H_

#include "Global.h"
#include "Triangle.h"
#include <limits.h>

class Ray{ // origin + direction * t;

public:
    // ray information
    vec3d origin, direction;

    // intersection information
    double t;
    vec3d b_corrd;
    TriMesh tri; // Intersection unit of mesh

    Ray() : t( std::numeric_limits<double>::max() ){};

    static vec3d RayTrace(Ray& ray);

};




#endif /* _RAY_H_ */