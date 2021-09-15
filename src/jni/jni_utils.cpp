#if defined(ANDROID) && ANDROID
#    include "pch.h"
#    include "jni_utils.h"
jbyteArray jni_utils::to_java_bytes(JNIEnv* env, const uint8_t data[], int64_t len)
{
    if (data == nullptr || len <= 0)
    {
        return nullptr;
    }
    jbyte*     by     = (jbyte*)data;
    jbyteArray jarray = env->NewByteArray(len);
    env->SetByteArrayRegion(jarray, 0, len, by);
    return jarray;
}

std::string jni_utils::from_java_bytes(JNIEnv* env, jbyteArray jbytes, jint offset)
{
    if (nullptr == jbytes)
    {
        return "";
    }

    jint len = env->GetArrayLength(jbytes);
    if (offset > len - 1)
    {
        return "";
    }

    uint8_t* cbytes = (uint8_t*)(env->GetByteArrayElements(jbytes, 0));
    return std::string(cbytes, cbytes + offset + len);
}

std::string jni_utils::from_java_string(JNIEnv* env, jstring jstr)
{
    const char* c_str = env->GetStringUTFChars(jstr, 0);
    if (c_str == nullptr)
    {
        return "";
    }
    return c_str;
}

jstring jni_utils::to_java_string(JNIEnv* env, const std::string& str)
{
    jclass     strClass = (env)->FindClass("java/lang/String");
    jmethodID  ctorID   = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes    = (env)->NewByteArray(str.length());
    (env)->SetByteArrayRegion(bytes, 0, str.length(), (jbyte*)str.data());
    jstring encoding = (env)->NewStringUTF("UTF8");
    return (jstring)(env)->NewObject(strClass, ctorID, bytes, encoding);
}

jni_utils::int_array_t jni_utils::from_java_int_array(JNIEnv* env, jintArray jarray, jint offset)
{
    if (nullptr == env || nullptr == jarray || offset < 0)
    {
        return int_array_t();
    }

    jint len = env->GetArrayLength(jarray);
    if (offset > len - 1)
    {
        return int_array_t();
    }

    int32_t* cIntArray = (int32_t*)(env->GetIntArrayElements(jarray, 0));
    return int_array_t(cIntArray, len);
}
#endif // defined(ANDROID) && ANDROID