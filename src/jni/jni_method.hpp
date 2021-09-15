#pragma once
#if defined(ANDROID) && ANDROID
#    include <jni.h>
#    include <string>
#    include <memory>

#    define DECLARE_JAVA_CLASS_REFLECTION(type, typeName)                                                                                            \
        namespace java                                                                                                                               \
        {                                                                                                                                            \
            template <>                                                                                                                              \
            struct type_reflection_st<type>                                                                                                          \
            {                                                                                                                                        \
                static const char* get_type_name()                                                                                                   \
                {                                                                                                                                    \
                    return "L" typeName ";";                                                                                                         \
                }                                                                                                                                    \
            };                                                                                                                                       \
        }

namespace java
{
    template <typename T>
    struct type_reflection_st
    {
        static const char* get_type_name()
        {
            return "Ljava/lang/Object;";
        }
    }; // Default java type reflection.
#    define SPECIALIZED_TYPE_TEMPLATE(jtype, typeName)                                                                                               \
        template <>                                                                                                                                  \
        struct type_reflection_st<jtype>                                                                                                             \
        {                                                                                                                                            \
            static const char* get_type_name()                                                                                                       \
            {                                                                                                                                        \
                return typeName;                                                                                                                     \
            }                                                                                                                                        \
        }

#    define SPECIALIZED_ARRAY_TEMPLATE(jtype, typeName)                                                                                              \
        SPECIALIZED_TYPE_TEMPLATE(jtype, typeName);                                                                                                  \
        template <>                                                                                                                                  \
        struct type_reflection_st<jtype##Array>                                                                                                      \
        {                                                                                                                                            \
            static const char* get_type_name()                                                                                                       \
            {                                                                                                                                        \
                return "[" typeName;                                                                                                                 \
            }                                                                                                                                        \
        }

    /* ************************ Specialize templates ************************ */
    SPECIALIZED_TYPE_TEMPLATE(void, /*******/ "V");
    SPECIALIZED_TYPE_TEMPLATE(jstring, /****/ "Ljava/lang/String;");
    SPECIALIZED_ARRAY_TEMPLATE(jint, /******/ "I");
    SPECIALIZED_ARRAY_TEMPLATE(jlong, /*****/ "J");
    SPECIALIZED_ARRAY_TEMPLATE(jboolean, /**/ "Z");
    SPECIALIZED_ARRAY_TEMPLATE(jbyte, /*****/ "B");
    SPECIALIZED_ARRAY_TEMPLATE(jchar, /*****/ "C");
    SPECIALIZED_ARRAY_TEMPLATE(jshort, /****/ "S");
    SPECIALIZED_ARRAY_TEMPLATE(jfloat, /*****/ "F");
    SPECIALIZED_ARRAY_TEMPLATE(jdouble, /***/ "D");
    /* *********************************************************************** */

#    undef SPECIALIZED_ARRAY_TEMPLATE
#    undef SPECIALIZED_TYPE_TEMPLATE
} // namespace java

namespace java
{
    class basic_method
    {
    public:
        basic_method(JNIEnv* env)
            : _env(env)
        {
        }

        ~basic_method() = default;

        virtual bool is_valid() const = 0;

    protected:
        JNIEnv* _env;
    };
} // namespace java

static inline std::string _unpack(int count, const char* begin, ...)
{
    std::string res;
    va_list     arglist;
    va_start(arglist, begin);
    if (nullptr != begin)
    {
        res.append(begin);
    }
    for (int index = 0; index < count - 1; index++)
    {
        auto val = va_arg(arglist, decltype(begin));
        if (nullptr != val)
        {
            res.append(val);
        }
    }
    va_end(arglist);
    return res;
}

static inline std::string _unpack(int count)
{
    return "";
}

namespace java
{
    template <class invoker_type, typename caller_type, typename return_type, typename... arg_types>
    class method : public basic_method
    {
    public:
        using invoker_t = invoker_type;

    public:
        static const std::string signature;

    public:
        method(const std::string& className, const std::string& methodName, const std::string& sig)
            : _class_name(className)
            , _method_name(methodName)
            , _signature(sig)
            , _object(nullptr)
        {
        }

        method(const std::string& className, const std::string& methodName, const std::string& sig, JNIEnv* env)
            : basic_method(env)
            , _class_name(className)
            , _method_name(methodName)
            , _signature(sig)
            , _object(nullptr)
        {
        }

        method(const std::string& className, const std::string& methodName, const std::string& sig, jobject object)
            : basic_method()
            , _class_name(className)
            , _method_name(methodName)
            , _signature(sig)
            , _object(object)
        {
        }

