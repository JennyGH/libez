#include "pch.h"
#include "file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fstream>
#include "path.h"
#ifndef _MSC_VER
#    include <unistd.h>
#    include <dirent.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#else
#    include <io.h>
#    include <direct.h>
#    include <Windows.h>
#endif // !_MSC_VER

static int _walk(
    const ez::base::path&                path,
    ez::base::file_system::walk_filter_t filter,
    int                                  depth,
    ez::base::file_system::paths_t&      paths)
{
#ifndef _MSC_VER
    DIR* dir_ptr = opendir(path.to_string().c_str());
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
        const std::string current_path = path.join(dir_info->d_name);

        int rv = lstat(current_path.c_str(), &file_state);
        if (0 == rv)
        {
            if (((filter & ez::base::file_system::walk_filter_t::file) != 0 && !S_ISDIR(file_state.st_mode)) ||
                ((filter & ez::base::file_system::walk_filter_t::directory) != 0 && S_ISDIR(file_state.st_mode)))
            {
                paths.push_back(current_path);
            }
            if (depth > 0 && S_ISDIR(file_state.st_mode))
            {
                _walk(current_path, filter, depth - 1, paths);
            }
        }
    }
#else
    WIN32_FIND_DATAA data;
    HANDLE           handle = ::FindFirstFileA(path.join("*").to_string().c_str(), &data);
    if (handle == NULL || INVALID_HANDLE_VALUE == handle)
    {
        return -1;
    }
    do
    {
        if ((strncmp(data.cFileName, ".", 1) == 0) || (strncmp(data.cFileName, "..", 2) == 0))
        {
            continue;
        }
        const auto current_path = path.join(data.cFileName);
        if (((filter & ez::base::file_system::walk_filter_t::file) != 0 && (data.dwFileAttributes & _A_SUBDIR) == 0) ||
            ((filter & ez::base::file_system::walk_filter_t::directory) != 0 &&
             (data.dwFileAttributes & _A_SUBDIR) != 0))
        {
            paths.push_back(current_path);
        }
        if (depth > 0 && data.dwFileAttributes & _A_SUBDIR)
        {
            _walk(current_path, filter, depth - 1, paths);
        }
    } while (::FindNextFileA(handle, &data));
    ::FindClose(handle);
#endif
    return paths.size();
}

bool ez::base::file_system::exists(const std::string& path)
{
    return ::access(path.c_str(), 0) == 0;
}

bool ez::base::file_system::mkdir(const std::string& path)
{
#ifdef _MSC_VER
    const ez::base::path native_path = path;
    const ez::base::path parent_path = native_path.parent();
    if (!file_system::exists(parent_path))
    {
        file_system::mkdir(parent_path);
    }
    return 0 == ::mkdir(native_path.to_string().c_str());
#else
    return 0 == ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif // _MSC_VER
}

bool ez::base::file_system::remove(const std::string& path)
{
    if (!file_system::exists(path))
    {
        return false;
    }
#ifdef _MSC_VER
    DWORD current_path_attr = ::GetFileAttributesA(path.c_str());
    if (current_path_attr & _A_SUBDIR)
    {
        const auto entries = file_system::walk(path, file_system::all);
        for (const auto entry : entries)
        {
            if (!file_system::remove(entry))
            {
                return false;
            }
        }
        return ::RemoveDirectoryA(path.c_str());
    }
    else
    {
        return ::DeleteFileA(path.c_str());
    }
#else
    {
        const auto entries = file_system::walk(path, file_system::directory);
        for (const auto entry : entries)
        {
            if (!file_system::remove(entry))
            {
                return false;
            }
        }
    }
    {
        const auto entries = file_system::walk(path, file_system::file);
        for (const auto entry : entries)
        {
            if (0 != ::remove(entry.c_str()))
            {
                return false;
            }
        }
    }
    return 0 == ::remove(path.c_str());
#endif // _MSC_VER
}

bool ez::base::file_system::rename(const std::string& from, const std::string& to)
{
    if (!file_system::exists(from))
    {
        return false;
    }
    return 0 == ::rename(from.c_str(), to.c_str());
}

ez::base::file_system::paths_t ez::base::file_system::walk(const path_t& path, walk_filter_t filter, int depth)
{
    paths_t paths;
    int     count = _walk(path, filter, depth, paths);
    return paths;
}

ez::base::file_system::bytes_t ez::base::file_system::load(const path_t& path)
{
    FILE* file = ::fopen(path.c_str(), "rb");
    if (nullptr == file)
    {
        return bytes_t();
    }
    SCOPE_PTR_OF(file, ::fclose);

    bytes_t       bytes;
    unsigned char buffer[256] = {0};
    while (true)
    {
        size_t read_size = ::fread(buffer, 1, sizeof(buffer), file);
        if (0 == read_size)
        {
            break;
        }
        bytes.append(buffer, read_size);
        if (read_size < sizeof(buffer))
        {
            break;
        }
    }

    return bytes;
}

size_t ez::base::file_system::save(const ez::base::file_system::path_t& path, const void* src, const size_t& src_size)
{
    FILE* file = ::fopen(path.c_str(), "wb");
    if (nullptr == file)
    {
        return 0;
    }
    size_t output_size = ::fwrite(src, 1, src_size, file);
    ::fclose(file);
    return output_size;
}

size_t ez::base::file_system::copy(const path_t& from, const path_t& to)
{
    if (from.empty() || to.empty())
    {
        return 0;
    }
    if (!exists(from))
    {
        return 0;
    }

    FILE* from_file = ::fopen(from.c_str(), "rb");
    if (nullptr == from_file)
    {
        return 0;
    }
    SCOPE_PTR_OF(from_file, ::fclose);

    FILE* to_file = ::fopen(to.c_str(), "wb");
    if (nullptr == to_file)
    {
        return 0;
    }
    SCOPE_PTR_OF(to_file, ::fclose);

    size_t        wrote_size  = 0;
    unsigned char buffer[256] = {0};
    while (true)
    {
        size_t read_size = ::fread(buffer, 1, sizeof(buffer), from_file);
        if (read_size == 0)
        {
            break;
        }
        wrote_size += ::fwrite(buffer, 1, read_size, to_file);
        if (read_size < sizeof(buffer))
        {
            break;
        }
    }

    return wrote_size;
}
