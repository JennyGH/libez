#include "pch.h"
#include "basic_endpoint.h"

static inline void _free_sockaddr(void* ptr)
{
    if (nullptr != ptr)
    {
        ::free(ptr);
    }
}

static inline void* _dup_sockaddr(const void* sockaddr, const size_t& size)
{
    if (nullptr != sockaddr && size > 0)
    {
        void* ptr = ::malloc(size);
        memcpy_s(ptr, size, sockaddr, size);
        return ptr;
    }
    return nullptr;
}

ez::net::basic_endpoint::basic_endpoint(const void* sockaddr, const size_t& size)
    : _addr(_dup_sockaddr(sockaddr, size), _free_sockaddr)
    , _size(size)
{
}

ez::net::basic_endpoint::basic_endpoint(basic_endpoint&& that) noexcept
    : _addr(std::move(that._addr))
    , _size(std::move(that._size))
{
}

ez::net::basic_endpoint::basic_endpoint(const basic_endpoint& that)
    : _addr(that._addr)
    , _size(that._size)
{
}

ez::net::basic_endpoint::~basic_endpoint() {}

void* ez::net::basic_endpoint::get_addr() const
{
    return _addr.get();
}

const size_t& ez::net::basic_endpoint::get_size() const
{
    return _size;
}
