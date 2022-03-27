#ifndef _RAY_H_
#define _RAY_H_

#include "Global.h"
#include "Triangle.h"
#include <limits.h>

class Ray{ // origin + direction * t;

public:
    
    enum RayType
    {
        DIFFUSE,
        SPECULAR
    };
    
    // ray information
    vec3d origin, direction;

    // intersection information
    bool bool_intersection;
    double t;
    vec3d b_corrd;
    vec3d inter_point; // intersection point
    TriMesh tri; // Intersection unit of mesh
    RayType raytype;
    

    Ray() : t( std::numeric_limits<double>::max() ), bool_intersection(false){};

    Ray(vec3d o, vec3d d) : origin(o), direction(d), t( std::numeric_limits<double>::max()), bool_intersection(false){};

    Ray(vec3d o, vec3d d, RayType type) : origin(o), direction(d), raytype(type), t( std::numeric_limits<double>::max()), bool_intersection(false){};

    static vec3d RayTrace(Ray& ray);
    void clear(){
        t = std::numeric_limits<double>::max();
        bool_intersection = false;
    }

};




#endif /* _RAY_H_ */