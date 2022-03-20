#ifndef _TRIANGEN_H_
#define _TRIANGEN_H_

#include "Global.h"
#include "Material.h"
#include "AABBbox.h"

class TriMesh{

public:
    vec3d v1, v2, v3;
    vec2d t1, t2, t3;

    Material* material;
    AABBbox box;

    TriMesh();
    bool InitBox();
    AABBbox GetBox();
    vec3d GetCenter();
    bool JudgeIntersection(Ray& ray, double& t, vec3d& b_corrd);
};


#endif //