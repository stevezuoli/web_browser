package com.duokan.reader.domain.account.oauth;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.apache.http.client.methods.HttpUriRequest;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.duokan.reader.DkApp;
import com.duokan.reader.R;
import com.duokan.reader.domain.account.oauth.OAuthDialog.OAuthCallback;
import com.duokan.reader.domain.account.oauth.evernote.EvernoteSession;
import com.duokan.reader.domain.account.oauth.evernote.EvernoteUtil;
import com.duokan.reader.domain.bookshelf.Annotation;
import com.duokan.reader.domain.bookshelf.Book;
import com.duokan.reader.domain.bookshelf.Comment;
import com.duokan.reader.domain.cloud.DkCloudComment;
import com.duokan.reader.domain.document.ContentEntry;
import com.duokan.reader.ui.general.DkProgressDialog;
import com.duokan.reader.ui.general.DkToast;
import com.evernote.edam.notestore.NoteFilter;
import com.evernote.edam.notestore.NoteMetadata;
import com.evernote.edam.notestore.NoteStore.Client;
import com.evernote.edam.notestore.NotesMetadataList;
import com.evernote.edam.notestore.NotesMetadataResultSpec;
import com.evernote.edam.type.Note;
import com.evernote.edam.type.Notebook;

public class ThirdYinxiang extends ThirdOAuth {
	
	private static final String TAG = ThirdYinxiang.class.getName();
	private EvernoteSession mSession;
	
	public ThirdYinxiang(Activity activity, String thirdName) {
		super(activity, thirdName);
		mSession = EvernoteSession.init(DkApp.get().getCurrentActivity(), 
				thirdName,
				ThirdConstans.YINXIANG_APP_KEY, 
				ThirdConstans.YINGXIANG_APP_SECRET, 
				getBaseServerUrl(), 
				null);
	}
	
