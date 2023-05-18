#include "GPUDevice.h"

const int BufferCount = 2;
GPUDevice::GPUDevice():
	m_BufferPtrs(nullptr),
	m_IsFirstBackBuffer(false),
	m_WindowName(""), 
	m_FPS(0), 
	m_LastClock(0)
{
	RenderingThread m_RenderingThread;
}

void GPUDevice::CreateFrameBuffers(UINT width, UINT height, DepthBit depthBit)
{
	Release();

	m_BufferPtrs = new Mat* [BufferCount];
	m_BufferPtrs[0] = new Mat (width, height, IMREAD_COLOR);
	m_BufferPtrs[0]->setTo(-128);
	m_IsFirstBackBuffer = true;

	m_BufferPtrs[1] = new Mat(width, height, IMREAD_COLOR);
	m_BufferPtrs[1]->setTo(128);

}



void GPUDevice::InitWindow(const String& windowName)
{
	m_WindowName = windowName;
	namedWindow(windowName, WINDOW_AUTOSIZE);
}

Mat* GPUDevice::BackBuffer()
{
	return m_IsFirstBackBuffer ? m_BufferPtrs[0] : m_BufferPtrs[1];
}

Mat* GPUDevice::FrontBuffer()
{
	return m_IsFirstBackBuffer ? m_BufferPtrs[1] : m_BufferPtrs[0];;
}

void GPUDevice::Present()
{

	auto now = clock();

	m_FPS = m_FPS * 0.9 + (100.0 / (now - m_LastClock + std::numeric_limits<float>::epsilon()));
	

	auto backBuffer = *BackBuffer();
	//imshow(m_WindowName, backBuffer);
	putText(backBuffer,  "FPS:" + std::to_string(m_FPS), Point(40, 50), FONT_HERSHEY_SIMPLEX, 1.0, (0, 255, 0), 2);
	imshow(m_WindowName, backBuffer);

	m_IsFirstBackBuffer = !m_IsFirstBackBuffer;

}

void GPUDevice::Clear()
{
	BackBuffer()->setTo(-128);
	imshow(m_WindowName, *BackBuffer());
}

void GPUDevice::OnRendering()
{
	//m_RenderingThread.DispatchWork(1);



	auto renderTarget = *BackBuffer();

	std::vector<float> vertices;

	vertices.push_back(0.0f);
	vertices.push_back(0.5f);
	vertices.push_back(0.0f);

	vertices.push_back(0.5f);
	vertices.push_back(-0.5f);
	vertices.push_back(0.0f);

	vertices.push_back(-0.5f);
	vertices.push_back(-0.5f);
	vertices.push_back(0.0f);

	std::vector<int> indices;

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(6);


	
	

	/*
	* 1.��������һ��ɨ���ߣ�����ĳ���ߵĽ���Ϊ��һ��ɨ���ߺ͸ñߵĽ������һ������б����ص�ƫ�ơ�
	* 2.ÿ�������¶˵��ֵ�ǹ̶��ģ��϶˵�y������¶˵�y����Ϊ�ñ�����Ļ�ϵĿ�ȣ���ɨ����ɨ����������Ⱥ󣬾Ͳ��ٺ͸ñ��ཻ��
	* 3.���ڵڶ����в������ཻ�ıߣ�ɨ���߻�͸ñ����ڽӵ���һ�����ཻ��������λ�õ�xֵΪ�������϶˵��x��
	* �ο� https://zhuanlan.zhihu.com/p/96824553
	*/
	// TODO: ɨ����


}

void GPUDevice::BeforeRendering()
{
	m_LastClock = clock();
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
