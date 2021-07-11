#ifndef _NET_MACROS_H_
#define _NET_MACROS_H_
#define THROW_NET_EXCEPTION(fmt, ...) throw net_exception(ez::base::format(fmt, ##__VA_ARGS__))
#endif // !_NET_MACROS_H_
