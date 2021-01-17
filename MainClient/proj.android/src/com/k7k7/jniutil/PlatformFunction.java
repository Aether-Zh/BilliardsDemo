package com.k7k7.jniutil;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.AlarmManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.ClipboardManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.os.Vibrator;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.Toast;

import com.k7k7.shengjihd.R;
import com.sobot.chat.SobotApi;
import com.sobot.chat.api.model.Information;
import com.tencent.mm.opensdk.modelmsg.SendMessageToWX;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.modelmsg.WXMiniProgramObject;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;
import com.umeng.socialize.PlatformConfig;
import com.umeng.socialize.ShareAction;
import com.umeng.socialize.UMAuthListener;
import com.umeng.socialize.UMShareAPI;
import com.umeng.socialize.UMShareListener;
import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.media.UMWeb;

public class PlatformFunction {
	 
	private static Cocos2dxActivity thisActivity = null;
	
	// 电池监控
	private static BroadcastReceiver batteryChangedReceiver = null;
	private static float batteryLevel = 0;
	private static int temperature = 0;

	// 友盟分享
	
	// 平台类型
	private static final int SOCIAL_PLATFORM_WECHATSESSION = 0;
	private static final int SOCIAL_PLATFORM_WECHATTIMELINE = 1;
	private static final int SOCIAL_PLATFORM_QQ = 2;
	private static final int SOCIAL_PLATFORM_QZONE = 3;
	private static final int SOCIAL_PLATFORM_SINA = 4;
	
	private static ClipboardManager clipboard = null;
	private static ClipboardManager.OnPrimaryClipChangedListener clipboardListener = null;
	private static String clipText = "";

	// 分享类型
	private static final int SOCIAL_PAGE_CATEGORY_COMMON = 0;
	private static final int SOCIAL_PAGE_CATEGORY_MOBILEFEE = 1;
    private static final int SOCIAL_PAGE_CATEGORY_ACTIVITY = 2;
    
    // IWXAPI 是第三方app和微信通信的openApi接口
    private static IWXAPI api;
    
    private static UMShareListener umShareListener = new UMShareListener() {
		
		@Override
		public void onStart(SHARE_MEDIA platform) {
		}
		
		@Override
		public void onResult(SHARE_MEDIA platform) {
			thisActivity.runOnGLThread(new Runnable() {
				
				@Override
				public void run() {
					PlatformFunction.onShareFinish(true);
				}
			});
		}
		
		@Override
		public void onError(SHARE_MEDIA platform, Throwable t) {
			thisActivity.runOnGLThread(new Runnable() {
				
				@Override
				public void run() {
					PlatformFunction.onShareFinish(false);
				}
			});
		}
		
		@Override
		public void onCancel(SHARE_MEDIA platform) {
			thisActivity.runOnGLThread(new Runnable() {
				
				@Override
				public void run() {
					PlatformFunction.onShareFinish(false);
				}
			});
		}
	};
	
