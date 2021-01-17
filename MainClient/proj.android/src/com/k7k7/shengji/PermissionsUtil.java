package com.k7k7.shengji;

import java.util.ArrayList;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;


public class PermissionsUtil {

	public static HashMap<String, String> requiredPermissionCfg = new HashMap<String, String>();
	public static ArrayList<String> requestPermissionList = new ArrayList<String>();
//	private static Cocos2dxActivity _activity = null;
	private static Activity _activity = null;
	private static boolean _gotoSettings = false;
	
//	public static void permissionsUtilInit(final Cocos2dxActivity activity){
	public static void permissionsUtilInit(final Activity activity){
		_activity = activity;
		// 运行时权限配置
		requiredPermissionCfg.clear();
		requestPermissionList.clear();
		configureRequiredPermission(Manifest.permission.READ_PHONE_STATE, "需要【电话】权限用于确定本机识别码，以保证账号安全性。请开启【电话】权限，以正常使用游戏功能。");
		configureRequiredPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, "需要【存储】权限用于记录游戏数据。请开启【存储】权限，以正常使用游戏功能。");
	}
	
	
	public static boolean checkPermissions(){
		// 过滤已允许权限
		ArrayList<String> filterdRequestPermissionList = new ArrayList<String>();
		for (String permission : requestPermissionList) {
			int result = ActivityCompat.checkSelfPermission(_activity, permission);
			if (result != PackageManager.PERMISSION_GRANTED) {
				filterdRequestPermissionList.add(permission);
			}
		}
		if (filterdRequestPermissionList.size() > 0){
			return true;
		}else{
			return false;
		}		
	}
	
	//	从系统的授权设置界面返回；
	public static boolean isGotoSettings(){
		return _gotoSettings;
	}
	public static void resetGotoSettings(){
		_gotoSettings = false;
	}
	private static void configureRequiredPermission(final String permission, final String description) {
		requiredPermissionCfg.put(permission, description);
		requestPermissionList.add(permission);
	}

	public static void requestRequiredPermissions() {
		// 过滤已允许权限
		ArrayList<String> filterdRequestPermissionList = new ArrayList<String>();
		for (String permission : requestPermissionList) {
			int result = ActivityCompat.checkSelfPermission(_activity, permission);
			if (result != PackageManager.PERMISSION_GRANTED) {
				filterdRequestPermissionList.add(permission);
			}
		}
		requestPermissionList = filterdRequestPermissionList;
		if (requestPermissionList.size() > 0) {
			String[] permissions = requestPermissionList.toArray(new String[requestPermissionList.size()]);
			ActivityCompat.requestPermissions(_activity, permissions, 0);
		} else {			
			return;
		}
	}
	
	public static void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {

		for (int i = 0; i < permissions.length; i++) {
			String permission = permissions[i];
			int result = grantResults[i];
			// 清理已允许权限
			if (result == PackageManager.PERMISSION_GRANTED) {
				requestPermissionList.remove(permission);
			}
		}

		ArrayList<String> showRationalePermissionList = new ArrayList<String>();
		for (String permission : requestPermissionList) {
			// 记录没有彻底禁止的权限
			if (ActivityCompat.shouldShowRequestPermissionRationale(_activity, permission)) {
				showRationalePermissionList.add(permission);
			}
		}

		if (showRationalePermissionList.size() > 0) {
			// 优先提示没有彻底禁止的权限，不需要前往设置
			String permission = showRationalePermissionList.get(0);
			showRequestPermissionRationale(permission, requiredPermissionCfg.get(permission), false);
		} else if (requestPermissionList.size() > 0) {
			// 次要提示彻底禁止的权限，需要前往设置
			String permission = requestPermissionList.get(0);
			showRequestPermissionRationale(permission, requiredPermissionCfg.get(permission), true);
		} else {
			return;
		}
	}

	public static void showRequestPermissionRationale(final String permission, 
												final String message, 
												final boolean gotoSettings) {
		AlertDialog.Builder builder = new AlertDialog.Builder(_activity, AlertDialog.THEME_DEVICE_DEFAULT_LIGHT);
		builder.setCancelable(false);
		builder.setTitle("权限申请");
		builder.setMessage(message);
		builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				_activity.finish();
				System.exit(0);
			}
		});
		builder.setPositiveButton("去设置", new DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				if (gotoSettings) {
					launchSettings();
				} else {
					ActivityCompat.requestPermissions(_activity, new String[] { permission }, 0);
				}
			}
		});
		builder.show();
	}

	private static void launchSettings() {
		Intent intent = new Intent();
		intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
		intent.setData(Uri.fromParts("package", _activity.getPackageName(), null));
		_activity.startActivity(intent);
		_gotoSettings = true;
//		_activity.finish();		
//		System.exit(0);
	}
}