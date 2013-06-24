/*
 * Copyright 2012 Evernote Corporation
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

/**
 * A container class for the results of a successful OAuth authorization with
 * the Evernote service.
 */
public class AuthenticationResult {

  private String authToken;
  private String noteStoreUrl;
  private String webApiUrlPrefix;
  private int userId;

  /**
   * Create a new AuthenticationResult.
   *
   * @param authToken An Evernote authentication token.
   * @param noteStoreUrl The URL of the Evernote NoteStore for the authenticated user.
   * @param webApiUrlPrefix The URL of misc. Evernote web APIs for the authenticated user.
   * @param userId The numeric ID of the Evernote user.
   */
  public AuthenticationResult(String authToken, String noteStoreUrl,
    String webApiUrlPrefix, int userId) {
    this.authToken = authToken;
    this.noteStoreUrl = noteStoreUrl;
    this.webApiUrlPrefix = webApiUrlPrefix;
    this.userId = userId;
  }

  /**
   * Get the authentication token that will be used to make authenticated API requests.
   */
  public String getAuthToken() {
    return authToken;
  }

  /**
   * Get the URL that will be used to access the NoteStore service.
   */
  public String getNoteStoreUrl() {
    return noteStoreUrl;
  }

  /**
   * Get the URL prefix that can be used to access non-Thrift API endpoints.
   */
  public String getWebApiUrlPrefix() {
    return webApiUrlPrefix;
  }

  /**
   * Get the numeric user ID of the user who authorized access to their
   * Evernote account.
   */
  public int getUserId() {
    return userId;
  }
}
