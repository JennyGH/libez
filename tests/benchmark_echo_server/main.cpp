#include "pch.h"
#include "scope_socket.h"
#include "global_initializer.h"

#define THREAD_COUNT      8
#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define ERRMSG            ez::base::os::get_last_error_message().c_str()

static void               _initialize_socket_environment();
static global_initializer g_global_intializer(_initialize_socket_environment, nullptr);

static void _initialize_socket_environment()
{
#if __WINDOWS__
    WSADATA wsaData;
    WORD    wVersionRequested = MAKEWORD(2, 2);
    INT     nError            = ::WSAStartup(wVersionRequested, &wsaData);
    CONSOLE("WSAStartup return: %d", nError);
#endif // __WINDOWS__
}

static int _send_all(const scope_socket& skt, const void* data, const size_t& length, int flags)
{
    int rv         = 0;
    int send_bytes = 0;
    do
    {
        rv = ::send(skt, (const char*)data + send_bytes, length - send_bytes, 0);
        if (rv <= 0)
        {
            CONSOLE("Unable to send %ld bytes to remote server, because: %s", (length - send_bytes), ERRMSG);
            return rv;
        }
        send_bytes += rv;
    } while (send_bytes < length);

    return send_bytes;
}

static void _initialize_sockaddr(const std::string& ip, unsigned short port, sockaddr_in& addr)
{
    ::memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr(ip.c_str());
    addr.sin_port        = htons(port);
}

static std::string _generate_send_buffer(const size_t& length = 1024)
{
    return std::string(length, 'x');
}

static size_t _test_tcp_transfer(const sockaddr_in& addr, const void* data, const size_t& length)
{
    int rv = 0;
    // Connect to server.
    scope_socket skt = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == skt)
    {
        CONSOLE("Unable to create socket, because: %s", ERRMSG);
        exit(-1);
    }

    rv = ::connect(skt, (const sockaddr*)&addr, sizeof(sockaddr_in));
    if (SOCKET_ERROR == rv)
    {
        CONSOLE("Unable to connect remote server.");
        exit(-1);
    }

    rv = _send_all(skt, (const char*)data, length, 0);
    if (rv <= 0)
    {
        exit(-1);
    }

    return rv;
}

static void test_tcp_transfer(benchmark::State& state)
{
    static const std::string    ip          = "192.168.201.184";
    static const unsigned short port        = 8080;
    static const std::string    buffer      = "ping"; //_generate_send_buffer(32 * 1024);
    int64_t                     output_size = 0;
    int64_t                     items       = 0;

    // Initialize client address.
    sockaddr_in addr;
    _initialize_sockaddr(ip, port, addr);

    // Connect to server.
    scope_socket skt = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == skt)
    {
        CONSOLE("Unable to create socket, because: %s", ERRMSG);
        exit(-1);
    }

    int rv = ::connect(skt, (const sockaddr*)&addr, sizeof(sockaddr_in));
    if (SOCKET_ERROR == rv)
    {
        CONSOLE("Unable to connect remote server.");
        exit(-1);
    }

    for (auto _ : state)
    {
        rv = _send_all(skt, buffer.data(), buffer.length(), 0);
        if (rv <= 0)
        {
            exit(-1);
        }
        output_size += rv;
        items++;
        // output_size += _test_tcp_transfer(addr, buffer.data(), buffer.length());
    }
    state.SetItemsProcessed(items);
    state.SetBytesProcessed(state.iterations() * output_size);
}
BENCHMARK(test_tcp_transfer)->Unit(benchmark::kMillisecond)->Threads(THREAD_COUNT)->UseRealTime();

BENCHMARK_MAIN();