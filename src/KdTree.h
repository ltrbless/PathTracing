#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "Global.h"
#include <vector>
#include "Ray.h"
#include "Triangle.h"
#include "AABBbox.h"
#include <iostream>

class KdTree{

public:

    std::vector<TriMesh> tri_lst;
    KdTree* left;
    KdTree* right;
    bool leaf;
    AABBbox box;
    static int all_node;

    KdTree() : tri_lst(std::vector<TriMesh>()), left(nullptr), right(nullptr), leaf(false) {};
    
    void BuildKdTree(std::vector<TriMesh>& tri_lst, int deep);
    void BuildKdTree2(std::vector<TriMesh>& tri_lst, int deep);
    void GetIntersection(Ray& ray);
    void ViewKdTree(std::string path);

};


#endif // _KDTREE_H_