	private static UMAuthListener umAuthListener = new UMAuthListener() {
        /**
         * @desc 授权开始的回调
         * @param platform 平台名称
         */
        @Override
        public void onStart(SHARE_MEDIA platform) {
        	Log.i("UMAuthListener", "onStart");
        }

        /**
         * @desc 授权成功的回调
         * @param platform 平台名称
         * @param action 行为序号，开发者用不上
         * @param data 用户资料返回
         */
        @Override
        public void onComplete(SHARE_MEDIA platform, int action, Map<String, String> data) {
            try{
            	/**
                 *  Log.i("UMAuthListener", "onComplete[openid=]" + data.get("openid"));
                 *  Log.i("UMAuthListener", "onComplete[uid=]" + data.get("unionid"));
                 *  Log.i("UMAuthListener", "onComplete[accessToken=]" + data.get("accessToken"));
                 *  Log.i("UMAuthListener", "onComplete[RefreshToken=]" + data.get("refreshToken"));
                 *  Log.i("UMAuthListener", "onComplete[expiration=]" + data.get("expiration"));
                 */
            	final Map<String, String> mapData = data;
                thisActivity.runOnGLThread(new Runnable() {
                	
                    @Override
                    public void run() {
                        try {
                        	System.out.println("umAuth callback onComplete");
                            if (mapData != null)
                            {
                                PlatformFunction.onAuthFinish(mapData.get("accessToken"), mapData.get("refreshToken"), mapData.get("openid"), false);
                            }
                            else
                            {
                                PlatformFunction.onAuthFinish("", "", "", true);
                            }
                        }catch (Exception e){
                        	
                        }
                    }
                    
                });
            }catch (Exception e){
                
            }
        }
        
        /**
         * @desc 授权失败的回调
         * @param platform 平台名称
         * @param action 行为序号，开发者用不上
         * @param t 错误原因
         */
        @Override
        public void onError(SHARE_MEDIA platform, int action, Throwable t) {
        	try{
        		thisActivity.runOnGLThread(new Runnable() {

    				@Override
    				public void run() {
    					System.out.println("umAuth callback onError");
    					PlatformFunction.onAuthFinish("", "", "", true);
    				}
            		
            	});
        	}catch (Exception e){
                
            }
        }

        /**
         * @desc 授权取消的回调
         * @param platform 平台名称
         * @param action 行为序号，开发者用不上
         */
        @Override
        public void onCancel(SHARE_MEDIA platform, int action) {
        	
        	System.out.println("umAuth callback onCancel");
        }
    };

	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
		clipboard = (ClipboardManager)thisActivity.getSystemService(Context.CLIPBOARD_SERVICE);
		clipboardListener = new ClipboardManager.OnPrimaryClipChangedListener() {
			@Override
			public void onPrimaryClipChanged() {
				if (clipboard.hasPrimaryClip() && clipboard.getPrimaryClipDescription().hasMimeType(ClipDescription.MIMETYPE_TEXT_PLAIN)) {
					ClipData clipData = clipboard.getPrimaryClip();
					if (clipData.getItemCount() > 0) {
						ClipData.Item item = clipData.getItemAt(0);
						if (item != null && item.getText() != null) {
							clipText = item.getText().toString();
						}
					}
				}
			}
		};
		clipboard.addPrimaryClipChangedListener(clipboardListener);
		clipboardListener.onPrimaryClipChanged();

		// 电池监控
		batteryChangedReceiver = new BroadcastReceiver() {
			
			@Override
			public void onReceive(Context context, Intent intent) {
				int level = intent.getIntExtra("level", -1);
				int scale = intent.getIntExtra("scale", -1);
				if (level >= 0 && scale > 0) {
					batteryLevel = (float)level / scale;
				}
				temperature = intent.getIntExtra("temperature", 99999);
			}
		};
		thisActivity.registerReceiver(batteryChangedReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
		
		// 友盟分享
		PlatformConfig.setWeixin(PlatformFunction.getWXAppId(), PlatformFunction.getWXAppSecret());
		PlatformConfig.setQQZone(PlatformFunction.getQQZoneAppId(), PlatformFunction.getQQZoneAppKey());
		PlatformConfig.setSinaWeibo(PlatformFunction.getSinaAppKey(), PlatformFunction.getSinaAppSecret(), "http://sns.whalecloud.com/sina2/callback");
		UMShareAPI.get(thisActivity);
		
		// 微信分享
		api = WXAPIFactory.createWXAPI(thisActivity, PlatformFunction.getWXAppId(), true);
		api.registerApp(PlatformFunction.getWXAppId());
	}
	
	public static void destroy() {
		// 电池监控
		thisActivity.unregisterReceiver(batteryChangedReceiver);
		batteryChangedReceiver = null;
		// 剪切板
		clipboard.removePrimaryClipChangedListener(clipboardListener);
		clipboardListener = null;
		clipboard = null;
		thisActivity = null;
	}
	
