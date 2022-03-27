#ifndef _TOOL_H_
#define _TOOL_H_

#include <random>

namespace tool {

// Ref:https://www.cnblogs.com/egmkang/archive/2012/09/06/2673253.html
inline double GetUniformRandomDouble(double x = 0.0, double y = 0.0)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double> dist(x, y);
    return dist(rng);
}

inline void AddProcessBar(int ind, int h)
{
    std::cout << "[";
    int loc = 100 * (ind + 1) / h;
    for(int p = 0; p < 100; p++)
    {
        if(p < loc) std::cout << "=";
        else std::cout << ".";
    }
    std::cout << "]  " << loc << "%\r";
    std::cout.flush();
}

inline void CoutVec3d(vec3d a){
    std::cout << a.x() << " " << a.y() << " " << a.z() << "\n";
}

inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

}

#endif // tool.h