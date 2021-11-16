#include "path.h"
#include "path.h"
#include "path.h"
#include "pch.h"
#include "path.h"

#define UNIX_PATH_SEPARATOR    "/"
#define WINDOWS_PATH_SEPARATOR "\\"
#ifdef _MSC_VER
#    define CURRENT_PATH_SEPARATOR WINDOWS_PATH_SEPARATOR
#    define OTHER_PATH_SEPARATOR   UNIX_PATH_SEPARATOR
#else
#    define CURRENT_PATH_SEPARATOR UNIX_PATH_SEPARATOR
#    define OTHER_PATH_SEPARATOR   WINDOWS_PATH_SEPARATOR
#endif // _MSC_VER

static std::list<std::string> _split(const std::string& path)
{
    std::list<std::string> output;
    const size_t           length  = path.length();
    size_t                 begin   = 0;
    size_t                 current = 0;
    for (size_t i = begin; i < length; i++)
    {
        const char chr = path[i];
        if ((chr == CURRENT_PATH_SEPARATOR[0] || chr == OTHER_PATH_SEPARATOR[0]) && i > begin)
        {
            output.push_back(path.substr(begin, i - begin));
            begin = i + 1;
        }
    }
    if (begin < length - 1)
    {
        output.push_back(path.substr(begin, length - begin));
    }
    return output;
}

ez::base::path::path(const items_t::const_iterator& begin, const items_t::const_iterator& end)
    : _items(begin, end)
{
}

ez::base::path::path() {}

ez::base::path::path(const std::string& str)
    //: _value(_to_native_path_format(str))
    : _items(_split(str))
{
}

ez::base::path::path(const path& that)
    //: _value(that._value)
    : _items(that._items)
{
}

ez::base::path::~path() {}

std::string ez::base::path::root_name() const
{
    if (_items.empty())
    {
        return "";
    }
    return _items.front();
}

std::string ez::base::path::base_name() const
{
    if (_items.empty())
    {
        return "";
    }
    return _items.back();
}

std::string ez::base::path::dir_name() const
{
    return this->parent().base_name();
}

std::string ez::base::path::suffix() const
{
    const std::string basename = this->base_name();

    std::size_t pos = basename.find_last_of('.');
    if (std::string::npos == pos)
    {
        return "";
    }
    return basename.substr(pos + 1);
}

ez::base::path ez::base::path::parent() const
{
    auto iter = this->_items.begin();
    std::advance(iter, this->_items.size() - 1);
    return ez::base::path(this->_items.begin(), iter);
}

ez::base::path ez::base::path::join(const std::string& sub_path) const
{
    ez::base::path cloned = *this;

    const auto splited = _split(sub_path);
    for (const auto& item : splited)
    {
        cloned._items.push_back(item);
    }

    return cloned;
}

std::string ez::base::path::to_string(const style_t& type) const
{
    std::string res;

    for (const auto& item : _items)
    {
        res.append(item).append(type == style_t::unix_style ? UNIX_PATH_SEPARATOR : WINDOWS_PATH_SEPARATOR);
    }

    if (!res.empty())
    {
        res.resize(res.length() - 1);
    }

    return res;
}

bool ez::base::path::operator==(const path& path) const
{
    return this->_items == path._items;
}

ez::base::path::operator std::string() const
{
    return this->to_string();
}

bool ez::base::path::has_parent() const
{
    return _items.size() > 1;
}
