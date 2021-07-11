#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <base/os.h>
#include <base/codec.h>

#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

int main(int argc, char** argv)
{
    ez::base::os::endian_t current_endian = ez::base::os::get_current_endian();
    if (ez::base::os::big_endian == current_endian)
    {
        CONSOLE("Current endian is BIG endian.");
    }
    else
    {
        CONSOLE("Current endian is LITTLE endian.");
    }

    unsigned int  val                     = 0x12345678;
    unsigned char serialized[sizeof(val)] = {0};
    size_t        serialized_size         = 0;

    ez::base::os::serialize(val, ez::base::os::big_endian, serialized);
    CONSOLE("0x%08x serialized as BIG endian   : 0x%s", val, HEX(serialized));

    ez::base::os::serialize(val, ez::base::os::little_endian, serialized);
    CONSOLE("0x%08x serialized as LITTLE endian: 0x%s", val, HEX(serialized));

    return 0;
}