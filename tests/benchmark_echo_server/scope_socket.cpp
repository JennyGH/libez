#include "pch.h"
#include "scope_socket.h"

scope_socket::scope_socket(int skt)
    : _socket(skt)
{
}

scope_socket::~scope_socket()
{
    if (INVALID_SOCKET != _socket)
    {
        ::closesocket(_socket);
        _socket = INVALID_SOCKET;
    }
}

scope_socket::operator int() const
{
    return _socket;
}
