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
	m_BufferPtr[0] = imread(current_working_directory() + "\\GAMES101_back.jpg", IMREAD_COLOR);
	m_BufferPtr[1] = imread(current_working_directory() + "\\GAMES101_front.jpg", IMREAD_COLOR);

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
