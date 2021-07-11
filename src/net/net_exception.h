#ifndef _NET_NET_EXCEPTION_H_
#define _NET_NET_EXCEPTION_H_
#include <stdexcept>
namespace ez
{
    namespace net
    {
        using net_exception = std::runtime_error;
    } // namespace net
} // namespace ez
#endif // !_NET_NET_EXCEPTION_H_