	public static void openURL(final String url) {
		thisActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				thisActivity.startActivity(intent);
			}
		});
	}
	
	/**
	 * 功能描述：获取版本号
	 * @return 当前应用版本号
	 */
	public static String getVersionName() {
		String version = null;
		try {
			PackageManager manager = thisActivity.getPackageManager();
			PackageInfo info = manager.getPackageInfo(thisActivity.getPackageName(), 0);
			version = info.versionName;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return version;
	}
	
	/**
	 * 功能描述：获取版本号
	 * @return 当前应用版本号
	 */
	public static int getVersionCode() {
		int versionCode = 0;
		try {
			PackageManager manager = thisActivity.getPackageManager();
			PackageInfo info = manager.getPackageInfo(thisActivity.getPackageName(), 0);
			versionCode = info.versionCode;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return versionCode;
	}
	
	public static void openApplication(final String packageName, final String sDownLoadLink) {
        thisActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                boolean bSuccess = false;
                PackageManager manager = thisActivity.getPackageManager();
                Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
                mainIntent.addCategory(Intent.CATEGORY_LAUNCHER);
                List<ResolveInfo> resolveInfos = manager.queryIntentActivities(mainIntent, 0);
                Collections.sort(resolveInfos, new ResolveInfo.DisplayNameComparator(manager));
                if (resolveInfos == null)
                {
                    bSuccess = false;
                }
                for (ResolveInfo reInfo : resolveInfos) {
                    if (reInfo.activityInfo.packageName.contentEquals(packageName)) {
                        String activityName = reInfo.activityInfo.name;
                        Intent launchIntent = new Intent();
                        launchIntent.setComponent(new ComponentName(packageName, activityName));
                        thisActivity.startActivity(launchIntent);
                        thisActivity.finish();
                        Cocos2dxHelper.terminateProcess();
                        bSuccess = true;
                        break;
                    }
                }
                if (!bSuccess)
                {
                    openURL(sDownLoadLink);
                }
            }
        });
    }
	
	public static boolean canOpenApplication(final String packageName) {
		boolean bRet = false;
		PackageManager manager = thisActivity.getPackageManager();
		Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
		mainIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		List<ResolveInfo> resolveInfos = manager.queryIntentActivities(mainIntent, 0);
		Collections.sort(resolveInfos, new ResolveInfo.DisplayNameComparator(manager));
		if (resolveInfos == null) {
			bRet = false;
		}
		for (ResolveInfo reInfo : resolveInfos) {
			if (reInfo.activityInfo.packageName.contentEquals(packageName)) {
				bRet = true;
				break;
			}
		}
		return bRet;
    }
	
	public static float getBatteryLevel() {
		return batteryLevel;
	}

	public static int getConnectedType() {
		if (!SysUtil.getInstance().isNetworkConnected())
		{
			return 0;
		}
		
		int nType = SysUtil.getInstance().getConnectedType();
		if (nType == 1)  // Wi-Fi
		{
			return 2;
		}
		if (nType == 0)	 // 数据流量
		{
			return 1;
		}
		return -1;
	}

	public static void showFeedback() {
		// 反馈游戏名称
		String gameName = "升级";
		// 反馈用户昵称
		String nickName = PlatformFunction.getFeedbackUserId();
		if (nickName.isEmpty()) {
			nickName = String.format("单机用户@%s", gameName);
		} else {
			nickName = String.format("联网用户%s@%s", nickName, gameName);
		}
		// 设备唯一识别码
		String deviceId = SysUtil.getInstance().getMobileID();
		// 反馈用户唯一标识
		String userId = deviceId;
		// 反馈附加信息
		String appInfo = String.format("%s/%s/%d", thisActivity.getString(R.string.app_name), SysUtil.getInstance().getVersionName(), SysUtil.getInstance().getVersionCode());
		String deviceInfo = String.format("%s/%s/%s", Build.BRAND, Build.MODEL, Build.VERSION.RELEASE);
		Map<String,String> customInfo = new HashMap<String, String>();
		customInfo.put("应用信息", appInfo);
		customInfo.put("渠道信息", PlatformFunction.getChannelType());
		customInfo.put("设备信息", deviceInfo);
		customInfo.put("设备编号", deviceId);
		
		// 配置启动参数
		Information info = new Information();
		info.setAppkey(PlatformFunction.getSobotAppKey());
		info.setSkillSetId(PlatformFunction.getSobotSkillSetId());
		info.setUname(nickName);
		info.setUid(userId);
		info.setCustomInfo(customInfo);
		
		Log.i("aaa", "showFeedback()");
		// 启动
		SobotApi.startSobotChat(thisActivity, info);
	}
	
	public static void sharePage(int platform, int category, String text, String title, String url) {
		if (text == null || text.equals("")) {
			return;
		}
		// 标题
		if (title == null || title.equals("")) {
			title = thisActivity.getString(R.string.app_name);
		}
		// Url
		if (url == null || url.equals("")) {
			url = "";
		}
		// 平台
		SHARE_MEDIA media = SHARE_MEDIA.GENERIC;
		switch (platform) {
		case SOCIAL_PLATFORM_WECHATSESSION:
			media = SHARE_MEDIA.WEIXIN;
			break;
		case SOCIAL_PLATFORM_WECHATTIMELINE:
			media = SHARE_MEDIA.WEIXIN_CIRCLE;
			// 设置分享文字，微信朋友圈分享消息只显示标题
			title = text;
			break;
		case SOCIAL_PLATFORM_QQ:
			media = SHARE_MEDIA.QQ;
			break;
		case SOCIAL_PLATFORM_QZONE:
			media = SHARE_MEDIA.QZONE;
			break;
		case SOCIAL_PLATFORM_SINA:
		default:
			// 默认使用新浪微博分享
			media = SHARE_MEDIA.SINA;
			break;
		}
		// 图片
		int imageResId = R.drawable.socialpage_thumb_common;
		switch (category) {
		case SOCIAL_PAGE_CATEGORY_COMMON:
			imageResId = R.drawable.socialpage_thumb_common;
			break;
		case SOCIAL_PAGE_CATEGORY_MOBILEFEE:
			imageResId = R.drawable.socialpage_thumb_common;
			break;
		default:
			imageResId = R.drawable.socialpage_thumb_common;
			break;
		}
		Bitmap bmp = BitmapFactory.decodeResource(thisActivity.getResources(), imageResId);
		UMImage image = new UMImage(thisActivity, bmp);
		// 设置分享
		ShareAction action = new ShareAction(thisActivity);
		action.setPlatform(media).setCallback(umShareListener);
		if (media == SHARE_MEDIA.SINA) {
			// 新浪微博无客户端分享网页时无图，以图文+显式链接的方式特殊处理
			action.withText(text + url).withMedia(image);
		} else {
			// 其他方式以分享网页方式正常处理
			UMWeb web = new UMWeb(url);
			web.setTitle(title);
			web.setDescription(text);
			web.setThumb(image);
			action.withMedia(web);
		}
		action.share();
	}
	
	public static void shareImage(int platform, String absoluteImagePath) {
		if (absoluteImagePath == null || absoluteImagePath.equals("")) {
			return;
		}
		// 平台
		SHARE_MEDIA media = SHARE_MEDIA.GENERIC;
		switch (platform) {
		case SOCIAL_PLATFORM_WECHATSESSION:
			media = SHARE_MEDIA.WEIXIN;
			break;
		case SOCIAL_PLATFORM_WECHATTIMELINE:
			media = SHARE_MEDIA.WEIXIN_CIRCLE;
			break;
		case SOCIAL_PLATFORM_QQ:
			media = SHARE_MEDIA.QQ;
			break;
		case SOCIAL_PLATFORM_QZONE:
			media = SHARE_MEDIA.QZONE;
			break;
		case SOCIAL_PLATFORM_SINA:
		default:
			// 默认使用新浪微博分享
			media = SHARE_MEDIA.SINA;
			break;
		}
		Bitmap bmp = BitmapFactory.decodeFile(absoluteImagePath);
		UMImage image = new UMImage(thisActivity, bmp);
		// 设置分享
		ShareAction action = new ShareAction(thisActivity);
		action.setPlatform(media).setCallback(umShareListener);
		if (media == SHARE_MEDIA.SINA) {
			// 新浪微博无客户端分享图片时必须填充文本
			action.withText("分享图片");
		}
		action.withMedia(image).share();
	}
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		UMShareAPI.get(thisActivity).onActivityResult(requestCode, resultCode, data);
	}
	
	// 删除指定本地推送
	public static void removeAllLocalNotification()
	{
		NotificationManager mNotifyMgr = (NotificationManager) thisActivity.getSystemService(Context.NOTIFICATION_SERVICE);
        mNotifyMgr.cancelAll();
	}

	// 添加本地推送
	public static void addLocalNotification(final String sText, final String sTitle, final long nDelay, final String sKey)
	{
		thisActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	Intent intent = new Intent(thisActivity, NotificationReceiver.class);
                intent.setClass(thisActivity, NotificationReceiver.class);
                intent.setData(Uri.parse(sKey)); // 通知名字,作为通知id使用
                intent.putExtra("msg", "play_hskay");
                intent.putExtra("content", sText); // 通知内容
                PendingIntent pi = PendingIntent.getBroadcast(thisActivity, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
                AlarmManager am = (AlarmManager) thisActivity.getSystemService(Context.ALARM_SERVICE);
                am.set(AlarmManager.RTC_WAKEUP , System.currentTimeMillis() + nDelay * 1000, pi);
            }
        });
	}
	
	// 拷贝到剪切板
	public static void copyToDipboard(final String sText)
	{
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				clipboard.setPrimaryClip(ClipData.newPlainText(null, sText));
			}
		}); 
	}
    
	public static final String getCopyText() {
		String sText = FilterUtil.getFilteredText(clipText);
		return sText;
	}
    
    public static void openWeChat()
    {
    	openApplication("com.tencent.mm", "");
    }
    
    public static void shake()
    {
		Vibrator vib = (Vibrator) thisActivity
				.getSystemService(Context.VIBRATOR_SERVICE);
		vib.vibrate(300);
    }
    
    public static boolean isEmulator() {
    	boolean bRet = false;
	    if (temperature == 0) {
	        //这是通过电池的温度来判断是真机还是模拟器
	    	bRet = true;
	    }
	    return bRet;
	}

	public static boolean isIntentSafe(Uri uri) {
        Intent mapCall = new Intent(Intent.ACTION_VIEW, uri);
        PackageManager packageManager = thisActivity.getPackageManager();
        List<ResolveInfo> activities = packageManager.queryIntentActivities(mapCall, 0);
        return activities.size() > 0;
    }

    public static void onShowComment(final String sMarketPackage)
    {
        thisActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (sMarketPackage == null || sMarketPackage.equals("")){
                    return;
                }
                String address = "market://details?id=" + thisActivity.getPackageName();
                if (isIntentSafe(Uri.parse(address)) && SysUtil.getInstance().isAppInstalled(sMarketPackage)) {
                    Intent marketIntent = new Intent("android.intent.action.VIEW");
                    marketIntent.setData(Uri.parse(address));
                    //不同的渠道需要设置
                    marketIntent.setPackage(sMarketPackage);
                    thisActivity.startActivity(marketIntent);
                }
                // 没有安装市场
                else {
                    Toast.makeText(thisActivity, "无法打开应用市场", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
    
	public static void onSaveImage(final String sPath)
	{
		if (sPath == null || sPath.equals("")) {
			return;
		}
	
		Bitmap bmp = BitmapFactory.decodeFile(sPath);
		File appDir = new File(Environment.getExternalStorageDirectory(), "Gouji");
	    if (!appDir.exists()) {
	        appDir.mkdir();
	    }
	    String fileName = System.currentTimeMillis() + ".jpg";
	    File file = new File(appDir, fileName);
	    try {
	        FileOutputStream fos = new FileOutputStream(file);
	        bmp.compress(CompressFormat.JPEG, 100, fos);
	        fos.flush();
	        fos.close();
	    } catch (FileNotFoundException e) {
	        e.printStackTrace();
	    } catch (IOException e) {
	        e.printStackTrace();
		}
	    
	    // 其次把文件插入到系统图库
	    try {
	        MediaStore.Images.Media.insertImage(thisActivity.getContentResolver(),
					file.getAbsolutePath(), fileName, null);
	    } catch (FileNotFoundException e) {
	        e.printStackTrace();
	    }
	    final String saveAs = Environment.getExternalStorageDirectory().getPath() + "/Gouji/" + fileName;
	    Uri contentUri = Uri.fromFile(new File(saveAs));
	    // 最后通知图库更新
	    thisActivity.sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, contentUri));
	    boolean success = true;
	    try
	    {
	    	File savefile = new File(saveAs);
	    	if (!savefile.exists())
	    	{
	    		success = false;
	    	}
	    }catch (Exception e){
	    	success = false;
	    }
	    onSaveImageFinish(success);
	}

	public static void OpenSystemSettings() {
    	Intent intent = new Intent(
		android.provider.Settings.ACTION_SETTINGS);
		thisActivity .startActivity(intent);
	}

	public static void openAppInAppMarket(final String appLink, final String marketAddress) {
		if (appLink == null || appLink.equals("")) {
			return;
		}
		if (marketAddress == null || marketAddress.equals("")) {
			return;
		}
		thisActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Uri uri = Uri.parse("market://details?id=" + appLink);
				if (isIntentSafe(uri) && SysUtil.getInstance().isAppInstalled(marketAddress)) {
					Intent marketIntent = new Intent("android.intent.action.VIEW");
					marketIntent.setData(uri);
					marketIntent.setPackage(marketAddress);
					thisActivity.startActivity(marketIntent);
				} else {
					// 没有安装市场
					Toast.makeText(thisActivity, "无法打开应用市场", Toast.LENGTH_SHORT).show();
				}
			}
		});
	}
	
	public static void openCurrentAppInAppMarket(final String marketAddress) {
		openAppInAppMarket(thisActivity.getPackageName(), marketAddress);
	}
	
	public static boolean canOpenAppMarket(final String address) {
		if (address == null || address.equals("")) {
			return false;
		}
		Uri uri = Uri.parse("market://details?id=" + thisActivity.getPackageName());
		return isIntentSafe(uri) && SysUtil.getInstance().isAppInstalled(address);
	}


	public static void onSaveSchemesResult(final String path, final String data) {
		thisActivity.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				onSaveSchemesData(path, data);
			}
		});
	}

	public static void doWeChatAuth() {
    	UMShareAPI.get(thisActivity).getPlatformInfo(thisActivity, SHARE_MEDIA.WEIXIN, umAuthListener);
    }
	
    public static boolean isInstallWeChat() {
    	return UMShareAPI.get(thisActivity).isInstall(thisActivity, SHARE_MEDIA.WEIXIN);
    }

    public static void shareToWXMiniGame(final String url, final String userName, final String thumbUrl) {
    	WXMiniProgramObject miniProgramObject = new WXMiniProgramObject();
    	miniProgramObject.webpageUrl = url;
    	miniProgramObject.miniprogramType = WXMiniProgramObject.MINIPTOGRAM_TYPE_RELEASE;
    	miniProgramObject.userName = userName;
    	WXMediaMessage msg = new WXMediaMessage(miniProgramObject);
    	msg.title = "确认过眼神，就是我要的牌";
    	msg.description = "";
    	Bitmap bmp = BitmapFactory.decodeResource(thisActivity.getResources(), R.drawable.socialpage_thumb_common);
    	URL conurl = null;
    	try {
    		conurl = new URL(thumbUrl);
    		HttpURLConnection con = (HttpURLConnection) conurl.openConnection();
    		bmp = BitmapFactory.decodeStream(con.getInputStream());
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
		UMImage image = new UMImage(thisActivity, bmp);
    	msg.thumbData = image.toByte();
    	
    	SendMessageToWX.Req req = new SendMessageToWX.Req();
    	req.transaction = String.valueOf("miniProgram");
    	req.message = msg;
    	req.scene = SendMessageToWX.Req.WXSceneSession; 
    	api.sendReq(req);
    }

	public static native void onShareFinish(boolean success);
	public static native void onSaveImageFinish(boolean success);
	public static native String getFeedbackUserId();
	public static native String getSobotAppKey();
	public static native String getSobotSkillSetId();
	public static native String getChannelType();
	public static native String getUmengAppKey();
	public static native String getUmengChannelId();
	public static native String getWXAppId();
	public static native String getWXAppSecret();
	public static native String getQQZoneAppId();
	public static native String getQQZoneAppKey();
	public static native String getSinaAppKey();
	public static native String getSinaAppSecret();
	public static native void onSaveSchemesData(String path, String data);
	public static native void onAuthFinish(String accessToken, String refreshToken, String openId, boolean error);
	public static native String getWXMiniUserName();
	public static native String getWXMiniThumbUrl();
}
