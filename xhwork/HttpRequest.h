#pragma once

#include <Windows.h>
#include <string>
#include <map>
using namespace std;

#include "curl.h"

class HttpRequest
{
public:
	HttpRequest(void);
	~HttpRequest(void);

public:
	BOOL Init(LPCTSTR str_SessionName);

	void SetHttpProxy(std::string p, std::string auth);

	/// ���HTTP����ͷ
	void AddHeader(CString key, CString value);

	void SetCookie(LPCSTR pszCookies);

	void SaveCookieFile(LPCSTR pszFile);

	void SetCookieFile(LPCSTR pszFile);

	CString GetHeadersAsString();

	DWORD GetContentLength();

	BOOL RecvContent(CStringA & strResult);

	BOOL Recv_Multi_Perform( CStringA & strResult );

	BOOL Recv_Multi_Perform2( CStringA & strResult );

	//�����ļ�
	BOOL DownloadToFile( LPCTSTR strUrl, LPCTSTR strPath ,void *AppleThis);
	// ���浽�ļ�
	BOOL RecvContentToFile( LPCTSTR strUrl, LPCTSTR strPath );

	BOOL RecvRedirectLocation(CString& strResult );

	CString GetRealUrl();

	void GetErrorBuff(string& sBuff);

	static DWORD WINAPI Easy_Perform_Multi(void *p);

	BOOL RecvContent_Multi( CStringA & strResult );
public:
	virtual void InitTimeout(int seconds);
	virtual BOOL Request(LPCTSTR strUrl);

	virtual DWORD GetStatusCode();

	virtual BOOL AddSendData(LPCSTR strSendData) { return FALSE; };
	virtual BOOL AddSendData(LPCWSTR lpszKey, LPCWSTR lpszValue){ return FALSE; };
	virtual BOOL AddSendData(LPCSTR lpszKey, LPCSTR lpszValue){ return FALSE; };

	static void GlobalInit();
	static void GlobalUnInit();
protected:
	CURL * m_Curl;
	struct curl_slist *m_Headerlist;

	string m_sResult;
	string m_sHeader;
	string m_sUrl;
	string m_sRedirectUrl;
	string m_sCookie;
	string m_sCookieFile;
	DWORD  m_dwNetCode;
	char   m_pErrorBuff[CURL_ERROR_SIZE];

	int m_nTimeout;
};

class HttpRequestGet : public HttpRequest
{
public:
	virtual BOOL Request(LPCTSTR strUrl);
};

class HttpRequestPost : public HttpRequest
{
public:

	virtual BOOL Request(LPCTSTR strUrl);

	BOOL AddSendData(LPCSTR strSendData); 

protected:
	CStringA m_PostData;

};

class HttpRequestPostForm : public HttpRequestPost
{
public:
	virtual BOOL Request(LPCTSTR strUrl);

	BOOL AddSendData(LPCWSTR lpszKey, LPCWSTR lpszValue);
	BOOL AddSendData(LPCSTR lpszKey, LPCSTR lpszValue);
};

typedef void (* PROCESS_CALLBACK)(double dltotal, double dlnow, void* user);

class DownloadInfo
{	
public:
	DownloadInfo()
	{
		newtask		= false;
		id			= 0;
		callback	= NULL;
		startpos	= 0;
		data		= NULL;
	}
public:
	bool				newtask;	// �Ƿ�ϵ�������Ϊtrue����������������Ϊfalse������
	unsigned long		id;			// ����id,��������ʱ��Ҫ���ã����û�����ã����ؿ�������ʱ�������ֵ
	unsigned long		startpos;	// ����ʼλ��
	string				url;		// ����url
	string				savepath;	// �ļ�����·��
	PROCESS_CALLBACK	callback;	// ���Ȼص�����
	void*				data;		// �û�����
};

class HttpDownload
{
public:
	HttpDownload();

	virtual void Init();

	BOOL Download(DownloadInfo* pInfo);
private:
	CURL * m_Curl;

	DWORD m_dwCode;
};
