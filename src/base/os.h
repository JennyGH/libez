#ifndef _BASE_OS_H_
#define _BASE_OS_H_
#include <string>
#ifdef __cplusplus
namespace ez
{
    namespace base
    {
        namespace os
        {

            enum endian_t
            {
                big_endian = 0,
                little_endian
            };

            endian_t get_current_endian();

            void serialize(const void* src, const size_t& src_size, const endian_t& to_endian, void* dst, const size_t& dst_size);

            template <typename integer_t>
            inline void serialize(const integer_t& src, const endian_t& to_endian, void* dst, const size_t& dst_size)
            {
                serialize(&src, sizeof(src), to_endian, dst, dst_size);
            }

            template <typename integer_t, typename buffer_t, size_t dst_size>
            inline void serialize(const integer_t& src, const endian_t& to_endian, buffer_t (&dst)[dst_size])
            {
                serialize(src, to_endian, dst, dst_size);
            }

            void deserialize(const void* src, const size_t& src_size, const endian_t& to_endian, void* dst, const size_t& dst_size);

            template <typename integer_t>
            inline void deserialize(const void* src, const size_t& src_size, const endian_t& to_endian, integer_t& dst)
            {
                deserialize(src, src_size, to_endian, &dst, sizeof(dst));
            }

            int         get_last_error();
            std::string get_last_error_message();
            std::string get_error_message(int errcode);

        } // namespace os

    } // namespace base

} // namespace ez
#endif // __cplusplus
#endif // !_BASE_OS_H_