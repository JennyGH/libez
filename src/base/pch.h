#ifndef _BASE_PCH_H_
#define _BASE_PCH_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <memory>
#include "platform_defs.h"
#include "platform_compatibility.h"
#if !__WINDOWS__
#    include <unistd.h>
#    include <dirent.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#endif // !__WINDOWS__
#define SCOPE_PTR_OF(obj, ...) std::shared_ptr<typename std::remove_pointer<decltype(obj)>::type> _scope_##obj##_ptr_(obj, ##__VA_ARGS__)
#endif // !_BASE_PCH_H_