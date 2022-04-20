#include "Triangle.h"
#include "Material.h"
#include "Ray.h"
#include "tool.h"

TriMesh::TriMesh() : v1(0, 0, 0), v2(0, 0, 0), v3(0, 0, 0) {};

bool TriMesh::InitBox()
{
    this->box.minn = v1.cwiseMin(v2).cwiseMin(v3);
    this->box.maxx = v1.cwiseMax(v2).cwiseMax(v3);
    return 1;
}

AABBbox TriMesh::GetBox()
{
    // InitBox();
    return this->box;
}

vec3d TriMesh::GetCenter()
{
    return (v1 + v2 + v3) / 3.0;
}

vec3d TriMesh::GetRandomPoint()
{
    double a, b, c, sum;
    a = tool::GetUniformRandomDouble(0, 1);
    b = tool::GetUniformRandomDouble(0, 1);
    c = tool::GetUniformRandomDouble(0, 1);
    sum = a + b + c;
    a /= sum;
    b /= sum;
    c = 1 - a - b;
    return v1 * a + v2 * b + v3 * c;
}

double TriMesh::GetArea()
{
    vec3d e1 = v2 - v1;
    vec3d e2 = v3 - v1;
    return e1.cross(e2).norm() / 2;
}

//Ref: Moller Trumbore Algorithm
bool TriMesh::JudgeIntersection(Ray& ray, double& t, vec3d& b_corrd, vec3d& inter_p)
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

    inter_p = b_corrd.x() * v1 + b_corrd.y() * v2 + b_corrd.z() * v3;
    
    return ( ans.x() >= 0 && ans.y() >= 0 && ans.z() >= 0 && (1 - ans.y() - ans.z()) >= 0);
}

vec3d TriMesh::GetNormal()
{
    vec3d e12 = v2 - v1;
    vec3d e13 = v3 - v1;
    vec3d normal = e12.cross(e13);
    normal.normalize();
    return normal;
}

vec2d TriMesh::GetTextureCorrd(vec3d b_corrd)
{
    return this->t1 * b_corrd.x() + this->t2 * b_corrd.y() + this->t3 * b_corrd.z();
}