#include "Render.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif 

#include <algorithm>

void Render::RunRender(int ssp, std::string savepath)
{
    DM.SetSavePathByPng(savepath);
    RunRender(ssp);
}

void Render::RunRender(int ssp)
{
    std::cout << "Render ssp is : " << ssp << "\n";
    std::cout << "light facet number is : " << DM.tri_light_lst.size() << "\n";
    // for(int j = 1, k = 1; k ; k = 0)
    for(int j = 1; j < DM.camera.height; j++)
    {
        // #pragma omp parallel for schedule(dynamic, 1) // openmp accelerate

        // #pragma omp parallel
        // for(int i = 200, k2 = 1; k2; k2 = 0)
        #pragma omp parallel for
        for(int i = 0; i < DM.camera.width; i++)
        {
            for(int k = 0; k < ssp; k++)
            {
                double u = (i + tool::GetUniformRandomDouble(0, 1)) / DM.camera.width;
                double v = (j + tool::GetUniformRandomDouble(0, 1)) / DM.camera.height;
                Ray ray = DM.camera.GetRay(u, v);
                // std::cout << u << " " << v << " " << "\n";
                // std::cout << "ray direction : " <<  ray.direction.x() << " " << ray.direction.y() << " " << ray.direction.z() << "\n";
                // std::cout << "ray origin : " << ray.origin.x() << " " << ray.origin.y() << " " << ray.origin.z() << "\n";
                DM.RGB_framebuffer[j][i] += this->PathTracing(ray, 0);
                // std::cout << DM.RGB_framebuffer[j][i].x() << " " << DM.RGB_framebuffer[j][i].y() << " " << DM.RGB_framebuffer[j][i].z() << "\n";
            }
            DM.RGB_framebuffer[j][i] /= double(ssp);
            double maxColor = std::max(DM.RGB_framebuffer[j][i].x(), std::max(DM.RGB_framebuffer[j][i].y(), DM.RGB_framebuffer[j][i].z()));
            if(maxColor > 1) DM.RGB_framebuffer[j][i] /= maxColor;
            DM.RGB_framebuffer[j][i] = DM.RGB_framebuffer[j][i].cwiseMax(vec3d(0, 0, 0));
            DM.RGB_framebuffer[j][i] = DM.RGB_framebuffer[j][i].cwiseMin(vec3d(1, 1, 1));
            // DM.RGB_framebuffer[j][i] = vec3d(sqrt(DM.RGB_framebuffer[j][i].x()), sqrt(DM.RGB_framebuffer[j][i].y()), sqrt(DM.RGB_framebuffer[j][i].z()));
            // std::cout << "j = " << j << " i = " << i << "  " << DM.RGB_framebuffer[j][i].x() << " " << DM.RGB_framebuffer[j][i].y() << " " << DM.RGB_framebuffer[j][i].z() << "\n";
            // DM.RGB_framebuffer[j][i] = DM.RGB_framebuffer[j][i].sqrt(); // gamma fixed
            // std::cout << "here\n";
        }
        // view process 
        tool::AddProcessBar(j, DM.camera.height); 
    }
    tool::AddProcessBar(DM.camera.height, DM.camera.height); 

    unsigned char* pixels = new unsigned char[DM.camera.width * DM.camera.height * 3];
    int base = 0;
    for(int j = DM.camera.height - 1; j >= 0; j--)
    {
        for(int i = 0; i < DM.camera.width; i++)
        {
            pixels[base + 0] = (unsigned char)(255 * DM.RGB_framebuffer[j][i].x());
            pixels[base + 1] = (unsigned char)(255 * DM.RGB_framebuffer[j][i].y());
            pixels[base + 2] = (unsigned char)(255 * DM.RGB_framebuffer[j][i].z());
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

#include <chrono>
vec3d Render::PathTracing(Ray& ray, int deep)
{
    this->DM.kdtree.GetIntersection(ray);
    // std::cout << "Material : " << ray.tri.material->name << "  Kd : " << ray.tri.material->Kd.x() << " " << ray.tri.material->Kd.y() << " " << ray.tri.material->Kd.z() << "\n";
    
    if(ray.bool_intersection == false || deep == 5) return vec3d(0, 0, 0); // black background color
    if(ray.tri.material->Ke.norm() > 0) return ray.tri.material->Ke; // ray lookat on light

    // std::cout << "intersection point is : " << ray.inter_point.x() << " " << ray.inter_point.y() << " " << ray.inter_point.z() << "\n";

    // all light = direct light + indirect light.
    vec3d directlight = vec3d(0, 0, 0);
    vec3d indirectlight = vec3d(0, 0, 0);

    // direct light
    //=====================================================
    vec3d current_intersection = ray.inter_point;
    for(int i = 0; i < DM.light_group_lst.size(); i++)
    {
        int sample_num = 30;
        double sample_area = 0;
        vec3d tmp_directlight = vec3d(0,0,0);

        for(int j = 0; j < sample_num; j++)
        {
            int random_ind = tool::GetUniformRandomDouble(0, 1) * DM.light_group_lst[i].size();
            sample_area += DM.light_group_lst[i][random_ind].GetArea();

            vec3d light_tri_center = DM.light_group_lst[i][random_ind].GetRandomPoint();
            vec3d p_to_light_vec = (light_tri_center - current_intersection).normalized();

            if(DM.light_group_lst[i][random_ind].GetNormal().dot(p_to_light_vec) >= 0) continue;

            double len_p_light = (light_tri_center - current_intersection).norm();
            vec3d half_vec = (p_to_light_vec - ray.direction).normalized();
            Ray tmpray(current_intersection, p_to_light_vec);
            DM.kdtree.GetIntersection(tmpray);
            if(tmpray.bool_intersection == 0) continue;
            
            if( fabs(len_p_light - tmpray.t) < 1e-5)
            {
                double cos_theta = ray.tri.GetNormal().dot(p_to_light_vec);
                double cos_theta_ = DM.light_group_lst[i][random_ind].GetNormal().dot( -p_to_light_vec );
                double f = std::max(0.0, half_vec.dot(ray.tri.GetNormal()));
                f = power(f, ray.tri.material->Ns);
                tmp_directlight += DM.light_group_lst[i][random_ind].material->Ke.cwiseProduct(ray.tri.material->Kd) * cos_theta * cos_theta_ * DM.light_group_lst[i][random_ind].GetArea() / (len_p_light * len_p_light);
                tmp_directlight += DM.light_group_lst[i][random_ind].material->Ke.cwiseProduct(ray.tri.material->Ks) * f * cos_theta * cos_theta_ * DM.light_group_lst[i][random_ind].GetArea() * 30 / (len_p_light * len_p_light);
            }
        }
        tmp_directlight /= sample_area;
        tmp_directlight *= DM.light_group_area_lst[i];
        directlight += tmp_directlight;
    }

    //========================================================================================
    // for(int i = 0; i < DM.tri_light_lst.size(); i++)
    // {
    //     vec3d light_tri_center = DM.tri_light_lst[i].GetCenter();
    //     vec3d current_intersection = ray.inter_point;
    //     vec3d p_to_light_vec = (light_tri_center - current_intersection).normalized();

    //     if(DM.tri_light_lst[i].GetNormal().dot(p_to_light_vec) >= 0) continue;

    //     double len_p_light = (light_tri_center - current_intersection).norm();
    //     vec3d half_vec = (p_to_light_vec - ray.direction).normalized();
    //     Ray tmpray(current_intersection, p_to_light_vec);
    //     DM.kdtree.GetIntersection(tmpray);
    //     if(tmpray.bool_intersection == 0) continue;
        
    //     if( fabs(len_p_light - tmpray.t) < 1e-5)
    //     {
    //         double cos_theta = ray.tri.GetNormal().dot(p_to_light_vec);
    //         double cos_theta_ = DM.tri_light_lst[i].GetNormal().dot( -p_to_light_vec );
    //         double f = max(0, half_vec.dot(ray.tri.GetNormal()));
    //         f = this->power(f, ray.tri.material->Ns);
    //         directlight += DM.tri_light_lst[i].material->Ke.cwiseProduct(ray.tri.material->Kd) * cos_theta * cos_theta_ * DM.tri_light_lst[i].GetArea() / (len_p_light * len_p_light);
    //         // std::cout << directlight << '\n';
    //         // directlight += DM.tri_light_lst[i].material->Ke.cwiseProduct(ray.tri.material->Ks) * f * cos_theta * cos_theta_ * DM.tri_light_lst[i].GetArea() / (len_p_light * len_p_light);
    //     }
    // }

    //======================================================================
    // double RR = tool::GetUniformRandomDouble(0, 1);
    // if(RR < 0.8)
    // {
    //     Ray newray = SampleRay(ray);
    //     vec3d light_power = PathTracing(newray, deep + 1);
    //     double cos_theta = ray.tri.GetNormal().dot(newray.direction);
    //     if(newray.raytype == Ray::RayType::DIFFUSE)
    //     {
    //         indirectlight += light_power.cwiseProduct(ray.tri.material->Kd) * cos_theta * 2 * PI / 0.8;
    //     }
    //     else
    //     {
    //         vec3d half_vec = newray.direction - ray.direction;
    //         double f = max(0, half_vec.dot(ray.tri.GetNormal()));
    //         f = this->power(f, ray.tri.material->Ns);
    //         indirectlight += light_power.cwiseProduct(ray.tri.material->Ks) * f * cos_theta * 2 * PI / 0.8;
    //     }
    // }

    return directlight + indirectlight;
}

double Render::power(double a, int b)
{
    double ans = 1, base = a;
    while(b != 0){
        if(b & 1 != 0){   //判断什么时候该乘什么时候不该乘
            ans *= base;
        }
        base *= base;  //位数加倍
        b >>= 1;
    }
    return ans;
}

    // save framebuffer to file
    // FILE* fp = fopen("binary.ppm", "wb");
    // std::cout << DM.camera.width << " " << DM.camera.height << " ==\n";
    // (void)fprintf(fp, "P6\n%d %d\n255\n", DM.camera.width, DM.camera.height);
    // for(int i = 0; i < DM.camera.height; i++)
    // {
    //     for(int j = 0; j < DM.camera.width; j++)
    //     {
    //         static unsigned char color[3];
    //         color[0] = (unsigned char)(255 * std::pow(tool::clamp(0, 1, DM.RGB_framebuffer[i][j].x()), 0.6f));
    //         color[1] = (unsigned char)(255 * std::pow(tool::clamp(0, 1, DM.RGB_framebuffer[i][j].y()), 0.6f));
    //         color[2] = (unsigned char)(255 * std::pow(tool::clamp(0, 1, DM.RGB_framebuffer[i][j].z()), 0.6f));
    //         fwrite(color, 1, 3, fp);
    //     }
    // }
    // fclose(fp);  