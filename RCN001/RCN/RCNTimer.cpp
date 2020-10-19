/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTimer.cpp
Description:    Provides a common timer interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNTimer.h"
#include <windows.h>

// -----------------------------------------
// STATIC DEFINITIONS

double CRCNTimer::s_dStartTick = 0;

// -----------------------------------------
// CLASS DEFINITIONS

CRCNTimer::CRCNTimer()
{
	Reset();
}

void CRCNTimer::Initialise()
{
	Reset();
}

// Reset this timer
void CRCNTimer::Reset()
{
	// WARNING: GetTickCount() isn't very high precision (~15ms on some older systems)
	// but will suffice for our immediate needs. 
	s_dStartTick = GetTickCount();
}

// Get ticks since last Reset()
double CRCNTimer::GetTicks()
{
	double dCurrentTick = 0;
	dCurrentTick = GetTickCount();
	return dCurrentTick - s_dStartTick;
}
