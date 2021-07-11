#ifndef _PCH_H_
#define _PCH_H_
#include <private/platform_compatibility.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#if defined(_MSC_VER) && _MSC_VER > 0
#    include <Windows.h>
#    include <WinSock2.h>
using socket_t = SOCKET;
#else
#    include <unistd.h>
#    include <arpa/inet.h>
#    include <sys/types.h>
#    include <sys/socket.h>
#    include <sys/epoll.h>
#    include <sys/ioctl.h>
#    include <netinet/in.h>
using socket_t = int;
#endif // defined(_MSC_VER) && _MSC_VER > 0
#include <benchmark/benchmark.h>
#include <os.h>
#include <argument_parser.h>
#endif // !_PCH_H_