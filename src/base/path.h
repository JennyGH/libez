#ifndef _BASE_PATH_H_
#define _BASE_PATH_H_
#ifdef __cplusplus
#    include <string>
namespace ez
{
    namespace base
    {
        class path
        {
        public:
            path(const std::string& str);
            path(const path& that);
            ~path();

        public:
            std::string basename() const;
            std::string dirname() const;
            std::string suffix() const;
            path        parent() const;
            path&       join(const std::string& str);
            path        join(const std::string& str) const;
            path&       operator+=(const std::string& str);
            path        operator+(const std::string& str) const;
                        operator std::string() const;
        };

    } // namespace base

} // namespace ez

#endif // __cplusplus
#endif // !_BASE_PATH_H_