#ifndef _BASE_CODEC_HEX_H_
#define _BASE_CODEC_HEX_H_
#include <string>

#define HEX(...)  ez::base::codec::hex::encode(__VA_ARGS__)
#define CHEX(...) HEX(__VA_ARGS__).c_str()

namespace ez
{
    namespace base
    {
        namespace codec
        {
            namespace hex
            {
                using buffer_t = std::basic_string<unsigned char>;

                std::string encode(const void* data, const size_t& length, bool is_upper_case = false);

                template <typename T, size_t length>
                std::string encode(const T (&data)[length], bool is_upper_case = false)
                {
                    return ez::base::codec::hex::encode(data, length, is_upper_case);
                }

                template <typename T>
                std::string encode(const std::basic_string<T>& data, bool is_upper_case = false)
                {
                    return ez::base::codec::hex::encode(data.data(), data.length(), is_upper_case);
                }

                buffer_t decode(const std::string& hex);

                buffer_t decode(const char* hex, const size_t& length);

            } // namespace hex

        } // namespace codec

    } // namespace base

} // namespace ez

#endif // !_BASE_CODEC_HEX_H_