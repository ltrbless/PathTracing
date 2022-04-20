#include "KdTree.h"

int KdTree::all_node = 0;

void KdTree::BuildKdTree(std::vector<TriMesh>& tri_lsts, int deep)
{
    KdTree::all_node++;
    if(tri_lsts.size() < 5 || deep == 10)
    {
        for(int i = 0; i < tri_lsts.size(); i++) 
        {
            this->box.AddBox(tri_lsts[i].GetBox());
        }

        this->leaf = true;
        this->tri_lst = tri_lsts;
        this->left = this->right = nullptr;
        return ;
    }

    vec3d mid(0,0,0);
    double bfb = 1.0 / tri_lsts.size();
    for(int i = 0; i < tri_lsts.size(); i++) 
    {
        this->box.AddBox(tri_lsts[i].GetBox());
        mid += tri_lsts[i].GetCenter() * bfb;
    }
    int axis = this->box.GetLongestAxis();

    this->leaf = false;
    this->tri_lst = std::vector<TriMesh>();
    this->left = new KdTree();
    this->right = new KdTree();


    std::vector<TriMesh> left_tri;
    std::vector<TriMesh> right_tri;

    for(int i = 0; i < tri_lsts.size(); i++)
    {
        if(tri_lsts[i].GetCenter()(axis) < mid(axis)) left_tri.push_back(tri_lsts[i]);
        else right_tri.push_back(tri_lsts[i]);
    }

    // if(left_tri.size() == 0 || right_tri.size() == 0)
    // {
    //     this->leaf = true;
    //     delete this->left; delete this->right;
    //     this->left = nullptr; this->right = nullptr;
    //     this->tri_lst = tri_lsts;
    //     for(int i = 0; i < tri_lsts.size(); i++) 
    //     {
    //         this->box.AddBox(tri_lsts[i].GetBox());
    //     }
    //     return ;
    // }

    if(left_tri.size() == 0) this->left = nullptr;
    else this->left->BuildKdTree(left_tri, deep + 1);
    if(right_tri.size() == 0) this->right = nullptr;
    else this->right->BuildKdTree(right_tri, deep + 1);
}


void KdTree::BuildKdTree2(std::vector<TriMesh>& tri_lsts, int deep)
{
    KdTree::all_node++;
    if(tri_lsts.size() < 5 || deep > 20)
    {
        if(tri_lsts.size() == 0)
        {
            this->box.minn = vec3d(0, 0, 0);
            this->box.maxx = vec3d(0, 0, 0);
        }
        else{
            for(int i = 0; i < tri_lsts.size(); i++) 
            {
                this->box.AddBox(tri_lsts[i].GetBox());
            }
        }

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
    
    if(left_tri.size() == 0) this->left = nullptr;
    else this->left->BuildKdTree2(left_tri, deep + 1);

    if(right_tri.size() == 0) this->right = nullptr;
    else this->right->BuildKdTree2(right_tri, deep + 1);
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

    if(this->leaf == true)
    {
        // std::cout << this->tri_lst.size() << "\n";
        for(int i = 0; i < this->tri_lst.size(); i++)
        {
            double t_dis;
            vec3d b_corrd;
            vec3d inter_p;
            if(this->tri_lst[i].JudgeIntersection(ray, t_dis, b_corrd, inter_p))
            {
                if(t_dis < ray.t && t_dis > 1e-7)
                {
                    ray.t = t_dis;
                    ray.b_corrd = b_corrd;
                    ray.inter_point = inter_p;
                    ray.tri = this->tri_lst[i];
                    ray.bool_intersection = true;
                }
            }
        }
        return ;
    }

    if(this->left != nullptr) this->left->GetIntersection(ray);
    if(this->right != nullptr) this->right->GetIntersection(ray);
}

void KdTree::ViewKdTree(std::string path)
{
    std::cout << this->box.minn.x() << " " << this->box.minn.y() << " " << this->box.minn.z() << std::endl;
    std::cout << this->box.maxx.x() << " " << this->box.maxx.z() << " " << this->box.maxx.y() << std::endl;

    std::string lpath = path + "L";
    std::string rpath = path + "R";

    if(this->left != nullptr) this->ViewKdTree(lpath);
    if(this->right != nullptr) this->ViewKdTree(rpath);
}

