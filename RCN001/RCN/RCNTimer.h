/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTimer.h
Description:    Provides a common timer interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNTIMER_H_
#define _RCN_RCNTIMER_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNTimer
{
public:
	// Constructor
	CRCNTimer();

	static void Initialise();
	static void Reset();

	// Get ticks since last Reset()
	static double GetTicks();

private:
	static double s_dStartTick;	// Holds the number of ticks at Reset()

};

#endif	// _RCN_RCNTIMER_H_