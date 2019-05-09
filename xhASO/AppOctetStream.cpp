// AppOctetStream.cpp: ʵ��CAppOctetStream��.
//
#include "stdafx.h"
#include "AppOctetStream.h"
#include "Base64.h"
#include "MailMessage.h"


#define BYTES_TO_READ 54 // ������֤����������е���󳤶�.

//---------------------------------------------------------------------------
CAppOctetStream::CAppOctetStream()
{
}

//---------------------------------------------------------------------------
CAppOctetStream::~CAppOctetStream()
{
}

//---------------------------------------------------------------------------
BOOL CAppOctetStream::AppendPart(LPSTR szContent, 
								 LPSTR szParameters, 
								 int nEncoding, 
								 string & sDestination,BOOL bPath)
{
	CStdioFile fAttachment;
	TCHAR strContent[MAX_PATH] = {0};

	ASSERT( szContent != NULL );

	// ����ֻ������,���Ժ���bPath����.
	if( szContent == NULL )
	{	
		return FALSE;
	}

	::MultiByteToWideChar(CP_ACP,0,szContent,strlen(szContent),strContent,MAX_PATH);

	if( !fAttachment.Open(strContent,(CFile::modeRead |
		                      CFile::shareDenyWrite | CFile::typeBinary) ) )
	{
		return FALSE;
	}
	
	sDestination += build_sub_header( szContent,szParameters,
									  nEncoding, bPath);
	attach_file( &fAttachment, CMailMessage::BASE64, sDestination );
	fAttachment.Close();
	return TRUE;
}

//---------------------------------------------------------------------------
string CAppOctetStream::build_sub_header(LPSTR szContent, 
										  LPSTR szParameters, 
										  int nEncoding,BOOL bPath)
{
	string sSubHeader;
	char sTemp[MAX_PATH] = {0};
	char szFName[ _MAX_FNAME ] = {0};
	char szExt[ _MAX_EXT ] = {0};

	_splitpath( szContent, NULL, NULL, szFName, szExt );

	// �������szParameters �� nEncoding.
	// �������Լ��Ĳ�����ֻ���� Base64 ����.
	if( bPath )
	{
		sprintf(sTemp,"; file=%s%s", szFName, szExt );
	}
	else
	{
		memset(sTemp,0x0,MAX_PATH);
	}
	
	sSubHeader = "Content-Type: ";
	sSubHeader += GetContentTypeString();
	sSubHeader += sTemp;
	sSubHeader += "\r\n";
	sSubHeader += "Content-Transfer-Encoding: base64\r\nContent-Disposition: attachment; filename=";
	sSubHeader += szFName;
	sSubHeader += szExt;
	sSubHeader += "\r\n";

	// ��ʾ�ӱ�ͷ����.
	sSubHeader += "\r\n"; 
	
	return sSubHeader;
}

//---------------------------------------------------------------------------
string CAppOctetStream::GetContentTypeString()
{
	string s;
	s = "application/octet-stream";
	return s;
}

//---------------------------------------------------------------------------
// �����ʼ�����.
void CAppOctetStream::attach_file(CStdioFile* pFileAtt, 
								  int nEncoding, 
								  string & sDestination)
{
	CBase64* pEncoder;
	int nBytesRead;
	char szBuffer[ BYTES_TO_READ + 1 ];
 
	ASSERT( pFileAtt != NULL );
	if( pFileAtt == NULL )
		return;
	switch( nEncoding )
	{
		// ����ֻ���� 7bit ����, �������Ŀ����ڴ����.
		default:
		// ����7bit ����...
		case CMailMessage::BASE64:
			try 
			{
				pEncoder = new CBase64;
			}
			catch( CMemoryException* e )
			{
				delete e;
				return;
			}
	}
	if( pEncoder == NULL )
		return;
	do
	{
		try
		{
			nBytesRead = pFileAtt->Read( szBuffer, BYTES_TO_READ );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}
		szBuffer[ nBytesRead ] = 0;	// �ַ�����β.
		
		sDestination += pEncoder->Encode( szBuffer, nBytesRead );

		sDestination += "\r\n";
	} while( nBytesRead == BYTES_TO_READ );
	sDestination += "\r\n";
	delete pEncoder;
}
