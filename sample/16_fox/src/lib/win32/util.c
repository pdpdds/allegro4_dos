#ifdef _WIN32
#include <Windows.h>
#endif
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}

void usleep(unsigned int usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (__int64)usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

int vasprintf(char **strp, const char *fmt, va_list ap)
{
	int r = -1, size = _vscprintf(fmt, ap);

	if ((size >= 0) && (size < INT_MAX))
	{
		*strp = (char *)malloc(size + 1); //+1 for null
		if (*strp)
		{
			r = vsnprintf(*strp, size + 1, fmt, ap);  //+1 for null
			if ((r < 0) || (r > size))
			{
				free(*strp);
				r = -1;
			}
		}
	}
	else { *strp = 0; }

	return(r);
}