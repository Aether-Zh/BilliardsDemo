package com.k7k7.update;

public class AppUpdateConfig {
	
	/*
	 * 这个数字是平台里面游戏的编号，根据编号来确定是哪个游戏。
	 * 1	德州扑克
	 * 2	跑得快
	 * 3	升级
	 * 4	保皇
	 * 5	够级
	 * 6	掼蛋
	 * 7	双扣
	 * 8	跑胡子
	 * ……
	 */
	public static final String DUOLE_GAME_ID = "3"; 
	
	public static final String CHECK_URL = "http://update.duole.com/apk/check";
	
	public static final String REQ_PARAM_GAMEID = "gameid";
	public static final String REQ_PARAM_CHANNEL = "channel";
	public static final String REQ_PARAM_VERSIONCODE = "version_code";
	public static final String REQ_PARAM_PACKAGENAME = "package_name";
	public static final String REQ_PARAM_APKSIGNATURE = "apk_signature";
	
	public static final String RESP_PARAM_CODE = "code";
	public static final String RESP_PARAM_VERSIONNAME = "version_name";
	public static final String RESP_PARAM_UPDATELOG = "update_log";
	public static final String RESP_PARAM_APKURL = "apk_url";
	public static final String RESP_PARAM_APKMD5 = "apk_md5";
	public static final String RESP_PARAM_APKSIZE = "apk_size";
}
