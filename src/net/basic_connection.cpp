#include "pch.h"
#include "basic_connection.h"
#include "private/scope_socket.h"

ez::net::basic_connection::basic_connection(std::shared_ptr<scope_socket> skt, std::shared_ptr<basic_endpoint> endpoint)
    : _connect_time(nullptr == skt ? 0 : ::time(nullptr))
    , _socket(skt)
    , _endpoint(endpoint)
{
}

ez::net::basic_connection::basic_connection(const basic_connection& that)
    : _connect_time(that._connect_time)
    , _socket(that._socket)
    , _endpoint(that._endpoint)
{
}

ez::net::basic_connection::~basic_connection() {}

std::shared_ptr<ez::net::basic_endpoint> ez::net::basic_connection::get_endpoint() const
{
    return _endpoint;
}

const time_t& ez::net::basic_connection::get_connect_time() const
{
    return this->_connect_time;
}

int ez::net::basic_connection::get_socket() const
{
    return *this->_socket;
}
