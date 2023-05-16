#include "GPUDevice.h"

GPUDevice::GPUDevice():m_BufferPtr(nullptr),m_CurrentBackBufferIndex(-1)
{
}

void GPUDevice::CreateFrameBuffers()
{
	if (m_BufferPtr != nullptr) {
		delete m_BufferPtr;
		m_BufferPtr = nullptr;
	}

	m_BufferPtr = new Mat[2];
	m_BufferPtr[0] = 
}

void GPUDevice::WaitForGpu()
{
}

void GPUDevice::Present()
{
	if (m_CurrentBackBufferIndex == -1) {
		return;
	}


}

GPUDevice::~GPUDevice()
{
	if (m_BufferPtr != nullptr) {
		delete m_BufferPtr;
		m_BufferPtr = nullptr;
	}
}
