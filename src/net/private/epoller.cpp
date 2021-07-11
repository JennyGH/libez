#include "pch.h"
#include "epoller.h"
#if IS_SUPPORT_EPOLL
#    include "net_exception.h"
#    include <os.h>
#    include <format.h>
#    define ERRCODE                          ez::base::os::get_last_error()
#    define ERRMSG                           ez::base::os::get_last_error_message().c_str()
#    define IS_EVENT_SET(epoll_event_obj, e) (((epoll_event_obj.events) & e) == e)
#    define MAX_EVENT_COUNT                  256
#    define TRY_CALL(callback, ...)                                                                                    \
        do                                                                                                             \
        {                                                                                                              \
            if (nullptr != (callback))                                                                                 \
            {                                                                                                          \
                (callback)(__VA_ARGS__);                                                                               \
            }                                                                                                          \
        } while (0)

#    define TRY_COMMIT_TASK(function, ...)                                                                             \
        do                                                                                                             \
        {                                                                                                              \
            if (nullptr != (function))                                                                                 \
            {                                                                                                          \
                _thread_pool.commit((function), __VA_ARGS__);                                                          \
            }                                                                                                          \
        } while (0)

#    define ON_ERROR(...) TRY_CALL((_on_error_callback_t)_on_error, _on_error_context, ez::base::format(__VA_ARGS__))

using _on_readable_callback_t = ez::net::epoller::on_readable_callback_t<void>;
using _on_writable_callback_t = ez::net::epoller::on_writable_callback_t<void>;
using _on_accepted_callback_t = ez::net::epoller::on_accepted_callback_t<void>;
using _on_error_callback_t    = ez::net::epoller::on_error_callback_t<void>;

static int _set_no_block_socket(int fd, void* on_error_context, _on_error_callback_t on_error)
{
    int rv   = 0;
    int flag = ::fcntl(fd, F_GETFL);
    if (flag < 0)
    {
        if (nullptr != on_error)
        {
            on_error(
                on_error_context,
                ez::base::format("Unable to get socket flag, because: %s(%d).", ERRMSG, ERRCODE));
        }
        return flag;
    }
    rv = ::fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    if (flag < 0)
    {
        if (nullptr != on_error)
        {
            on_error(
                on_error_context,
                ez::base::format("Unable to set socket flag with `O_NONBLOCK`, because: %s(%d).", ERRMSG, ERRCODE));
        }
    }
    return rv;
}

ez::net::epoller::epoller(int fd)
    : _epollfd(::epoll_create(MAX_EVENT_COUNT))
    , _fd(fd)
    , _on_readable(nullptr)
    , _on_accepted(nullptr)
    , _on_error(nullptr)
    , _on_readable_context(nullptr)
    , _on_accepted_context(nullptr)
    , _on_error_context(nullptr)
    , _thread_pool(32)
{
    int rv = 0;
    if (_epollfd < 0)
    {
        THROW_NET_EXCEPTION("Unable to call `epoll_create`, because: %s(%d).", ERRMSG, _epollfd);
    }

    struct epoll_event server_epoll_event;
    memset(&server_epoll_event, 0, sizeof(server_epoll_event));
    server_epoll_event.data.fd = fd;
    server_epoll_event.events  = EPOLLIN | EPOLLET;

    // Add server socket to epoll control list.
    rv = ::epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &server_epoll_event);
    if (rv < 0)
    {
        THROW_NET_EXCEPTION("Unable to call `epoll_ctl`, because: %s(%d).", ERRMSG, rv);
    }
}

ez::net::epoller::~epoller()
{
    _thread_pool.stop();
}

void ez::net::epoller::start_loop()
{
    int rv = 0;

    rv = _set_no_block_socket(_fd, _on_error_context, (_on_error_callback_t)_on_error);
    if (rv < 0)
    {
        ON_ERROR("Unable to set no-block flag on fd %d, because: %s(%d).", (int)_fd, ERRMSG, ERRCODE);
    }

    while (true)
    {
        struct epoll_event epoll_events[MAX_EVENT_COUNT];
        memset(epoll_events, 0, sizeof(epoll_events));
        rv = ::epoll_wait(_epollfd, epoll_events, MAX_EVENT_COUNT, -1);
        if (rv < 0)
        {
            ON_ERROR("Unable to call `epoll_wait`, because: %s, errno: %d", ERRMSG, ERRCODE);
            continue;
        }
        if (rv == 0)
        {
            ON_ERROR("Epoll timeout.");
            continue;
        }
        int ready_count = rv;
        for (int i = 0; i < ready_count; i++)
        {
            struct epoll_event& event = epoll_events[i];
            if (!IS_EVENT_SET(event, EPOLLIN) && !IS_EVENT_SET(event, EPOLLOUT))
            {
                continue;
            }
            int fd = event.data.fd;
            if (fd != _fd)
            {
                if (IS_EVENT_SET(event, EPOLLIN))
                {
                    // READ event on client fd, need to receive data.
                    TRY_COMMIT_TASK((_on_readable_callback_t)(_on_readable), _on_readable_context, fd);
                    continue;
                }
                if (IS_EVENT_SET(event, EPOLLOUT))
                {
                    // WRITE event on client fd, need to receive data.
                    TRY_COMMIT_TASK((_on_writable_callback_t)(_on_writable), _on_writable_context, fd);
                    continue;
                }
            }

            // Accept all clients.
            while (true)
            {
                // READ event on server fd, need to accept a new client:
                std::shared_ptr<struct sockaddr> addr(new struct sockaddr(), ::free);
                memset(addr.get(), 0, sizeof(struct sockaddr));
                socklen_t addr_size = sizeof(struct sockaddr);

                fd = ::accept(_fd, addr.get(), &addr_size);
                if (fd < 0)
                {
                    rv = ERRCODE;
                    if (rv != EAGAIN && rv != ECONNABORTED && rv != EPROTO && rv != EINTR)
                    {
                        ON_ERROR("Unable to accept client socket, because: %s", ERRMSG);
                        break;
                    }
                    break;
                }
                rv = _set_no_block_socket(fd, _on_error_context, (_on_error_callback_t)_on_error);
                if (rv < 0)
                {
                    ON_ERROR("Unable to set no-block flag on fd %d, because: %s", fd, ERRMSG);
                }

                struct epoll_event client_epoll_event;
                memset(&client_epoll_event, 0, sizeof(client_epoll_event));
                client_epoll_event.data.fd = fd;
                client_epoll_event.events  = EPOLLIN | EPOLLET | EPOLLOUT;
                // Add Client socket to epoll control list.
                rv = ::epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &client_epoll_event);
                if (rv < 0)
                {
                    ON_ERROR("Unable to call `epoll_ctl`, because: %s", ERRMSG);
                    continue;
                }
                // Call the callback.
                TRY_COMMIT_TASK((_on_accepted_callback_t)(_on_accepted), _on_accepted_context, fd, addr, addr_size);
            }
        }
    }
}
#endif // IS_SUPPORT_EPOLL