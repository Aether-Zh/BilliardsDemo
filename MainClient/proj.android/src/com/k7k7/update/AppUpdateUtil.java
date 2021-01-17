package com.k7k7.update;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import com.k7k7.jniutil.PlatformFunction;
import com.k7k7.jniutil.SysUtil;

public class AppUpdateUtil {
	
	private static Cocos2dxActivity thisActivity = null;
	
	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
	}
	
	public static void destroy() {
		thisActivity = null;
	}
	
	public static void doCheckUpdate() {
		onCheckUpdateBegan();
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					HashMap<String, String> map = new HashMap<String, String>();
					map.put(AppUpdateConfig.REQ_PARAM_GAMEID, AppUpdateConfig.DUOLE_GAME_ID);
					map.put(AppUpdateConfig.REQ_PARAM_CHANNEL, PlatformFunction.getChannelType());
					map.put(AppUpdateConfig.REQ_PARAM_VERSIONCODE, SysUtil.getInstance().getVersionCode() + "");
					map.put(AppUpdateConfig.REQ_PARAM_PACKAGENAME, SysUtil.getInstance().getPackageName());
					map.put(AppUpdateConfig.REQ_PARAM_APKSIGNATURE, SysUtil.getInstance().getApkSignatureMd5());
					String params = generateHttpRequestParams(map);
					
					// 新建一个URL对象
					URL url = new URL(AppUpdateConfig.CHECK_URL);
					// 打开一个HttpURLConnection连接
					HttpURLConnection conn = (HttpURLConnection) url.openConnection();
					// 设置连接超时时间
					conn.setConnectTimeout(15 * 1000);
					// 设置读取数据超时时间
					conn.setReadTimeout(15 * 1000);
					// POST请求不能使用缓存
					conn.setUseCaches(false);
					// POST请求必须设置允许输出，默认false
					conn.setDoOutput(true);
					// 设置请求允许输入，默认是true
					conn.setDoInput(true);
					// 设置本次连接是否自动处理重定向
					conn.setInstanceFollowRedirects(true);
					// 设置为POST请求
					conn.setRequestMethod("POST");
					// 配置请求Content-Type
					conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
					// 开始连接
					conn.connect();
		            // 发送请求参数
		            DataOutputStream os = new DataOutputStream(conn.getOutputStream());
		            os.write(params.getBytes());
					os.flush();
					os.close();
					if (conn.getResponseCode() == 200) {
						onCheckUpdateResult(true, convertStreamToString(conn.getInputStream()));
					} else {
						onCheckUpdateResult(false, "");
					}
					conn.disconnect();
				} catch (Exception e) {
					e.printStackTrace();
					onCheckUpdateResult(false, "");
				}
			}
		}).start();
	}
	
	private static String generateHttpRequestParams(HashMap<String, String> map) {
		StringBuilder buffer = new StringBuilder();
		try {
			int pos = 0;
			for (String key : map.keySet()) {
				if (pos > 0) {
					buffer.append("&");
				}
				buffer.append(String.format("%s=%s", key, URLEncoder.encode(map.get(key), "UTF-8")));
				pos++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return buffer.toString();
	}
	
	private static String convertStreamToString(InputStream is) {
		try {
			ByteArrayOutputStream os = new ByteArrayOutputStream();
			byte[] buffer = new byte[1024];
			int len = 0;
			while ((len = is.read(buffer)) != -1) {
				os.write(buffer, 0, len);
			}
			os.close();
			return new String(os.toByteArray());
		} catch (Exception e) {
			e.printStackTrace();
		}
		return "";
	}
	
	private static void onCheckUpdateBegan() {
		thisActivity.runOnGLThread(new Runnable() {
			
			@Override
			public void run() {
				onNativeCheckUpdateBegan();
			}
		});
	}
	
	private static void onCheckUpdateResult(boolean succeed, String resp) {
		if (succeed) {
			try {
				JSONObject json = new JSONObject(resp);
				int code = json.getInt(AppUpdateConfig.RESP_PARAM_CODE);
				if (code == 0) {
					final String updateLog = json.getString(AppUpdateConfig.RESP_PARAM_UPDATELOG);
					final String versionName = json.getString(AppUpdateConfig.RESP_PARAM_VERSIONNAME);
					final String url = json.getString(AppUpdateConfig.RESP_PARAM_APKURL);
					thisActivity.runOnGLThread(new Runnable() {
						
						@Override
						public void run() {
							onNativeCheckUpdateResultWithUpdate(updateLog, versionName, url);
						}
					});
					return;
				}
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
		thisActivity.runOnGLThread(new Runnable() {
			
			@Override
			public void run() {
				onNativeCheckUpdateResultWithoutUpdate();
			}
		});
	}
	
	public static native void onNativeCheckUpdateBegan();
	public static native void onNativeCheckUpdateResultWithoutUpdate();
	public static native void onNativeCheckUpdateResultWithUpdate(String updateLog, String versionName, String url);
}