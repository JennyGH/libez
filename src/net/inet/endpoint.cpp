#include "pch.h"
#include "inet/endpoint.h"

static inline struct sockaddr_in* _new_sockaddr(const std::string& ipv4, uint16_t port)
{
    struct sockaddr_in* ptr = new struct sockaddr_in();
    memset(ptr, 0, sizeof(*ptr));
    ptr->sin_family      = AF_INET;
    ptr->sin_port        = htons(port);
    ptr->sin_addr.s_addr = inet_addr(ipv4.c_str());
    return ptr;
}

static inline struct sockaddr_in* _new_sockaddr()
{
    struct sockaddr_in* ptr = new struct sockaddr_in();
    memset(ptr, 0, sizeof(*ptr));
    return ptr;
}

ez::net::inet::endpoint::endpoint()
    : basic_endpoint(_new_sockaddr(), sizeof(struct sockaddr_in))
{
}

ez::net::inet::endpoint::endpoint(const void* sockaddr)
    : basic_endpoint(sockaddr, sizeof(struct sockaddr_in))
{
}

ez::net::inet::endpoint::endpoint(const std::string& ipv4, uint16_t port)
    : basic_endpoint(_new_sockaddr(ipv4, port), sizeof(struct sockaddr_in))
{
}

ez::net::inet::endpoint::endpoint(endpoint&& that) noexcept
    : basic_endpoint(std::move(that))
{
}

ez::net::inet::endpoint::endpoint(const endpoint& that)
    : basic_endpoint(that)
{
}

ez::net::inet::endpoint::~endpoint() {}