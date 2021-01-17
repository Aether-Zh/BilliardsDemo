package com.k7k7.sdk.x5.wendu;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;
import android.webkit.JavascriptInterface;

import com.k7k7.sdk.x5.X5WebViewUtil;
import com.k7k7.sdk.x5.wendu.dsbridge.CompletionHandler;

/**
 * Created by du on 16/12/31.
 */

public class JsApi{
	
    @JavascriptInterface
    public void callCocos(Object msg, CompletionHandler<String> handler) throws JSONException {
        Log.d("JsApi", "callCocos : " + msg);
        JSONObject jsonObject= (JSONObject) msg;
        X5WebViewUtil.callCocos(jsonObject.getString("sKey"), jsonObject.getString("sJson"), handler);
    }
}