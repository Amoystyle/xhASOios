#ifndef BRANK_API
#define BRANK_API extern "C" _declspec(dllimport)
#endif

BRANK_API BOOL InitializeModule(LPVOID lpBuffer);			    //����iTunesģ��ӿ�
BRANK_API void UnInitializeModule(void);					    //�ͷ�iTunesģ��
BRANK_API BOOL GetMachineGUID(LPVOID lpGsaServices,LPVOID lpBuffer);				    //���ɻ���guidֵ
BRANK_API BOOL GetEncryptKbsyncValue(IN LPVOID lpGsaServices,LPVOID lpBuffer,char* lpDsid,LONGLONG dsid,bool bAuthLogin);	//�õ�kbsyncֵ,��¼�ɹ����õ�dsid��ֵ
BRANK_API LPVOID CreateGsaServicesObject();                                      //�õ����ڼ���gsavervices��ȫ�ֶ���
BRANK_API VOID ReleaseGsaServicesObject(LPVOID lpGsaServices);
BRANK_API BOOL generateX_JS_SP_TOKEN_Value(LPVOID lpGsaServices,LPVOID lpSrcData,int dataLen,LPVOID lpOutData);//����JS-SP-TOKE��ֵ

BRANK_API BOOL generateSPXAppleActionSignature(IN LPVOID lpGsaServices,
											   IN LPVOID lpDsid,    //�������disd
											   IN LPVOID lpSaleId,  //�������appId
											   IN LPVOID lpTimeStamp,//ʱ���
											   IN LPVOID lpXAppleStoreFront,//����
											   IN LPVOID lpSignData, //������� signature��ֵ
											   IN int	signLength,  //����signature��ֵ����
											   OUT LPVOID lpOutSignature,//�������actionsigature��ֵ
											   OUT int& outSignLength,//��������ĳ���
											   IN BOOL bHighVersion);

BRANK_API BOOL generateCheckinacksarValue(IN LPVOID lpGsaServices,
										  IN LPVOID lpRentalbagResponse,
										  int bagResLen,
										  IN LPVOID lpCheckinsar,
										  int checksarLen,
										  OUT LPVOID lpCheckacksar);

BRANK_API BOOL generateRbsyncValue(IN LPVOID lpGsaServices,OUT LPVOID lpRbsyncValue);

BRANK_API BOOL generateClientData(IN LPVOID lpGsaServices,
								  IN LPVOID lpXappleMdData,//�����������ĵ�x-apple-amd-dataֵ
								  IN int xAppleMdDataLen,  //������������x-apple-amd-dataֵ����
								  IN LPVOID lpDsid,        //dsid��ָ��
								  OUT LPVOID lpClientData, //�������,clientData��ֵ
								  OUT int& nvContext);     //�������,�����ĵ�ָ��

BRANK_API BOOL generateCpimData(IN LPVOID lpGsaServices,
								IN LPVOID lpSpimData,      //������������spim��ֵ
								IN int spimLen,            //spim��ֵ����
								OUT LPVOID lpCpimData,     //�������:cpim��ֵ
								OUT int& nvContext);       //�������,�����ĵ�ָ��

BRANK_API BOOL SetFinishProvisionData(IN LPVOID lpGsaServices,
									  IN LPVOID lpTKTransportKeyData,//TK,TransportKey��ֵ,����֮һ
									  IN int tkTransportKeyLen,      //TK,TransportKey��ֵ��������֮һ
									  IN LPVOID lpPtmSettingInfoData, //ptm��SettingInfo��ֵ����һ
									  IN int ptmSettingInfoLen,       //ptm��SettingInfo�ĳ�������һ
									  IN int nvContext,               //�����ĵ�ָ��
									  IN BOOL bGSAProvision);         //��ֵ�������ΪSettingInfo��transportKey��ֵʱ��ΪFALSE

BRANK_API BOOL generateA2kValue(IN LPVOID lpGsaServices,OUT LPVOID lpA2kValue,OUT int& a2kDataLen);//�������,a2k��ֵ�ͳ���

BRANK_API BOOL generateM1Value(IN LPVOID lpGsaServices,
							   IN LPVOID lpAppleId,   //apple Id:XXXXX@XXXX.com
						       IN LPVOID lpPassword,//����
							   IN int dw_i_data,    //i ��ֵ
							   IN LPVOID lp_s_data, //s ��ֵ
							   IN int s_data_Len,   // s �ĳ���
							   IN LPVOID lp_B_data, // B ��ֵ
							   IN int B_data_Len,   // B �ĳ���
							   OUT LPVOID lpM1,     // �������,M1��ֵ
							   OUT int& m1DataLen); //�������M1�ĳ���

BRANK_API BOOL setSrpReponseData(IN LPVOID lpGsaServices,
								 IN LPVOID lpSpd, //spd ��ֵ
								 IN int spdLen, //spd �ĳ���
								 IN LPVOID lpM2,//M2 ��ֵ
								 IN int M2Len,  //M2 �ĳ���
								 IN LPVOID lpNp,//np ��ֵ
								 IN int npLen); //np �ĳ���

BRANK_API BOOL generateSrpPETPassword(IN LPVOID lpGsaServices,OUT LPVOID lpPetPassword,OUT int& petPasswordLen);//�������PET����ͳ���

