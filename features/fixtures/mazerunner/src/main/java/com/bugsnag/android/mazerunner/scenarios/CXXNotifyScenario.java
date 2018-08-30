package com.bugsnag.android.mazerunner.scenarios;

import android.content.Context;

import com.bugsnag.android.Configuration;

import org.jetbrains.annotations.NotNull;

public class CXXNotifyScenario extends Scenario {
    static {
        System.loadLibrary("entrypoint");
    }

    public native void activate();

    public CXXNotifyScenario(@NotNull Configuration config, @NotNull Context context) {
        super(config, context);
    }

    @Override
    public void run() {
        super.run();
        activate();
    }
}