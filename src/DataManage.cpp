#include "DataManage.h"

bool DataManage::LoadDate(const std::string filename)
{
    // Load the date from xml
    //----------------------------------------------------------------
    std::string xmlfile, materialfile;
    size_t dotpos = filename.find_last_of('.');
    xmlfile = filename.substr(0, dotpos) + ".xml";
    materialfile = filename.substr(0, dotpos) + ".mtl";

    std::cout << "xmlfile path is : " << xmlfile << '\n';
    std::cout << "materialfile path is : " << materialfile << '\n';
    std::cout << "----------------------------------------------------------------\n";

    std::cout << "Parse " << xmlfile << " ...\n";

    pugi::xml_document doc;
    if(doc.load_file(xmlfile.c_str(), pugi::parse_default, pugi::encoding_utf8))
    {
        pugi::xml_node camera_node = doc.child("camera");
        pugi::xml_node light_node = doc.child("light");

        // camera_node ------------------------------
        std::string tmpstr;
        std::string tmpval;
        std::stringstream ss;
        char* token;
        const char* split = ",";
        int idx = 0;

        // position --------------------------------
        idx = 0;
        tmpstr.clear();
        tmpstr = camera_node.child("eye").attribute("value").value();
        tmpstr.erase(std::remove_if(tmpstr.begin(), tmpstr.end(),::isspace), tmpstr.end());
        token = strtok( const_cast<char*>(tmpstr.c_str()), split);
        while(token != nullptr)
        {
            tmpval = token;
            ss.clear();
            ss.str(tmpval);
            ss >> camera.position(idx);
            token = strtok(nullptr, split);
            idx++;
        }

        // lookat ------------------------------------------
        idx = 0;
        tmpstr = camera_node.child("lookat").attribute("value").value();
        tmpstr.erase(std::remove(tmpstr.begin(), tmpstr.end(),' '), tmpstr.end());
        token = strtok( const_cast<char*>(tmpstr.c_str()), split);
        while(token != nullptr)
        {
            tmpval = token;
            ss.clear();
            ss.str(tmpval);
            ss >> camera.lookAt(idx);
            token = strtok(nullptr, split);
            idx++;
        }

        // up ------------------------------
        idx = 0;
        tmpstr = camera_node.child("up").attribute("value").value();
        tmpstr.erase(std::remove(tmpstr.begin(), tmpstr.end(),' '), tmpstr.end());
        token = strtok( const_cast<char*>(tmpstr.c_str()), split);
        while(token != nullptr)
        {
            tmpval = token;
            ss.clear();
            ss.str(tmpval);
            ss >> camera.up(idx);
            token = strtok(nullptr, split);
            idx++;
        }

        // fovy ------------------------------
        tmpstr = camera_node.child("fovy").attribute("value").value();
        tmpstr.erase(std::remove(tmpstr.begin(), tmpstr.end(),' '), tmpstr.end());
        ss.clear();
        ss.str(tmpstr);
        ss >> camera.fovy;

        // width ------------------------------
        tmpstr = camera_node.child("width").attribute("value").value();
        tmpstr.erase(std::remove(tmpstr.begin(), tmpstr.end(),' '), tmpstr.end());
        ss.clear();
        ss.str(tmpstr);
        ss >> camera.width;

        // height ------------------------------
        tmpstr = camera_node.child("height").attribute("value").value();
        tmpstr.erase(std::remove(tmpstr.begin(), tmpstr.end(),' '), tmpstr.end());
        ss.clear();
        ss.str(tmpstr);
        ss >> camera.height;


        // light_node ------------------------------
        for(pugi::xml_node_iterator iter = light_node.begin(); iter != light_node.end();++iter)
        {
            std::string name = iter->attribute("mtlname").value();
            tmpstr = iter->attribute("randiance").value();
            vec3d light_;

            idx = 0;
            tmpstr.erase(std::remove(tmpstr.begin(), tmpstr.end(),' '), tmpstr.end());
            token = strtok( const_cast<char*>(tmpstr.c_str()), split);
            while(token != nullptr)
            {
                tmpval = token;
                ss.clear();
                ss.str(tmpval);
                ss >> light_(idx);
                token = strtok(nullptr, split);
                idx++;
            }

            this->mp_lights[name] = light_;
        }

    }
    std::cout << "Parse " << xmlfile << " Finish.\n";

    std::string err = tinyobj::LoadObj(this->tiny_shapes, this->tiny_materials, filename.c_str(), materialfile.c_str());

    if(!err.empty())
    {
        std::cerr << "Error: " << err << std::endl;
        exit(0);
    }
    
    // Parse materialfile ----------------------------------
    std::cout << "Parse " << materialfile << " ...\n";
    for(int i = 0; i < this->tiny_materials.size(); i++)
    {
        Material* tmp_m = new Material();
        tmp_m->name = tiny_materials[i].name;
        if(!tiny_materials[i].diffuse_texname.empty()){
            tmp_m->mp_kd.LoadTexture(tiny_materials[i].diffuse_texname);
        }
        tmp_m->Kd = vec3d(tiny_materials[i].diffuse[0], tiny_materials[i].diffuse[1], tiny_materials[i].diffuse[2]);
        tmp_m->Ks = vec3d(tiny_materials[i].specular[0], tiny_materials[i].specular[1], tiny_materials[i].specular[2]);
        tmp_m->Ni = tiny_materials[i].ior;
        tmp_m->Ns = tiny_materials[i].shininess;

        if(this->mp_lights.find(tmp_m->name) != this->mp_lights.end())
            tmp_m->Ke = this->mp_lights[tmp_m->name];
        else tmp_m->Ke = vec3d(0.0, 0.0, 0.0);

        this->mp_materials[tmp_m->name] = tmp_m;
        this->material_lst.push_back(tmp_m);
    }
    std::cout << "Parse " << materialfile << " Finish.\n";

    // Parse object ---------------------------------------

    std::cout << "Parse " << filename << " ...\n";
    for(size_t i = 0; i < tiny_shapes.size(); i++)
    {
        for(size_t j = 0; j < tiny_shapes[i].mesh.indices.size() / 3; j++)
        {
            TriMesh tri;
            tri.v1 = vec3d( tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j] * 3 ],
                            tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j] * 3 + 1],
                            tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j] * 3 + 2]);
            

            tri.v2 = vec3d( tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j + 1] * 3 ],
                            tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j + 1] * 3 + 1],
                            tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j + 1] * 3 + 2]);
            
            tri.v3 = vec3d( tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j + 2] * 3 ],
                            tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j + 2] * 3 + 1],
                            tiny_shapes[i].mesh.positions[ tiny_shapes[i].mesh.indices[3 * j + 2] * 3 + 2]);
            
            if(tiny_shapes[i].mesh.indices[3 * j + 2] * 2 + 1 < tiny_shapes[i].mesh.texcoords.size())
            {
                tri.t1 = vec2d( tiny_shapes[i].mesh.texcoords[ tiny_shapes[i].mesh.indices[3 * j] * 2],
                                tiny_shapes[i].mesh.texcoords[ tiny_shapes[i].mesh.indices[3 * j] * 2 + 1]);

                tri.t2 = vec2d( tiny_shapes[i].mesh.texcoords[ tiny_shapes[i].mesh.indices[3 * j + 1] * 2],
                                tiny_shapes[i].mesh.texcoords[ tiny_shapes[i].mesh.indices[3 * j + 1] * 2 + 1]);

                tri.t3 = vec2d( tiny_shapes[i].mesh.texcoords[ tiny_shapes[i].mesh.indices[3 * j + 2] * 2],
                                tiny_shapes[i].mesh.texcoords[ tiny_shapes[i].mesh.indices[3 * j + 2] * 2 + 1]);
            }
            else
            {
                tri.t1 = tri.t2 = tri.t3 = vec2d(0, 0);
            }

            if(tiny_shapes[i].mesh.material_ids[j] < material_lst.size())
            {
                tri.material = this->material_lst[ tiny_shapes[i].mesh.material_ids[j] ];
            }
            else
            {
                tri.material = nullptr;
            }

            tri.InitBox();
            this->tri_lst.push_back(tri);
        }
    }
    std::cout << "Parse " << filename << " Finish.\n";

    std::cout << "Build KdTree ing ...\n";
    this->kdtree.BuildKdTree(this->tri_lst, 0);
    std::cout << "Build KdTree Finish.\n";
    return 1;
}