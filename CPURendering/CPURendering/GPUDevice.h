#pragma once
#include"stdafx.h"
#include "Geometry.h"
#include "RenderingThread.h"
#include "Stats.h"

struct RootSignature
{
	VertexBuffer vob;
	Primitive primitive;
};

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
	void Present(Stats& stat);
	void Clear();
	void BeforeRendering();
	void Draw(const RootSignature& rootSignature, Stats& stat);
	void DrawCoordianteAxis();
	void DrawLineWithSlop(const VertexBuffer& vbo);
	void DrawLineWithBresenham(const VertexBuffer& vbo);
	void InitWindow(const String& windowName);

	Mat* BackBuffer();
	Mat* FrontBuffer();

	~GPUDevice();
private:
	void DrawLineSlop(const Line2D& line2D);
	void DrawLineBresenham(const Line2D& line2D);

	void Release();
	Mat** m_BufferPtrs;
	bool m_IsFirstBackBuffer;
	/* the ratio of (height / width) */
	float m_AspectRatio;
	String m_WindowName;
	long m_LastClock;
	RenderingThread m_RenderingThread;
};

