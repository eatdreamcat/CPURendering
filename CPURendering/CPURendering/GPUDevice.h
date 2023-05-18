#pragma once
#include"stdafx.h"



class GPUDevice
{
public :
	enum DepthBit {
		None,
		_8Bit,
		_16Bit,
		_32Bit
	};

	GPUDevice();
	void CreateFrameBuffers(UINT width, UINT height, DepthBit depthBit = DepthBit::None);
	void WaitForGpu();
	void Present();
	void Clear();
	void BeforeRendering();
	void OnRendering();
	void InitWindow(const String& windowName);

	Mat* BackBuffer();
	Mat* FrontBuffer();

	~GPUDevice();
private:
	void Release();
	Mat** m_BufferPtrs;
	bool m_IsFirstBackBuffer;
	float m_AspectRatio;
	String m_WindowName;
	float m_FPS;
	long m_LastClock;
};

