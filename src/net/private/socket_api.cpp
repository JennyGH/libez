#include "pch.h"
#include "socket_api.h"
#include "basic_connection.h"
#include <base/os.h>

int ez::net::recv(const basic_connection& connection, std::string& output, int flag)
{
    int rv = 0;
    output.clear();
    while (true)
    {
        unsigned char buffer[256] = {0};

        rv = ::recv(connection.get_socket(), buffer, sizeof(buffer), flag);
        if (rv < 0)
        {
            rv = ez::base::os::get_last_error();
            if (rv != EAGAIN && rv != ECONNABORTED && rv != EPROTO && rv != EINTR)
            {
                return rv;
            }
            break;
        }

        if (rv == 0)
        {
            return 0;
        }

        if (rv > 0)
        {
            output.append(buffer, buffer + rv);
        }
    }
    return output.length();
}

int ez::net::send(const basic_connection& connection, const void* data, const size_t& length, int flag)
{
    return ::send(connection.get_socket(), data, length, flag);
}