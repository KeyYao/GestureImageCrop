package moe.key.yao.gestureimagecrop.utils;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;

import java.util.ArrayList;
import java.util.List;

import moe.key.yao.gestureimagecrop.GestureImageCropApplication;
import moe.key.yao.gestureimagecrop.bean.AlbumBean;
import moe.key.yao.gestureimagecrop.bean.FileBean;

public class MediaUtils {

    private static Gson gson = new GsonBuilder().setDateFormat("yyyy-MM-dd HH:mm:ss").create();
    
    private static String getAlbumList() {
        Context mContext = GestureImageCropApplication.getApplication();
        
        List<AlbumBean> albumList = new ArrayList<>();
        String[] projection = {MediaStore.Images.ImageColumns.BUCKET_ID, MediaStore.Images.ImageColumns.BUCKET_DISPLAY_NAME, MediaStore.Images.ImageColumns.DATA};
        String selection = " 0 = 0 ) group by ( " + MediaStore.Images.ImageColumns.BUCKET_ID;
        Uri uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
    
        Cursor cursor = mContext.getContentResolver().query(uri, projection, selection, null, null);
        if (cursor != null) {
            while (cursor.moveToNext()) {
                String albumId = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.BUCKET_ID));
                String albumName = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.BUCKET_DISPLAY_NAME));
                String albumUrl = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DATA));
                AlbumBean bean = new AlbumBean(albumId, albumName, albumUrl);
                albumList.add(bean);
            }
            cursor.close();
        }
    
        return gson.toJson(albumList);
    }

    private static String getPhotoFileList(String albumId) {
        Context mContext = GestureImageCropApplication.getApplication();
        List<FileBean> fileList = new ArrayList<>();
        
        Cursor cursor = mContext.getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, new String[]{MediaStore.Images.Media.DATA, MediaStore.Images.Media.DISPLAY_NAME}, MediaStore.Images.Media.BUCKET_ID + " = ? ", new String[]{albumId}, MediaStore.Images.Media._ID + " desc");
        while (cursor.moveToNext()) {
            String path = cursor.getString(cursor.getColumnIndex(MediaStore.Images.ImageColumns.DATA));
            String name = cursor.getString(cursor.getColumnIndex(MediaStore.Images.ImageColumns.DISPLAY_NAME));
            FileBean bean = new FileBean(name, path);
            fileList.add(bean);
        }
        cursor.close();
        
        return gson.toJson(fileList);
    }
    
}
