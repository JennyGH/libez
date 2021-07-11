#ifndef _NET_TCP_CONNECTION_H_
#define _NET_TCP_CONNECTION_H_
#include "basic_connection.h"
namespace ez
{
    namespace net
    {
        class scope_socket;
        namespace inet
        {
            class connection : public basic_connection
            {
            public:
                connection(int skt, const void* sockaddr, const size_t& sockaddr_size);
                ~connection();
            };
        } // namespace inet

    } // namespace net

} // namespace ez
#endif // !_NET_TCP_CONNECTION_H_