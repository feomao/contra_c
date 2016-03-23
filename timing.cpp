/*

  timing.cpp

*/

#include <windows.h>
#include <stdio.h>
#include "timing.h"

static LARGE_INTEGER oldt;
static __int64 oneframe;
static unsigned char rand8;

//初始化时间变量
//	取得高精度计数器频率
//	计算每帧所需的高精度计数
//	上一帧的高精度计数 =现高精度计数
int init_timer(int frate)
{
	LARGE_INTEGER pfq;
	__int64 pfq64;
	BOOL rv;

	// check for high precision timer

	rv=QueryPerformanceFrequency(&pfq);
	if (rv==0)
		return 0;

	pfq64=pfq.QuadPart;
	oneframe=pfq64/frate;

	rv=QueryPerformanceCounter(&oldt);
	if (rv==0)
		return 0;

	return 1;
}

//限速为60帧/秒
void trim_speed()
{
	LARGE_INTEGER curt;
	__int64 curt64,oldt64;
	
	//读取高精度计数器的现计数
	QueryPerformanceCounter(&curt);
	curt64=curt.QuadPart;
	//取得上一帧的高精度计数
	oldt64=oldt.QuadPart;

	rand8 =(unsigned char) (curt64 &0xff);

	//等待，直到 现计数-上一帧计数 达到 每帧所需的计数
	while ((curt64-oldt64)<oneframe)
	{
		QueryPerformanceCounter(&curt);
		curt64=curt.QuadPart;
	}

	//更新 上一帧计数=现计数
	oldt=curt;
}

unsigned char get_rand8(){
	return rand8;
}

