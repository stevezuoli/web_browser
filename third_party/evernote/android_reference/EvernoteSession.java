/*
 * Copyright 2012 Evernote Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.duokan.reader.domain.account.oauth.evernote;

import java.io.File;
import java.util.Locale;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;

import com.duokan.reader.domain.account.oauth.OAuthDialog.OAuthCallback;
import com.duokan.reader.domain.account.oauth.TokenStore;
import com.evernote.edam.notestore.NoteStore;
import com.evernote.edam.userstore.UserStore;
import com.evernote.thrift.protocol.TBinaryProtocol;
import com.evernote.thrift.transport.TTransportException;

/**
 * Represents a session with the Evernote web service API. Used to authenticate
 * to the service via OAuth and obtain NoteStore.Client objects, which are used
 * to make authenticated API calls.
 *
 * To use EvernoteSession, first initialize the EvernoteSession singleton and
 * initiate authentication at an appropriate time:
 * <pre>
 *   EvernoteSession session = EvernoteSession.init(...);
 *   if (!session.isLoggedIn()) {
 *     session.authenticate(...);
 *   }
 * </pre>
 *
 * When authentication completes, you will want to trap the result in onActivityResult
 * to see if it was successful:
 * <pre>
 *   public void onActivityResult(int requestCode, int resultCode, Intent data) {
 *     super.onActivityResult(requestCode, resultCode, data);
 *       switch(requestCode) {
 *       case EvernoteSession.REQUEST_CODE_OAUTH:
 *         if (resultCode == Activity.RESULT_OK) {
 *           // OAuth login was successful, do the appropriate thing for your app
 *         }
 *         break;
 *     }
 *   }
 * </pre>
 *
 * Later, you can make any Evernote API calls that you need by obtaining a
 * NoteStore.Client from the session and using the session's auth token:
 * <pre>
 *   NoteStore.client noteStore = session.createNoteStore();
 *   Notebook notebook = noteStore.getDefaultNotebook(session.getAuthToken());
 * </pre>
 */
public class EvernoteSession {

  // Standard hostnames
  public static final String HOST_SANDBOX = "sandbox.evernote.com";
  public static final String HOST_PRODUCTION = "www.evernote.com";
  public static final String HOST_CHINA = "app.yinxiang.com";

  public static final int REQUEST_CODE_OAUTH = 1010101;

  private String mEvernoteName;
  private String mConsumerKey;
  private String mConsumerSecret;
//  private String mEvernoteHost;
  private String mUserAgentString;

  private AuthenticationResult mAuthenticationResult;
  private File mDataDirectory;

  private static EvernoteSession sInstance = null;

  /**
   * Use to acquire a singleton instance of the EvernoteSession for authentication.
   * If the singleton has already been initialized, the existing instance will
   * be returned (and the parameters passed to this method will be ignored).
   *
   * @param ctx
   * @param consumerKey The consumer key portion of your application's API key.
   * @param consumerSecret The consumer secret portion of your application's API key.
   * @param evernoteHost The hostname for the Evernote service instance that you wish
   * to use. Development and testing is typically performed against {@link #HOST_SANDBOX}.
   * The production Evernote service is {@link #HOST_PRODUCTION}. The production Yinxiang Biji
   * (Evernote China) service is {@link #HOST_CHINA}.
   * @param dataDir the data directory to store evernote files
   *
   * @return The EvernoteSession singleton instance.
   */
  public static EvernoteSession init(Context ctx,
		  							String evernoteName,
                                     String consumerKey,
                                     String consumerSecret,
                                     String evernoteHost,
                                     File dataDir) {
    // are set to meaningless values (i.e. "Your consumer key")
	sInstance = new EvernoteSession(ctx, evernoteName, consumerKey, consumerSecret, evernoteHost, dataDir);
    return sInstance;
  }

  /**
   * Used to access the initialized EvernoteSession singleton instance.
   *
   * @return The previously initialized EvernoteSession instance,
   * or null if {@link #init(Context, String, String, String, File)} has not been called yet.
   */
  public static EvernoteSession getSession() {
    return sInstance;
  }

  /**
   * Private constructor.
 * @param evernoteHost2 
   */
  private EvernoteSession(Context ctx, String evernoteName, String consumerKey, String consumerSecret, String evernoteHost, File dataDir) {
	mEvernoteName = evernoteName;
    mConsumerKey = consumerKey;
    mConsumerSecret = consumerSecret;
//    mEvernoteHost = evernoteHost;
    initUserAgentString(ctx);

    mAuthenticationResult = getAuthenticationResult(ctx);
    if(dataDir != null) {
      mDataDirectory = dataDir;
    } else {
      mDataDirectory = ctx.getFilesDir();
    }
  }

  /**
   * Restore an AuthenticationResult from shared preferences.
   * @return The restored AuthenticationResult, or null if the preferences
   * did not contain the required information.
   */
  private AuthenticationResult getAuthenticationResult(Context context) {
    return TokenStore.getInstence().getAuthenticationResult(context, mEvernoteName);
  }

  /**
   * Check whether the session has valid authentication information
   * that will allow successful API calls to be made.
   */
  public boolean isLoggedIn() {
    return mAuthenticationResult != null;
  }

  /**
   * Clear all stored authentication information.
   */
// suppress lint check on Editor.apply()
  public void logOut(Context ctx, String name) {
    mAuthenticationResult = null;

    TokenStore.getInstence().logOut(ctx, name);

    // evernote.com cookies.
//    CookieSyncManager.createInstance(ctx);
//    CookieManager cookieManager = CookieManager.getInstance();
//    cookieManager.removeAllCookie();
  }

