#include "pch.h"
#include "connection.h"
#include "inet/endpoint.h"
#include "private/scope_socket.h"

ez::net::inet::connection::connection(int skt, const void* sockaddr, const size_t& sockaddr_size)
    : basic_connection(std::make_shared<scope_socket>(skt), std::make_shared<inet::endpoint>(sockaddr))
{
}

ez::net::inet::connection::~connection() {}
