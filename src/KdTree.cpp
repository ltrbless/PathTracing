#include "KdTree.h"

void KdTree::BuildKdTree(std::vector<TriMesh>& tri_lsts, int deep)
{

    for(int i = 0; i < tri_lsts.size(); i++) this->box.AddBox(tri_lsts[i].GetBox());
    
    if(tri_lsts.size() <= 5 || deep == 30)
    {
        this->leaf = true;
        this->tri_lst = tri_lsts;
        this->left = this->right = nullptr;
        return ;
    }

    int axis = this->box.GetLongestAxis();

    sort(tri_lsts.begin(), tri_lsts.end(), [&axis](TriMesh& v1, TriMesh& v2){
        double t1 = v1.GetCenter()(axis);
        double t2 = v2.GetCenter()(axis);
        return t1 < t2;
    });

    this->leaf = false;
    this->tri_lst = std::vector<TriMesh>();
    this->left = new KdTree();
    this->right = new KdTree();

    std::vector<TriMesh> left_tri;
    std::vector<TriMesh> right_tri;

    for(int i = 0; i < tri_lsts.size(); i++)
    {
        if(i < tri_lsts.size() / 2.0) left_tri.push_back(tri_lsts[i]);
        else right_tri.push_back(tri_lsts[i]);
    }

    this->left->BuildKdTree(left_tri, deep + 1);
    this->right->BuildKdTree(right_tri, deep + 1);
}

/**
 * @brief 交点的属性附在了射线上 
 * 
 * @param ray 
 */
void KdTree::GetIntersection(Ray& ray)
{
    double t_min;
    bool b_intersection = this->box.JudgeIntersection(ray, t_min);
    if(!b_intersection) return ;
    if(ray.t <= t_min) return ;

    if(this->leaf == true)
    {
        for(int i = 0; i < this->tri_lst.size(); i++)
        {
            double t;
            vec3d b_corrd;
            if(this->tri_lst[i].JudgeIntersection(ray, t, b_corrd))
            {
                if(t < ray.t)
                {
                    ray.t = t;
                    ray.b_corrd = b_corrd;
                    ray.tri = this->tri_lst[i];
                }
            }
        }
        return ;
    }

    this->left->GetIntersection(ray);
    this->right->GetIntersection(ray);
}

