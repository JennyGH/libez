#ifndef _NET_HTTP_MULTIPART_FORM_DATA_ITEM_H_
#define _NET_HTTP_MULTIPART_FORM_DATA_ITEM_H_
#include <string>
namespace ez
{
    namespace net
    {
        namespace http
        {
            class multipart_form_data_item
            {
            public:
                multipart_form_data_item();
                multipart_form_data_item(const char* src, const size_t& length);
                ~multipart_form_data_item();
            };

        } // namespace http

    } // namespace net

} // namespace ez
#endif // !_NET_HTTP_MULTIPART_FORM_DATA_ITEM_H_