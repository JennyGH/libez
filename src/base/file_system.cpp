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
#    include <Windows.h>
#endif // !_MSC_VER

static int _walk(const ez::base::path& path, ez::base::file_system::walk_filter_t filter, int depth, ez::base::file_system::paths_t& paths)
{
#ifndef _MSC_VER
    DIR* dir_ptr = opendir(path);
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

        paths.push_back(path.join(dir_info->d_name));
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
            CONSOLE(
                "[ERROR] `lstat` return: %d, because: %s(%d).",
                rv,
                ez::base::os::get_last_error_message().c_str(),
                ez::base::os::get_last_error());
        }
    }
#else
    WIN32_FIND_DATAA data;
    HANDLE           handle = ::FindFirstFileA(path.join("*"), &data);
    if (handle == NULL)
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
            ((filter & ez::base::file_system::walk_filter_t::directory) != 0 && (data.dwFileAttributes & _A_SUBDIR) != 0))
        {
            paths.push_back(current_path);
        }
        if (depth > 0 && data.dwFileAttributes & _A_SUBDIR)
        {
            _walk(current_path, filter, depth - 1, paths);
        }
    } while (::FindNextFileA(handle, &data));
#endif
    ::FindClose(handle);
    return paths.size();
}

bool ez::base::file_system::exists(const std::string& path)
{
    return ::access(path.c_str(), 0) == 0;
}

bool ez::base::file_system::mkdir(const std::string& path)
{
    const std::string native_path = ez::base::path(path);
#ifdef _MSC_VER
    return ::system(std::string("md ").append(native_path).c_str()) == 0;
#else
    return ::system(std::string("mkdir -p ").append(native_path).c_str()) == 0;
#endif // _MSC_VER
}

bool ez::base::file_system::rmdir(const std::string& path)
{
    const std::string native_path = ez::base::path(path);
#ifdef _MSC_VER
    return ::system(std::string("rd /s /q ").append(native_path).c_str()) == 0;
#else
    return ::system(std::string("rm -rf ").append(native_path).c_str()) == 0;
#endif // _MSC_VER
}

ez::base::file_system::paths_t ez::base::file_system::walk(const path_t& path, walk_filter_t filter, int depth)
{
    paths_t paths;
    int     count = _walk(path, filter, depth, paths);
    return paths;
}

ez::base::file_system::bytes_t ez::base::file_system::load(const path_t& path)
{
    std::basic_ifstream<bytes_t::value_type> file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        return bytes_t();
    }
    bytes_t bytes((std::istreambuf_iterator<bytes_t::value_type>(file)), std::istreambuf_iterator<bytes_t::value_type>());
    return bytes;
}

size_t ez::base::file_system::save(const path_t& path, const bytes_t& bytes)
{
    return save(path, bytes.data(), bytes.length());
}

size_t ez::base::file_system::save(const ez::base::file_system::path_t& path, const void* src, const size_t& src_size)
{
    std::basic_ofstream<bytes_t::value_type> file(path.c_str(), std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        return 0;
    }
    file.write((const bytes_t::value_type*)src, src_size);
    file.close();
    return src_size;
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
    const bytes_t data = load(from);
    if (data.empty())
    {
        return 0;
    }
    return save(to, data);
}
