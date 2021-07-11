#ifndef _BASE_CODEC_BASE64_H_
#define _BASE_CODEC_BASE64_H_
#include <string>

#define BASE64(...) ez::base::codec::base64::encode(__VA_ARGS__).c_str()

namespace ez
{
    namespace base
    {
        namespace codec
        {
            namespace base64
            {
                std::string encode(const void* data, const size_t& length);

                template <typename T, size_t length>
                std::string encode(const T (&data)[length])
                {
                    return ez::base::codec::base64::encode(data, length);
                }

                template <typename T>
                std::string encode(const std::basic_string<T>& data)
                {
                    return ez::base::codec::base64::encode(data.data(), data.length());
                }

                std::string decode(const std::string& base64);

                std::string decode(const char* base64, const size_t& length);

            } // namespace base64

        } // namespace codec

    } // namespace base

} // namespace ez

#endif // !_BASE_CODEC_BASE64_H_