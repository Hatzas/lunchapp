// http://www.amin-ahmadi.com

package org.qtproject.LunchApp;

import org.qtproject.qt5.android.bindings.QtApplication;
import org.qtproject.qt5.android.bindings.QtActivity;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class LunchApp extends QtActivity
{
    static final int REQUEST_OPEN_IMAGE = 1;
    static final int REQUEST_TAKE_PHOTO = 2;

    public static native void fileSelected(String fileName);

    private static LunchApp m_instance;

    String mCurrentPhotoPath;


    public LunchApp()
    {
        Log.d( "LunchApp", "initialized" );
        m_instance = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate( savedInstanceState );
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
    }

    static void openAnImage()
    {
        Log.d( "LunchApp", "openAnImage()" );

        m_instance.dispatchOpenGallery();
    }

    static void takePicture()
    {
        Log.d( "LunchApp", "takePicture()" );

        m_instance.dispatchTakePicture();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        Log.d( "LunchApp", "onActivityResult:" );

        if( resultCode == RESULT_OK)
        {
            if( requestCode == REQUEST_OPEN_IMAGE )
            {
                String filePath = getRealPathFromURI(getApplicationContext(), data.getData());
                if( filePath == null )
                    Log.d("LunchApp", "Empty result");

                fileSelected( filePath );
            }
            else if( requestCode == REQUEST_TAKE_PHOTO )
            {
                if( mCurrentPhotoPath == null )
                    Log.d( "LunchApp", "Empty photo path" );

                fileSelected( mCurrentPhotoPath );
            }
        }
        else
        {
            fileSelected( "-" );
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    private void dispatchOpenGallery()
    {
        Log.d("LunchApp","dispatchOpenGallery()");

        Intent intent = new Intent( Intent.ACTION_GET_CONTENT );
        intent.setType( "image/*" );
        startActivityForResult( intent, REQUEST_OPEN_IMAGE );
    }

    private void dispatchTakePicture()
    {
        Log.d( "LunchApp","dispatchTakePicture()" );

/*      Intent takePictureIntent = new Intent( MediaStore.ACTION_IMAGE_CAPTURE );
        if( takePictureIntent.resolveActivity( getPackageManager() ) != null )
        {
            startActivityForResult( takePictureIntent, REQUEST_IMAGE_CAPTURE );
        }
*/
        Intent takePictureIntent = new Intent( MediaStore.ACTION_IMAGE_CAPTURE );

        // Ensure that there's a camera activity to handle the intent
        if( takePictureIntent.resolveActivity( getPackageManager() ) != null )
        {
            // Create the File where the photo should go
            File photoFile = null;
            try
            {
                photoFile = createImageFile();
            }
            catch (IOException ex)
            {
                // Error occurred while creating the File
                Log.d( "LunchApp", "dispatchTakePicture(): error creating file" );
            }

            // Continue only if the File was successfully created
            if( photoFile != null )
            {
                takePictureIntent.putExtra( MediaStore.EXTRA_OUTPUT, Uri.fromFile( photoFile ) );
                startActivityForResult( takePictureIntent, REQUEST_TAKE_PHOTO );
            }
        }
    }

    private String getRealPathFromURI( Context context, Uri contentUri )
    {
        Cursor cursor = null;
        try
        {
            String[] proj = { MediaStore.Images.Media.DATA };
            cursor = context.getContentResolver().query(contentUri,  proj, null, null, null);
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            return cursor.getString(column_index);
        }
        finally
        {
            if (cursor != null)
            {
                cursor.close();
            }
        }
    }

    private File createImageFile() throws IOException
    {
        // Create an image file name
        String timeStamp = new SimpleDateFormat( "yyyyMMdd_HHmmss" ).format( new Date() );
        String imageFileName = "JPEG_" + timeStamp + "_";
        File storageDir = Environment.getExternalStoragePublicDirectory( Environment.DIRECTORY_PICTURES );
        File image = File.createTempFile(
            imageFileName,  /* prefix */
            ".jpg",         /* suffix */
            storageDir      /* directory */
        );

        // Save a file: path for use with ACTION_VIEW intents
        mCurrentPhotoPath = image.getAbsolutePath();
        return image;
    }
}

