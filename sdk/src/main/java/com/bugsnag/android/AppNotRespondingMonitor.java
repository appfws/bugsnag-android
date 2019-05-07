package com.bugsnag.android;

import android.support.annotation.NonNull;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

final class AppNotRespondingMonitor {

    private static final int CHECK_INTERVAL_MS = 100;

    interface Delegate {

        /**
         * Invoked when an ANR occurs
         *
         * @param thread the thread being monitored
         */
        void onAppNotResponding(Thread mainThread);
    }

    private final ByteBuffer sentinelBuffer;
    private final Handler threadHandler;
    private final Delegate delegate;

    private final Runnable checker = new Runnable() {
        @Override
        public void run() {
            try {
                char indicator = sentinelBuffer.getChar(0);
                if (indicator == 'a') {
                    delegate.onAppNotResponding(Looper.getMainLooper().getThread());
                    sentinelBuffer.putChar(0, '\0');
                }
            } catch (IndexOutOfBoundsException ex) {
                // Buffer is empty
            } finally {
                threadHandler.postDelayed(this, CHECK_INTERVAL_MS);
            }
        }
    };

    AppNotRespondingMonitor(Delegate delegate) {
        sentinelBuffer = ByteBuffer.allocateDirect(8).order(ByteOrder.nativeOrder());
        HandlerThread watchdogHandlerThread = new HandlerThread("bugsnag-anr-watchdog");
        watchdogHandlerThread.start();
        threadHandler = new Handler(watchdogHandlerThread.getLooper());
        this.delegate = delegate;
    }

    void start() {
        threadHandler.postDelayed(checker, CHECK_INTERVAL_MS);
    }

    @NonNull
    ByteBuffer getSentinelBuffer() {
        return sentinelBuffer;
    }
}
