#ifndef _NET_SOCKET_API_H_
#define _NET_SOCKET_API_H_
#include <string>
namespace ez
{
    namespace net
    {
        class basic_connection;
        int recv(const basic_connection& connection, std::string& output, int flag = 0);
        int send(const basic_connection& connection, const void* data, const size_t& length, int flag = 0);
    } // namespace net

} // namespace ez

#endif // !_NET_SOCKET_API_H_