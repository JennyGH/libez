#if defined(ANDROID) && ANDROID
#    include "pch.h"
#    include "jni_environment.h"

std::shared_ptr<java::environment> java::environment::initialize(JavaVM* jvm, int jvmVersion)
{
    std::shared_ptr<java::environment> ret = std::make_shared<java::environment>();

    JNIEnv* env = nullptr;
    if (nullptr != jvm)
    {
        jvm->GetEnv((void**)&env, jvmVersion);
        jvm->AttachCurrentThread(&env, nullptr);
    }
    ret->_jvm.reset(jvm, [](JavaVM* ptr) { /* Do nothing. */ });
    ret->_env.reset(env, [jvm](JNIEnv* ptr) {
        jvm->DetachCurrentThread();
    });
    return ret;
}

java::environment::environment(JavaVM* jvm)
    : _jvm(nullptr)
    , _env(nullptr)
{
    JNIEnv* env = nullptr;
    jvm->AttachCurrentThreadAsDaemon(&env, nullptr);
    _env.reset(env, [](JNIEnv* env) {});
}

java::environment::environment()
    : _jvm(nullptr)
    , _env(nullptr)
{
    // if (nullptr != g_jvm)
    //{
    //	g_jvm->GetEnv((void**)&_env, JNI_VERSION_1_8);
    //	g_jvm->AttachCurrentThread((void**)&_env, nullptr);
    //}
}

java::environment::~environment()
{
    // if (nullptr != g_jvm)
    //{
    //	g_jvm->DetachCurrentThread();
    //}
}

java::environment::operator JNIEnv*()
{
    return _env.get();
}

java::jni_env_ptr_t java::environment::operator->()
{
    return _env;
}

bool java::environment::is_valid() const
{
    return _env != nullptr;
}
#endif // defined(ANDROID) && ANDROID