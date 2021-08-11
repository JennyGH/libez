#include "pch.h"
#include "argument_parser.h"

static inline void _parse_arg(const std::string& arg, ez::base::_argument_map& args)
{
    std::size_t pos = arg.find_first_of('=');
    if (std::string::npos == pos)
    {
        args[arg.substr(1)] = "true";
    }
    else
    {
        args[arg.substr(1, pos - 1)] = arg.substr(pos + 1);
    }
}

ez::base::argument_parser::argument_parser(int argc, char** argv)
    : _module_name(argv[0])
{
    if (argc <= 1)
    {
        return;
    }
    for (int index = 1; index < argc; index++)
    {
        std::string str(argv[index]);
        _parse_arg(str, _args);
    }
}

ez::base::argument_parser::argument_parser(const char* cmd_line)
{
    if (nullptr == cmd_line)
    {
        return;
    }
    std::string cmd_string(cmd_line);
    std::size_t pos      = 0;
    std::size_t next_pos = 0;
    do
    {
        pos      = cmd_string.find_first_of("-", pos);
        next_pos = cmd_string.find(" -", pos);
        if (std::string::npos != pos)
        {
            auto key_value = cmd_string.substr(pos, next_pos);
            _parse_arg(key_value, _args);
            pos = next_pos;
        }
    } while (pos != std::string::npos);
}

ez::base::argument_parser::~argument_parser() {}

std::string ez::base::argument_parser::get_module_name() const
{
    return _module_name;
}

ez::base::_argument_map::size_type ez::base::argument_parser::count() const
{
    return _args.size();
}

ez::base::argument_not_found_exception::argument_not_found_exception(
    const std::string& module_name,
    const std::string& arg_name) noexcept
    : std::runtime_error("The argument `" + arg_name + "` is required.")
    , _module_name(module_name)
{
}

ez::base::argument_not_found_exception::~argument_not_found_exception() noexcept {}

const char* ez::base::argument_not_found_exception::get_module_name() const
{
    return _module_name.c_str();
}