	public boolean exchangeUserName() {
		return false;
	};
	@Override
	public boolean supportShareBitmap() {
		return true;
	}
	@Override
	public boolean supportShortUrl(boolean login) {
		return false;
	}
	@Override
	public void oauth(final OAuthCallback callback) {
		mSession.authenticate(getActivity(), callback);
	}
	@Override
	public void queryAccount(final QueryAccountListener listener) {
		assert listener != null;
		if (mSession.isLoggedIn()) {
			listener.onQueryAccountOk();
		} else {
            oauth(new OAuthCallback() {
				@Override
				public void onOauthSuccess() {
					DkToast.makeText(getActivity(), R.string.account_success, Toast.LENGTH_SHORT).show();
					listener.onQueryAccountOk();
				}
				@Override
				public void onOauthFailed() {
					DkToast.makeText(getActivity(), R.string.account_failed, Toast.LENGTH_SHORT).show();
					listener.onQueryAccountError();
				}
				@Override
				public void onGetUserNameFailed() {
					DkToast.makeText(getActivity(), R.string.account_get_name_failed, Toast.LENGTH_SHORT).show();
					listener.onQueryAccountOk();
				}
			});
		}
	}
	/**
	 * 导出
	 * @param title
	 * @param content
	 * @param handler
	 */
	public void output(final String bookid, final String title, final String content, final UpdateHandler handler) {
        final DkProgressDialog mProgressDialog = new DkProgressDialog(getActivity());
        mProgressDialog.setIndeterminate(true);
		mProgressDialog.setCancelable(true);
		mProgressDialog.setMessage(getActivity().getString(R.string.account__oauth__sending));
        mProgressDialog.show();
        
		ThirdSession session = new ThirdSession() {
			
			@Override
			protected void onSessionTry() throws Exception {
				// step1 查询文件夹
				String noteBookName = getActivity().getString(R.string.reading__shared__note_book_name);
				Client noteStore = mSession.createNoteStore();
				List<Notebook> noteBooks = noteStore.listNotebooks(mSession.getAuthToken());
				Notebook duokanNote = null;
				for (Notebook notebook : noteBooks) {
					if (notebook.getName().equals(noteBookName)) {
						duokanNote = notebook;
						break;
					}
				}
                if (ThirdConstans.DEBUG) {
					Log.w(TAG, "duokanNote " + (duokanNote == null));
				}
				// step2 创建文件夹
				if (duokanNote == null) {
					duokanNote = new Notebook();
					duokanNote.setName(noteBookName);
					duokanNote = noteStore.createNotebook(mSession.getAuthToken(), duokanNote);
					if (ThirdConstans.DEBUG) {
						Log.w(TAG, "create noteBook " + (duokanNote == null));
					}
				}
				assert duokanNote != null;
				// step3 查询笔记
				NoteFilter filter = new NoteFilter();
				filter.setNotebookGuid(duokanNote.getGuid());
				filter.setWords(String.format("duokanbookid:%s", bookid));
				NotesMetadataResultSpec resultSpec = new NotesMetadataResultSpec();
				NotesMetadataList notes = noteStore.findNotesMetadata(mSession.getAuthToken(), filter, 0, 1, resultSpec);
				if (ThirdConstans.DEBUG) {
					Log.w(TAG, "search note " + notes.getTotalNotes());
				}
				
				// step4 发表笔记
				Note note = new Note();
				note.setNotebookGuid(duokanNote.getGuid());
				List<String> tag = new LinkedList<String>();
				tag.add(getString(R.string.reading__shared__note_tag));
                note.setTagNames(tag);
                
                String vaildTitle = title.replaceAll("\\p{Cc}\\p{Zl}\\p{Zp}", "");
                if (vaildTitle.length() > 255) {
					vaildTitle = vaildTitle.substring(0, 255);
				}
				note.setTitle(vaildTitle);
				StringBuffer sb = new StringBuffer();
				sb.append(EvernoteUtil.NOTE_PREFIX).append(content).append(EvernoteUtil.NOTE_SUFFIX);
				note.setContent(sb.toString());
				if (notes.getTotalNotes() == 0) {
					// 创建新笔记
					noteStore.createNote(mSession.getAuthToken(), note);
					if (ThirdConstans.DEBUG) {
						Log.w(TAG, "createNote ");
					}
				} else {
					// 更新笔记
					List<NoteMetadata> noteMetadatas = notes.getNotes();
					note.setGuid(noteMetadatas.get(0).getGuid());
					noteStore.updateNote(mSession.getAuthToken(), note);
					Log.w(TAG, "updateNote ");
				}
			}
			
			@Override
			protected void onSessionSucceeded() {
				mProgressDialog.dismiss();
				handler.onUpdateOk();
			}
			
			@Override
			protected void onSessionFailed() {
				mProgressDialog.dismiss();
				handler.onUpdateError();
			}
		};
		session.open();
	}
	public String makeContent(Book book, HashMap<Annotation, ContentEntry> chapter, List<Annotation> annotations) {
		StringBuffer sb = new StringBuffer();
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd"); 
		
		// 头部
		sb.append(makeHeader(book.getItemName(), book.getAuthor()));
		boolean boder = false;
		for (int i = 0; i < annotations.size(); i++) {
			Annotation annotation = annotations.get(i);
			boder = true;
			if (chapter.containsKey(annotation)) {
				// 该章节的第一个
				boder = false;
				sb.append(String.format(getActivity().getString(R.string.reading__shared__chapter), chapter.get(annotation).getTitle()));
			}
			if (i == annotations.size()) {
				// 最后一条, 不用画分割线
				boder = false;
			}
			String date = formatter.format(new Date(annotation.getAddedDate()));
			Comment comment = (Comment) annotation;
			String colorString = Color.red(comment.getHighlightColor()) + "," + Color.green(comment.getHighlightColor()) + "," + Color.blue(comment.getHighlightColor());
			String note = comment.getNoteText();
			sb.append(String.format(getActivity().getString(R.string.reading__shared__comment), 
					boder == false ? "" : getActivity().getString(R.string.reading__shared__comment_split),
					colorString,
					date,
					escapeIllegalCharacters(comment.getSample(false)),
					TextUtils.isEmpty(note) ? "" : String.format(getActivity().getString(R.string.reading__shared__note), escapeIllegalCharacters(note))));
		}
		// 底部
		sb.append(String.format(getActivity().getString(R.string.reading__shared__foot), book.getBookUuid()));
		
		return sb.toString();
	}
	public String makeContent(String bookUuid, String bookName, String author, List<DkCloudComment> annotations) {
		StringBuffer sb = new StringBuffer();
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd"); 
		
		// 头部
		sb.append(makeHeader(bookName, author));
		
		for (int i = 0; i < annotations.size(); i++) {
			DkCloudComment annotation = annotations.get(i);
			String date = formatter.format(annotation.getCreationDate());
			String colorString = Color.red(annotation.getHighlightColor()) + "," + Color.green(annotation.getHighlightColor()) + "," + Color.blue(annotation.getHighlightColor());
			String note = annotation.getNoteText();
			sb.append(String.format(getActivity().getString(R.string.reading__shared__comment), 
					getActivity().getString(R.string.reading__shared__comment_split),
					colorString,
					date,
					escapeIllegalCharacters(annotation.getSample()),
					TextUtils.isEmpty(note) ? "" : String.format(getActivity().getString(R.string.reading__shared__note), escapeIllegalCharacters(note))));
		}
		
		// 底部
		sb.append(String.format(getActivity().getString(R.string.reading__shared__foot), bookUuid));
		return sb.toString();
	}
	public String makeHeader(String bookName, String author) {
		StringBuffer sb = new StringBuffer();
		
		sb.append(getActivity().getString(R.string.reading__shared__title)).append(bookName).append(getActivity().getString(R.string.reading__shared__title_suffix));
		if (TextUtils.isEmpty(author) == false) {
			sb.append(String.format(getActivity().getString(R.string.reading__shared__author), author));
		}
		return sb.toString();
	}
    public String makeTitle(String bookName, String author) {
    	StringBuffer sb = new StringBuffer();
		
		sb.append(String.format(getActivity().getString(R.string.reading__shared__note_title), bookName));
		if (TextUtils.isEmpty(author) == false) {
			sb.append(String.format(getActivity().getString(R.string.reading__shared__note_title_author), author));
		}
		return sb.toString();
    }
	/**
	 * 该函数不可用
	 */
	@Override
	public void update(String content, Bitmap bitmap, UpdateHandler handler) {
		assert false;
		// 假设印象笔记只用于导出, 而不用于分享.
	}

