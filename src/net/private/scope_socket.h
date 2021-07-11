#ifndef _NET_SCOPE_SOCKET_H_
#define _NET_SCOPE_SOCKET_H_
#include <stdint.h>
namespace ez
{
    namespace net
    {
        class scope_socket
        {
        public:
            scope_socket(const uint32_t& skt);
            ~scope_socket();

            operator uint32_t() const;

        private:
            uint32_t _socket;
        };
    } // namespace net
} // namespace ez
#endif // !_NET_SCOPE_SOCKET_H_