#ifndef _BASE_PLATFORM_DEFS_H_
#define _BASE_PLATFORM_DEFS_H_
#if defined(_MSC_VER) && (_MSC_VER > 0)
#    define __WINDOWS__ 1
#    define __LINUX__   0
#else
#    define __WINDOWS__ 0
#    define __LINUX__   1
#endif // defined(_MSC_VER) && (_MSC_VER > 0)
#endif // !_BASE_PLATFORM_DEFS_H_