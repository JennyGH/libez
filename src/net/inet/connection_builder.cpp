#include "pch.h"
#include "connection_builder.h"
#include "inet/connection.h"

std::shared_ptr<ez::net::basic_connection> ez::net::inet::connection_builder::new_connection(
    int           skt,
    const void*   sockaddr,
    const size_t& sockaddr_size) const
{
    return std::make_shared<inet::connection>(skt, sockaddr, sockaddr_size);
}
