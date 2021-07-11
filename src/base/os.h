#ifndef _BASE_OS_H_
#define _BASE_OS_H_
#include <string>
namespace ez
{
    namespace base
    {
        namespace os
        {

            int         get_last_error();
            std::string get_last_error_message();
            std::string get_error_message(int errcode);

        } // namespace os

    } // namespace base

} // namespace ez
#endif // !_BASE_OS_H_