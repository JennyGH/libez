#ifndef _GLOBAL_INITIALIZER_H_
#define _GLOBAL_INITIALIZER_H_
class global_initializer
{
public:
    using initialize_callback_t   = void (*)();
    using deinitialize_callback_t = void (*)();

public:
    global_initializer(
        initialize_callback_t   init_callback,
        deinitialize_callback_t deinit_callback);

    ~global_initializer();

private:
    deinitialize_callback_t _deinit;
};
#endif // !_GLOBAL_INITIALIZER_H_