#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <base/file_system.h>

#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

int main(int argc, char** argv)
{
    auto walked = ez::base::file_system::walk("/usr/local", 3);

    for (const auto& path : walked)
    {
        CONSOLE("%s", path.c_str());
    }

    return 0;
}