BRANK_API BOOL generateSrpHBToken(IN LPVOID lpGsaServices,OUT LPVOID lpHBToken,OUT int& hbTokenLen); //HBToken��ֵ�ͳ���

BRANK_API BOOL generateSrpAdsid(IN LPVOID lpGsaServices,OUT LPVOID lpAdsid,OUT int& adsidLen);// adsid��ֵ�ͳ���

BRANK_API BOOL generateXAppleHBToken(IN LPVOID lpGsaServices,OUT LPVOID lpXAppleHBToken,OUT int& hbTokenLen); //X-Apple-HB-Token��ֵ�ͳ���

BRANK_API void releaseGsaAllocate(IN LPVOID lpGsaServices); //�ͷ�GsaServices2�ڴ�

BRANK_API BOOL generateMidOtpValue(IN LPVOID lpGsaServices,IN LPVOID lpMidValue,IN LPVOID lpOtpValue);

BRANK_API BOOL generateX_Apple_AMD_Value(IN LPVOID lpGsaServices,char* lpDsid,char* lpXAppleAMDM,char* lpXAppleAMD);//�������amd-m��amd��ֵ

BRANK_API BOOL generateX_Apple_I_MD_Value(IN LPVOID lpGsaServices,OUT LPVOID lpXAppleIMDM,OUT LPVOID lpXAppleIMD);//�������I_md-m��I_md��ֵ

BRANK_API BOOL generateX_Apple_MD_Value(IN LPVOID lpGsaServices,IN LPVOID lpDsid,OUT LPVOID lpXAppleMDM,OUT LPVOID lpXAppleMD);//�������md-m��md��ֵ

BRANK_API BOOL generateSbsyncValue(IN LPVOID lpGsaServices,
								   IN LPVOID lpDsid,          //dsid ��ָ��
								   IN LPVOID lpXApple_amd_m,  //amd-m��ֵ
								   IN int amd_m_Len,          //amd-m�ĳ���
								   OUT LPVOID lpSbsyncValue,  //�������sbsync��ֵ
								   OUT int& sbsyncLen);       //�������sbsync�ĳ���

BRANK_API void setKeyBagData(IN LPVOID lpGsaServices,IN LPVOID lpKeyBagData,IN int keyBagDataLen);

BRANK_API void setKeyBagDataEx(IN LPVOID lpGsaServices,IN LONGLONG dsid,IN LPVOID lpKeyBagData,IN int keyBagDataLen);

//��¼ʱ�õ�SignsapSetupBuffer��ֵ����¼ʱ���õĽӿ�
BRANK_API int GetSignsapSetupBuffer(IN LPVOID lpGsaServices,
									LPVOID lpSignSap,//�������:signature��ֵ
									int signSize,    //�������:��С
									LPVOID lpBuffer, //����������洢�õ���ǩ��ֵ
									int& OutDataLength,//�����������С
									BOOL bSet,//�������
									LPVOID lpUserXmlInfo,//�������
									int userXmlSize);//�������

BRANK_API int  GetiTunesFunAddr_1(IN LPVOID lpGsaServices);
BRANK_API int  GetiTunesFunAddr_2(IN LPVOID lpGsaServices,char* lpUdid);

//�õ�׼��ע��֮ǰ��SignsapSetupBufferֵ,ע��ʱ��ĵ��ýӿ�
BRANK_API int GetRegisterSignsapSetupBuffer(IN LPVOID lpGsaServices,
											LPVOID lpBuffer,//�����������һ�δӷ������õ���sign-sap-setup-cert�ַ��������������
											int signSize,   //�ַ�������
										    LPVOID lpXAppleActionSignature,//����������㷨���ɵ����sign-sap-setup-bufferֵ
											int& outSignLen); //���������ǩ�����ݵĳ���
									 
//�õ�ע��ʱ������������X-apple-signature��ֵ��ֻ�����ֵ��ȷ�ˣ�ע�������ת������ҳ�����½���ע��
BRANK_API int GetRegisterSignupWizardXappleSignature(IN LPVOID lpGsaServices,
													 IN LPVOID lpBuffer,//�������,�ڶ��ν����ӷ������õ���sign-sap-set-buffer�ַ��������������
													 IN int inLength,   //���������sign-sap-set-buffer����
													 OUT LPVOID sXappleSignature,//���X-Apple-signature��ֵ
													 OUT int& signDataLen);//X-Apple-signature��ֵ�ĳ���

//���ô���ҳ��������ǩ��X-Apple-ActionSignature��ֵ
BRANK_API int SetXappleActionSignatureForPage(IN LPVOID lpGsaServices,LPVOID lpXAppleActionSignature,int signLen);


//�õ�Boundary��ֵ
BRANK_API char* GetBoundaryValue();

//ͬ��ҳ�淢����Ҫ��xAppleActionSignature��ֵ
BRANK_API int GenerateAgreeWebPageXAppleActionSignature(IN LPVOID lpGsaServices,
														IN LPVOID lpAgreeWebPageUUID,//�������,ͬ��ҳ���uuid
													    IN LPVOID lpUdid,              //�������,udid��ֵ
													    IN LPVOID lpAppleId,           //���������ע��ʱ����������˺�
													    OUT LPVOID lpXAppleSignature,  //�������,���ɵ�XAppleActionSignature
													    OUT int& signDataLen);         //�������,XAppleActionSignature�ĳ���
