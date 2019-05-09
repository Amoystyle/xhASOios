// SMTP.cpp: ʵ��CSMTP��.
//
#include "stdafx.h"
#include "SMTP.h"
#include "base64.h"

//---------------------------------------------------------------------------
CSMTP::CSMTP( LPSTR szSMTPServerName, UINT nPort )
{
	ASSERT( szSMTPServerName != NULL );
	AfxSocketInit();
	m_sSMTPServerHostName = szSMTPServerName;
	m_nPort = nPort;
	m_bConnected = FALSE;
	m_sError = _T( "OK" );
}

//---------------------------------------------------------------------------
CSMTP::~CSMTP()
{
	if( m_bConnected )
		Disconnect();
}

//---------------------------------------------------------------------------
BOOL CSMTP::Connect()
{
	char sHello[MAX_PATH] = {0};
	TCHAR local_host[80] = {0};
	char szLocal_host[80] = {0};

	if( m_bConnected )
	{
		return TRUE;
	}

    // ����Socket.
	if( !m_wsSMTPServer.Create() )
	{
		m_sError = _T( "Unable to create the socket." );
		return FALSE;
	}

    // ��ʼ����.
	if( !m_wsSMTPServer.Connect( GetServerHostName(), GetPort() ) )
	{
		m_sError = _T( "Unable to connect to server" );
		m_wsSMTPServer.Close();
		return FALSE;
	}

    // ����Ӧ�����Ƿ�Ϊ220.
	if(!get_response(_T("Connect"),220)){
		m_wsSMTPServer.Close();
		return FALSE;
	}

    // ����HELO��ʶ.
	gethostname( szLocal_host, 80 );
	sprintf(sHello,"HELO %s\r\n", szLocal_host );

	int totalSend = m_wsSMTPServer.Send((void*)sHello, strlen(sHello) );
	if (SOCKET_ERROR == totalSend)
	{
		return FALSE;
	}

    // ����Ӧ�����Ƿ�Ϊ250.
	if(!get_response(_T("HELO"),250))
	{
		m_wsSMTPServer.Close();
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CSMTP::Disconnect()
{
	BOOL ret;
	if( !m_bConnected )
	{
		return TRUE;
	}

    // ������������˳���ʶ�͹ر�Socket.
	char sQuit[] = "QUIT\r\n";
	m_wsSMTPServer.Send( (void*)sQuit,strlen(sQuit));

    // ��֤QUIT,����Ҫ����.	
	ret =get_response(_T("QUIT"),221);

	m_wsSMTPServer.Close();
	m_bConnected = FALSE;

	return ret;
}

//---------------------------------------------------------------------------
void CSMTP::SetServerProperties( LPCTSTR sServerHostName, UINT nPort)
{
	ASSERT( sServerHostName != NULL );

	// û�з�����������.
	if( sServerHostName == NULL )
		return;
	
	m_sSMTPServerHostName = sServerHostName;
	m_nPort = nPort;
}

//---------------------------------------------------------------------------
// �������Ϊϣ����Ӧ����.
BOOL CSMTP::get_response(LPCTSTR sDes,UINT response_expected )
{
	char retValue[4] = {0};
	UINT response = 0;

	// m_wsSMTPServerΪCSocket������󣬵���Receive()��Ӧ������յ�����
	// response_buf��.
	memset(response_buf,0x0,RESPONSE_BUFFER_SIZE);
	if( m_wsSMTPServer.Receive( response_buf, RESPONSE_BUFFER_SIZE ) == SOCKET_ERROR )
	{
		m_sError = _T( "Socket Error" );
		return FALSE;
	}

	memcpy(retValue,response_buf,3);
	response = atoi(retValue);
 
	// �����յ���Ӧ�����Ƿ�����ϣ���õ���.
	if( response != response_expected )
	{
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CSMTP::AuthCheck(CMailMessage msg)
{
	// ������֤��ʶ.	
	char sTemp[] = "AUTH LOGIN\r\n";
	m_wsSMTPServer.Send( (char*)sTemp,strlen(sTemp));

    // ����Ӧ�����Ƿ�Ϊ334.
	if(!get_response(_T("AUTH LOGIN"),334)) 
	{
		return FALSE;
	}

    // ��֤�û���.
	CBase64 code;
	string sUsername=msg.m_sUsername;
	string strEncode = code.Encode((LPSTR)sUsername.c_str(),sUsername.length());
	strEncode += "\r\n";
	m_wsSMTPServer.Send((void*)strEncode.c_str(),strEncode.length() );

    // ����Ӧ�����Ƿ�Ϊ334.
	if(!get_response(_T("Username"),334)) 
	{
		return FALSE;
	}

    // ��֤����.     
	string sPassword=msg.m_sPassword;
	strEncode = code.Encode((LPSTR)sPassword.c_str(),sPassword.length());
	strEncode += "\r\n";
	m_wsSMTPServer.Send((void*)strEncode.c_str(),strEncode.length() );
    
	// ����Ӧ�����Ƿ�Ϊ235.
	if(!get_response(_T("Password"),235)) 
	{
		return FALSE;
	}
    
	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CSMTP::SendEmailMessage(CMailMessage msg)
{
	char sFrom[MAX_PATH] = {0};
	char sTo[MAX_PATH] = {0};
	char sTemp[MAX_PATH] = {0};

    // ���ӷ�����.
    if(!Connect()) 
	{
		return FALSE;
	}

	msg.FormatMessage();

    // �����֤.
	if(msg.m_bCheck)
	{
	    if(!AuthCheck(msg)) 
		{
			return FALSE;
		}
	}

    // ��ʽ��������MAIL��������ա���֤������Ӧ����.
	sprintf(sFrom,"MAIL FROM: <%s>\r\n",msg.m_sFrom.c_str());
	m_wsSMTPServer.Send((void*)sFrom, strlen(sFrom) );

    // ����Ӧ�����Ƿ�Ϊ250.
	if(!get_response(_T("MAIL FROM:"),250)) 
	{
		return FALSE;
	}

    // ��ʽ��������RCPT����,�����ա���֤������Ӧ����.
	sprintf(sTo,"RCPT TO: <%s>\r\n",msg.m_sTo.c_str());
	m_wsSMTPServer.Send( (void*)sTo, strlen(sTo) );

    // ����Ӧ�����Ƿ�Ϊ250.
	if(!get_response(_T("RCPT TO:"),250))
	{
		return FALSE;
	}
	
    // ��ʽ��������DATA��������ա���֤������Ӧ����.
	char strData[] = "DATA\r\n";
	m_wsSMTPServer.Send((void*)strData, strlen(strData) );

	// ����Ӧ�����Ƿ�Ϊ354.
	if(!get_response(_T("DATA"),354)) 
	{
		return FALSE;
	}

	// ���͸���RFC 822�ĵ��涨��ʽ�������ʼ�ͷ.
	strcpy(sTemp,msg.m_sHeader.c_str());
	m_wsSMTPServer.Send( (void*)sTemp, strlen(sTemp) );

    // �����ʼ���.
	string strTmp = cook_body( msg.m_sBody );
	m_wsSMTPServer.Send((void*)strTmp.c_str(),strTmp.length());

    // �����������ݽ�����־"<CRLF>.<CRLF>".
	char strEndTag[] = "\r\n.\r\n";
	m_wsSMTPServer.Send( (void*)strEndTag,strlen(strEndTag));
    
	// ����Ӧ�����Ƿ�Ϊ250.
	if(!get_response(_T("."),250)) 
	{
		return FALSE;
	}

    // �Ͽ�������.
	if(!Disconnect())
	{
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
string CSMTP::cook_body(string sBody)
{
	string sTemp;
	string sCooked = "";
	LPSTR szBad   = "\r\n.\r\n";
	LPSTR szGood  = "\r\n..\r\n";
	int nPos;
	int nStart = 0;
	int nBadLength = strlen( szBad );
	sTemp = sBody;

	if( sTemp.substr(0,3) == ".\r\n" )
		sTemp = "." + sTemp;

	while( (nPos = sTemp.find( szBad )) > -1 )
	{
		sCooked = sTemp.substr( nStart, nPos );
		sCooked += szGood;
		sTemp = sCooked + sTemp.substr(nPos + nBadLength, sTemp.length() - (nPos + nBadLength) );
	}

	return sTemp;
}

//---------------------------------------------------------------------------
void CSMTP::Cancel()
{
   // ȡ�������.
   m_wsSMTPServer.CancelBlockingCall();

   // �����ر�SOCKET.
   m_wsSMTPServer.Close();
}
