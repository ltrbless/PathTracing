#include "Triangle.h"

TriMesh::TriMesh() : v1(0, 0, 0), v2(0, 0, 0), v3(0, 0, 0) {};

bool TriMesh::InitBox()
{
    this->box.minn = v1.cwiseMin(v2).cwiseMin(v3);
    this->box.maxx = v1.cwiseMax(v2).cwiseMax(v3);
    return 1;
}

AABBbox TriMesh::GetBox()
{
    return this->box;
}

vec3d TriMesh::GetCenter()
{
    return (v1 + v2 + v3) / 3.0;
}

//Ref: Moller Trumbore Algorithm
bool TriMesh::JudgeIntersection(Ray& ray, double& t, vec3d& b_corrd)
{
    vec3d e1 = v2 - v1;
    vec3d e2 = v3 - v1;
    vec3d s = ray.origin - v1;
    vec3d s1 = ray.direction.cross(e2);
    vec3d s2 = s.cross(e1);
    vec3d ans = vec3d(s2.dot(e2), s1.dot(s), s2.dot(ray.direction)) / (s1.dot(e1));
    
    t = ans.x();
    b_corrd.x() = 1 - ans.y() - ans.z();
    b_corrd.y() = ans.y();
    b_corrd.z() = ans.z();
    
    return ( ans.x() >= 0 && ans.y() >= 0 && ans.z() >= 0 && (1 - ans.y() - ans.z()) >= 0);
}