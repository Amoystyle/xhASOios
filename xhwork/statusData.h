#ifndef __STATUSDATA_H
#define __STATUSDATA_H
#define    MAX_ALLOC_SIZE          1024*8
#define	   ALLOCATE_SIZE		   1024*4
#define	   SIGNATURE_SIZE	       1024*2
#define	   EXECTUEFAILED           -6  //ִ���쳣
#define    SPIM_NULL               -5  //SPIMֵΪ��
#define	   OBJECT_NULL			   -4  //����Ϊ��
#define	   MODULELOAD_FAILED	   -3  //ģ�����ʧ��
#define	   GSASERVEVICEOBJERROR    -2  //����GSA����ʧ��
#define    DETECTDATA_FAILED       -1  //�ض���ʧ��
#define    STATUS_FAILED            0  //ʧ��---->��������һ��
#define    STATUS_SUCCESS           1  //�ɹ�---->��������һ��
#define	   RE_LOGIN                 2  //���µ�¼
#define	   ACCOUNT_DISABLED		    3  //�˺Ž���
#define	   SP_AUTH_ERROR		    4  //��AMDȨ��
#define	   PASSWORD_ERROR		    5  //�������
#define	   SP_MD_AUTH_ERROR         6  //��ҪMDֵ
#define	   AUNKNOWN_ERROR           7  //δ֪����
#define	   AMD_BUYPRODUCT_ERROR     8  //AMDֵ����
#define	   GSA_FAILED				9  //GSAʧ��
#define	   LOGIN_FAILED				0xA//��¼ʧ��-->��������һ��
#define	   DOWNLOADFAILED			0xB//����ʧ��-->��������һ��
#define	   SEARCHFAILED				0xC//����ʧ��-->��������һ��
#define	   DOWNLOADSUCCESS			0xD//���سɹ�
#define	   SEARCHSUCCESS			0xE//�����ɹ�
#define	   COMMENTSUCCESS			0xF//���۳ɹ�
#define	   COMMENTFAILED			0x10//����ʧ��-->��������һ��
#define    ACTIVEAPPFAILED			0x11//����ʧ��-->��������һ��
#define	   EXECUTE_TIME_OUT			0x12//ִ�г�ʱ-->��������һ��
#define	   LOGIN_SUCCESS			0x13//��¼�ɹ�
#define    BUY_SUCCESS              0x14//����ɹ�
#define    BUY_FAILED               0x15//����ʧ��-->��������һ��
#define    ACCOUNT_LOCKED           0x18 //�˺ű���
#define	   ACCOUNT_FAULT			0x19 //�˺���Ϣ��ȫ
#define    NEED_SET_KEYBAG          0x20 //��Ҫ����keybag��ֵ
#define    CHANGE_CONDITION        0xBDE //messageCode:3038 ����������Ѿ����ģ���Ҫͬ��ȷ��
#define    BUY_LOGIN_VERIFY        0x818 //messageCode: 2072 ����ʱ��Ҫ��¼��֤
#define    SUBMIT_PAID_BUY_VERIFY  0xC07 //messageCode:3079  �ύ���ѹ�����֤
#define	   AGREE_BUY_VERIFY		   0x7E8 //messageCode:2024  ����������Ҫ��֤
#define	   UNKNOWN_ERROR		   0x138A//failureTypecode:5002
#define	   UNKNOWN_DEVICE          0x3F0 //messagecode:1008 �޷���֤�����豸����ԣ�����ϵ����֧��Ѱ�����
#define	   NEED_ALOWAYS_PWD        0x4b5 //�жϹ����Ƿ�һ����Ҫ����
#define    SAVE_PWD_FOR_FREE_APP   0x4b6 //Ϊ�����Ŀ��������
#define	   ACCOUNT_STOP		       0x7D4 //messagecode:2004 �˺�ͣ��
#define	   PRICE_MISMATCH          0x7E3 //messagecode:2019 �۸�ƥ�䣬���ܹ������Ŀ
#define    CAPTCHA_REQUIRED        0x7E4 //messagecode:2020  (�Զ������)��Ҫ��֤��


#define    PROXY_NETWORK           0x4

#endif