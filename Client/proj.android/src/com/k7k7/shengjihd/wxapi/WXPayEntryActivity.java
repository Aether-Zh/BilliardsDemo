package com.k7k7.shengjihd.wxapi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.k7k7.jniutil.PlatformFunction;
import com.k7k7.sdk.wechat.WeChatSdkUtil;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler {
	
    private IWXAPI api;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
    	api = WXAPIFactory.createWXAPI(this, PlatformFunction.getWXAppId());
        api.handleIntent(getIntent(), this);
    }

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
        api.handleIntent(intent, this);
	}

	@Override
	public void onReq(BaseReq req) {
		WeChatSdkUtil.onReq(req);
		finish();
	}

	@Override
	public void onResp(BaseResp resp) {
		WeChatSdkUtil.onResp(resp);
		finish();
	}
}
