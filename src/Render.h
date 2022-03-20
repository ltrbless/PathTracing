#ifndef _RENDER_H_
#define _RENDER_H_

#include "DataManage.h"

class Render{

public:

    DataManage& DM;

    Render(DataManage& DM_) : DM(DM_){};

    void RunRender(int ssp);

}


#endif // _RENDER_H_