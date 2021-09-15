#pragma once
#if defined(ANDROID) && ANDROID
#    include <jni.h>
#    include <string>
#    include <vector>
namespace java
{
    using java_vm_ptr_t           = std::shared_ptr<JavaVM>;
    using jni_env_ptr_t           = std::shared_ptr<JNIEnv>;
    using java_vm_option_ptr_t    = std::shared_ptr<JavaVMOption>;
    using java_vm_init_args_ptr_t = std::shared_ptr<JavaVMInitArgs>;
    class environment
    {
    public:
        environment();
        environment(JavaVM* jvm);
        ~environment();
        static std::shared_ptr<environment> initialize(JavaVM* jvm, int jvm_version = JNI_VERSION_1_4);

    public:
        jni_env_ptr_t operator->();

        bool is_valid() const;

        operator JNIEnv*();

    private:
        java_vm_ptr_t _jvm;
        jni_env_ptr_t _env;
    };
} // namespace java
#endif // defined(ANDROID) && ANDROID