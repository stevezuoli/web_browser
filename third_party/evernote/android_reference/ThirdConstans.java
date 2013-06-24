package com.duokan.reader.domain.account.oauth;


public class ThirdConstans {

	public static final String EXTRA_THIRD_CHOICE = "third.choice";
	public static final String EXTRA_SHARE_AFTER_OAUTH = "third.share.after.oauth";
	public static final String EXTRA_SHARE_CONTENT = "third.share.content";
	public static final String EXTRA_SHARE_PIC = "third.share.content.pic";
    public static final String EXTRA_SHARE_BOOK_TITLE = "third.share.book.title";
    public static final String EXTRA_SHARE_BOOK_AUTHOR = "third.share.book.author";
    
	// ============== 新浪 ============ start
    public static final String SINA_NAME = "sina";
	public static final String SINA_APP_KEY = "2347354897";
	public static final String SINA_APP_SECRET = "9abe59b8a186bc42e699f05c9e5256e6";
    public static final String SINA_AUTHORIZE_URL = "https://api.weibo.com/oauth2/authorize";
    public static final String SINA_ACCESS_URL = "https://api.weibo.com/oauth2/access_token";
    public static final String SINA_CALLBACK = "https://api.weibo.com/oauth2/default.html";
    public static final String SINA_USER_INFO_URL = "https://api.weibo.com/2/users/show.json";
    public static final String SINA_SHORT_URL = "https://api.weibo.com/2/short_url/shorten.json";
    /**
     * 不带图片的微博
     */
    public static final String SINA_UPDATE_URL = "https://api.weibo.com/2/statuses/update.json";
    /**
     * 带图片的微博
     */
    public static final String SINA_UPLOAD_URL = "https://upload.api.weibo.com/2/statuses/upload.json";
	// ============== 新浪 ============ end    
    
    // ============== 腾讯微博 ========= start
    public static final String QQ_NAME = "qq";
	public static final String QQ_APP_KEY = "801003510";
	public static final String QQ_APP_SECRET = "7f9ae2357d3f77bddf8b396db5a4fcaf";
	public static final String QQ_AUTHORIZE_URL = "https://open.t.qq.com/cgi-bin/oauth2/authorize?";
	public static final String QQ_CALLBACK = "https://open.t.qq.com/cgi-bin/oauth2/default.html";
    public static final String QQ_ACCESS_URL = "https://open.t.qq.com/cgi-bin/oauth2/access_token?";
    public static final String QQ_SHORT_URL = "https://open.t.qq.com/api/short_url/shorten"; 
    /**
     * 不带图片的微博
     */
    public static final String QQ_UPDATE_URL = "https://open.t.qq.com/api/t/add";
    /**
     * 带图片的微博
     */
    public static final String QQ_UPLOAD_URL = "https://open.t.qq.com/api/t/add_pic";
    // ============== 腾讯微博 ========= end

    // ============== 人人网 ========= start
    public static final String RENREN_NAME = "renren";
    public static final String RENREN_APP_ID = "908147ddb76e4b25b89ea1c3fc4a5d86";
	public static final String RENREN_APP_SECRET = "d09d4f48231242b99d164e0da1f8292b";
    public static final String RENREN_AUTHORIZE_URL = "https://graph.renren.com/oauth/authorize?";
	public static final String RENREN_ACCESS_URL = "https://graph.renren.com/oauth/token";
	public static final String RENREN_CALLBACK = "http://graph.renren.com/oauth/login_success.html";
	public static final String RENREN_SCOPE = "status_update";
	/**
	 * 发表状态
	 */
	public static final String RENREN_SERVER = "http://api.renren.com/restserver.do";
    // ============== 人人网 ========= end
    
    // ============== 开心网 ========= start
    public static final String KAIXIN_NAME = "kaixin";
	public static final String KAIXIN_APP_KEY = "82234591565955fafe7cfe59bd48b8b1";
	public static final String KAIXIN_APP_SECRET = "3ef42b0759f1be413bd521c01c13b051";
	public static final String KAIXIN_AUTHORIZE_URL = "http://api.kaixin001.com/oauth2/authorize?";
	public static final String KAIXIN_ACCESS_URL = "https://api.kaixin001.com/oauth2/access_token";
	public static final String KAIXIN_USER_INFO_URL = "https://api.kaixin001.com/users/me.json";
	public static final String KAIXIN_CALLBACK = "oob";
	public static final String KAIXIN_SCOPE = "create_records";
    /**
     * 发表状态
     */
	public static final String KAIXIN_SERVER = "https://api.kaixin001.com/records/add.json";
    // ============== 开心网 ========= end
	
	// ==============印象笔记 区别于evernote =========== start
	public static final String YINXIANG_NAME = "yinxiang";
	public static final String EVERNOTE_NAME = "evernote";
	public static final String YINXIANG_APP_KEY = "pkunetspy-0221"; //"pangpengde";
	public static final String YINGXIANG_APP_SECRET = "905a975954fa7a3d"; //"6351e3edb548658e";
    
	// ==============印象笔记           ====================== end
    public static final boolean DEBUG = false;
    
}
