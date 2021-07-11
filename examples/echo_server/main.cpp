#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <net/inet/server.h>
#include <net/inet/connection_builder.h>
#include <base/argument_parser.h>

#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

int main(int argc, char** argv)
{
    ez::base::argument_parser args(argc, argv);

    std::string ip   = args.get<std::string>("ip", "127.0.0.1");
    uint16_t    port = args.get<uint16_t>("port", 8080);
    CONSOLE("Echo server will start up on `%s:%u`", ip.c_str(), port);

    try
    {
        ez::net::inet::endpoint address(ip.c_str(), port);
        ez::net::inet::server   server(address);
        server.set_connection_builder(std::make_shared<ez::net::inet::connection_builder>());
        server.start_up();
    }
    catch (const std::exception& e)
    {
        CONSOLE("Oops! %s", e.what());
        return -1;
    }

    return 0;
}