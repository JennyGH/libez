#include "pch.h"
#include "encoding.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _console
#    define _console(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)
#endif // !_console

#if __WINDOWS__
#    include <Windows.h>
static size_t _utf8_to_ansi(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    // UTF-8转unicode
    int      unicode_length = ::MultiByteToWideChar(CP_UTF8, 0, inbuf, inlen, NULL, 0);
    wchar_t* unicode        = new wchar_t[unicode_length]();
    ::wmemset(unicode, 0, unicode_length);
    ::MultiByteToWideChar(CP_UTF8, 0, inbuf, inlen, unicode, unicode_length);

    // unicode转gbk
    int gbk_length = ::WideCharToMultiByte(CP_ACP, 0, unicode, unicode_length, NULL, 0, NULL, NULL);
    if (outlen < gbk_length)
    {
        _console("outlen < gbk_length, need: %d", gbk_length);
        return 0;
    }

    // char* gbk = new char[gbk_length]();
    //::memset(gbk, 0, gbk_length);
    WideCharToMultiByte(CP_ACP, 0, unicode, unicode_length, outbuf, outlen, NULL, NULL);

    // delete[] gbk;
    delete[] unicode;

    return gbk_length;
}

static size_t _ansi_to_utf8(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    // gbk转unicode
    int      unicode_length = ::MultiByteToWideChar(CP_ACP, 0, inbuf, inlen, NULL, 0);
    wchar_t* unicode        = new wchar_t[unicode_length]();
    ::wmemset(unicode, 0, unicode_length);
    ::MultiByteToWideChar(CP_ACP, 0, inbuf, inlen, unicode, unicode_length);

    // unicode转UTF-8
    int utf8_length = ::WideCharToMultiByte(CP_UTF8, 0, unicode, unicode_length, NULL, 0, NULL, NULL);
    if (outlen < utf8_length)
    {
        _console("outlen < utf8_length, need: %d", utf8_length);
        return 0;
    }
    // char* utf8 = new char[utf8_length]();
    //::memset(utf8, 0, utf8_length);
    ::WideCharToMultiByte(CP_UTF8, 0, unicode, unicode_length, outbuf, outlen, NULL, NULL);

    // delete[] utf8;
    delete[] unicode;

    return utf8_length;
}
#else
#    include <iconv.h>

//代码转换:从一种编码转为另一种编码
static size_t _charset_convert(const char* from_charset, const char* to_charset, char* inbuf, size_t inbuf_size, char* outbuf, size_t outbuf_size)
{
    int     rv               = 0;
    size_t  inbuf_left_size  = inbuf_size;
    size_t  outbuf_left_size = outbuf_size;
    iconv_t cd               = ::iconv_open(to_charset, from_charset);
    if (cd == 0)
    {
        goto fail;
    }
    memset(outbuf, 0, outbuf_size);
    rv = ::iconv(cd, &inbuf, &inbuf_left_size, &outbuf, &outbuf_left_size);
    if (rv == -1)
    {
        goto fail;
    }

    ::iconv_close(cd);
    return outbuf_size - outbuf_left_size;

fail:
    int errcode = errno;
    _console("iconv failed, because: %s (%d).", ::strerror(errcode), errcode);
    if (0 != cd)
    {
        ::iconv_close(cd);
    }
    return 0;
}

static size_t _utf8_to_ansi(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    return _charset_convert("UTF-8", "GBK", inbuf, inlen, outbuf, outlen);
}

static size_t _ansi_to_utf8(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
    return _charset_convert("GBK", "UTF-8", inbuf, inlen, outbuf, outlen);
}

#endif // _MSC_VER

#if 0
typedef std::codecvt_utf8<wchar_t>                         utf8_convertor_type;
typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> gbk_convertor_type;

static std::string wstring_to_utf8string(const std::wstring& str)
{
    static std::wstring_convert<utf8_convertor_type> convertor;
    return convertor.to_bytes(str);
}

static std::wstring utf8string_to_wstring(const std::string& str)
{
    static std::wstring_convert<utf8_convertor_type> convertor;
    return convertor.from_bytes(str);
}

static std::string
wstring_to_string(const std::wstring& str, const std::string& locale = "chs")
{
    static std::wstring_convert<gbk_convertor_type> convertor(
        new gbk_convertor_type(locale));
    return convertor.to_bytes(str);
}

static std::wstring
string_to_wstring(const std::string& str, const std::string& locale = "chs")
{
    static std::wstring_convert<gbk_convertor_type> convertor(
        new gbk_convertor_type(locale));
    return convertor.from_bytes(str);
}
#endif

bool is_utf8_string(const std::string& str)
{
    char        count_of_bytes = 0;    // UFT8可用1-6个字节编码,ASCII用一个字节
    bool        is_all_ansi    = true; //如果全部都是ASCII, 说明不是UTF-8
    std::size_t length         = str.length();

    for (std::size_t i = 0; i < length; i++)
    {
        unsigned char chr = str[i];

        // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,
        // 但用一个字节存,最高位标记为0,o0xxxxxxx
        if ((chr & 0x80) != 0)
        {
            is_all_ansi = false;
        }

        if (count_of_bytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
        {
            if (chr >= 0x80)
            {
                if (chr >= 0xFC && chr <= 0xFD)
                    count_of_bytes = 6;
                else if (chr >= 0xF8)
                    count_of_bytes = 5;
                else if (chr >= 0xF0)
                    count_of_bytes = 4;
                else if (chr >= 0xE0)
                    count_of_bytes = 3;
                else if (chr >= 0xC0)
                    count_of_bytes = 2;
                else
                {
                    return false;
                }
                count_of_bytes--;
            }
        }
        else //多字节符的非首字节,应为 10xxxxxx
        {
            if ((chr & 0xC0) != 0x80)
            {
                return false;
            }
            count_of_bytes--;
        }
    }

    if (count_of_bytes > 0) //违返规则
        return false;

    if (is_all_ansi) //如果全部都是ASCII, 说明不是UTF-8
        return false;

    return true;
}

std::string ez::base::encoding::utf8_to_ansi(const std::string& utf8)
{
    if (!is_utf8_string(utf8))
    {
        return utf8;
    }
    size_t      in_buf_size  = utf8.length();
    size_t      out_buf_size = in_buf_size;
    char*       input        = const_cast<char*>(utf8.c_str());
    char*       output       = new char[out_buf_size + 1]();
    size_t      converted    = _utf8_to_ansi(input, in_buf_size, output, out_buf_size);
    std::string ansi(output, converted);
    delete[] output;
    return ansi;
}
std::string ez::base::encoding::ansi_to_utf8(const std::string& ansi)
{
    if (is_utf8_string(ansi))
    {
        return ansi;
    }
    size_t      in_buf_size  = ansi.length();
    size_t      out_buf_size = in_buf_size * 2;
    char*       input        = const_cast<char*>(ansi.c_str());
    char*       output       = new char[out_buf_size]();
    size_t      converted    = _ansi_to_utf8(input, in_buf_size, output, out_buf_size);
    std::string utf8(output, converted);
    delete[] output;
    return utf8;
}