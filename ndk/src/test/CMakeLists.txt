include_directories(
    ../main/jni/
    ../main/jni/deps
    cpp/
    cpp/deps
)
add_library(bugsnag-ndk-test SHARED
               cpp/main.c
               cpp/test_utils_string.c
               cpp/test_utils_serialize.c
               cpp/test_breadcrumbs.c)
target_link_libraries(bugsnag-ndk-test bugsnag-ndk)
