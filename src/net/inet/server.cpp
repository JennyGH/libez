#include "pch.h"
#include "inet/server.h"
#include "net_exception.h"
#include "private/socket_api.h"
#include "private/scope_socket.h"
#include <base/os.h>
#include <base/format.h>
#include <base/codec.h>

#define CONSOLE(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

#define ERRCODE ez::base::os::get_last_error()
#define ERRMSG  ez::base::os::get_last_error_message().c_str()

ez::net::inet::server::server(const inet::endpoint& address)
    : basic_server(AF_INET, SOCK_STREAM, IPPROTO_IP)
{
    int rv = 0;
    int on = 1;

#ifdef SIGPIPE
    ::signal(SIGPIPE, SIG_IGN);
#endif // SIGPIPE

    rv = ::setsockopt(*_socket, SOL_SOCKET, SO_REUSEADDR, (const void*)&on, sizeof(on));
    if (rv < 0)
    {
        THROW_NET_EXCEPTION("Unable to enable reuse address on fd %d, because: %s(%d).", (int)*_socket, ERRMSG, ERRCODE);
    }

    rv = ::setsockopt(*_socket, SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
    if (rv < 0)
    {
        THROW_NET_EXCEPTION("Unable to enable reuse port on fd %d, because: %s(%d).", (int)*_socket, ERRMSG, ERRCODE);
    }

    rv = ::bind(*_socket, (const sockaddr*)address.get_addr(), address.get_size());
    if (rv < 0)
    {
        THROW_NET_EXCEPTION("Unable to call `bind`, because: %s(%d).", ERRMSG, ERRCODE);
    }
    rv = ::listen(*_socket, 0);
    if (rv < 0)
    {
        THROW_NET_EXCEPTION("Unable to call `listen`, because: %s(%d).", ERRMSG, ERRCODE);
    }
}

ez::net::inet::server::server::~server() {}

void ez::net::inet::server::on_accepted(const basic_connection& connection)
{
    CONSOLE("Connection on socket: %d connected, time: %ld", connection.get_socket(), connection.get_connect_time());
}

void ez::net::inet::server::on_writable(const basic_connection&)
{
    // ...
}

void ez::net::inet::server::on_readable(const basic_connection& connection, const std::string& message)
{
    CONSOLE("Received message: %s", message.c_str());
    ez::net::send(connection, message.c_str(), message.length(), 0);
}

void ez::net::inet::server::on_disconnected(const basic_connection& connection)
{
    CONSOLE("Connection on socket: %d disconnected.", connection.get_socket());
}
