#ifndef _NET_NONMOVEABLE_H_
#define _NET_NONMOVEABLE_H_
namespace ez
{
    namespace net
    {
        class nonmoveable
        {
            nonmoveable(noncopyable&&) = delete;
            noncopyable& operator=(noncopyable&&) = delete;

        public:
            nonmoveable()  = default;
            ~nonmoveable() = default;
        };
    } // namespace net
} // namespace ez
#endif // !_NET_NONMOVEABLE_H_
