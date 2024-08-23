#include "core/core.hpp"

using namespace scsr;

int runtime(int argc, char* argv[])
{
    LOG_RT_INFO("Runtime started");

    Window wd;

    bool status = true;
    while (status)
    {
        status = wd.OnUpdate();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }  
    
    return 0;
}
