#ifndef BUGSNAG_ANR_HANDLER_H
#define BUGSNAG_ANR_HANDLER_H
/**
 * The Application Not Responding (ANR) handler captures SIGQUIT being raised,
 * interpreting it as the operating system sending an indication that the app
 * has not responded to user input for some time.
 *
 * References:
 * * https://android.googlesource.com/platform/dalvik2/+/refs/heads/master/vm/SignalCatcher.cpp
 */

#include "bugsnag_ndk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Monitor for ANRs, writing to a byte buffer when detected
 * @param byte_buffer a pre-allocated reference to a direct byte buffer
 */
bool bsg_handler_install_anr(void *byte_buffer);

void bsg_handler_uninstall_anr(void);

#ifdef __cplusplus
extern "C" {
#endif

#endif
