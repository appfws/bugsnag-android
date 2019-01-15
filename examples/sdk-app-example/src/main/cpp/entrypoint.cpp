#include <jni.h>

extern "C" {

static void __attribute__((used)) somefakefunc(void) {}

int crash_write_read_only() {
    // Write to a read-only page
    volatile char *ptr = (char *)somefakefunc;
    *ptr = 0;

    return 5;
}

JNIEXPORT void JNICALL Java_com_bugsnag_android_example_ExampleActivity_doCrash(JNIEnv *env, jobject instance) {
    crash_write_read_only();
}

JNIEXPORT void JNICALL Java_com_bugsnag_android_example_ExampleActivity_notifyFromCXX(JNIEnv *env, jobject instance) {
    // Set the current user
}
}
