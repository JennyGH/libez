#pragma once
#ifdef _MSC_VER
#    include <Windows.h>
template <typename T>
class simple_thread_local
{
public:
    simple_thread_local()
        : _tls(::TlsAlloc())
    {
    }

    simple_thread_local(const T& value)
        : _tls(::TlsAlloc())
    {
        this->set_value(value);
    }

    ~simple_thread_local()
    {
        if (TLS_OUT_OF_INDEXES != _tls)
        {
            ::TlsFree(_tls);
            _tls = TLS_OUT_OF_INDEXES;
        }
    }

    void set_value(const T& value)
    {
        if (TLS_OUT_OF_INDEXES != _tls)
        {
            T* old_value = static_cast<T*>(::TlsGetValue(_tls));
            if (nullptr != old_value)
            {
                delete old_value;
                old_value = nullptr;
            }
            ::TlsSetValue(_tls, new T(value));
        }
    }

    T get_value() const
    {
        T* value = static_cast<T*>(::TlsGetValue(_tls));
        if (nullptr == value)
        {
            return T();
        }
        return *value;
    }

    simple_thread_local& operator=(const T& value)
    {
        this->set_value(value);
        return *this;
    }

    operator T() const
    {
        return this->get_value();
    }

private:
    DWORD _tls;
};
#endif // _MSC_VER
#define SIMPLE_THREAD_LOCAL(type) simple_thread_local<type>