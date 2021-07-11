#ifndef _NET_NONCOPYABLE_H_
#define _NET_NONCOPYABLE_H_
namespace ez
{
    namespace net
    {
        class noncopyable
        {
            noncopyable(const noncopyable&) = delete;
            noncopyable& operator=(const noncopyable&) = delete;

        public:
            noncopyable()  = default;
            ~noncopyable() = default;

        private:
        };
    } // namespace net
} // namespace ez
#endif // !_NET_NONCOPYABLE_H_
