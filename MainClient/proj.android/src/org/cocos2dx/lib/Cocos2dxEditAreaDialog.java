package org.cocos2dx.lib;

import android.app.Dialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.text.InputFilter;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class Cocos2dxEditAreaDialog extends Dialog{
	private final String mTitle;
    private final String mPlaceholder;
    private final String mText;
    private final int mMaxLength;
    private Button mButtonBack;
    private TextView mTextViewTitle;
    private Button mButtonDone; 
    private EditText mEditText;
    public Cocos2dxEditAreaDialog(final Context pContext, final String pTitle, final String pPlaceholder, final String pText, final int pMaxLength) {
    	super(pContext, android.R.style.Theme_Translucent_NoTitleBar_Fullscreen);
    	this.mTitle = pTitle;
    	this.mPlaceholder = pPlaceholder;
    	this.mText = pText;
    	this.mMaxLength = pMaxLength;
    }
    
    @Override
    protected void onCreate(final Bundle pSavedInstanceState) {
        super.onCreate(pSavedInstanceState);

        this.getWindow().setBackgroundDrawable(new ColorDrawable(0x80000000));
     
        // Header布局
        RelativeLayout headerLayout = new RelativeLayout(this.getContext());
        headerLayout.setBackgroundColor(0xFF3D3C3F);
		RelativeLayout.LayoutParams headerParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, convertPixelToDip(48));
		headerParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
		headerLayout.setLayoutParams(headerParams);
        // 返回按钮
        this.mButtonBack = new Button(this.getContext());
        this.mButtonBack.setTextColor(Color.BLACK);
        this.mButtonBack.setText("返回");
        this.mButtonBack.setTextSize(TypedValue.COMPLEX_UNIT_SP, 15);
        RelativeLayout.LayoutParams backParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        backParams.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
        backParams.addRule(RelativeLayout.CENTER_VERTICAL);
        backParams.leftMargin = convertPixelToDip(8);
        backParams.width = convertPixelToDip(100);
        headerLayout.addView(this.mButtonBack, backParams);
        this.mButtonBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View paramView) {
            	Cocos2dxEditAreaDialog.this.closeView();
            }
        });
        // 标题
        this.mTextViewTitle = new TextView(this.getContext());
        this.mTextViewTitle.setTextColor(Color.WHITE);
        this.mTextViewTitle.setTextSize(TypedValue.COMPLEX_UNIT_SP, 25);
        this.mTextViewTitle.setText(this.mTitle);
        this.mTextViewTitle.setGravity(Gravity.CENTER);
        RelativeLayout.LayoutParams titleParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        titleParams.addRule(RelativeLayout.CENTER_HORIZONTAL);
        titleParams.addRule(RelativeLayout.CENTER_VERTICAL);
        headerLayout.addView(this.mTextViewTitle, titleParams);
        headerLayout.setId(1);
        // 完成
        this.mButtonDone = new Button(this.getContext());
        this.mButtonDone.setTextColor(Color.BLACK);
        this.mButtonDone.setText("完成");
        this.mButtonDone.setTextSize(TypedValue.COMPLEX_UNIT_SP, 15);
        RelativeLayout.LayoutParams doneParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        doneParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
        doneParams.addRule(RelativeLayout.CENTER_VERTICAL);
        doneParams.rightMargin = convertPixelToDip(8);
        doneParams.width = convertPixelToDip(100);
        headerLayout.addView(this.mButtonDone, doneParams);
        this.mButtonDone.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View paramView) {
            	Cocos2dxEditAreaDialog.this.finishInput();
            }
        });
        // 整体布局
        RelativeLayout wholeLayout = new RelativeLayout(this.getContext());
        RelativeLayout.LayoutParams wholeParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        wholeLayout.addView(headerLayout);
        // 编辑
        this.mEditText = new EditText(this.getContext());
        this.mEditText.setGravity(Gravity.TOP);
        this.mEditText.setBackgroundColor(0xFFEEEEEE);
        this.mEditText.setHint(this.mPlaceholder);
        this.mEditText.setPadding(convertPixelToDip(10), convertPixelToDip(10), convertPixelToDip(10), convertPixelToDip(10));
        this.mEditText.setTextSize(TypedValue.COMPLEX_UNIT_SP, 15);
        this.mEditText.setTextColor(Color.BLACK);
        this.mEditText.setText(this.mText);
        if (this.mMaxLength > 0) {
            this.mEditText.setFilters(new InputFilter[] {
            		new InputFilter.LengthFilter(this.mMaxLength)
            });
        }
        final Handler initHandler = new Handler();
        initHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
            	Cocos2dxEditAreaDialog.this.mEditText.requestFocus();
            	Cocos2dxEditAreaDialog.this.mEditText.setSelection(Cocos2dxEditAreaDialog.this.mEditText.length());
                Cocos2dxEditAreaDialog.this.openKeyboard();
            }
        }, 200);
        this.mEditText.setOnEditorActionListener(new OnEditorActionListener() {
            @Override
            public boolean onEditorAction(final TextView v, final int actionId, final KeyEvent event) {
                // If user didn't set keyboard type, this callback will be invoked twice with 'KeyEvent.ACTION_DOWN' and 'KeyEvent.ACTION_UP'.
                if (actionId != EditorInfo.IME_NULL || (actionId == EditorInfo.IME_NULL && event != null && event.getAction() == KeyEvent.ACTION_DOWN)) {
                    finishInput();
                    return true;
                }
                return false;
            }
        });

        RelativeLayout.LayoutParams editParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        editParams.addRule(RelativeLayout.BELOW, 1);
        editParams.addRule(RelativeLayout.CENTER_IN_PARENT);
        wholeLayout.addView(this.mEditText, editParams);
        this.setContentView(wholeLayout, wholeParams);
    }
    
    private int convertPixelToDip(int nP)
    {
    	return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, nP, this.getContext().getResources().getDisplayMetrics());
    }
    
    private void openKeyboard() {
        final InputMethodManager imm = (InputMethodManager) this.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(this.mEditText, 0);
    }
    
    private void closeKeyboard() {
        final InputMethodManager imm = (InputMethodManager) this.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(this.mEditText.getWindowToken(), 0);
    }
    
    private void closeView() {
    	this.closeKeyboard();
    	this.dismiss();
    }
    
    private void finishInput() {
    	Cocos2dxHelper.setEditAreaDialogResult(Cocos2dxEditAreaDialog.this.mEditText.getText().toString());
    	this.closeView();
    }
}
