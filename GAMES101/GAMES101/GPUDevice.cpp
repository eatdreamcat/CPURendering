#include "GPUDevice.h"

const int BufferCount = 2;
GPUDevice::GPUDevice():m_BufferPtrs(nullptr),m_CurrentBackBufferIndex(-1), m_WindowName("")
{
}

void GPUDevice::CreateFrameBuffers(UINT width, UINT height, DepthBit depthBit)
{
	Release();

	m_BufferPtrs = new Mat* [BufferCount];
	m_BufferPtrs[0] = new Mat(width, height, IMREAD_COLOR);
	m_BufferPtrs[0]->setTo(-128);
	m_BufferPtrs[1] = new Mat(width, height, IMREAD_COLOR);
	m_BufferPtrs[1]->setTo(128);
	m_CurrentBackBufferIndex = 0;

}

void GPUDevice::WaitForGpu()
{
}

void GPUDevice::InitWindow(const String& windowName)
{
	m_WindowName = windowName;
	namedWindow(windowName, WINDOW_NORMAL);
}

void GPUDevice::Present()
{
	if (m_CurrentBackBufferIndex == -1) {
		return;
	}

	auto backBuffer = *m_BufferPtrs[m_CurrentBackBufferIndex];
	imshow(m_WindowName, backBuffer);

	m_CurrentBackBufferIndex = (m_CurrentBackBufferIndex + 1) % 2;

}

GPUDevice::~GPUDevice()
{
	Release();
}

void GPUDevice::Release()
{

	if (m_BufferPtrs != nullptr) {
		auto i = 0;
		while (i < BufferCount)
		{
			if (m_BufferPtrs[i] != nullptr) {
				delete m_BufferPtrs[i];
				m_BufferPtrs[i] = nullptr;
			}

			++i;
		}

		delete []m_BufferPtrs;
		m_BufferPtrs = nullptr;
	}
}