        method(const std::string& className, const std::string& methodName, const std::string& sig, jobject object, JNIEnv* env)
            : basic_method(env)
            , _class_name(className)
            , _method_name(methodName)
            , _signature(sig)
            , _object(object)
        {
        }

        ~method() = default;

        bool is_valid() const
        {
            return !_class_name.empty() && !_method_name.empty() && !_signature.empty() && nullptr != _env;
        }

        return_type operator()(arg_types... args)
        {
            return this->invoke(args...);
        }

        return_type invoke(arg_types... args)
        {
            return invoker_type::invoke(_env, _object, _class_name, _method_name, _signature, args...);
        }

    protected:
        std::string _class_name;
        std::string _method_name;
        std::string _signature;
        jobject     _object;
    };
} // namespace java

template <class invoker_type, typename caller_type, typename return_type, typename... arg_types>
const std::string java::method<invoker_type, caller_type, return_type, arg_types...>::signature =
    ("(" + _unpack(sizeof...(arg_types), java::type_reflection_st<arg_types>::get_type_name()...) + ")" +
     java::type_reflection_st<return_type>::get_type_name());

namespace java
{
#    define DECLARE_INVOKE_FUNCTION(returnType, functionName)                                                                                        \
        static returnType                                                                                                                            \
        invoke(JNIEnv* env, jobject object, const std::string& className, const std::string& methodName, const std::string& sig, arg_types... args)  \
        {                                                                                                                                            \
            if (nullptr == env)                                                                                                                      \
            {                                                                                                                                        \
                throw std::runtime_error("JNI enviroment is not ready");                                                                             \
            }                                                                                                                                        \
            auto clazz = env->FindClass(className.c_str());                                                                                          \
            if (nullptr == clazz)                                                                                                                    \
            {                                                                                                                                        \
                if (env->ExceptionCheck())                                                                                                           \
                {                                                                                                                                    \
                    env->ExceptionDescribe();                                                                                                        \
                    env->ExceptionClear();                                                                                                           \
                }                                                                                                                                    \
                throw std::runtime_error(("Can not find class: " + className).c_str());                                                              \
            }                                                                                                                                        \
            auto methodId = env->GetStaticMethodID(clazz, methodName.c_str(), sig.c_str());                                                          \
            if (nullptr == methodId)                                                                                                                 \
            {                                                                                                                                        \
                if (env->ExceptionCheck())                                                                                                           \
                {                                                                                                                                    \
                    env->ExceptionDescribe();                                                                                                        \
                    env->ExceptionClear();                                                                                                           \
                }                                                                                                                                    \
                throw std::runtime_error(                                                                                                            \
                    ("Can not find static method \"" + methodName + "\" in class: \"" + className + "\", sig: " + sig).c_str());                     \
            }                                                                                                                                        \
            return (returnType)env->functionName(clazz, methodId, args...);                                                                          \
        }

    template <typename return_type, typename... arg_types>
    struct static_method_invoker_st
    {
        DECLARE_INVOKE_FUNCTION(return_type, CallStaticObjectMethod)
    };

#    define DECLARE_INVOKER(returnType, functionName)                                                                                                \
        template <typename... arg_types>                                                                                                             \
        struct static_method_invoker_st<returnType, arg_types...>                                                                                    \
        {                                                                                                                                            \
            DECLARE_INVOKE_FUNCTION(returnType, functionName)                                                                                        \
        }

    DECLARE_INVOKER(void, CallStaticVoidMethod);
    DECLARE_INVOKER(jint, CallStaticIntMethod);
    DECLARE_INVOKER(jbyte, CallStaticByteMethod);
    DECLARE_INVOKER(jchar, CallStaticCharMethod);
    DECLARE_INVOKER(jlong, CallStaticLongMethod);
    DECLARE_INVOKER(jfloat, CallStaticFloatMethod);
    DECLARE_INVOKER(jshort, CallStaticShortMethod);
    DECLARE_INVOKER(jdouble, CallStaticDoubleMethod);
    DECLARE_INVOKER(jobject, CallStaticObjectMethod);
    DECLARE_INVOKER(jboolean, CallStaticBooleanMethod);

#    undef DECLARE_INVOKER
#    undef DECLARE_INVOKE_FUNCTION

    template <typename return_type, typename... arg_types>
    class static_method : public method<static_method_invoker_st<return_type, arg_types...>, jclass, return_type, arg_types...>
    {
        using base_type_t = method<static_method_invoker_st<return_type, arg_types...>, jclass, return_type, arg_types...>;

