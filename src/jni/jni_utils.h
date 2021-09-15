#pragma once
#if defined(ANDROID) && ANDROID
#    include <jni.h>
#    include <cstdint>
#    include <string>
#    include <cstring>

namespace jni_utils
{
    typedef std::basic_string<int32_t> int_array_t;

    jbyteArray  to_java_bytes(JNIEnv* env, const uint8_t data[], int64_t len);
    std::string from_java_bytes(JNIEnv* env, jbyteArray jbytes, jint offset = 0);
    std::string from_java_string(JNIEnv* env, jstring jstr);
    jstring     to_java_string(JNIEnv* env, const std::string& str);
    int_array_t from_java_int_array(JNIEnv* env, jintArray jarray, jint offset = 0);

} // namespace jni_utils

#endif // defined(ANDROID) && ANDROID