	@Override
	protected HttpUriRequest makeShortenUrlRequest(String url) throws Exception {
		return null;
	}

	@Override
	protected String handleShortenUrlResponse(String response) throws Exception {
		return null;
	}

	// ### 实现函数  ###
	private String getBaseServerUrl() {
		if (ThirdConstans.YINXIANG_NAME.equals(mThirdName)) {
			// 印象笔记
			return EvernoteSession.HOST_CHINA;
		} else if (ThirdConstans.EVERNOTE_NAME.equals(mThirdName)) {
			// evernote
			return EvernoteSession.HOST_PRODUCTION;
		} else {
			assert false;
			return null;
		}
//		return EvernoteSession.HOST_SANDBOX;
	}

	// ========= 使用第三方sdk无需重载以下方法  ==================
	@Override
	public String makeAuthorizeRequestUrl() {
		assert false;
		return null;
	}
	@Override
	public int checkAuthorize(String url) {
		assert false;
		return 0;
	}
	@Override
	protected HttpUriRequest makeAccessRequest() throws Exception {
		assert false;
		return null;
	}
	@Override
	protected boolean handleAccessResponse(String response) {
		assert false;
		return false;
	}
	@Override
	protected HttpUriRequest makeFetchUserInfoRequest() {
		assert false;
		return null;
	}
	@Override
	protected boolean handleUserInfoResponse(String response) {
		assert false;
		return false;
	}
	@Override
	protected HttpUriRequest makeUpdateRequest(String content, Bitmap bitmap) throws Exception {
		assert false;
		return null;
	}
	@Override
	protected boolean handleUpdateResponse(String response) {
		assert false;
		return false;
	}
	private String escapeIllegalCharacters(String src) {
		String result = src.replace("&", "&amp;");
		result = result.replace("<", "&lt;");
		result = result.replace(">", "&gt;");
		result = result.replace("'", "&apos;");
		result = result.replace("\"", "&quot;");
		return result;
	}
}
