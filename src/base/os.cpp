#include "pch.h"
#include "os.h"
#include "platform_defs.h"
#ifdef __cplusplus
#    if __WINDOWS__
#        include <Windows.h>
#    else
#        include <errno.h>
#        include <stdlib.h>
#        include <string.h>
#    endif // __WINDOWS__

ez::base::os::endian_t ez::base::os::get_current_endian()
{
    union
    {
        char           c;
        unsigned short s;
    } u;
    u.s = 0x1234;
    if (u.s == 0x12)
    {
        return ez::base::os::big_endian;
    }
    return ez::base::os::little_endian;
}

void ez::base::os::serialize(const void* src, const size_t& src_size, const endian_t& to_endian, void* dst, const size_t& dst_size)
{
    if (nullptr == src || src_size == 0)
    {
        return;
    }
    if (nullptr == dst || dst_size == 0)
    {
        return;
    }
    if (dst_size < src_size)
    {
        return;
    }

    ez::base::os::endian_t from_endian = get_current_endian();
    if (from_endian == to_endian)
    {
        memcpy_s(dst, dst_size, src, src_size);
        return;
    }

    const unsigned char* src_bytes = (const unsigned char*)(src);
    unsigned char*       dst_bytes = (unsigned char*)(dst);
    for (size_t i = 0; i < src_size; i++)
    {
        dst_bytes[i] = src_bytes[src_size - i - 1];
    }
}

void ez::base::os::deserialize(const void* src, const size_t& src_size, const endian_t& to_endian, void* dst, const size_t& dst_size)
{
    serialize(src, src_size, to_endian, dst, dst_size);
}

int ez::base::os::get_last_error()
{
#    if __WINDOWS__
    return ::GetLastError();
#    else
    return errno;
#    endif // __WINDOWS__
}

std::string ez::base::os::get_error_message(int errcode)
{
    std::string res;
#    if __WINDOWS__
    LPVOID lpMsgBuf = NULL;
    DWORD  dwLength = ::FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMsgBuf,
        0,
        NULL);
    if (NULL != lpMsgBuf)
    {
        res.assign((LPCSTR)lpMsgBuf, dwLength);
        ::LocalFree(lpMsgBuf);
        lpMsgBuf           = NULL;
        std::size_t length = res.size();
        while (length && (res[length - 1] == '\n' || res[length - 1] == '\r'))
        {
            res.erase(res.size() - 1);
            length--;
        }
    }
#    else
    char buffer[1024] = {0};
#        if ANDROID || iOS || ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE)
    int  ret          = ::strerror_r(errcode, buffer, sizeof(buffer));
    res.assign(buffer);
#        else
    char* str = ::strerror_r(errcode, buffer, sizeof(buffer));
    res.assign(str);
#        endif // ANDROID || iOS || ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >=
               // 600) && !_GNU_SOURCE)
#    endif     // __WINDOWS__
    return res;
}

std::string ez::base::os::get_last_error_message()
{
    return ez::base::os::get_error_message(ez::base::os::get_last_error());
}
#endif // __cplusplus