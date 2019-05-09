#pragma once
#include <gdiplus.h>
//////////////////////////////////////////////////////////////////////////
//�����ʹ��GDIPLUS ������GIF Ȼ����Խ����е�֡��ȡ����

class GifProcesser
{
public:
	GifProcesser();
	~GifProcesser();
	bool Create(IStream* stream);
	void GetFrame(IStream* stream, INT idx);
	UINT GetFrameCount(){ return _frameCount; }
protected:
	ULONG_PTR m_gdiplusToken;
private:
	bool GetEncoderClsid();
private:
	Gdiplus::Image* _image;
	Gdiplus::PropertyItem* _items; 
	GUID* _guids;
	CLSID _clsid;
	UINT _frameDimensionsCount;
	UINT _width;
	UINT _height;
	UINT _frameCount;
};

