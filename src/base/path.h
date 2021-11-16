#ifndef _BASE_PATH_H_
#define _BASE_PATH_H_
#ifdef __cplusplus
#    include <list>
#    include <string>
#    ifdef _MSC_VER
#        define DEFAULT_PATH_STYLE windows_style
#    else
#        define DEFAULT_PATH_STYLE unix_style
#    endif // _MSC_VER
namespace ez
{
    namespace base
    {
        class path
        {
        public:
            enum style_t
            {
                unix_style    = 0,
                windows_style = 1
            };

        private:
            typedef std::list<std::string> items_t;

            path(const items_t::const_iterator& begin, const items_t::const_iterator& end);

        public:
            path();
            path(const std::string& str);
            path(const path& that);
            ~path();

        public:
            std::string root_name() const;
            std::string base_name() const;
            std::string dir_name() const;
            std::string suffix() const;
            path        parent() const;
            path        join(const std::string& sub_path) const;
            std::string to_string(const style_t& type = DEFAULT_PATH_STYLE) const;
            bool        operator==(const path& path) const;
                        operator std::string() const;
            bool        has_parent() const;

        private:
            items_t _items;
        };

    } // namespace base

} // namespace ez

#endif // __cplusplus
#endif // !_BASE_PATH_H_