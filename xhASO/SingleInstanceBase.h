#pragma once

/******************************************��ʵ��ģ�����****************************************************/
template<class T> 
class CSingleInstanceBase
{
public:
	CSingleInstanceBase(void){}
	virtual ~CSingleInstanceBase(void){}

	static T* GetInstance()
	{
		static T s_Instance;
		return &s_Instance;
	}
};

