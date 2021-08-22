#include "pch.h"
#include "codec.h"

static inline unsigned char _hex_decode(const char& hex)
{
    if ('0' <= hex && hex <= '9')
    {
        return hex - '0';
    }
    if ('A' <= hex && hex <= 'Z')
    {
        return hex - 'A' + 10;
    }
    if ('a' <= hex && hex <= 'z')
    {
        return hex - 'a' + 10;
    }
    return 0x00;
}

std::string ez::base::codec::hex::encode(const void* data, const size_t& length, bool is_upper_case)
{
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    std::string          encoded;
    for (size_t i = 0; i < length; i++)
    {
        char hex[3] = {0};
        sprintf(hex, is_upper_case ? "%02X" : "%02x", bytes[i]);
        encoded.append(hex);
    }
    return encoded;
}

ez::base::codec::hex::buffer_t ez::base::codec::hex::decode(const std::string& hex)
{
    return decode(hex.c_str(), hex.length());
}

ez::base::codec::hex::buffer_t ez::base::codec::hex::decode(const char* hex, const size_t& length)
{
    buffer_t bytes;
    for (size_t i = 0; i < length;)
    {
        unsigned char h    = _hex_decode(hex[i++]);
        unsigned char l    = _hex_decode(hex[i++]);
        unsigned char byte = (h << 4) + l;
        bytes.append(1, byte);
    }
    return bytes;
}
