#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <codec.h>

using namespace ez::base::codec;

static const unsigned char bytes[] = {0x00};

#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

int main(int argc, char** argv)
{
    CONSOLE("%s", HEX(bytes));
    auto encoded = ez::base::codec::base64::encode(bytes);
    CONSOLE("base64 encoded: %s", encoded.c_str());
    auto decoded = ez::base::codec::base64::decode(encoded);
    CONSOLE("base64 decoded: %s", decoded.c_str());
    return 0;
}