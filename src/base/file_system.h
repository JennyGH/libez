#ifndef _BASE_FILE_SYSTEM_H_
#define _BASE_FILE_SYSTEM_H_
#ifdef __cplusplus
#    include <list>
#    include <time.h>
#    include <string>
namespace ez
{
    namespace base
    {
        namespace file_system
        {
            using path_t      = std::string;
            using paths_t     = std::list<path_t>;
            using file_info_t = struct
            {
                std::string    path;
                size_t         size;
                bool           exists;
                char           type;
                unsigned short access_rights;
                time_t         create_time;
                time_t         access_time;
                time_t         modified_time;
            };

            bool    exists(const path_t& path);
            bool    is_dir(const path_t& path);
            bool    is_file(const path_t& path);
            bool    is_link(const path_t& path);
            size_t  get_size(const path_t& path);
            void    get_file_info(const path_t& path, file_info_t& output_info);
            size_t  load(const path_t& path, void* dst, const size_t& dst_size);
            size_t  save(const path_t& path, const void* src, const size_t& src_size);
            paths_t walk(const path_t& path, int depth = 0);
        } // namespace file_system

    } // namespace base

} // namespace ez
#endif // __cplusplus
#endif // !_BASE_FILE_SYSTEM_H_