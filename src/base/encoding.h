#pragma once
#include <string>
namespace ez
{
    namespace base
    {
        namespace encoding
        {
            std::string utf8_to_ansi(const std::string& utf8);
            std::string ansi_to_utf8(const std::string& ansi);

        } // namespace encoding

    } // namespace base

} // namespace ez
