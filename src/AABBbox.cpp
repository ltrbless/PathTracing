#include "AABBbox.h"
#include "Ray.h"
#include <iostream>

AABBbox::AABBbox()
{
    double mx = 10000000;//std::numeric_limits<double>::max();
    double mi = -10000000; //std::numeric_limits<double>::min();

    this->minn = vec3d( mx, mx, mx );
    this->maxx = vec3d( mi, mi, mi );

}

void AABBbox::AddBox(AABBbox otherbox)
{
    this->minn = otherbox.minn.cwiseMin(this->minn);
    this->maxx = otherbox.maxx.cwiseMax(this->maxx);
}

int AABBbox::GetLongestAxis()
{
    vec3d tmp = this->maxx - this->minn;
    if(tmp.x() > tmp.y() && tmp.x() > tmp.z()) return 0;
    if(tmp.y() > tmp.z() && tmp.y() > tmp.x()) return 1;
    return 2;
}

bool AABBbox::JudgeIntersection(Ray& ray, double& t)
{
    double t_min, t_max;
    t_min = -100000000;
    t_max = 100000000;
    
    // x axis
    for(int i = 0; i < 3; i++)
    {
        if( fabs(ray.direction(i)) < 1e-5 ) 
        {
            if(ray.origin(i) < this->minn(i) || ray.origin(i) > this->maxx(i)) return false;
        }
        else
        {
            double tmp_min = (this->minn(i) - ray.origin(i)) / ray.direction(i);
            double tmp_max = (this->maxx(i) - ray.origin(i)) / ray.direction(i);
            if(tmp_min > tmp_max) std::swap(tmp_min, tmp_max);
            t_min = std::max(t_min, tmp_min);
            t_max = std::min(t_max, tmp_max);
        }
    }
    if(t_min > 0) t = t_min;
    else t = t_max;

    return (t_min < t_max && t_max >= 0);
}