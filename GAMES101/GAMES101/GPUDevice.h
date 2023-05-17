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
	void InitWindow(const String& windowName);

	~GPUDevice();
private:
	void Release();
	Mat** m_BufferPtrs;
	int m_CurrentBackBufferIndex;
	float m_AspectRatio;
	String m_WindowName;
};

