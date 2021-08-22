#pragma once
#include <vector>
#include <string>
namespace ez
{
    namespace base
    {
        template <typename character_type = char>
        class basic_string : public std::basic_string<character_type>
        {
        public:
            typedef std::basic_string<character_type> string_t;
            typedef std::vector<basic_string>         splited_t;
            typedef void (*foreach_func)(character_type);
            typedef void (*foreach_func_ref)(character_type&);
            typedef void (*foreach_func_const_ref)(const character_type&);

        public:
            basic_string() {};
            basic_string(const string_t& src)
                : string_t(src) {};
            basic_string(const character_type src[])
                : string_t(src) {};
            basic_string(const character_type src[], typename string_t::size_type len)
                : string_t(src, len) {};
            basic_string(const unsigned char src[], typename string_t::size_type len)
                : string_t(src, src + len) {};
            basic_string(
                const std::istreambuf_iterator<character_type>& begin,
                const std::istreambuf_iterator<character_type>& end)
                : string_t(begin, end) {};
            ~basic_string() {};

            basic_string& remove(character_type target)
            {
                typename string_t::size_type _pos = this->find(target);
                while (_pos != string_t::npos)
                {
                    this->erase(_pos, 1);
                    _pos = this->find(target);
                }
                return *this;
            }

            basic_string& remove(const string_t& target)
            {
                typename string_t::size_type _pos = this->find(target);
                while (_pos != string_t::npos)
                {
                    this->erase(_pos, target.length());
                    _pos = this->find(target);
                }
                return *this;
            }

            basic_string& tolower()
            {
                typename string_t::iterator iter = this->begin();
                typename string_t::iterator end  = this->end();
                for (; iter != end; ++iter)
                {
                    character_type c = *iter;
                    if ((0x41 <= c && c <= 0x5a) || (0x61 <= c && c <= 0x7a))
                    {
                        *iter = ::tolower(*iter);
                    }
                }
                return *this;
            }

            basic_string& toupper()
            {
                typename string_t::iterator iter = this->begin();
                typename string_t::iterator end  = this->end();
                for (; iter != end; ++iter)
                {
                    character_type c = *iter;
                    if ((0x41 <= c && c <= 0x5a) || (0x61 <= c && c <= 0x7a))
                    {
                        *iter = ::toupper(*iter);
                    }
                }
                return *this;
            }

            splited_t split(const character_type& separator) const
            {
                return this->split(string_t(&separator, 1));
            }

            splited_t split(const string_t& separator) const
            {
                splited_t                    output;
                typename string_t::size_type current = 0;
                typename string_t::size_type end     = -1;
                typename string_t::size_type len     = separator.length();
                while (true)
                {
                    end = this->find(separator, current);
                    if (end == string_t::npos)
                    {
                        if (current < this->length())
                        {
                            output.push_back(this->substr(current));
                        }
                        break;
                    }
                    else
                    {
                        if (end - current > 0)
                        {
                            output.push_back(this->substr(current, end - current));
                        }
                        current = end + len;
                    }
                }

                if (this->find_last_of(separator) == this->length() - separator.length())
                {
                    output.push_back("");
                }

                return output;
            }

            basic_string& replace(const basic_string& from, const basic_string& to)
            {
                basic_string&                me        = *this;
                typename string_t::size_type total     = 0;
                splited_t                    split_res = split(from);
                for (typename splited_t::const_iterator iter = split_res.begin(); iter != split_res.end(); ++iter)
                {
                    total += iter->length();
                }
                total += ((split_res.size() - 1) * to.length());
                this->clear();
                this->reserve(total + 1);
                for (typename splited_t::const_iterator iter = split_res.begin(); iter != split_res.end(); ++iter)
                {
                    if (iter != split_res.begin())
                    {
                        me += to;
                    }
                    me += *iter;
                }
                return *this;
            }

            basic_string& foreach (foreach_func func)
            {
                if (func != nullptr)
                {
                    typename string_t::const_iterator iter = this->begin();
                    typename string_t::const_iterator end  = this->end();
                    for (; iter != end; ++iter)
                    {
                        func(*iter);
                    }
                }
                return *this;
            }
            basic_string& foreach (foreach_func_ref func)
            {
                if (func != nullptr)
                {
                    typename string_t::iterator iter = this->begin();
                    typename string_t::iterator end  = this->end();
                    for (; iter != end; ++iter)
                    {
                        func(*iter);
                    }
                }
                return *this;
            }
            basic_string& foreach (foreach_func_const_ref func)
            {
                if (func != nullptr)
                {
                    typename string_t::const_iterator iter = this->begin();
                    typename string_t::const_iterator end  = this->end();
                    for (; iter != end; ++iter)
                    {
                        func(*iter);
                    }
                }
                return *this;
            }
        };
        typedef basic_string<char>    string;
        typedef basic_string<wchar_t> wstring;
    } // namespace base
} // namespace ez
