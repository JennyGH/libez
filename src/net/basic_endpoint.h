#ifndef _NET_BASIC_ENDPOINT_H_
#define _NET_BASIC_ENDPOINT_H_
#include <memory>
namespace ez
{
    namespace net
    {
        class basic_endpoint
        {
        protected:
            basic_endpoint(const void* sockaddr, const size_t& size);

        public:
            basic_endpoint(basic_endpoint&& that) noexcept;
            basic_endpoint(const basic_endpoint& that);
            virtual ~basic_endpoint();

            void*         get_addr() const;
            const size_t& get_size() const;

        private:
            std::shared_ptr<void> _addr;
            size_t                _size;
        };
    } // namespace net
} // namespace ez
#endif // !_NET_BASIC_ENDPOINT_H_