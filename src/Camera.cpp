#include "Camera.h"

Ray Camera::GetRay(double u, double v) // u aligned to x axis.
{
    vec3d direction = this->leftlowercorner + u * this->x_axis_normal + v * this->y_axis_normal;
    direction.normalize();
    return Ray( this->position, direction );
}