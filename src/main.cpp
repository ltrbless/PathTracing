#include <iostream>

#include "DataManage.h"
#include "Render.h"

#include <chrono>

int main(int argc, char **argv)
{
    DataManage DM;


    auto start = std::chrono::system_clock::now();
    // DM.LoadDate("../file/bedroom/bedroom.obj");
    DM.LoadDate("../file/veach-mis/veach-mis.obj");
    // DM.LoadDate("../file/veach-mis/veach-mis.obj");
    auto stop = std::chrono::system_clock::now();
    std::cout << "Load Data Time : " << 
    std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " h " <<
    std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " m " << 
    std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " s\n";

    std::cout << "\nCamera base attributes : " << "\n";
    std::cout << DM.camera << std::endl;


    Render render(DM);
    double ssp = 30;
    start = std::chrono::system_clock::now();
    render.RunRender(ssp);
    stop = std::chrono::system_clock::now();
    std::cout << "\nRender Scene Time : " << 
    std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " h " <<
    std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " m " << 
    std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " s\n";

    std::cout << "Finish rendering\n";

}