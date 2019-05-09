#pragma once

#ifdef __cplusplus
extern"C"
{
#endif

#define SHA1_SIZE_BYTE 20  


typedef struct SHAstate_st
{
	unsigned long h[SHA1_SIZE_BYTE / 4]; /* ���ժҪ���(32*5=160 bits)*/
	unsigned long Nl;
	unsigned long Nh;       /*�����Ϣ��λ����Nh����32λ��Nl����32λ*/
	unsigned long data[16]; /*���ݴӵ�0���ĸ�8λ��ʼ���η���*/
	int FlagInWord;     /*��ʶһ��dataԪ����ռ�õ��ֽ������Ӹ�->�ͣ���ȡֵ0,1,2,3*/
	int msgIndex;       /*��ǰ���������data����Ԫ������*/
	int isTooMang;      /*����Ϊ0�����������Ϣ����2^64 bitsʱΪ1��*/
} SHA1_Context;

int SHA1_Init(SHA1_Context *c);
int SHA1_Update(SHA1_Context *c, const unsigned char *chBuff, unsigned int n);
int SHA1_Final(SHA1_Context *c, unsigned char * md);
int SHA1_Clear_data(SHA1_Context *c);
#ifdef __cplusplus
}
#endif