#pragma once
#include"stdafx.h"



class GPUDevice
{
public :
	enum BufferCount {
		Double,
		Default = Double
	};

	GPUDevice();
	void CreateFrameBuffers();
	void WaitForGpu();
	void Present();

	~GPUDevice();
private:
	Mat* m_BufferPtr;
	int m_CurrentBackBufferIndex;
	float m_AspectRatio;
};

