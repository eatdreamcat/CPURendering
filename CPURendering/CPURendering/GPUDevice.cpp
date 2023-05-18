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
	m_BufferPtrs[0] = new Mat (width, height, CV_8UC3);
	m_BufferPtrs[0]->setTo(0);
	m_IsFirstBackBuffer = true;

	m_BufferPtrs[1] = new Mat(width, height, CV_8UC3);
	m_BufferPtrs[1]->setTo(256);

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
	putText(backBuffer,  "FPS:" + std::to_string(m_FPS), Point(40, 50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
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

	struct line {
		float x0;
		float y0;
		float x1;
		float y1;
	};

	int trangleCount = indices.size() / 3;
	for (int trangleIndex = 0; trangleIndex < trangleCount; ++trangleIndex) {

		auto range = (trangleIndex + 1) * 3;
		for (int i = trangleIndex * 3; i < range; ++i) {

			auto pointAIndex = indices[i];
			auto pointBIndex = indices[(i + 1) % range];

			line l = {
				vertices[pointAIndex] * 0.5f + 0.5f, vertices[pointAIndex + 1] * 0.5f + 0.5f,
				vertices[pointBIndex] * 0.5f + 0.5f, vertices[pointBIndex + 1] * 0.5f + 0.5f,
			};
			
			// TODO : x1 - x0 == 0 ? 
			auto k = (l.y1 - l.y0) / (l.x1 - l.x0);
			auto b = l.y0 - k * l.x0;

			int yStart = min(l.y0, l.y1) * renderTarget.rows;
			int yEnd = max(l.y0, l.y1) * renderTarget.rows;

			int xStart = min(l.x0, l.x1) * renderTarget.cols;
			xStart *= renderTarget.channels();
			int xEnd = max(l.x0, l.x1) * renderTarget.cols;
			xEnd *= renderTarget.channels();
			
			//for (int y = yStart; y <= yEnd; ++y) {
			//	auto row_ptr = renderTarget.ptr<char>(y);
			//	for (int x = xStart; x <= xEnd; x += renderTarget.channels()) {
			//		// BGR -> [0,1,2] 
			//		row_ptr[x+1] = (char)128;
			//	}
			//}

			for (int x = xStart; x <= xEnd; x += renderTarget.channels()) {
				int y = x * k + b;
				if (y >= renderTarget.rows || y <= 0) continue;
				std::cout << "x:"<< x << ",y:" << y << ", k:" << k << ",b:" << b << std::endl;
				auto row_ptr = renderTarget.ptr<char>(y);
				row_ptr[x + 1] = (char)128;
			}
		}
	}
	
	

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
