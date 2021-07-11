#include "pch.h"
#include "base64.h"

using namespace ez::base::codec::base64;

static const unsigned char g_base64_encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static unsigned char _get_char_code_at(const char& chr)
{
    if ('A' <= chr && chr <= 'Z')
    {
        return chr - 'A' + 26 * 0;
    }
    if ('a' <= chr && chr <= 'z')
    {
        return chr - 'a' + 26 * 1;
    }
    if ('0' <= chr && chr <= '9')
    {
        return chr - '0' + 26 * 2;
    }
    if ('+' == chr || '-' == chr)
    {
        return 62;
    }
    if ('/' == chr || '_' == chr)
    {
        return 63;
    }
    return 0;
}

static inline char _get_base64_char(unsigned char char_code, bool is_url_safe)
{
    char c = g_base64_encode_table[char_code];
    if (is_url_safe)
    {
        if (c == '+')
        {
            return '-';
        }
        if (c == '/')
        {
            return '_';
        }
    }
    return c;
}

static void _print_bits(const void* src, const size_t& length)
{
    const unsigned char* bytes = static_cast<const unsigned char*>(src);
    for (size_t i = 0; i < length; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            printf("%d", 0x01 & (bytes[i] >> (7 - j)));
        }
        printf(" ");
    }
    printf("\n");
}

template <typename T, size_t length>
static void _print_bits(const T (&src)[length])
{
    _print_bits(src, length);
}

static void _base64_encode(const unsigned char* src, const size_t& length, std::string& encoded)
{
    unsigned char char_codes[4] = {0x00};

    bool is_no_padding = false; // (mode & ez::base::codec::base64::no_padding) == ez::base::codec::base64::no_padding;
    bool is_url_safe   = false; // (mode & ez::base::codec::base64::url_safe) == ez::base::codec::base64::url_safe;

    if (length >= 3)
    {
        char_codes[0] = src[0] >> 2;
        char_codes[1] = ((src[0] & 0x03) << 4) | (src[1] >> 4);
        char_codes[2] = ((src[1] & 0x0F) << 2) | (src[2] >> 6);
        char_codes[3] = src[2] & 0x3F;
        encoded.append(1, _get_base64_char(char_codes[0], is_url_safe));
        encoded.append(1, _get_base64_char(char_codes[1], is_url_safe));
        encoded.append(1, _get_base64_char(char_codes[2], is_url_safe));
        encoded.append(1, _get_base64_char(char_codes[3], is_url_safe));
    }
    else if (length == 2)
    {
        char_codes[0] = src[0] >> 2;
        char_codes[1] = ((src[0] & 0x03) << 4) | (src[1] >> 4);
        char_codes[2] = (src[1] & 0x0F) << 2;
        encoded.append(1, _get_base64_char(char_codes[0], is_url_safe));
        encoded.append(1, _get_base64_char(char_codes[1], is_url_safe));
        encoded.append(1, _get_base64_char(char_codes[2], is_url_safe));
        if (!is_no_padding)
        {
            encoded.append(1, '=');
        }
    }
    else if (length == 1)
    {
        char_codes[0] = src[0] >> 2;
        char_codes[1] = (src[0] & 0x03) << 4;
        encoded.append(1, _get_base64_char(char_codes[0], is_url_safe));
        encoded.append(1, _get_base64_char(char_codes[1], is_url_safe));
        if (!is_no_padding)
        {
            encoded.append(2, '=');
        }
    }
    else
    {
        // Bad source data.
    }
    _print_bits(char_codes, length + 1);
}

static void _base64_decode(const char* src, const size_t& length, std::string& decoded)
{
    unsigned char char_code_1 = 0x00;
    unsigned char char_code_2 = 0x00;
    unsigned char char_code_3 = 0x00;
    unsigned char char_code_4 = 0x00;

    if (length >= 4)
    {
        char_code_1 = _get_char_code_at(src[0]);
        char_code_2 = _get_char_code_at(src[1]);
        decoded.append(1, ((char_code_1 & 0x3F) << 2) | ((char_code_2 & 0xF0) >> 4));
        if ('=' != src[2])
        {
            char_code_3 = _get_char_code_at(src[2]);
            decoded.append(1, ((char_code_2 & 0x0F) << 4) | ((char_code_3 & 0x3F) >> 2));
        }
        if ('=' != src[3])
        {
            char_code_4 = _get_char_code_at(src[3]);
            decoded.append(1, ((char_code_3 & 0x03) << 6) | ((char_code_4 & 0x3F) >> 0));
        }
    }
    else if (length == 3)
    {
        char_code_1 = _get_char_code_at(src[0]);
        char_code_2 = _get_char_code_at(src[1]);
        decoded.append(1, ((char_code_1 & 0x3F) << 2) | ((char_code_2 & 0xF0) >> 4));
        if ('=' != src[2])
        {
            char_code_3 = _get_char_code_at(src[2]);
            decoded.append(1, ((char_code_2 & 0x0F) << 4) | ((char_code_3 & 0x3F) >> 2));
        }
    }
    else if (length == 2)
    {
        char_code_1 = _get_char_code_at(src[0]);
        char_code_2 = _get_char_code_at(src[1]);
        decoded.append(1, ((char_code_1 & 0x3F) << 2) | ((char_code_2 & 0xF0) >> 4));
    }
    else if (length == 1)
    {
        char_code_1 = _get_char_code_at(src[0]);
        decoded.append(1, ((char_code_1 & 0x3F) << 2) | ((char_code_2 & 0xF0) >> 4));
    }
    else
    {
        // Bad source data.
    }
}

std::string ez::base::codec::base64::encode(const void* data, const size_t& length)
{
    const unsigned char* bytes = static_cast<const unsigned char*>(data);

    std::string encoded;
    for (size_t i = 0; i < length; i += 3)
    {
        _base64_encode(bytes + i, length - i, encoded);
    }
    return encoded;
}

std::string ez::base::codec::base64::decode(const std::string& base64)
{
    return ez::base::codec::base64::decode(base64.c_str(), base64.length());
}

std::string ez::base::codec::base64::decode(const char* base64, const size_t& length)
{
    _print_bits(base64, length);
    std::string decoded;
    for (size_t i = 0; i < length; i += 4)
    {
        _base64_decode(base64 + i, length - i, decoded);
    }
    return decoded;
}
