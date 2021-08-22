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

                using buffer_t = std::basic_string<unsigned char>;

                std::string encode(const void* data, const size_t& length, bool no_padding = true, bool url_encode = true);

                template <typename T, size_t length>
                std::string encode(const T (&data)[length], bool no_padding = true, bool url_encode = true)
                {
                    return ez::base::codec::base64::encode(data, length, no_padding, url_encode);
                }

                template <typename T>
                std::string encode(const std::basic_string<T>& data, bool no_padding = true, bool url_encode = true)
                {
                    return ez::base::codec::base64::encode(data.data(), data.length(), no_padding, url_encode);
                }

                buffer_t decode(const std::string& base64);

                buffer_t decode(const char* base64, const size_t& length);

            } // namespace base64

        } // namespace codec

    } // namespace base

} // namespace ez

#endif // !_BASE_CODEC_BASE64_H_