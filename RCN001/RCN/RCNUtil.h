/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNUtil.h
Description:    Provides simple utility functions.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNUTIL_H_
#define _RCN_RCNUTIL_H_

#if defined RCNDEBUG
// -----------------------------------------
// DEBUG

void OutputDebugStringf(const char * _Format, ...);
#define RCNPrintf(...) OutputDebugStringf(__VA_ARGS__)

#else
#define RCNPrintf(...)
#endif

// -----------------------------------------
// RANDOM NUMBERS

int Random(int iMin, int iMax);
#define RCNRandom(iMin, iMax) Random(iMin, iMax)

// -----------------------------------------
// MATHS

#define PI			3.14159265359f
#define HALFPI		0.01745329251f
#define RAD_TO_DEG	57.2957795131f
#define DEG_TO_RAD	HALFPI

float RCNSin(float fRadians);
float RCNCos(float fRadians);
float RCNSquareRoot(float fA);
float RCNACos(float fRadians);

int Min(int iA, int iB);
int Max(int iA, int iB);
int MinMax(int iMin, int iMax, int iA);


#endif	// _RCN_RCNUTIL_H_