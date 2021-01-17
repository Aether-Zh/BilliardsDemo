package com.k7k7.jniutil;

import java.io.DataOutputStream;
import java.io.File;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Environment;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.widget.Toast;

public class SysUtil {
	// 常量定义
	public static final String TAG = "SysUtil";

	private static SysUtil instance = null;

	private Activity context = null;
	private static String prePagename = "";
    private static String lastPagename = "";
    private static long appStartTime = 0;
	private static boolean firstInstall = false;
	/**
	 * A signal handler in native code has been triggered. As our last gasp,
	 * launch the crash handler (in its own process), because when we return
	 * from this function the process will soon exit.
	 */
	public static void nativeCrashed() {
		new RuntimeException("crashed here (native trace should follow after the Java trace)").printStackTrace();
	}

	public static Object rtnObject() {
		return SysUtil.getInstance();
	}

	public static SysUtil getInstance() {
		if (instance == null) {
			instance = new SysUtil();
		}
		return instance;
	}

	public void init(Activity activity) {
		context = activity;
	}

	public void destroy() {
		context = null;
	}

	// 取设备信息
	public String getMobileID() {
		String sRet = "";
		TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
		sRet = tm.getDeviceId();
		if (sRet == null || sRet.length() < 2) {// ESN
			sRet = tm.getSubscriberId();
			if (sRet == null || sRet.length() < 2) {
				int iSDK = android.os.Build.VERSION.SDK_INT;
				if (iSDK > 8) {
					sRet = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
				} else {
					sRet = android.os.Build.ID + android.os.Build.CPU_ABI;
				}
			}
		}
		return sRet;
	}
	
	// 取得手机号码
	public String getPhoneNumber() {
		TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		return tm.getLine1Number();
	}

	// 取得IMSI
	public String getIMSI() {
		TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
		return tm.getSubscriberId();
	}
	
	public String getCacheDir() {
		File cacheDir = context.getCacheDir();
		return cacheDir.getParent()
				+ File.separator
				+ cacheDir.getName()
				+ File.separator;
	}
	
	public String getExternalCacheDir() {
		File cacheDir = context.getExternalCacheDir();
		return cacheDir.getParent()
				+ File.separator
				+ cacheDir.getName()
				+ File.separator;
	}
	
	public String getFilesDir() {
		File fileDir = context.getFilesDir();
		return fileDir.getParent()
				+ File.separator
				+ fileDir.getName()
				+ File.separator;
	}
	
	public String getExternalStorageDirectory() {
		return Environment.getExternalStorageDirectory().getAbsolutePath()
				+ File.separator;
	}
	
	public String getApkSignatureMd5() {
		StringBuffer buffer = new StringBuffer();
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
			Signature[] signs = packageInfo.signatures;
			byte[] signBytes = signs[0].toByteArray();
			MessageDigest md5 = MessageDigest.getInstance("MD5");
			byte[] md5Bytes = md5.digest(signBytes);
			for (int i = 0; i < md5Bytes.length; i++) {
				String hex = Integer.toHexString(md5Bytes[i] & 0xFF);
				if (hex.length() == 1) {
					buffer.append("0");
				}
				buffer.append(hex);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return buffer.toString().toLowerCase();
	}
	
	public String getPackageName() {
		return context.getPackageName();
	}

	// 取得包版本号
	public String getVersionName() {
		String version = "Unknown";
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(getPackageName(), 0);
			version = packageInfo.versionName;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return version;
	}
	
	// 取得包版本号
	public int getVersionCode() {
		int versionCode = 0;
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(getPackageName(), 0);
			versionCode = packageInfo.versionCode;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return versionCode;
	}

	// 取Application中meta-data
	public String getApplicationMetaData(String strName) {
		String strVal = "";
		try {
			ApplicationInfo appInfo = context.getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
			strVal = appInfo.metaData.getString(strName);
		} catch (PackageManager.NameNotFoundException e) {
		}
		return strVal;
	}
	
	// 判断网络是否有效
	public boolean isNetworkConnected() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mNetworkInfo = mConnectivityManager.getActiveNetworkInfo();
		if (mNetworkInfo != null) {
			return mNetworkInfo.isAvailable();
		}
		return false;
	}

