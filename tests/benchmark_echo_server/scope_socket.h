#ifndef _BENCHMARK_TEST_ECHO_SERVER_SCOPE_SOCKET_H_
#define _BENCHMARK_TEST_ECHO_SERVER_SCOPE_SOCKET_H_
class scope_socket
{
public:
    scope_socket(int skt);
    ~scope_socket();

    operator int() const;

private:
    int _socket;
};
#endif // !_BENCHMARK_TEST_ECHO_SERVER_SCOPE_SOCKET_H_