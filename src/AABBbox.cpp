#include "AABBbox.h"
#include "Ray.h"

AABBbox::AABBbox()
{
    double mx = std::numeric_limits<double>::max();
    double mi = std::numeric_limits<double>::min();

    this->minn = vec3d( mx, mx, mx );
    this->maxx = vec3d( mi, mi, mi );

}

void AABBbox::AddBox(AABBbox& otherbox)
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
    vec3d normal;
    double tx_min, ty_min, tz_min;
    double tx_max, ty_max, tz_max;

    // x axis
    normal = vec3d(1, 0, 0);
    tx_min = (this->minn - ray.origin).dot(normal) / ( ray.direction.dot(normal) );
    tx_max = (this->maxx - ray.origin).dot(normal) / ( ray.direction.dot(normal) );
    if(tx_min > tx_max) std::swap(tx_min, tx_max);

    // y axis
    normal = vec3d(0, 1, 0);
    ty_min = (this->minn - ray.origin).dot(normal) / ( ray.direction.dot(normal) );
    ty_max = (this->maxx - ray.origin).dot(normal) / ( ray.direction.dot(normal) );
    if(ty_min > ty_max) std::swap(ty_min, ty_max);

    // z axis
    normal = vec3d(0, 0, 1);
    tz_min = (this->minn - ray.origin).dot(normal) / ( ray.direction.dot(normal) );
    tz_max = (this->maxx - ray.origin).dot(normal) / ( ray.direction.dot(normal) );
    if(tz_min > tz_max) std::swap(tz_min, tz_max);

    double min_max = std::max( tx_min, std::max( ty_min, tz_min ) );
    double max_min = std::min( tx_max, std::min( ty_max, tz_max ) );

    t = min_max;

    return (min_max < max_min && min_max >= 0);
}