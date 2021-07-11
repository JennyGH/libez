#ifndef _BASE_ARGUMENT_PARSER_H_
#define _BASE_ARGUMENT_PARSER_H_
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

#ifndef __cpp11
#    ifdef _MSC_VER
#        define __cpp11 (_MSC_VER > 1600 || __cplusplus > 199711L)
#    else
#        define __cpp11                                                                                                \
            (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ > 40805) || (__cplusplus > 199711L)
#    endif // _MSC_VER
#endif     // !__cpp11

#if !__cpp11
#    ifndef noexcept
#        define noexcept throw()
#    endif // !noexcept
#endif     // !__cpp11

namespace ez
{
    namespace base
    {
        typedef std::map<std::string, std::string> _argument_map;

        class argument_not_found_exception : public std::runtime_error
        {
        public:
            argument_not_found_exception(const std::string& module_name, const std::string& arg_name) noexcept;
            ~argument_not_found_exception() noexcept;
            const char* get_module_name() const;

        private:
            std::string _module_name;
        };

        static void _is_argument_exist(
            const std::string&   arg_name,
            const std::string&   module_name,
            const _argument_map& arguments)
        {
            if (arguments.find(arg_name) == arguments.end())
            {
                throw argument_not_found_exception(module_name, arg_name);
            }
        }

        template <typename value_type>
        struct _argument_getter
        {
            static value_type value_of(
                const std::string&   arg_name,
                const std::string&   module_name,
                const _argument_map& arguments)
            {
                _is_argument_exist(arg_name, module_name, arguments);
                std::stringstream ss;
                value_type        val = value_type();
                ss << arguments.find(arg_name)->second;
                ss >> val;
                return val;
            }
            static value_type value_of(
                const std::string&   arg_name,
                const std::string&   module_name,
                const _argument_map& arguments,
                value_type           default_value)
            {
                value_type                    val   = default_value;
                _argument_map::const_iterator found = arguments.find(arg_name);
                if (found != arguments.end())
                {
                    std::stringstream ss;
                    ss << found->second;
                    ss >> val;
                }
                return val;
            }
        };

        template <>
        struct _argument_getter<bool>
        {
            static bool value_of(
                const std::string&   arg_name,
                const std::string&   module_name,
                const _argument_map& arguments)
            {
                _is_argument_exist(arg_name, module_name, arguments);
                bool                          val   = false;
                _argument_map::const_iterator found = arguments.find(arg_name);
                if (found != arguments.end())
                {
                    val = found->second == "true";
                }
                return val;
            }
            static bool value_of(
                const std::string&   arg_name,
                const std::string&   module_name,
                const _argument_map& arguments,
                bool                 default_value)
            {
                bool                          val   = default_value;
                _argument_map::const_iterator found = arguments.find(arg_name);
                if (found != arguments.end())
                {
                    val = found->second == "true";
                }
                return val;
            }
        };

        template <>
        struct _argument_getter<std::string>
        {
            static std::string value_of(
                const std::string&   arg_name,
                const std::string&   module_name,
                const _argument_map& args)
            {
                _is_argument_exist(arg_name, module_name, args);
                _argument_map::const_iterator found = args.find(arg_name);
                return found->second;
            }
            static std::string value_of(
                const std::string&   arg_name,
                const std::string&   module_name,
                const _argument_map& args,
                const std::string&   default_value)
            {
                std::string                   val   = default_value;
                _argument_map::const_iterator found = args.find(arg_name);
                if (found == args.end())
                {
                    return val;
                }
                return found->second;
            }
        };

        class argument_parser
        {
        public:
            argument_parser(int argc, char** argv);

            ~argument_parser();

            template <typename T>
            T get(const std::string& arg_name) const
            {
                return _argument_getter<T>::value_of(arg_name, _module_name, _args);
            }

            template <typename T>
            T get(const std::string& arg_name, T default_value) const
            {
                return _argument_getter<T>::value_of(arg_name, _module_name, _args, default_value);
            }

            std::string get_module_name() const;

            _argument_map::size_type count() const;

        private:
            std::string   _module_name;
            _argument_map _args;
        };
    } // namespace base
} // namespace ez

#endif // !_BASE_ARGUMENT_PARSER_H_