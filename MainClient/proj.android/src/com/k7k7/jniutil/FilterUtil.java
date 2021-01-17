package com.k7k7.jniutil;

import java.io.UnsupportedEncodingException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class FilterUtil {
	// Emoji过滤（同Cocos2dxHelper.java)
	public static String getFilteredText(final String sText) {
		String sRet = "";
        try {
        	sRet = new String(sText.getBytes("UTF8"), "UTF8");
			Pattern emoji = Pattern.compile(
					"[^\\u0020-\\u007E\\u00A0-\\u00BE\\u2E80-\\uA4CF\\uF900-\\uFAFF\\uFE30-\\uFE4F\\uFF00-\\uFFEF\\u0080-\\u009F\\u2000-\\u201F\r\n]",
					Pattern.UNICODE_CASE | Pattern.CASE_INSENSITIVE) ;
	        Matcher emojiMatcher = emoji.matcher(sRet);
	        if (emojiMatcher.find()) {
	        	 sRet = emojiMatcher.replaceAll(""); 
	        }
        } catch (UnsupportedEncodingException pUnsupportedEncodingException) {
            /* Nothing. */
        	sRet = "";
        }
        return sRet;
    }
}
