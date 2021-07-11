#ifndef _NET_BASIC_CONNECTION_H_
#define _NET_BASIC_CONNECTION_H_
#include <ctime>
#include <memory>
#include "basic_endpoint.h"
namespace ez
{
    namespace net
    {
        class scope_socket;
        class basic_connection
        {
        protected:
            basic_connection(std::shared_ptr<scope_socket> skt, std::shared_ptr<basic_endpoint> endpoint);

        public:
            basic_connection(const basic_connection& that);
            virtual ~basic_connection();

            std::shared_ptr<basic_endpoint> get_endpoint() const;

            const time_t& get_connect_time() const;

            int get_socket() const;

        protected:
            time_t                          _connect_time;
            std::shared_ptr<scope_socket>   _socket;
            std::shared_ptr<basic_endpoint> _endpoint;
        };
    } // namespace net
} // namespace ez
#endif // !_NET_BASIC_CONNECTION_H_