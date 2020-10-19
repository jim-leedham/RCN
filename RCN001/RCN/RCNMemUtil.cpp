/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNMemUtil.cpp
Description:    Mediates all heap allocations. Overrides any platform-specific
				new/delete functions with versions that can perform additional
				operations. Supports the adding of custom bit patterns to the 
				start of all allocations for the purpose of catching memory 
				overruns. Also tracks the number of allocations in order to
				catch memory leaks during shutdown. Stores the filename and 
				line number of each allocation to assist in tracing memory leaks

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNMemUtil.h"

// -----------------------------------------
// STATIC DEFINITIONS

int								CRCNMemUtil::s_iNumberOfActiveAllocations = 0;
CRCNMemUtil::TAllocationData	CRCNMemUtil::s_pActiveAllocationPtrs[ms_iMaxAllocations] = { 0 };
bool							CRCNMemUtil::s_bAllocationGuardsEnabled = false;
bool							CRCNMemUtil::s_bAllocationTracking = false;

#if defined WIN32
#include <DbgHelp.h>

#define RCN_GUARDBAND (0xab)
#define RCN_GUARDBAND_SIZE 1	// in bytes

void* operator new(std::size_t n) throw(std::bad_alloc)
{
	using namespace std;

	while(true) {
		void* allocated_memory = ::operator new(n, nothrow);
		if (allocated_memory != 0) return allocated_memory;

		// Store the global new handler
		new_handler global_handler = set_new_handler(0);
		set_new_handler(global_handler);

		if (global_handler) {
			global_handler();
		} else {
			throw bad_alloc();
		}
	}
}

void* operator new(std::size_t n, std::nothrow_t const&) throw()
{
	if (n == 0) 
		n = 1;

	return CRCNMemUtil::RCNAlloc(n);
}

void* operator new[](std::size_t n) throw(std::bad_alloc)
{
	return ::operator new(n);
}

void* operator new[](std::size_t n, std::nothrow_t const&) throw()
{
	return ::operator new(n, std::nothrow);
}

void operator delete(void* p) throw()
{
	CRCNMemUtil::RCNFree(p);
}

void operator delete(void* p, std::nothrow_t const&) throw()
{
	::operator delete(p);
}

void operator delete[](void* p) throw()
{
	::operator delete(p);
}

void operator delete[](void* p, std::nothrow_t const&) throw()
{
	::operator delete(p);
}

void* CRCNMemUtil::RCNAlloc(std::size_t n)
{
	void* ptr;

	if(s_bAllocationGuardsEnabled)
	{
		RCNAssertStr(s_iNumberOfActiveAllocations < ms_iMaxAllocations, "Cannot track any more heap allocations. Consider increasing MAX_ALLOCATIONS");

		ptr = malloc(n + RCN_GUARDBAND_SIZE);

		*((byte*)ptr) = RCN_GUARDBAND;

		ptr = (byte*)ptr + RCN_GUARDBAND_SIZE;
	}
	else
	{
		ptr = malloc(n);
	}

	if(s_bAllocationTracking)
	{
		int iIndex = 0;
		for(iIndex = 0; iIndex < ms_iMaxAllocations; iIndex++)
		{
			if(s_pActiveAllocationPtrs[iIndex].pPtr == NULL)
				break;
		}
		s_pActiveAllocationPtrs[iIndex].pPtr = ptr;

#if defined RCNDEBUG
		// This code can be quite a performance hit, so only use it in Debug
		SymSetOptions(SYMOPT_LOAD_LINES);
		HANDLE process = GetCurrentProcess();
		SymInitialize( process, NULL, TRUE );
		PVOID stack[ 4 ] = { 0 };
		unsigned short frames = CaptureStackBackTrace( 0, 4, stack, NULL );

		IMAGEHLP_LINE64 line;
		DWORD  dwDisplacement;
		SymGetLineFromAddr64(process, (DWORD64)stack[3], &dwDisplacement, &line);
		strncpy_s(s_pActiveAllocationPtrs[iIndex].sFilename, line.FileName, 128);
		s_pActiveAllocationPtrs[iIndex].iLineNumber = line.LineNumber;
#endif
	}

	s_iNumberOfActiveAllocations++;

	return ptr;
}

// -----------------------------------------
// CLASS DEFINITIONS

void CRCNMemUtil::RCNFree(void* ptr)
{
	if(ptr == NULL)
		return;

	if(s_bAllocationTracking)
	{
		for(int i = 0; i < ms_iMaxAllocations; i++)
		{
			if(s_pActiveAllocationPtrs[i].pPtr == ptr)
			{
				s_pActiveAllocationPtrs[i].pPtr = NULL;
				break;
			}
		}
	}

	if(s_bAllocationGuardsEnabled)
	{
		ptr = (byte*)ptr - RCN_GUARDBAND_SIZE;

		CheckGuardbandValidity((byte*)ptr);
	}

	free(ptr);
	s_iNumberOfActiveAllocations--;
}
void CRCNMemUtil::EnableAllocationTracking()
{
	s_bAllocationTracking = true;
	for(int i = 0; i < ms_iMaxAllocations; i++)
		s_pActiveAllocationPtrs[i].pPtr = NULL;
}

void CRCNMemUtil::EnableAllocationGuards()
{
	s_bAllocationGuardsEnabled = true;
}

void CRCNMemUtil::CheckHeapValidity()
{
	for(int i = 0; i < s_iNumberOfActiveAllocations; i++)
	{
		void* ptr = s_pActiveAllocationPtrs[i].pPtr;
		ptr = (byte*)ptr - RCN_GUARDBAND_SIZE;
		CheckGuardbandValidity((byte*)ptr);
	}
}

void CRCNMemUtil::CheckGuardbandValidity(byte* ptr)
{		
	// This type of memory corruption is typically due to buffer overruns
	RCNAssertStr(*ptr == RCN_GUARDBAND, "Memory corruption detected!");
}

void CRCNMemUtil::Shutdown()
{
	RCNAssertStr(s_iNumberOfActiveAllocations == 0, "Memory leak detected");
}

#pragma warning(default: 4290)
#endif