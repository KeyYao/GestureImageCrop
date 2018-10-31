package moe.key.yao.gestureimagecrop;

import android.content.Context;

public class GestureImageCropApplication extends org.qtproject.qt5.android.bindings.QtApplication {
    
    private static GestureImageCropApplication mContext = null;
    
    public static GestureImageCropApplication getApplication() {
        return mContext;
    }
    
    @Override
    public void onCreate() {
        super.onCreate();
        mContext = this;
    }

}
