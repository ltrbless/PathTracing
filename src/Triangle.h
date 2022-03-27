#ifndef _TRIANGEN_H_
#define _TRIANGEN_H_

#include "Global.h"
#include "AABBbox.h"

class Material;
class Ray;
class AABBbox;

class TriMesh{

public:
    vec3d v1, v2, v3;
    vec2d t1, t2, t3;

    Material* material;
    AABBbox box;
    int id;

    TriMesh();
    bool InitBox();
    AABBbox GetBox();
    vec3d GetCenter();
    vec3d GetRandomPoint();
    vec3d GetNormal();
    double GetArea();
    bool JudgeIntersection(Ray& ray, double& t, vec3d& b_corrd, vec3d& inter_p);
};


#endif //