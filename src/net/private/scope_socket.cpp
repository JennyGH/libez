#include "pch.h"
#include "scope_socket.h"

ez::net::scope_socket::scope_socket(const uint32_t& skt)
    : _socket(skt)
{
}

ez::net::scope_socket::~scope_socket()
{
    ::closesocket(_socket);
}

ez::net::scope_socket::operator uint32_t() const
{
    return _socket;
}
