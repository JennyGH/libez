#include "pch.h"
#include "argument_parser.h"

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
        std::size_t pos = str.find_first_of('=');
        if (std::string::npos == pos)
        {
            _args[str.substr(1)] = "true";
        }
        else
        {
            _args[str.substr(1, pos - 1)] = str.substr(pos + 1);
        }
    }
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
