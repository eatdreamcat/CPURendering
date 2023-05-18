#include "RenderingThread.h"

void RenderingThread::DispatchWork(UINT threadCount)
{
	int count = 0;
	std::atomic<int> complete = 0;
	while (count++ < threadCount)
	{
		auto t = std::thread([&complete]() {

			++complete;
			});

		t.detach();

	}

	auto wait = std::thread([&complete, &threadCount]() {

		while (complete < threadCount);

	});

	wait.join();

}
