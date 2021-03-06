#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Global.h"
#include "Ray.h"
#include <iostream>

class Camera {
public:
    vec3d position;
    vec3d lookAt;
    vec3d front;
    vec3d up;
    vec3d right;
    double fovy;
    int width;
    int height;

    double x_half_len;
    double y_half_len;
    vec3d leftlowercorner;
    vec3d x_axis_normal;
    vec3d y_axis_normal;

    // Camera();
    // Camera(vec3d pos, vec3d lookAt, vec3d up);
    friend std::ostream &operator<<( std::ostream &output, const Camera &C )
    { 
        output <<   "eye_position : " << C.position.x() << " " << C.position.y() << " " << C.position.z() << " \n" << \
                    "eye_front   : " << C.front.x() << " " << C.front.y() << " " << C.front.z() << " \n" << \
                    "eye_up       : " << C.up.x() << " " << C.up.y() << " " << C.up.z() << " \n" << \
                    "eye_right       : " << C.right.x() << " " << C.right.y() << " " << C.right.z() << " \n" << \
                    "fovy         : " << C.fovy << " \n" << \
                    "width        : " << C.width << " \n" << \
                    "height       : " << C.height << " \n";
        return output;            
    }

    Ray GetRay(double u, double v);


};


#endif //