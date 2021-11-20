#ifndef _NET_EPOLLER_H_
#define _NET_EPOLLER_H_
#include "pch.h"
#if __WINDOWS__
#    define IS_SUPPORT_EPOLL 0
#else
#    include <sys/socket.h>
#    include <memory>
#    include <base/thread_pool.h>
#    include "basic_event_loop.h"
#    include "private/noncopyable.h"
#    include "private/nonmoveable.h"
#    define IS_SUPPORT_EPOLL 1
#endif // __WINDOWS__
namespace ez
{
    namespace net
    {
#if IS_SUPPORT_EPOLL
        class epoller
            : public noncopyable
            , public nonmoveable
            , public basic_event_loop
        {
        public:
            template <typename context_type>
            using on_accepted_callback_t = void (*)(context_type*, int, std::shared_ptr<struct sockaddr>, const socklen_t&);
            template <typename context_type>
            using on_readable_callback_t = void (*)(context_type*, int);
            template <typename context_type>
            using on_writable_callback_t = void (*)(context_type*, int);
            template <typename context_type>
            using on_error_callback_t = void (*)(context_type*, const std::string&);

        private:
            void accept_all(int fd);
            void start_worker_loop();

        public:
            epoller(int fd);
            ~epoller();

            void start_loop();

            template <typename context_type>
            epoller& set_accepted_callback(context_type* context, on_accepted_callback_t<context_type> callback)
            {
                this->_on_accepted_context = context;
                this->_on_accepted         = (void*)callback;
                return *this;
            }

            template <typename context_type>
            epoller& set_readable_callback(context_type* context, on_readable_callback_t<context_type> callback)
            {
                this->_on_readable_context = context;
                this->_on_readable         = (void*)callback;
                return *this;
            }

            template <typename context_type>
            epoller& set_writable_callback(context_type* context, on_writable_callback_t<context_type> callback)
            {
                this->_on_writable_context = context;
                this->_on_writable         = (void*)callback;
                return *this;
            }

            template <typename context_type>
            epoller& set_error_callback(context_type* context, on_error_callback_t<context_type> callback)
            {
                this->_on_error_context = context;
                this->_on_error         = (void*)callback;
                return *this;
            }

        private:
            int                   _epollfd;
            int                   _fd;
            void*                 _on_readable;
            void*                 _on_writable;
            void*                 _on_accepted;
            void*                 _on_error;
            void*                 _on_readable_context;
            void*                 _on_writable_context;
            void*                 _on_accepted_context;
            void*                 _on_error_context;
            ez::base::thread_pool _thread_pool;
        };
#else
        class epoller;
#endif // IS_SUPPORT_EPOLL
    }  // namespace net
} // namespace ez
#endif // !_NET_EPOLLER_H_