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
}

}


#endif // tool.h