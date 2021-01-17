package org.cocos2dx.lib;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.provider.MediaStore;
import android.widget.Toast;

public class Cocos2dxImagePicker {
	public final static int PICK_SOURCETYPE_PHOTOLIBRARY = 102;
	public final static int CROP_PHOTO = 103;
	private static Cocos2dxImagePicker instance = null;
	private Activity context = null;
	private int mRatioX = 1;
	private int mRatioY = 1;
	private int mWidth = 0;
	private int mHeight = 0;
	private String mFilePath = "";
	
	public static Cocos2dxImagePicker getInstance() {
		if (instance == null) {
			instance = new Cocos2dxImagePicker();
		}
		return instance;
	}

	public void init(Activity activity) {
		context = activity;
	}

	public void destroy() {
		context = null;
	}
	
	public void show(final int pRatioX, final int pRatioY, final int pWidth, final int pHeight) {
		mRatioX = pRatioX;
		mRatioY = pRatioY;
		mWidth = pWidth;
		mHeight = pHeight;
		
		Intent openPhotoLibrary = new Intent(Intent.ACTION_PICK);
		openPhotoLibrary.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, "image/*");
		Cocos2dxImagePicker.this.context.startActivityForResult(openPhotoLibrary, Cocos2dxImagePicker.PICK_SOURCETYPE_PHOTOLIBRARY);
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == Cocos2dxImagePicker.PICK_SOURCETYPE_PHOTOLIBRARY) {
			if (resultCode == Activity.RESULT_OK) {
				if (data == null || data.getData() == null){
					cancelCrop();
				} else {
					startCrop(data.getData());
				}
			} else {
				cancelCrop();
			}
		}
		if (requestCode == Cocos2dxImagePicker.CROP_PHOTO) {
			if (resultCode == Activity.RESULT_OK) {
				finishCrop();
			} else {
				cancelCrop();
			}
		}
	}
	
	protected void startCrop(Uri uri) {
		if (uri == null || uri.toString().equals("file:///")){
			Toast toast = Toast.makeText(this.context, "此文件不可用", Toast.LENGTH_SHORT);  
            toast.show();  
            cancelCrop();
            return;
		}
		float outputX = px2dp(mWidth);
		float outputY = px2dp(mHeight);
		Intent crop = new Intent("com.android.camera.action.CROP");
		crop.setDataAndType(uri, "image/*");
		crop.putExtra("crop", true);
		crop.putExtra("aspectX", mRatioX);
		crop.putExtra("aspectY", mRatioY);
		crop.putExtra("outputX", outputX);
		crop.putExtra("outputY", outputY);
		crop.putExtra("scale", true);
		File dir = context.getCacheDir();
		// 手机装有SDCard,并且可以进行读写
		if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())  
	            || !Environment.isExternalStorageRemovable()) {
			dir = context.getExternalCacheDir();
		}
		File file = new File(dir, "avatartmp.jpg");
		mFilePath = file.getAbsolutePath();
		crop.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(file));
		crop.putExtra("outputFormat", Bitmap.CompressFormat.JPEG.toString());
		crop.putExtra("return-data", false);
		this.context.startActivityForResult(crop, CROP_PHOTO);
	}
	
	protected Boolean fitSize(String pathName) {
		if (mFilePath.isEmpty() || mWidth == 0 || mHeight == 0) {
			return false;
		}
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		Bitmap bitmap = BitmapFactory.decodeFile(pathName, options);
		int outWidth = options.outWidth;
		int outHeight = options.outHeight;
		if (outWidth == mWidth && outHeight == mHeight) {
			System.out.println("Cocos2dxImagePicker >>> fitSize: already fit.");
			return true;
		}
		System.out.println("Cocos2dxImagePicker >>> fitSize: org=(" + outWidth + "," + outHeight + ")");
		options.inJustDecodeBounds = false;
		options.inPurgeable = true;  
		options.inInputShareable = true;
		options.inSampleSize = Math.max(outWidth / mWidth, outHeight / mHeight);
		if (options.inSampleSize < 1) {
			options.inSampleSize = 1;
		}
		bitmap = BitmapFactory.decodeFile(pathName, options);
		if (bitmap == null)
		{
			return false;
		}
		bitmap = Bitmap.createScaledBitmap(bitmap, mWidth, mHeight, false);
		OutputStream os = null;
		try {
			os = new FileOutputStream(pathName);
			bitmap.compress(Bitmap.CompressFormat.JPEG, 75, os);
			os.flush();
			os.close();
			return true;
		} catch(FileNotFoundException e) {
			e.printStackTrace();
			return false;
		} catch(IOException e) {
			e.printStackTrace();
			return false;
		}
	}
	
	protected void finishCrop() {
		final Handler initHandler = new Handler();
        initHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
            	Boolean succeed = fitSize(mFilePath);
            	Cocos2dxHelper.setImagePickerResult(mFilePath, succeed);
            }
        }, 200);
	}
	
	protected void cancelCrop() {
		Cocos2dxHelper.setImagePickerResult("", false);
	}
	
	protected float px2dp(int px) {
		final float scale = context.getResources().getDisplayMetrics().density;  
		return  (px / scale + 0.5f);
	}
}
