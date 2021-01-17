package com.k7k7.jniutil;

import java.util.List;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.k7k7.shengjihd.R;

public class NotificationReceiver extends BroadcastReceiver {
	
	public NotificationReceiver(){
		
	}
	
	@Override
    public void onReceive(Context context, Intent itent) {
        String msg = itent.getStringExtra("msg");
        String content = itent.getStringExtra("content");
      //判断应用是否在前台运行 
        if (isTopActivity(context)) {
            return;
        }
       //推送一条通知
        showNotification(context,content);
        return;
    }
	
	public void showNotification(Context context, String msg)  {
		NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(context);
		mBuilder.setSmallIcon(R.drawable.icon);
		mBuilder.setContentTitle(context.getResources().getString(R.string.app_name));
		mBuilder.setContentText(msg);
		mBuilder.setAutoCancel(true); 
		mBuilder.setWhen(System.currentTimeMillis());
		mBuilder.setDefaults(Notification.DEFAULT_SOUND);
		Intent resultIntent = new Intent(context, com.k7k7.shengji.AppActivity.class);
        
		PendingIntent resultPendingIntent = PendingIntent.getActivity(context,0,resultIntent,PendingIntent.FLAG_UPDATE_CURRENT);
		mBuilder.setContentIntent(resultPendingIntent);
		int mNotificationId = 001;
		NotificationManager mNotifyMgr = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
		mNotifyMgr.notify(mNotificationId, mBuilder.build());
    }
	
	protected static boolean isTopActivity(Context activity) {
		String packageName = activity.getPackageName();
		ActivityManager activityManager = (ActivityManager) activity.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningTaskInfo>  tasksInfo = activityManager.getRunningTasks(1); 
		if(tasksInfo.size() > 0){  
            Log.d("test","---------------包名-----------"+tasksInfo.get(0).topActivity.getPackageName());
            //应用程序位于堆栈的顶层  
            if(packageName.equals(tasksInfo.get(0).topActivity.getPackageName())){  
                return true;  
            }  
        }  
		return false;
	}
}
