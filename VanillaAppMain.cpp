// ===============================================================================
// Copyright Deere & Company. For more information,
// please see COPYRIGHT file in root of source repository.
// ===============================================================================

#include "VanillaApp.h"
#include "ParameterIO/AppArgsChecker.h"

// -------------------------------------------------------------------------------
int main(int argc,
         char* argv[])
{
    if (!ParameterIO::CheckArgsAndInitG3Logger(argc, argv))
    {
        return -1;
    }
    ;
    std::atomic<bool> killed(false);
    VanillaApp service(argv[1], killed);
    return service.Run();
}

// ===============================================================================
