#include <iostream>

#include "DataManage.h"
#include "Render.h"

#include <chrono>


// #define TEST1
#ifdef TEST1

int main(int argc, char **argv)
{
    AABBbox box;
    box.minn = vec3d(0.0, 0.0, 0.0);
    box.maxx = vec3d(1, 1, 1);

    Ray ray;
    ray.origin = vec3d(-0.5, -0.5, 0.5);
    ray.direction = vec3d(1, 1, 0);
    ray.direction.normalize();

    double dis;
    bool b = box.JudgeIntersection(ray, dis);

    std::cout << b << " " << dis << std::endl;
    return 0;
}
#endif

#define MAIN
#ifdef MAIN
int main(int argc, char **argv)
{
    DataManage DM;

    auto start = std::chrono::system_clock::now();
    // DM.LoadDate("../file/bedroom/bedroom.obj");
    DM.LoadDate("../file/veach-mis/veach-mis.obj");
    auto stop = std::chrono::system_clock::now();
    std::cout << "Load Data Time : " << 
    std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " h " <<
    std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " m " << 
    std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " s\n";

    std::cout << "KdTree node number is : " << KdTree::all_node << '\n';

    std::cout << "\nCamera base attributes : " << "\n";
    std::cout << DM.camera << std::endl;

    // std::cout << "\nAll material type : " << "\n";
    // for(int i = 0; i < DM.material_lst.size(); i++)
    // {
    //     std::cout << "==============   material " << i << "   ================\n";
    //     std::cout << *(DM.material_lst[i]) << "\n";
    // }

    Render render(DM);
    double ssp = 1;
    start = std::chrono::system_clock::now();
    render.RunRender(ssp);
    stop = std::chrono::system_clock::now();
    std::cout << "\nRender Scene Time : " << 
    std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " h " <<
    std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " m " << 
    std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " s\n";

    std::cout << "Finish rendering\n";

}

#endif


// #define TEST_cal_intersection_point
#ifdef TEST_cal_intersection_point

int main(int argc, char **argv)
{
    DataManage DM;
    // DM.LoadDate("../file/bedroom/bedroom.obj");
    DM.LoadDate("../file/veach-mis/veach-mis.obj");
    std::cout << "kdtree node number is : " << KdTree::all_node << '\n';
    std::cout << "all tri number is : " << DM.tri_lst.size() << "\n";

    Ray ray;
    ray.origin = vec3d(0.0188056 ,-3.41292, -2);
    ray.direction = vec3d(-0.582621, 0.746695, 0.320936);

//================================================================
    std::cout << "===========================kdtree=====================================\n";
    auto start = std::chrono::system_clock::now();
    DM.kdtree.GetIntersection(ray);
    auto stop = std::chrono::system_clock::now();

    std::cout << "Kdtree algorithm time is : " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n";
    std::cout << "dis is : " << ray.t << " "  << ray.tri.id << "\n";
    std::cout << "barycentric corrdinates is : "; tool::CoutVec3d(ray.b_corrd);
    std::cout << "intersection point corrdinates is : "; tool::CoutVec3d(ray.inter_point);

    std::cout << "===========================Volence=====================================\n";
    double ans_dis = DBL_MAX;
    vec3d ans_b_corrds;
    vec3d ans_inter_p;
    int ans_loc;

    start = std::chrono::system_clock::now();
    for(int i = 0; i < DM.tri_lst.size(); i++)
    {
        double dis;
        vec3d b_corrd;
        vec3d inter_p;
        if(DM.tri_lst[i].JudgeIntersection(ray, dis, b_corrd, inter_p))
        {
            if(dis < ans_dis && fabs(dis) > 1e-5 )
            {
                ans_dis = dis;
                ans_b_corrds = b_corrd;
                ans_inter_p = inter_p;
                ans_loc = i;
            }
        }
    }
    stop = std::chrono::system_clock::now();
    std::cout << "Volence algorithm time is : " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n";
    std::cout << "dis is : " << ans_dis << " "  << DM.tri_lst[ans_loc].id << "\n";
    std::cout << "barycentric corrdinates is : "; tool::CoutVec3d(ans_b_corrds);
    std::cout << "intersection point corrdinates is : "; tool::CoutVec3d(ans_inter_p);
    std::cout << "================================================================\n";
    


}


#endif

// #define TEST3
#ifdef TEST3

int main(int argc, char **argv)
{
    TriMesh tri;
    tri.v1 = vec3d(0, 0, 0);
    tri.v2 = vec3d(1, 0, 0);
    tri.v3 = vec3d(0, 1, 0);

    Ray ray;
    ray.origin = vec3d(0.5, 0.5, 1);
    ray.direction = - vec3d(1, 1, 0) + ray.origin;
    ray.direction.normalize();

    double dis;
    vec3d b_corrd;
    vec3d intersections_p;
    bool b_inter = tri.JudgeIntersection(ray, dis, b_corrd, intersections_p);

    std::cout << "bool intersection : " << b_inter << "\n" << 
                 "dis : " << dis << "\n" << 
                 "intersections_p : " << intersections_p.x() << " " << intersections_p.y() << " " << intersections_p.z() << "\n";

}

#endif // TEST3