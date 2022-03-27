#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Global.h"
#include "Texture.h"

class Material{

public:
    
    std::string name;
    vec3d Kd; // diffuse
    vec3d Ks; // specular
    vec3d Ke; // emissive
    double Ns; 
    double Ni;
    Texture mp_kd;

    friend std::ostream &operator<<( std::ostream &output, const Material &M )
    { 
        output << "Name is : " << M.name << "\n" <<
                  "Kd is   : " << M.Kd.x() << " " << M.Kd.y() << " " << M.Kd.z() << "\n" <<
                  "Ks is   : " << M.Ks.x() << " " << M.Ks.y() << " " << M.Ks.z() << "\n" <<
                  "Ke is   : " << M.Ke.x() << " " << M.Ke.y() << " " << M.Ke.z() << "\n" <<
                  "Ns is   : " << M.Ns << "\n" <<
                  "Ni is   : " << M.Ni << "\n" <<
                  "mpkd is : " << M.mp_kd.filename << std::endl;
                  
        return output;            
    }

};

#endif //