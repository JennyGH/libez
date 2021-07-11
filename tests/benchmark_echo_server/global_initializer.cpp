#include "pch.h"
#include "global_initializer.h"

global_initializer::global_initializer(
    initialize_callback_t   init_callback,
    deinitialize_callback_t deinit_callback)
{
    if (nullptr != init_callback)
    {
        init_callback();
    }
}

global_initializer::~global_initializer()
{
    if (nullptr != _deinit)
    {
        _deinit();
    }
}
