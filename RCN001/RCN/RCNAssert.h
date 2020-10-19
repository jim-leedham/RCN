/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNAssert.h
Description:    Provides a custom assert handler

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNASSERT_H_
#define _RCN_RCNASSERT_H_

#if defined WIN32

#include <assert.h>
#define RCNAssert(condition) RCNAssertStr(condition, "[NO COMMENT]")
#if defined RCNDEBUG
#include <stdlib.h>
#define RCNAssertStr(condition, message) 		\
	if (! (condition)) {						\
		RCNPrintf("\nAssertion failed!");		\
		RCNPrintf("\nFile: %s", __FILE__);		\
		RCNPrintf("\nLine: %d", __LINE__);		\
		RCNPrintf("\nInfo: %s\n", message);		\
		/* TODO: Rather than just exit, give the user the option to break into the program */\
		std::exit(EXIT_FAILURE);				\
	}											\
	else // This exists to allow the trailing semicolon
#else
#define RCNAssertStr(condition, message)
#endif
#endif

#endif	// _RCN_RCNASSERT_H_