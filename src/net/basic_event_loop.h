#ifndef _NET_BASIC_EVENT_LOOP_H_
#define _NET_BASIC_EVENT_LOOP_H_
namespace ez
{
    namespace net
    {
        class basic_event_loop
        {
        public:
            basic_event_loop()          = default;
            virtual ~basic_event_loop() = default;

            virtual void start_loop() = 0;
        };

    } // namespace net

} // namespace ez

#endif // !_NET_BASIC_EVENT_LOOP_H_