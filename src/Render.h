#ifndef _RENDER_H_
#define _RENDER_H_

#include "DataManage.h"
#include "Global.h"
#include <windows.h>

class Render{

public:

    enum SampleType{
        UNIFORM,
        CosineWeighted,
        SamplingSpecular
    };

    DataManage& DM;

    Render(DataManage& DM_) : DM(DM_){};

    void RunRender(int ssp);
    void RunRender(int ssp, std::string savepath);
    vec3d PathTracing(Ray& ray, int deep);
    Ray SampleRay(Ray& ray);
    vec3d ImportanceSampling(SampleType stype, vec3d direction, double Ns);

};


#endif // _RENDER_H_