#ifndef _NET_PCH_H_
#define _NET_PCH_H_
#include <stdarg.h>
#include <memory>
#include <vector>
#include <base/platform_defs.h>
#include <base/platform_compatibility.h>
#if __WINDOWS__
#    include <Windows.h>
#    include <WinSock2.h>
#else
#    include <fcntl.h>
#    include <unistd.h>
#    include <signal.h>
#    include <arpa/inet.h>
#    include <sys/types.h>
#    include <sys/socket.h>
#    include <sys/epoll.h>
#    include <sys/ioctl.h>
#    include <netinet/in.h>
#endif // __WINDOWS__
#include "private/macros.h"
#endif // !_NET_PCH_H_