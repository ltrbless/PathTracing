#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Global.h"
#include <iostream>

class Camera {
public:
    vec3d position;
    vec3d lookAt;
    vec3d up;
    double fovy;
    double width;
    double height;

    // Camera();
    // Camera(vec3d pos, vec3d lookAt, vec3d up);
    friend std::ostream &operator<<( std::ostream &output, const Camera &C )
    { 
        output <<   "eye_position : " << C.position.x() << " " << C.position.y() << " " << C.position.z() << " \n" << \
                    "eye_lookAt   : " << C.lookAt.x() << " " << C.lookAt.y() << " " << C.lookAt.z() << " \n" << \
                    "eye_up       : " << C.up.x() << " " << C.up.y() << " " << C.up.z() << " \n" << \
                    "fovy         : " << C.fovy << " \n" << \
                    "width        : " << C.width << " \n" << \
                    "height       : " << C.height << " \n";
        return output;            
    }


};


#endif //