#ifndef _NET_BASIC_CONNECTION_BUILDER_H_
#define _NET_BASIC_CONNECTION_BUILDER_H_
#include "basic_connection.h"
namespace ez
{
    namespace net
    {
        class scope_socket;
        class basic_connection_builder
        {
        public:
            basic_connection_builder()          = default;
            virtual ~basic_connection_builder() = default;

            virtual std::shared_ptr<basic_connection> new_connection(
                int           skt,
                const void*   sockaddr,
                const size_t& sockaddr_size) const = 0;
        };
    } // namespace net
} // namespace ez
#endif // !_NET_BASIC_CONNECTION_BUILDER_H_