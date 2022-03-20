#include "Render.h"

void Render::RunRender(int ssp)
{
    for(int j = 0; j < DM.camera.height; j++)
    {
        #pragma omp parallel for schedule(dynamic, 1) // openmp accelerate
        for(int i = 0; i < DM.camera.width; i++)
        {
            
        }
    }
}