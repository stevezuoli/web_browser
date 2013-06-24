package com.duokan.reader.domain.account.oauth.evernote;

import org.scribe.builder.ServiceBuilder;
import org.scribe.builder.api.EvernoteApi;
import org.scribe.model.Token;
import org.scribe.model.Verifier;
import org.scribe.oauth.OAuthService;

import android.app.Activity;
import android.app.Dialog;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.DecelerateInterpolator;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;

import com.duokan.reader.R;
import com.duokan.reader.domain.account.oauth.OAuthDialog.OAuthCallback;
import com.duokan.reader.domain.account.oauth.ThirdConstans;
import com.duokan.reader.domain.account.oauth.TokenStore;
import com.duokan.reader.ui.general.DkHeaderView;
import com.duokan.reader.ui.general.DkProgressDialog;
import com.duokan.reader.ui.general.OverlappedDialog;
import com.duokan.reader.ui.general.Rotate3dAnimation;
import com.duokan.reader.ui.general.ViewController;
import com.evernote.edam.error.EDAMSystemException;
import com.evernote.edam.error.EDAMUserException;
import com.evernote.edam.type.User;
import com.evernote.edam.userstore.Constants;
import com.evernote.edam.userstore.UserStore;
import com.evernote.thrift.TException;
import com.evernote.thrift.transport.TTransportException;

public class EvernoteOAuthDialog extends OverlappedDialog {

	private final static int SUCCESS = 1;
	private final static int FAILED = 2;
	private final static int INFO_FAILED = 3; 
	private String mConsumerKey = null;
	private String mConsumerSecret = null;
	private FrameLayout mRootView = null;
	private DkProgressDialog mProgressDialog = null;
	private OAuthCallback mCallback = null;
    private EvernoteOAuthController mYinxiangController = null;
    private EvernoteOAuthController mEvernoteController = null;
    private ViewController mCurController;
    private Animation mShowAnimation;
    private Animation mDisAnimation;
    
