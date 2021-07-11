#ifndef _NET_HTTP_MULTIPART_FORM_DATA_H_
#define _NET_HTTP_MULTIPART_FORM_DATA_H_
#include <list>
#include <string>
#include <memory>
namespace ez
{
    namespace net
    {
        namespace http
        {
            class multipart_form_data_item;
            class multipart_form_data
            {
                using multipart_form_data_item_ptr_t = std::shared_ptr<multipart_form_data_item>;
                using multipart_form_data_items_t    = std::list<multipart_form_data_item_ptr_t>;

            public:
                multipart_form_data();
                multipart_form_data(const char* src, const size_t& length);
                ~multipart_form_data();

            private:
                multipart_form_data_items_t _items;
            };

        } // namespace http

    } // namespace net

} // namespace ez
#endif // !_NET_HTTP_MULTIPART_FORM_DATA_H_