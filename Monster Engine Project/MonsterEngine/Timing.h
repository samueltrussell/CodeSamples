
#ifndef TIMING_H
#define TIMING_H

#define MAX_FRAME_RATE 120.0f

#include <WinBase.h>

namespace Engine
{

	namespace Timing
	{
		LARGE_INTEGER lastCount;
		LARGE_INTEGER initCount;
		LARGE_INTEGER frequency;

#ifdef MAX_FRAME_RATE
		double minFrameTime = 1000.0f / MAX_FRAME_RATE;
#endif

		bool Initialize()
		{
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&initCount);
			QueryPerformanceCounter(&lastCount);

			return true;
		}

		LARGE_INTEGER TotalRunTime()
		{
			LARGE_INTEGER currentCounts, totalTimeCounts;
			QueryPerformanceCounter(&currentCounts);
			totalTimeCounts.QuadPart = currentCounts.QuadPart - initCount.QuadPart;
			return totalTimeCounts;
		}

		double CalcLastFrameTime()
		{
			LARGE_INTEGER currentCount;
			
			QueryPerformanceCounter(&currentCount);
			
			LARGE_INTEGER elapsedCount;

			elapsedCount.QuadPart = currentCount.QuadPart - lastCount.QuadPart;

			elapsedCount.QuadPart *= 1000000;
			elapsedCount.QuadPart /= frequency.QuadPart;

			lastCount = currentCount;

			double dT = elapsedCount.QuadPart/1000.0f;

#ifdef MAX_FRAME_RATE
			if (dT > 3.0 * minFrameTime) {
				return minFrameTime;
			}
#endif

			return dT;
		}

		double  CalcCurrentFrameTime()
		{
			LARGE_INTEGER currentCount;

			QueryPerformanceCounter(&currentCount);

			LARGE_INTEGER elapsedCount;

			elapsedCount.QuadPart = currentCount.QuadPart - lastCount.QuadPart;

			elapsedCount.QuadPart *= 1000000;
			elapsedCount.QuadPart /= frequency.QuadPart;

			double dT = elapsedCount.QuadPart / 1000.0f;

			return dT;

		}

	}//namespace Timing

}//namespace Engine

#endif //TIMING_H
