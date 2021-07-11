#include "pch.h"
#include "basic_server.h"
#include "private/epoller.h"
#include "private/socket_api.h"
#include "private/scope_socket.h"
#include <base/os.h>

#define ERRCODE           ez::base::os::get_last_error()
#define ERRMSG            ez::base::os::get_last_error_message().c_str()
#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

static void _on_accepted(
    ez::net::basic_server*           context,
    int                              socket,
    std::shared_ptr<struct sockaddr> addr,
    const socklen_t&                 addr_size)
{
    if (nullptr == context)
    {
        CONSOLE("`context` can not be a null pointer.");
        return;
    }
    auto builder = context->get_connection_builder();
    if (nullptr == builder)
    {
        CONSOLE("No connection builder.");
        return;
    }
    auto connection = builder->new_connection(socket, addr.get(), addr_size);
    if (nullptr == connection)
    {
        CONSOLE("Unable to create connection.");
        return;
    }
    context->on_accepted(connection);
}

static void _on_readable(ez::net::basic_server* context, int socket)
{
    auto connection = context->get_connection_by_socket(socket);
    if (nullptr == connection)
    {
        CONSOLE("Unable to get connection by socket: %d.", socket);
        return;
    }
    std::string message;
    int         length = ez::net::recv(*connection, message);
    if (length <= 0)
    {
        context->on_disconnected(*connection);
        return;
    }
    context->on_readable(*connection, message);
}

static void _on_writable(ez::net::basic_server* context, int socket)
{
    auto connection = context->get_connection_by_socket(socket);
    if (nullptr == connection)
    {
        CONSOLE("Unable to get connection by socket: %d.", socket);
        return;
    }
    context->on_writable(*connection);
}

static void _on_error(ez::net::basic_server* context, const std::string& message)
{
    CONSOLE("basic_server::on_error: %s", message.c_str());
}

ez::net::basic_server::basic_server(int32_t af, int32_t type, int32_t protocol)
    : _af(af)
    , _type(type)
    , _protocol(protocol)
    , _socket(std::make_shared<scope_socket>(::socket(af, type, protocol)))
    , _event_loop(std::make_shared<epoller>(*_socket))
    , _connection_builder(nullptr)
{
    if (nullptr != _event_loop)
    {
        auto looper = std::static_pointer_cast<epoller>(_event_loop);
        looper->set_accepted_callback(this, _on_accepted);
        looper->set_writable_callback(this, _on_writable);
        looper->set_readable_callback(this, _on_readable);
        looper->set_error_callback(this, _on_error);
    }
}

ez::net::basic_server::~basic_server() {}

void ez::net::basic_server::start_up()
{
    if (nullptr != _event_loop)
    {
        _event_loop->start_loop();
    }
}

ez::net::basic_server::connections_t ez::net::basic_server::get_connections() const
{
    return _connections;
}

ez::net::basic_server::connection_t ez::net::basic_server::get_connection_by_socket(int skt) const
{
    auto found = _connections.find(skt);
    if (_connections.end() == found)
    {
        return nullptr;
    }
    return found->second;
}

ez::net::basic_server& ez::net::basic_server::set_connection_builder(
    std::shared_ptr<ez::net::basic_connection_builder> builder)
{
    this->_connection_builder = builder;
    return *this;
}

std::shared_ptr<ez::net::basic_connection_builder> ez::net::basic_server::get_connection_builder() const
{
    return this->_connection_builder;
}

void ez::net::basic_server::on_accepted(connection_t connection)
{
    {
        std::unique_lock<std::mutex> scope_lock(_mutex);
        _connections[connection->get_socket()] = connection;
    }
    on_accepted(*connection);
}

void ez::net::basic_server::on_disconnected(connection_t connection)
{
    int socket = connection->get_socket();
    {
        std::unique_lock<std::mutex> scope_lock(_mutex);
        if (_connections.end() != _connections.find(socket))
        {
            _connections.erase(socket);
        }
    }
    on_disconnected(*connection);
}