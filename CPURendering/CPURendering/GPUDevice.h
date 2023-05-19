#pragma once
#include"stdafx.h"
#include "Geometry.h"
#include "RenderingThread.h"

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
	void Present();
	void Clear();
	void BeforeRendering();
	void Draw();
	void DrawCoordianteAxis();
	void DrawLineWithSlop(const VertexBuffer& vbo);
	void InitWindow(const String& windowName);

	Mat* BackBuffer();
	Mat* FrontBuffer();

	~GPUDevice();
private:
	void Release();
	Mat** m_BufferPtrs;
	bool m_IsFirstBackBuffer;
	/* the ratio of (height / width) */
	float m_AspectRatio;
	String m_WindowName;
	float m_FPS;
	long m_LastClock;
	RenderingThread m_RenderingThread;
};

