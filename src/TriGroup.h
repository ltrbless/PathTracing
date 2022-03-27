#ifndef TRIGROUP
#define TRIGROUP

// #include "Triangle.h"
#include "AABBbox.h"
#include "KdTree.h"

class TriGroup{
public:
    std::vector<TriMesh> triVec;
    std::string materialName;
};

#endif //TRIG