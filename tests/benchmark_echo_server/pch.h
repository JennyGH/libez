#ifndef _PCH_H_
#define _PCH_H_
#include <base/platform_defs.h>
#include <base/platform_compatibility.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#if __WINDOWS__
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
#endif // __WINDOWS__
#include <benchmark/benchmark.h>
#include <base/os.h>
#include <base/argument_parser.h>
#endif // !_PCH_H_