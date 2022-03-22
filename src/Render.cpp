#include "Render.h"
#include "tool.h"
#include "Global.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif 

void Render::RunRender(int ssp, std::string savepath)
{
    DM.SetSavePathByPng(savepath);
    RunRender(ssp);
}

void Render::RunRender(int ssp)
{
    std::cout << "Render ssp is : " << ssp << "\n";
    for(int j = 0; j < DM.camera.height; j++)
    {
        // #pragma omp parallel for schedule(dynamic, 1) // openmp accelerate
        for(int i = 0; i < DM.camera.width; i++)
        {
            for(int k = 0; k < ssp; k++)
            {
                double u = (i + tool::GetUniformRandomDouble(0, 1)) / DM.camera.width;
                double v = (j + tool::GetUniformRandomDouble(0, 1)) / DM.camera.height;
                Ray ray = DM.camera.GetRay(u, v);
                DM.RGB_framebuffer[j][i] += this->PathTracing(ray, 50);
            }
            DM.RGB_framebuffer[j][i] /= double(ssp);
            DM.RGB_framebuffer[j][i] = DM.RGB_framebuffer[j][i].cwiseMax(vec3d(0, 0, 0));
            DM.RGB_framebuffer[j][i] = DM.RGB_framebuffer[j][i].cwiseMin(vec3d(1, 1, 1));
            // DM.RGB_framebuffer[j][i] = DM.RGB_framebuffer[j][i].sqrt(); // gamma fixed
        }
        // view process 
        tool::AddProcessBar(j, DM.camera.height); 
    }
    tool::AddProcessBar(DM.camera.height, DM.camera.height); 
    
    unsigned int* pixels = new unsigned int[DM.camera.width * DM.camera.height * 3];
    int base = 0;
    for(int j = DM.camera.height - 1; j >= 0; j--)
    {
        for(int i = 0; i < DM.camera.width; i++)
        {
            pixels[base + 0] = DM.RGB_framebuffer[j][i].x() * 255;
            pixels[base + 1] = DM.RGB_framebuffer[j][i].y() * 255;
            pixels[base + 2] = DM.RGB_framebuffer[j][i].z() * 255;
            base += 3;
        }
    }

    stbi_write_png(this->DM.savepngpath.c_str(), DM.camera.width, DM.camera.height, 3, pixels, 0);

    delete pixels;
}

vec3d Render::ImportanceSampling(Render::SampleType stype, vec3d direction, double Ns)
{
    if(stype == Render::SampleType::UNIFORM)
    {
        double theta = PI / 2 * tool::GetUniformRandomDouble(0, 1);
        double phi = 2 * PI * tool::GetUniformRandomDouble(0, 1);
        return vec3d( sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)).normalized();
    }
}

Ray Render::SampleRay(Ray& ray)
{
    // Refraction ----------------------------------------------------------------

    if(ray.tri.material->Ni > 1.0) // happen refraction
    {
        Ray refractionRay;
        double in_or_out = ray.direction.dot(ray.tri.GetNormal());
        double theta_i, theta_t;
        double ni, nt;
        vec3d normal;
        if(in_or_out < 0.0) // in
        {
            ni = 1.0;  // in air
            nt = ray.tri.material->Ni;
            normal = ray.tri.GetNormal();
        }
        else // go out
        {
            ni = ray.tri.material->Ni;
            nt = 1.0;
            normal = -ray.tri.GetNormal();
        }

        double R0 = (ni - nt) / (ni + nt);
        R0 = R0 * R0;
        double cos_5 = (1 + ray.direction.dot(normal) / (ray.direction.norm() * normal.norm()) );
        cos_5 = cos_5 * cos_5 * cos_5 * cos_5 * cos_5;

        double fresnel_term = R0 + (1 - R0) * cos_5; // Can view it as the probability of reflection

        if(tool::GetUniformRandomDouble(0, 1) > fresnel_term)// refraction
        {
            double cos_theta_2 = ray.direction.dot(normal);
            cos_theta_2 = cos_theta_2 * cos_theta_2;
            double flag = (1 - (ni / nt) * (ni / nt)) * (1 - cos_theta_2);
            if(flag >= 0.0) // if flag < 0.0 , cannot refraction
            {
                // sin1 / sin2 = n1 / n2 = ni / nt = eta;
                double eta = ni / nt;
                double cos1 = -ray.direction.dot(normal);
                double cos2 = std::sqrt( fabs( 1.0 - (1 - cos1 * cos1) / (eta * eta) ) );
                vec3d new_direction = ray.direction / eta + normal * ( cos1 / eta - cos2 );
                new_direction.normalize();
                return Ray(ray.inter_point, new_direction, Ray::RayType::SPECULAR);
            }
            else  // reflection
            {
                // R = I + 2 * N * (-N · I);
                vec3d new_direction = ray.direction + 2 * normal * (-normal.dot(ray.direction));
                new_direction.normalize();
                return Ray(ray.inter_point, new_direction, Ray::RayType::SPECULAR);
            }
        }
    }


    // Reflection --------------------------------------------------------------

    double kd = ray.tri.material->Kd.norm();
    double ks = ray.tri.material->Ks.norm();

    if(ks == 0 || (kd / (kd + ks)) >= tool::GetUniformRandomDouble(0, 1) ) // diffuse light
    {
        vec3d new_direction = this->ImportanceSampling(Render::SampleType::UNIFORM, ray.direction, ray.tri.material->Ns);
        return Ray(ray.inter_point, new_direction, Ray::RayType::DIFFUSE);
    }
    else
    {
        vec3d new_direction = this->ImportanceSampling(Render::SampleType::UNIFORM, ray.direction, ray.tri.material->Ns);
        return Ray(ray.inter_point, new_direction, Ray::RayType::SPECULAR);
    }
}

vec3d Render::PathTracing(Ray& ray, int deep)
{
    this->DM.kdtree.GetIntersection(ray);
    if(ray.bool_intersection == false) return vec3d(0, 0, 0); // black background color

    if(ray.tri.material->Ke != vec3d(0, 0, 0)) return ray.tri.material->Ke; // ray lookat on light

    // all light = direct light + indirect light.
    vec3d directlight;
    vec3d indirectlight;

    // direct light

    for(int i = 0; i < DM.tri_light_lst.size(); i++)
    {
        vec3d light_tri_center = DM.tri_light_lst[i].GetCenter();
        vec3d current_intersection = ray.inter_point;
        vec3d p_to_light_vec = (light_tri_center - current_intersection).normalized();
        Ray tmpray(current_intersection, p_to_light_vec);
        DM.kdtree.GetIntersection(tmpray);
        



    }



    // indirect light


    // intersection point
    // vec3d iter_point = ray.b_corrd.x() * ray.tri.v1 + ray.b_corrd.y() * ray.tri.v2 + ray.b_corrd.z() * ray.tri.v3;

    // Calculate indirect light.
    // Refraction --------------------------------



    // Reflection --------------------------------







}