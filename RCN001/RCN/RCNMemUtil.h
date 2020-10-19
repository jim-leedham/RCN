/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNMemUtil.h
Description:    Mediates all heap allocations. Overrides any platform-specific
				new/delete functions with versions that can perform additional
				operations. Supports the adding of custom bit patterns to the 
				start of all allocations for the purpose of catching memory 
				overruns. Also tracks the number of allocations in order to
				catch memory leaks during shutdown. Stores the filename and 
				line number of each allocation to assist in tracing memory leaks

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNMEMUTIL_H_
#define _RCN_RCNMEMUTIL_H_

// TODO: Add guard sections to the end of heap allocations. We already add them
// to the start of each allocation so this should be a trivial change.

#if defined WIN32

#include <new>
#include <stdexcept>

#pragma warning(disable: 4290) // Warning that VC++ does not support exception specifications

void* operator new(std::size_t n) throw(std::bad_alloc);
void* operator new(std::size_t n, std::nothrow_t const&) throw();
void* operator new[](std::size_t n) throw(std::bad_alloc);
void* operator new[](std::size_t n, std::nothrow_t const&) throw();

void operator delete(void* p) throw();
void operator delete(void* p, std::nothrow_t const&) throw();
void operator delete[](void* p) throw();
void operator delete[](void* p, std::nothrow_t const&) throw();

#define RCNNew		new
#define RCNDelete	delete
#endif

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNMemUtil
{
public:

	static void*	RCNAlloc(std::size_t n);
	static void		RCNFree(void* ptr);

	static void		EnableAllocationTracking();
	static void		EnableAllocationGuards();
	static void		CheckHeapValidity();

	static void		Shutdown();

private:

	struct TAllocationData
	{
		void*		pPtr;
		char		sFilename[128];
		int			iLineNumber;
	};

	static void		CheckGuardbandValidity(byte* ptr);

	static int					s_iNumberOfActiveAllocations;
	static const int			ms_iMaxAllocations = 1024;
	static TAllocationData		s_pActiveAllocationPtrs[ms_iMaxAllocations];
	static bool					s_bAllocationGuardsEnabled;
	static bool					s_bAllocationTracking;
};

#endif	// _RCN_RCNMEMUTIL_H_