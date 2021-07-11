#ifndef _NET_BASIC_SERVER_H_
#define _NET_BASIC_SERVER_H_
#include <map>
#include <mutex>
#include <memory>
#include <cstdint>
#include <basic_event_loop.h>
#include <basic_connection.h>
#include <basic_connection_builder.h>
namespace ez
{
    namespace net
    {
        class scope_socket;
        class basic_server
        {
        protected:
            using connection_t  = std::shared_ptr<basic_connection>;
            using connections_t = std::map<int, connection_t>;

        protected:
            basic_server(int32_t af, int32_t type, int32_t protocol);

        public:
            virtual ~basic_server();

            virtual void start_up();

            connections_t get_connections() const;

            connection_t get_connection_by_socket(int skt) const;

            basic_server& set_connection_builder(std::shared_ptr<basic_connection_builder> builder);
            std::shared_ptr<basic_connection_builder> get_connection_builder() const;

        public:
            void         on_accepted(connection_t connection);
            void         on_disconnected(connection_t connection);
            virtual void on_accepted(const basic_connection& connection)                             = 0;
            virtual void on_writable(const basic_connection& connection)                             = 0;
            virtual void on_readable(const basic_connection& connection, const std::string& message) = 0;
            virtual void on_disconnected(const basic_connection& connection)                         = 0;

        protected:
            int32_t                                   _af;
            int32_t                                   _type;
            int32_t                                   _protocol;
            std::mutex                                _mutex;
            std::shared_ptr<scope_socket>             _socket;
            connections_t                             _connections;
            std::shared_ptr<basic_event_loop>         _event_loop;
            std::shared_ptr<basic_connection_builder> _connection_builder;
        };
    } // namespace net
} // namespace ez
#endif // !_NET_BASIC_SERVER_H_