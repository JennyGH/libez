#ifndef _NET_TCP_CONNECTION_BUILDER_H_
#define _NET_TCP_CONNECTION_BUILDER_H_
#include <net/basic_connection_builder.h>
namespace ez
{
    namespace net
    {
        namespace inet
        {
            class connection_builder : public basic_connection_builder
            {
            public:
                connection_builder()  = default;
                ~connection_builder() = default;

                virtual std::shared_ptr<basic_connection> new_connection(
                    int           skt,
                    const void*   sockaddr,
                    const size_t& sockaddr_size) const override;
            };

        } // namespace inet

    } // namespace net

} // namespace ez
#endif // !_NET_TCP_CONNECTION_BUILDER_H_