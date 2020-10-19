/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNUtil.cpp
Description:    Provides simple utility functions.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNUtil.h"

#if defined RCNDEBUG
// -----------------------------------------
// DEBUG

#if defined WIN32
#include <stdio.h>
void OutputDebugStringf(LPCTSTR lpszFormat, ...)
{
	va_list marker;
	TCHAR szBuf[256];

	va_start(marker, lpszFormat);
	vsprintf_s(szBuf, lpszFormat, marker);
	va_end(marker);

	OutputDebugString(szBuf);
}
#endif
#endif

// -----------------------------------------
// RANDOM NUMBERS

#if defined WIN32
#include <stdlib.h>
#include <time.h>
int Random(int iMin, int iMax)
{
	static bool bSeeded = false;

	if(!bSeeded)
	{
		srand ((unsigned int)time(NULL));
		bSeeded = true;
	}

	return (rand() % (iMax+1)) + iMin;
}
#endif

// -----------------------------------------
// MATHS

#if defined WIN32
#include <math.h>

float RCNSin(float fRadians)
{
	return (float)sin(fRadians);
}

float RCNCos(float fRadians)
{
	return (float)cos(fRadians);
}

float RCNSquareRoot(float fA)
{
	return sqrt(fA);
}

float RCNACos(float fRadians)
{
	return acos(fRadians);
}

int Min(int iA, int iB)
{
	return iA < iB ? iA : iB;
}

int Max(int iA, int iB)
{
	return iA > iB ? iA : iB;
}

int MinMax(int iMin, int iMax, int iA)
{
	return iA < iMin ? iMin : iA > iMax ? iMax : iA;
}
#endif