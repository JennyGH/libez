#include "pch.h"
#include "file_system.h"
#include "os.h"

#define CONSOLE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

using namespace ez::base::file_system;

static int _walk(const path_t& path, int depth, paths_t& paths)
{
    path_t tmp_path = path;
    char   last_chr = tmp_path.back();
#if __LINUX__
    if (last_chr != '/')
    {
        tmp_path.push_back('/');
    }
    DIR* dir_ptr = opendir(tmp_path.c_str());
    if (NULL == dir_ptr)
    {
        return -1;
    }
    SCOPE_PTR_OF(dir_ptr, closedir);

    struct dirent* dir_info = NULL;
    struct stat    file_state;
    while ((dir_info = readdir(dir_ptr)))
    {
        if ((strncmp(dir_info->d_name, ".", 1) == 0) || (strncmp(dir_info->d_name, "..", 2) == 0))
        {
            continue;
        }

        paths.push_back(tmp_path + dir_info->d_name);
        int rv = lstat(paths.back().c_str(), &file_state);
        if (0 == rv)
        {
            if (depth > 0 && S_ISDIR(file_state.st_mode))
            {
                _walk(paths.back(), depth - 1, paths);
            }
        }
        else
        {
            CONSOLE("[ERROR] `lstat` return: %d, because: %s(%d).", rv, ez::base::os::get_last_error_message().c_str(), ez::base::os::get_last_error());
        }
    }
#elif __WINDOWS__
    if (last_chr != '/' && last_chr != '\\')
    {
        tmp_path.push_back('\\');
    }
    tmp_path.append("*");
    WIN32_FIND_DATA data;
    HANDLE          handle = ::FindFirstFile(path, &data);
    if (handle == NULL)
    {
        return -1;
    }
    SCOPE_PTR_OF(handle, ::FindClose);
    do
    {
        paths.push_back(tmp_path + data.cFileName);
        if (depth > 0 && data.dwFileAttributes & _A_SUBDIR)
        {
            _walk(paths.back(), depth - 1, paths);
        }
    } while (::FindNextFile(handle, &data));
#endif
    return paths.size();
}

bool ez::base::file_system::exists(const path_t& path)
{
    return ::access(path.c_str(), 0) == 0;
}

bool ez::base::file_system::is_dir(const path_t& path)
{
    struct stat file_state;
    memset(&file_state, 0, sizeof(file_state));
    stat(path.c_str(), &file_state);
    return S_ISDIR(file_state.st_mode);
}

bool ez::base::file_system::is_file(const path_t& path)
{
    struct stat file_state;
    memset(&file_state, 0, sizeof(file_state));
    stat(path.c_str(), &file_state);
    return S_ISREG(file_state.st_mode);
}

bool ez::base::file_system::is_link(const path_t& path)
{
    struct stat file_state;
    memset(&file_state, 0, sizeof(file_state));
    stat(path.c_str(), &file_state);
    return S_ISLNK(file_state.st_mode);
}

size_t ez::base::file_system::get_size(const path_t& path)
{
    struct stat file_state;
    memset(&file_state, 0, sizeof(file_state));
    stat(path.c_str(), &file_state);
    return file_state.st_size;
}

void ez::base::file_system::get_file_info(const path_t& path, file_info_t& output_info)
{
    // ...
}

size_t ez::base::file_system::load(const path_t& path, void* dst, const size_t& dst_size)
{
    size_t file_size = get_size(path);
    if (file_size > dst_size)
    {
        return 0;
    }
    if (is_dir(path))
    {
        return 0;
    }

    FILE* file_ptr = ::fopen(path.c_str(), "rb");
    if (nullptr == file_ptr)
    {
        return 0;
    }
    SCOPE_PTR_OF(file_ptr, ::fclose);

    size_t read_size = ::fread(dst, 1, file_size, file_ptr);

    return read_size;
}

size_t ez::base::file_system::save(const path_t& path, const void* src, const size_t& src_size)
{
    if (is_dir(path))
    {
        return 0;
    }

    FILE* file_ptr = ::fopen(path.c_str(), "wb");
    if (nullptr == file_ptr)
    {
        return 0;
    }
    SCOPE_PTR_OF(file_ptr, ::fclose);

    size_t read_size = ::fwrite(src, 1, src_size, file_ptr);

    return read_size;
}

paths_t ez::base::file_system::walk(const path_t& path, int depth)
{
    paths_t paths;
    int     count = _walk(path, depth, paths);
    return paths;
}
