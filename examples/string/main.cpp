#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <base/string.hpp>

#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

int main(int argc, char** argv)
{
    ez::base::string str("123456");
    auto             equals  = str == "123456";
    auto             removed = str.remove('1');
    auto             found   = str.find('2');
    ez::base::string substr  = str.substr(2);
    substr                   = substr.remove('6');

    str.foreach ([](char& c) {
        c = '0';
    });

    return 0;
}