	// 判断WIFI网络是否可用
	public boolean isWifiConnected() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mWiFiNetworkInfo = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		if (mWiFiNetworkInfo != null) {
			return mWiFiNetworkInfo.isAvailable();
		}
		return false;
	}
	
	// 判断MOBILE网络是否可用
	public boolean isMobileConnected() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mMobileNetworkInfo = mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
		if (mMobileNetworkInfo != null) {
			return mMobileNetworkInfo.isAvailable();
		}
		return false;
	}
	
	// 获取当前网络连接的类型信息
	public int getConnectedType() {
		ConnectivityManager mConnectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mNetworkInfo = mConnectivityManager.getActiveNetworkInfo();
		if (mNetworkInfo != null && mNetworkInfo.isAvailable()) {
			return mNetworkInfo.getType();
		}
		return -1;
	}	
	// 手机中是否有某个包名的应用
	public boolean isAppInstalled(final String packageName) {
        final PackageManager packageManager = context.getPackageManager();
        List<PackageInfo> pinfo = packageManager.getInstalledPackages(0);
        List<String> pName = new ArrayList<String>();
        if (pinfo != null) {
            for (int i = 0; i < pinfo.size(); i++) {
                String pn = pinfo.get(i).packageName;
                pName.add(pn);
            }
        }
        return pName.contains(packageName);
    }
	// 获取手机型号
    public String getBuildModel() {
        return android.os.Build.MODEL;
    }
    
 	// 取版本号
    public String getAndroidSDkInt() {
        String sRet = "";
        try {
            sRet = android.os.Build.VERSION.SDK_INT + "";
        } catch (Exception e) {

        }
        return sRet;
    }
	// 上报事件
    public static void reportPageEvent(final String pagename) {
        lastPagename = prePagename;
        prePagename = pagename;
        new Thread(new Runnable() {
             
             @Override
             public void run() {
                 try {
                     HashMap<String, String> map = new HashMap<String, String>();
                     map.put("sdkver", "4");
                     map.put("game", "shengji");
                     map.put("device", instance.getMobileID());
                     map.put("model", instance.getBuildModel());
                     map.put("channel", PlatformFunction.getChannelType());
                     map.put("version", instance.getVersionName());
                     map.put("platform", "android");
                     map.put("pagename", pagename);
                     map.put("lastpagename", lastPagename);
                     map.put("androidlevel", instance.getAndroidSDkInt());
                     map.put("firstinstall", getFirstInstall());
                     map.put("wasttime", getTimeMillisAppStarted());
                     String params = generateHttpRequestParams(map);
                     
                     // 新建一个URL对象
                     URL url = new URL("https://event.duole.com/event/page");
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
                     conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded;charset=UTF-8");
                     // 开始连接
                     conn.connect();
                      // 发送请求参数
                     DataOutputStream os = new DataOutputStream(conn.getOutputStream());
                     os.write(params.getBytes());
                     os.flush();
                     os.close();
                     if (conn.getResponseCode() == 200) {
                         
                     } else {
                         
                     }
                     conn.disconnect();
                 } catch (Exception e) {
                     e.printStackTrace();
                     
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
    
    public static void setStartTime(long time){
    	appStartTime = time;
    }
    
    public static void setFirstInstall(boolean bFirstInstall){
    	firstInstall = bFirstInstall;
    }
    
    private static String getFirstInstall() {
        return firstInstall ? "true" : "false";
    }
    
    private static String getTimeMillisAppStarted() {
		int delta = (int) (System.currentTimeMillis() - appStartTime);
		return delta + "";
	}
}