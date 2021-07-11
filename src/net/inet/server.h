#ifndef _NET_TCP_SERVER_H_
#define _NET_TCP_SERVER_H_
#include <net/basic_server.h>
#include <net/inet/endpoint.h>
namespace ez
{
    namespace net
    {
        namespace inet
        {
            class server : public basic_server
            {
            public:
                server(const inet::endpoint& address);
                ~server();

            protected:
                virtual void on_accepted(const basic_connection& connection) override;
                virtual void on_writable(const basic_connection& connection) override;
                virtual void on_readable(const basic_connection& connection, const std::string& message) override;
                virtual void on_disconnected(const basic_connection& connection) override;
            };
        } // namespace inet

    } // namespace net

} // namespace ez
#endif // !_NET_TCP_SERVER_H_