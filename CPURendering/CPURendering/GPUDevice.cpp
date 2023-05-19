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

void GPUDevice::CreateFrameBuffers(UINT rows, UINT cols, DepthBit depthBit)
{
	Release();

	m_BufferPtrs = new Mat* [BufferCount];
	m_BufferPtrs[0] = new Mat(rows, cols, CV_8UC3);
	m_BufferPtrs[0]->setTo(0);
	m_IsFirstBackBuffer = true;

	m_BufferPtrs[1] = new Mat(rows, cols, CV_8UC3);
	m_BufferPtrs[1]->setTo(256);

	m_AspectRatio = (float)rows / (float)cols;
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
	BackBuffer()->setTo(0);
	imshow(m_WindowName, *BackBuffer());
}

void GPUDevice::Draw()
{
	//m_RenderingThread.DispatchWork(1);

	std::vector<float> vertices;

	vertices.push_back(0.0f);
	vertices.push_back(0.5f); // A 0 
	vertices.push_back(0.0f);

	vertices.push_back(0.5f);
	vertices.push_back(-0.5f); // B 3 
	vertices.push_back(0.0f);

	vertices.push_back(-0.5f);
	vertices.push_back(-0.5f); // C 6
	vertices.push_back(0.0f);

	vertices.push_back(-0.5f);
	vertices.push_back(0.5f); // D 9
	vertices.push_back(0.0f);

	vertices.push_back(0.5f);
	vertices.push_back(0.5f); // E 12
	vertices.push_back(0.0f);

	vertices.push_back(0.0f);
	vertices.push_back(-0.5f); // F 15
	vertices.push_back(0.0f);

	std::vector<unsigned int> indices;

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(6);

	indices.push_back(9);
	indices.push_back(12);
	indices.push_back(15);

	indices.push_back(9);
	indices.push_back(15);
	indices.push_back(3);

	indices.push_back(9);
	indices.push_back(12);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(15);
	indices.push_back(3);

	indices.push_back(3);
	indices.push_back(15);
	indices.push_back(6);

	VertexBuffer vbo{
		vertices,
		indices,
		VertexLayout::Vertex
	};

	DrawLineWithSlop(vbo);

	//DrawCoordianteAxis();

	/*
	* 1.对于任意一条扫描线，它和某条边的交点为上一次扫描线和该边的交点加上一个跟边斜率相关的偏移。
	* 2.每条边上下端点的值是固定的，上端点y坐标减下端点y坐标为该边在屏幕上的跨度，当扫描线扫描完整个跨度后，就不再和该边相交。
	* 3.对于第二条中不会再相交的边，扫描线会和该边相邻接的下一条边相交，而交点位置的x值为这条边上端点的x。
	* 参考 https://zhuanlan.zhihu.com/p/96824553
	*/
	// TODO: 扫描线


}

void GPUDevice::DrawCoordianteAxis()
{
	auto renderTarget = *BackBuffer();

	for (int i = 0; i < renderTarget.cols * renderTarget.channels(); i++) {
		renderTarget.ptr<uchar>(renderTarget.rows / 2)[i] = (uchar)255;
	}
	for (int i = 0; i < renderTarget.rows; i++) {
		auto horizonal = renderTarget.ptr<uchar>(i);
		horizonal[renderTarget.cols * renderTarget.channels() / 2] = (uchar)255;
		horizonal[renderTarget.cols * renderTarget.channels() / 2 + 1] = (uchar)255;
		horizonal[renderTarget.cols * renderTarget.channels() / 2 + 2] = (uchar)255;
	}
}

void GPUDevice::DrawLineWithSlop(const VertexBuffer& vbo)
{
	auto renderTarget = *BackBuffer();
	int triangleCount = vbo.indices.size() / 3;
	for (int triangleIndex = 0; triangleIndex < triangleCount; ++triangleIndex) {

		int offset = triangleIndex * 3;
		for (int i = 0; i < 3; ++i) {

			auto pointAIndex = vbo.indices[i + offset];
			auto pointBIndex = vbo.indices[(i + 1) % 3 + offset];

			Line2D line2d = {
				Point2D{vbo.vertice[pointAIndex] * 0.5f + 0.5f, vbo.vertice[pointAIndex + 1] * 0.5f + 0.5f},
				Point2D{vbo.vertice[pointBIndex] * 0.5f + 0.5f, vbo.vertice[pointBIndex + 1] * 0.5f + 0.5f},
			};


			int yStart = min(line2d.start.y, line2d.end.y) * renderTarget.rows;
			int yEnd = max(line2d.start.y, line2d.end.y) * renderTarget.rows;

			int xStart = min(line2d.start.x, line2d.end.x) * renderTarget.cols;
			xStart *= renderTarget.channels();
			int xEnd = max(line2d.start.x, line2d.end.x) * renderTarget.cols;
			xEnd *= renderTarget.channels();


			// TODO : x1 - x0 == 0 ? 
			bool isSlopExist = !float_equal(line2d.end.x, line2d.start.x);

			float k;
			float b;
			
			if (isSlopExist) {
				k = (line2d.end.y - line2d.start.y) / (line2d.end.x - line2d.start.x) * m_AspectRatio;
				b = (line2d.start.y * renderTarget.rows - k * line2d.start.x * renderTarget.cols);
			}

			for (int x = xStart; x <= xEnd; x += renderTarget.channels()) {
				if (isSlopExist) {
					int y = (x * k / renderTarget.channels() + b);
					//if (y >= renderTarget.rows || y < 0) continue;
					//std::cout << "x:"<< x / renderTarget.channels() << ",y:" << y << ", k:" << k << ",b:" << b << std::endl;
					auto row_ptr = renderTarget.ptr<uchar>(renderTarget.rows - y - 1);
					row_ptr[x] = (uchar)255;
				}
				else {
					for (int y = yStart; y <= yEnd; ++y) {
						auto row_ptr = renderTarget.ptr<uchar>(y);
						row_ptr[x] = (uchar)255;
					}
				}
			}
		}
	}
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
