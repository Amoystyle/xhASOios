#ifndef _URL_CODE_H
#define _URL_CODE_H
///URLEncode�㷨
void urlencode(unsigned char* src,int src_len,unsigned char* dest,int dest_len);
unsigned char* urldecode(unsigned char* encd,unsigned char* decd);

#endif