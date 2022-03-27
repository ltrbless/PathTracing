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

    void AddBox(AABBbox otherBox);
    int GetLongestAxis();    
    bool JudgeIntersection(Ray& ray, double& t);

    friend std::ostream &operator<<( std::ostream &output, const AABBbox &B )
    { 
        output << B.minn.x() << " " << B.minn.y() << " " << B.minn.z() << "\n" <<
                  B.maxx.x() << " " << B.maxx.y() << " " << B.maxx.z() << "\n";
        return output;            
    }
};


#endif //