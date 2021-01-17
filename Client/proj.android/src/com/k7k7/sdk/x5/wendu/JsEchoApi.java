package com.k7k7.sdk.x5.wendu;

import android.webkit.JavascriptInterface;

import org.json.JSONException;

import com.k7k7.sdk.x5.wendu.dsbridge.CompletionHandler;

/**
 * Created by du on 16/12/31.
 */

public class JsEchoApi {

    @JavascriptInterface
    public Object syn(Object args) throws JSONException {
        return  args;
    }

    @JavascriptInterface
    public void asyn(Object args, CompletionHandler<Object> handler) {
        handler.complete(args);
    }
}