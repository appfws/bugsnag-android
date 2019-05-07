#include "anr_handler.h"
#include <pthread.h>
#include <signal.h>
#include <string.h>

#include "../utils/crash_info.h"
#include "../utils/serializer.h"
#include "../utils/string.h"

static pthread_t bsg_watchdog_thread;
static sigset_t bsg_anr_sigmask;
static char *bsg_anr_indicator_buffer = NULL;

void bsg_handle_sigquit(int signum, siginfo_t *info, void *user_context) {
  if (bsg_anr_indicator_buffer != NULL) {
    bsg_strncpy(bsg_anr_indicator_buffer, "a", 2);
  }
}

void *bsg_monitor_anrs(void *_arg) {
  struct sigaction *handler = calloc(sizeof(struct sigaction), 1);
  sigemptyset(&handler->sa_mask);
  handler->sa_sigaction = bsg_handle_sigquit;
  handler->sa_flags = SA_SIGINFO | SA_ONSTACK;
  sigaction(SIGQUIT, handler, NULL);

  return NULL;
}

bool bsg_handler_install_anr(void *byte_buffer) {
  bsg_anr_indicator_buffer = byte_buffer;
  // TODO: pthread_lock
  // TODO: check if thread already set

  sigemptyset(&bsg_anr_sigmask);
  sigaddset(&bsg_anr_sigmask, SIGQUIT);

  int mask_status = pthread_sigmask(SIG_SETMASK, &bsg_anr_sigmask, NULL);
  if (mask_status != 0) {
    BUGSNAG_LOG("Failed to mask SIGQUIT: %s", strerror(mask_status));
  } else {
    pthread_create(&bsg_watchdog_thread, NULL, bsg_monitor_anrs, NULL);
    // unblock the current thread
    pthread_sigmask(SIG_UNBLOCK, &bsg_anr_sigmask, NULL);
  }

  return true;
}

void bsg_handler_uninstall_anr() {
  // TODO: unset global state
  // TODO: kill sentinel thread
}
