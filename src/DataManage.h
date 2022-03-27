#ifndef _DATA_MANAGE_H_
#define _DATA_MANAGE_H_

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "pugixml.hpp"

#include "Camera.h"
#include "Triangle.h"
#include "KdTree.h"
#include "Material.h"
#include "TriGroup.h"

#include <string>
#include <map>

class DataManage {

public:

    //---------------------------- data ------------------------------------
    Camera camera;
    std::vector<tinyobj::shape_t> tiny_shapes;
    std::vector<tinyobj::material_t> tiny_materials;

    std::vector<TriMesh> tri_lst;
    std::vector<TriMesh> tri_light_lst;

    std::vector<std::vector<TriMesh>> light_group_lst; 
    std::vector<double> light_group_area_lst;

    std::vector<Material*> material_lst;
    std::map<std::string, Material*> mp_materials;
    std::map<std::string, vec3d> mp_lights;

    KdTree kdtree;    

    std::vector<std::vector<vec3d>> RGB_framebuffer;

    std::string savepngpath;

    //---------------------------- function --------------------------------
    bool LoadDate(const std::string filename);
    bool SetSavePathByPng(const std::string filename);
    // bool GetIntersection(Ray& ray);

    ~DataManage();


};



#endif 