	public EvernoteOAuthDialog(Activity context, String consumerKey, String consumerSecret, OAuthCallback callback) {
		super(context, false, true, R.style.general__account_dialog_anim);
        
		mConsumerKey = consumerKey;
		mConsumerSecret = consumerSecret;
		mCallback = callback;
		
		mRootView = new FrameLayout(getContext());
		setContentView(mRootView);
		
		mProgressDialog = new DkProgressDialog(getContext());
		mProgressDialog.setIndeterminate(true);
		mProgressDialog.setMessage(getContext().getString(R.string.account__oauth__loading));
		
		mYinxiangController = new EvernoteOAuthController(context, this, ThirdConstans.YINXIANG_NAME);
		mCurController = mYinxiangController;
		mEvernoteController = new EvernoteOAuthController(context, this, ThirdConstans.EVERNOTE_NAME);
		
		mRootView.addView(mYinxiangController.getContentView(), new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		mRootView.addView(mEvernoteController.getContentView(), new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		mEvernoteController.getContentView().setVisibility(View.GONE);
		
		mShowAnimation = AnimationUtils.loadAnimation(getContext(), R.anim.general__shared__alpha_show);
		mDisAnimation = AnimationUtils.loadAnimation(getContext(), R.anim.general__shared__alpha_dismiss);
	}
    
	public void start() {
		mYinxiangController.start();
	}
	
    private void doChange() {
    	if (mCurController == mYinxiangController) {
    		mYinxiangController.stop();
//			applyRotation(0, 90, mEvernoteController.getContentView(), mYinxiangController.getContentView());
    		mShowAnimation.setAnimationListener(new AnimationListener() {
				@Override
				public void onAnimationStart(Animation animation) {
				}
				@Override
				public void onAnimationRepeat(Animation animation) {
				}
				@Override
				public void onAnimationEnd(Animation animation) {
					mEvernoteController.getContentView().setVisibility(View.VISIBLE);
					mShowAnimation.setAnimationListener(null);
				}
			});
    		mDisAnimation.setAnimationListener(new AnimationListener() {
				@Override
				public void onAnimationStart(Animation animation) {
					mEvernoteController.getContentView().startAnimation(mShowAnimation);
				}
				@Override
				public void onAnimationRepeat(Animation animation) {
				}
				@Override
				public void onAnimationEnd(Animation animation) {
					mYinxiangController.getContentView().setVisibility(View.GONE);
                    mDisAnimation.setAnimationListener(null);
				}
			});
            mYinxiangController.getContentView().startAnimation(mDisAnimation);
            mCurController = mEvernoteController;
            mEvernoteController.start();
		} else {
			mEvernoteController.stop();
//			applyRotation(0, 90, mYinxiangController.getContentView(), mEvernoteController.getContentView());
			mShowAnimation.setAnimationListener(new AnimationListener() {
				@Override
				public void onAnimationStart(Animation animation) {
				}
				@Override
				public void onAnimationRepeat(Animation animation) {
				}
				@Override
				public void onAnimationEnd(Animation animation) {
					mYinxiangController.getContentView().setVisibility(View.VISIBLE);
					mShowAnimation.setAnimationListener(null);
				}
			});
    		mDisAnimation.setAnimationListener(new AnimationListener() {
				@Override
				public void onAnimationStart(Animation animation) {
					mYinxiangController.getContentView().startAnimation(mShowAnimation);
				}
				@Override
				public void onAnimationRepeat(Animation animation) {
				}
				@Override
				public void onAnimationEnd(Animation animation) {
					mEvernoteController.getContentView().setVisibility(View.GONE);
                    mDisAnimation.setAnimationListener(null);
				}
			});
    		mEvernoteController.getContentView().startAnimation(mDisAnimation);
			mCurController = mYinxiangController;
			mYinxiangController.start();
		}
    }
//    private void applyRotation(float start, float end, final View visibleView, final View goneView){
//		final float centerX = mRootView.getWidth() / 2.0f;
//        final float centerY = mRootView.getHeight() / 2.0f;
//        Rotate3dAnimation rotation = new Rotate3dAnimation(start, end, centerX, centerY, 200.0f, true);
//        rotation.setDuration(500);
//        rotation.setInterpolator(new AccelerateInterpolator());
//        rotation.setAnimationListener(new AnimationListener() {
//			@Override
//			public void onAnimationEnd(Animation arg0) {
//				mRootView.post(new Runnable() {
//					@Override
//					public void run() {
//						goneView.setVisibility(View.GONE);
//						visibleView.setVisibility(View.VISIBLE);
//						Rotate3dAnimation rotatiomAnimation = new Rotate3dAnimation(-90, 0, centerX, centerY, 200.0f, false);
//						rotatiomAnimation.setDuration(500);
//						rotatiomAnimation.setInterpolator(new DecelerateInterpolator());
//						
//						mRootView.startAnimation(rotatiomAnimation);
//					}
//				});
//			}
//			@Override
//			public void onAnimationRepeat(Animation arg0) {
//			}
//			@Override
//			public void onAnimationStart(Animation arg0) {
//			}
//        });
//        mRootView.startAnimation(rotation);
//	}
    private void exit(ViewController controller, final int success) {
		if (mCurController == controller) {
			mProgressDialog.dismiss();
            switch (success) {
			case SUCCESS:
				mCallback.onOauthSuccess();
				break;
			case INFO_FAILED:
				mCallback.onGetUserNameFailed();
				break;
			case FAILED:
			default:
				mCallback.onOauthFailed();
				break;
			}
            dismiss();
		}
	}	
    @Override
    public void dismiss() {
        mYinxiangController.stop();
        mEvernoteController.stop();
    	if (mProgressDialog != null) {
			mProgressDialog.dismiss();
		}
    	super.dismiss();
    }
	
	public class EvernoteOAuthController extends ViewController {

		private final String TAG = EvernoteOAuthController.class.getName();
		
		final static String EXTRA_EVERNOTE_HOST = "EVERNOTE_HOST";
		final static String EXTRA_CONSUMER_KEY = "CONSUMER_KEY";
		final static String EXTRA_CONSUMER_SECRET = "CONSUMER_SECRET";
		final static String EXTRA_REQUEST_TOKEN = "REQUEST_TOKEN";
		final static String EXTRA_REQUEST_TOKEN_SECRET = "REQUEST_TOKEN_SECRET";
		
		private String mEvernoteName = null;
		private String mEvernoteHost = null;
		private String mRequestToken = null;
		private String mRequestTokenSecret = null;
		
		private Dialog mDialog;
		private DkHeaderView mHeaderView;
		private WebView mWebView;
		
		private AsyncTask mBeginAuthSyncTask = null;
		private AsyncTask mCompleteAuthSyncTask = null;
		private AsyncTask mUserInfoAsyncTask = null;
		
		private WebViewClient mWebViewClient = new WebViewClient() {

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				Uri uri = Uri.parse(url);
				if (uri.getScheme().equals(getCallbackScheme())) {
					if (mCompleteAuthSyncTask == null) {
						mCompleteAuthSyncTask = new CompleteAuthAsyncTask().execute(uri);
					}
					return true;
				}
				return super.shouldOverrideUrlLoading(view, url);
			}
		};
		
		public EvernoteOAuthController(Activity context, Dialog dialog, String evernoteName) {
			super(context);
			
			mEvernoteName = evernoteName;
			mEvernoteHost = getBaseServerUrl();
            mDialog = dialog;
			
			setContentView(R.layout.account__oauth_web_view);
			
			mHeaderView = (DkHeaderView) findViewById(R.id.account__oauth_web_view__title);
	        mHeaderView.setBackgroundResource(R.drawable.general__dk_header_view__background3);
	        mHeaderView.setOnBackListener(new DkHeaderView.OnBackListener() {
				@Override
				public boolean onBack() {
                    stop();
                    mDialog.dismiss();
					return true;
				}
			});
	        initHeaderView();
			
			mWebView = (WebView) findViewById(R.id.account__oauth_web_view__web);
			mWebView.setWebViewClient(mWebViewClient);
			mWebView.getSettings().setJavaScriptEnabled(true);
			
			if (TextUtils.isEmpty(mConsumerKey) || TextUtils.isEmpty(mConsumerSecret)) {
				mDialog.cancel();
				return;
			}
		}

        public void start() {
        	mBeginAuthSyncTask = new BeginAuthAsyncTask().execute();
        }
        public void stop() {
        	mWebView.stopLoading();
            mWebView.clearView();
            mWebView.invalidate();
        	if (mBeginAuthSyncTask != null) {
				mBeginAuthSyncTask.cancel(true);
			}
        	if (mCompleteAuthSyncTask != null) {
				mCompleteAuthSyncTask.cancel(true);
			}
        	if (mUserInfoAsyncTask != null) {
				mUserInfoAsyncTask.cancel(true);
			}
        }
        
		private String getCallbackScheme() {
			return "en-oauth";
		}
		private OAuthService createService() {
			OAuthService builder = null;
			@SuppressWarnings("rawtypes")
			Class apiClass = null;

			if (mEvernoteHost.equals(EvernoteSession.HOST_SANDBOX)) {
				apiClass = EvernoteApi.Sandbox.class;
			} else if (mEvernoteHost.equals(EvernoteSession.HOST_PRODUCTION)) {
				apiClass = EvernoteApi.class;
			} else if (mEvernoteHost.equals(EvernoteSession.HOST_CHINA)) {
				apiClass = YinxiangApi.class;
			} else {
				throw new IllegalArgumentException("Unsupported Evernote host: " + mEvernoteHost);
			}
			builder = new ServiceBuilder().provider(apiClass).apiKey(mConsumerKey).apiSecret(mConsumerSecret)
					.callback(getCallbackScheme() + "://callback").build();

			return builder;
		}
		private String getBaseServerUrl() {
			if (TextUtils.equals(ThirdConstans.YINXIANG_NAME, mEvernoteName)) {
				// 印象笔记
				return EvernoteSession.HOST_CHINA;
			} else if (TextUtils.equals(ThirdConstans.EVERNOTE_NAME, mEvernoteName)) {
				// evernote
				return EvernoteSession.HOST_PRODUCTION;
			} else {
				assert false;
				return null;
			}
//			return EvernoteSession.HOST_SANDBOX;
		}
		private void initHeaderView() {
	    	if (TextUtils.equals(ThirdConstans.YINXIANG_NAME, mEvernoteName)) {
				// 印象笔记
	    		mHeaderView.setLeftTitle(R.string.account__oauth_web_view__title);
	            mHeaderView.clearRightButtonView();
	    		mHeaderView.addRightButtonView(getString(R.string.account__oauth_web_view__change1)).setOnClickListener(new View.OnClickListener() {
					@Override
					public void onClick(View v) {
						doChange();
					}
				});
			} else if (TextUtils.equals(ThirdConstans.EVERNOTE_NAME, mEvernoteName)) {
				// evernote
				mHeaderView.setLeftTitle(R.string.account__oauth_web_view__title1);
				mHeaderView.clearRightButtonView();
	    		mHeaderView.addRightButtonView(getString(R.string.account__oauth_web_view__change)).setOnClickListener(new View.OnClickListener() {
					@Override
					public void onClick(View v) {
						doChange();
					}
				});
			}
	    }
	    
		private class BeginAuthAsyncTask extends AsyncTask<Void, Void, String> {

			@Override
			protected void onPreExecute() {
				mProgressDialog.show();
			}

			@Override
			protected String doInBackground(Void... params) {
				String url = null;
				try {
					OAuthService service = createService();

					EvernoteSession session = EvernoteSession.getSession();
					if (session != null) {
						if (!session.createUserStore(mEvernoteHost).checkVersion(session.getUserAgentString(),
								Constants.EDAM_VERSION_MAJOR, Constants.EDAM_VERSION_MINOR)) {
							Log.e(TAG, "Evernote API version " + Constants.EDAM_VERSION_MAJOR + "."
									+ Constants.EDAM_VERSION_MINOR + " is no longer supported!");
							return null;
						}
					}

					Log.i(TAG, "Retrieving OAuth request token...");
					Token reqToken = service.getRequestToken();
					mRequestToken = reqToken.getToken();
					mRequestTokenSecret = reqToken.getSecret();

					Log.i(TAG, "Redirecting user for authorization...");
					url = service.getAuthorizationUrl(reqToken);
				} catch (Exception ex) {
					Log.e(TAG, "Failed to obtain OAuth request token", ex);
				}
				return url;
			}

			@Override
			protected void onPostExecute(String url) {
				mProgressDialog.dismiss();
				if (TextUtils.isEmpty(url) == false) {
					mWebView.loadUrl(url);
				} else {
					exit(EvernoteOAuthController.this, FAILED);
				}
			}
		}
		private class CompleteAuthAsyncTask extends AsyncTask<Uri, Void, EvernoteAuthToken> {

			@Override
			protected void onPreExecute() {
				mProgressDialog.show();
			}

			@Override
			protected EvernoteAuthToken doInBackground(Uri... uris) {
				EvernoteAuthToken authToken = null;
				if (uris == null || uris.length == 0) {
					return null;
				}
				Uri uri = uris[0];

				if (!TextUtils.isEmpty(mRequestToken)) {
					OAuthService service = createService();
					String verifierString = uri.getQueryParameter("oauth_verifier");
					if (TextUtils.isEmpty(verifierString)) {
						Log.i(TAG, "User did not authorize access");
					} else {
						Verifier verifier = new Verifier(verifierString);
						Log.i(TAG, "Retrieving OAuth access token...");
						try {
							Token reqToken = new Token(mRequestToken, mRequestTokenSecret);
							authToken = new EvernoteAuthToken(service.getAccessToken(reqToken, verifier));
						} catch (Exception ex) {
							Log.e(TAG, "Failed to obtain OAuth access token", ex);
						}
					}
				} else {
					Log.d(TAG, "Unable to retrieve OAuth access token, no request token");
				}

				return authToken;
			}

			@Override
			protected void onPostExecute(EvernoteAuthToken authToken) {
				mProgressDialog.dismiss();
				if (EvernoteSession.getSession() == null) {
					exit(EvernoteOAuthController.this, FAILED);
				} else {
					EvernoteSession.getSession().persistAuthenticationToken(getContext(), mEvernoteName, authToken);
					mUserInfoAsyncTask = new UserInfoAsyncTask(); 
					mUserInfoAsyncTask.execute();
				}
			}
		}
		
		private class UserInfoAsyncTask extends AsyncTask<Object, Void, User> {

			@Override
			protected User doInBackground(Object... params) {
				User user = null;
				try {
					UserStore.Client client = EvernoteSession.getSession().createUserStore(mEvernoteHost);
					user = client.getUser(EvernoteSession.getSession().getAuthToken());
				} catch (TTransportException e) {
					e.printStackTrace();
				} catch (EDAMUserException e) {
					e.printStackTrace();
				} catch (EDAMSystemException e) {
					e.printStackTrace();
				} catch (TException e) {
					e.printStackTrace();
				}
				return user;
			}
			
			@Override
			protected void onPostExecute(User result) {
				if (result != null) {
					String name = TextUtils.isEmpty(result.getName()) ? result.getUsername() : result.getName();
					TokenStore.getInstence().setUserName(getContext(), mEvernoteName, name);
					exit(EvernoteOAuthController.this, SUCCESS);
				} else {
					exit(EvernoteOAuthController.this, INFO_FAILED);
				}
			}
			
		}
	}
}