    public:
        static_method(const std::string& className, const std::string& methodName)
            : base_type_t(className, methodName, base_type_t::signature)
        {
        }
        static_method(const std::string& className, const std::string& methodName, JNIEnv* env)
            : base_type_t(className, methodName, base_type_t::signature, env)
        {
        }
    };
} // namespace java

namespace java
{
#    define DECLARE_INVOKE_FUNCTION(returnType, functionName)                                                                                        \
        static returnType                                                                                                                            \
        invoke(JNIEnv* env, jobject object, const std::string& className, const std::string& methodName, const std::string& sig, arg_types... args)  \
        {                                                                                                                                            \
            if (nullptr == env)                                                                                                                      \
            {                                                                                                                                        \
                throw std::runtime_error("JNI enviroment is not ready");                                                                             \
            }                                                                                                                                        \
            if (nullptr == object)                                                                                                                   \
            {                                                                                                                                        \
                if (env->ExceptionCheck())                                                                                                           \
                {                                                                                                                                    \
                    env->ExceptionDescribe();                                                                                                        \
                    env->ExceptionClear();                                                                                                           \
                }                                                                                                                                    \
                throw std::runtime_error("The caller of java member function is null.");                                                             \
            }                                                                                                                                        \
            auto clazz = env->FindClass(className.c_str());                                                                                          \
            if (nullptr == clazz)                                                                                                                    \
            {                                                                                                                                        \
                if (env->ExceptionCheck())                                                                                                           \
                {                                                                                                                                    \
                    env->ExceptionDescribe();                                                                                                        \
                    env->ExceptionClear();                                                                                                           \
                }                                                                                                                                    \
                throw std::runtime_error(("Can not find class: " + className).c_str());                                                              \
            }                                                                                                                                        \
            auto methodId = env->GetMethodID(clazz, methodName.c_str(), sig.c_str());                                                                \
            if (nullptr == methodId)                                                                                                                 \
            {                                                                                                                                        \
                if (env->ExceptionCheck())                                                                                                           \
                {                                                                                                                                    \
                    env->ExceptionDescribe();                                                                                                        \
                    env->ExceptionClear();                                                                                                           \
                }                                                                                                                                    \
                throw std::runtime_error(("Can not find member method \"" + methodName + "\" in class \"" + className + "\", sig: " + sig).c_str()); \
            }                                                                                                                                        \
            return (returnType)env->functionName(object, methodId, args...);                                                                         \
        }

    template <typename return_type, typename... arg_types>
    struct member_method_invoker_st
    {
        DECLARE_INVOKE_FUNCTION(return_type, CallObjectMethod)
    };

#    define DECLARE_INVOKER(returnType, functionName)                                                                                                \
        template <typename... arg_types>                                                                                                             \
        struct member_method_invoker_st<returnType, arg_types...>                                                                                    \
        {                                                                                                                                            \
            DECLARE_INVOKE_FUNCTION(returnType, functionName)                                                                                        \
        }

    DECLARE_INVOKER(void, CallVoidMethod);
    DECLARE_INVOKER(jint, CallIntMethod);
    DECLARE_INVOKER(jbyte, CallByteMethod);
    DECLARE_INVOKER(jchar, CallCharMethod);
    DECLARE_INVOKER(jlong, CallLongMethod);
    DECLARE_INVOKER(jfloat, CallFloatMethod);
    DECLARE_INVOKER(jshort, CallShortMethod);
    DECLARE_INVOKER(jdouble, CallDoubleMethod);
    DECLARE_INVOKER(jobject, CallObjectMethod);
    DECLARE_INVOKER(jboolean, CallBooleanMethod);
    DECLARE_INVOKER(jbyteArray, CallObjectMethod);

#    undef DECLARE_INVOKER

    template <typename return_type, typename... arg_types>
    class member_method : public method<member_method_invoker_st<return_type, arg_types...>, jobject, return_type, arg_types...>
    {
        using base_type_t = method<member_method_invoker_st<return_type, arg_types...>, jobject, return_type, arg_types...>;

    public:
        member_method(jobject object, const std::string& className, const std::string& methodName)
            : method<member_method_invoker_st<return_type, arg_types...>, jobject, return_type, arg_types...>(
                  className,
                  methodName,
                  base_type_t::signature,
                  object)
        {
        }
        member_method(jobject object, const std::string& className, const std::string& methodName, JNIEnv* env)
            : method<member_method_invoker_st<return_type, arg_types...>, jobject, return_type, arg_types...>(
                  className,
                  methodName,
                  base_type_t::signature,
                  object,
                  env)
        {
        }
    };
} // namespace java
#endif // defined(ANDROID) && ANDROID