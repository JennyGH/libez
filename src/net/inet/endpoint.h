#ifndef _NET_TCP_ENDPOINT_H_
#define _NET_TCP_ENDPOINT_H_
#include <cstdint>
#include <memory>
#include <string>
#include <net/basic_endpoint.h>
namespace ez
{
    namespace net
    {
        namespace inet
        {
            class endpoint : public basic_endpoint
            {
            public:
                endpoint();
                endpoint(const void* sockaddr);
                endpoint(const std::string& ipv4, uint16_t port = 0);
                endpoint(endpoint&& that) noexcept;
                endpoint(const endpoint& that);
                ~endpoint();
            };
        } // namespace inet

    } // namespace net

} // namespace ez
#endif // !_NET_TCP_ENDPOINT_H_