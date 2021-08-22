#ifndef _BASE_LEXICAL_CAST_HPP_
#define _BASE_LEXICAL_CAST_HPP_
#include <mutex>
#include <sstream>
namespace ez
{
    namespace base
    {
        template <typename output_type, typename input_type>
        static output_type lexical_cast(const input_type& input)
        {
            static std::mutex        lexical_cast_mutex;
            static std::stringstream lexical_cast_stream;

            output_type output;
            {
                std::lock_guard<std::mutex> scope_lock(lexical_cast_mutex);
                lexical_cast_stream << input;
                lexical_cast_stream >> output;
                lexical_cast_stream.str("");
                lexical_cast_stream.clear();
            }
            return output;
        }
    } // namespace base
} // namespace ez
#endif // !_BASE_LEXICAL_CAST_HPP_
