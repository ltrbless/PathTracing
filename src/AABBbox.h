#ifndef _AABB_H_
#define _AABB_H_

#include "Global.h"
#include <limits>

class Ray;

class AABBbox
{
public:
    vec3d minn, maxx;

    AABBbox();

    void AddBox(AABBbox& otherBox);
    int GetLongestAxis();    
    bool JudgeIntersection(Ray& ray, double& t);
};


#endif //