  /**
   * Get the authentication token that is used to make API calls
   * though a NoteStore.Client.
   *
   * @return the authentication token, or null if {@link #isLoggedIn()}
   * is false.
   */
  public String getAuthToken() {
    if (mAuthenticationResult != null) {
      return mAuthenticationResult.getAuthToken();
    } else {
      return null;
    }
  }

  /**
   * Get the authentication information returned by a successful
   * OAuth authentication to the Evernote web service.
   */
  public AuthenticationResult getmAuthenticationResult() {
    return mAuthenticationResult;
  }

  /**
   * Create a new NoteStore client. Each call to this method will return
   * a new NoteStore.Client instance. The returned client can be used for any
   * number of API calls, but is NOT thread safe.
   *
   * @throws IllegalStateException if @link #isLoggedIn() is false.
   * @throws TTransportException if an error occurs setting up the
   * connection to the Evernote service.
   */
  public NoteStore.Client createNoteStore() throws TTransportException {
    if (!isLoggedIn()) {
      throw new IllegalStateException();
    }
    TEvernoteHttpClient transport =
      new TEvernoteHttpClient(mAuthenticationResult.getNoteStoreUrl(),
          mUserAgentString, mDataDirectory);
    TBinaryProtocol protocol = new TBinaryProtocol(transport);
    return new NoteStore.Client(protocol, protocol);
  }

  /**
   * Create a new UserStore client. Each call to this method will return
   * a new UserStore.Client instance. The returned client can be used for any
   * number of API calls, but is NOT thread safe.
 * @param host 
   *
   * @throws IllegalStateException if @link #isLoggedIn() is false.
   * @throws TTransportException if an error occurs setting up the
   * connection to the Evernote service.
   */
  public UserStore.Client createUserStore(String host)  throws TTransportException {
    String url = "";
    if (!host.startsWith("http")) {
      url = host.contains(":") ? "http://" : "https://";
    }
    url += host + "/edam/user";

    TEvernoteHttpClient transport =
        new TEvernoteHttpClient(url, mUserAgentString, mDataDirectory);

    TBinaryProtocol protocol = new TBinaryProtocol(transport);
    return new UserStore.Client(protocol, protocol);

  }

  /**
   * Construct a user-agent string based on the running application and
   * the device and operating system information. This information is
   * included in HTTP requests made to the Evernote service and assists
   * in measuring traffic and diagnosing problems.
   */
  private void initUserAgentString(Context ctx) {
    // com.evernote.sample Android/216817 (en); Android/4.0.3; Xoom/15;"

    String packageName = null;
    int packageVersion = 0;
    try {
      packageName= ctx.getPackageName();
      packageVersion = ctx.getPackageManager().getPackageInfo(packageName, 0).versionCode;

    } catch (PackageManager.NameNotFoundException e) {
      Log.e("tag", e.getMessage());
    }

    String userAgent = packageName+ " Android/" +packageVersion;

    Locale locale = java.util.Locale.getDefault();
    if (locale == null) {
      userAgent += " ("+Locale.US+");";
    } else {
      userAgent += " (" + locale.toString()+ "); ";
    }
    userAgent += "Android/"+Build.VERSION.RELEASE+"; ";
    userAgent +=
        Build.MODEL + "/" + Build.VERSION.SDK_INT + ";";
    mUserAgentString = userAgent;
  }

  /**
   * Get the user-agent header value that will be included in all
   * HTTP requests made to the Evernote service.
   */
  public String getUserAgentString() {
    return mUserAgentString;
  }

  /**
   * Start the OAuth authentication process.
   *
   */
  public void authenticate(Activity ctx, OAuthCallback callback) {
	  EvernoteOAuthDialog dialog = new EvernoteOAuthDialog(ctx, mConsumerKey, mConsumerSecret, callback);
	  dialog.show();
	  dialog.start();
	  
    // Create an activity that will be used for authentication
//    Intent intent = new Intent(ctx, EvernoteOAuthActivity.class);
//    intent.putExtra(EvernoteOAuthActivity.EXTRA_EVERNOTE_HOST, mEvernoteHost);
//    intent.putExtra(EvernoteOAuthActivity.EXTRA_CONSUMER_KEY, mConsumerKey);
//    intent.putExtra(EvernoteOAuthActivity.EXTRA_CONSUMER_SECRET, mConsumerSecret);
//
//    if (ctx instanceof Activity) {
//      //If this is being called from an activity, an activity can register for the result code
//      ((Activity)ctx).startActivityForResult(intent, REQUEST_CODE_OAUTH);
//    } else {
//      //If this is being called from a service, the refresh will be handled manually
//      intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//      ctx.startActivity(intent);
//    }
  }

  /**
   * Called upon completion of the OAuth process to save resulting authentication
   * information into the application's SharedPreferences, allowing it to be reused
   * later.
   *
   * @param ctx Application Context or activity
   * @param authToken The authentication information returned at the end of a
   * successful OAuth authentication.
   */
  // suppress lint check on Editor.apply()
  protected boolean persistAuthenticationToken(Context ctx, String appName, EvernoteAuthToken authToken) {
    if (ctx == null || authToken == null) {
      return false;
    }
    TokenStore.getInstence().setAuthenticationToken(ctx, appName, authToken);
    
    mAuthenticationResult =
        new AuthenticationResult(
            authToken.getToken(),
            authToken.getNoteStoreUrl(),
            authToken.getWebApiUrlPrefix(),
            authToken.getUserId());
    return true;
  }
}
