#ifndef _BASE_FILE_SYSTEM_H_
#define _BASE_FILE_SYSTEM_H_
#include <list>
#include <string>
#include <fstream>
#include <stdint.h>

namespace ez
{
    namespace base
    {
        namespace file_system
        {
            using path_t          = std::string;
            using paths_t         = std::list<path_t>;
            using bytes_t         = std::basic_string<unsigned char>;
            using load_callback_t = void (*)(void* context, const uint8_t* bytes, const uint64_t& length);

            enum walk_filter_t
            {
                file      = (1 << 0),
                directory = (1 << 1),
                all       = file | directory
            };

            bool     exists(const std::string& path);
            bool     mkdir(const std::string& path);
            bool     remove(const std::string& path);
            bool     rename(const std::string& from, const std::string& to);
            paths_t  walk(const path_t& path, walk_filter_t filter = walk_filter_t::file, int depth = 0);
            bytes_t  load(const path_t& path);
            bytes_t  load(const path_t& path, const uint64_t& length, const uint64_t& offset = 0);
            uint64_t load(const path_t& path, const uint64_t& length, const uint64_t& offset, load_callback_t callback, void* context);
            uint64_t save(const path_t& path, const void* data, const uint64_t& length);
            template <typename T>
            static inline uint64_t save(const path_t& path, const std::basic_string<T>& data)
            {
                return file_system::save(path, data.data(), data.length());
            }
            uint64_t copy(const path_t& from, const path_t& to);

        } // namespace file_system

    } // namespace base

} // namespace ez

#endif // !_BASE_FILE_SYSTEM_H_