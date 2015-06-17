
#include "stdafx.h"
#include "StackWalk.h"

static DWORD DoStackWalk(STACKFRAME64& stackFrame, TCallStack& stack)
{
	// hash to be return to caller
	DWORD dwHash = 0;

	// find info on current process (StackWalk API requires it)
	HANDLE hProcess = ::GetCurrentProcess();
	HANDLE hCurThread = ::GetCurrentThread();

	// call StackWalk until we reached the top of the stack
	do 
	{
		BOOL bRes = ::StackWalk64(IMAGE_FILE_MACHINE_I386, hProcess, hCurThread, &stackFrame, NULL,
									NULL,
									SymFunctionTableAccess64,
									SymGetModuleBase64,
									NULL);

		// if call failed then notify the caller that the stackframe is unavailable
		if( ! bRes )
		{
			stack.push_back(0xFFFFFFF);
			break;
		}

		// get IP
		DWORD offset = (DWORD)stackFrame.AddrPC.Offset;
		stack.push_back(offset);

		// hash computation
		dwHash = 33*dwHash + offset;
	} while( 0 != stackFrame.AddrReturn.Offset );

	return dwHash;
}


DWORD TraceCallStack(TCallStack& stack)
{
	// get IP and stack info
	// there is no instruction to get EIP normally
	// register is read using a call (pushing EIP on stack) and poping it immediately
	CONTEXT ctx;
	_asm {
			call x
		x: 
			pop ctx.Eip
			mov ctx.Ebp, ebp
			mov ctx.Esp, esp
	}

	// build the required info for StackWalk function
	STACKFRAME64 stackFrame;
	::ZeroMemory(&stackFrame, sizeof(STACKFRAME64));

	// set instruction pointer
	stackFrame.AddrPC.Offset = ctx.Eip;
	stackFrame.AddrPC.Mode = AddrModeFlat;

	// set frame pointer
	stackFrame.AddrFrame.Offset = ctx.Ebp;		
	stackFrame.AddrFrame.Mode = AddrModeFlat;

	// set stack pointer
	stackFrame.AddrStack.Offset = ctx.Esp;
	stackFrame.AddrStack.Mode = AddrModeFlat;

	// walk the stack
	return DoStackWalk(stackFrame, stack);
}



void LoadSymbols(std::vector<SModuleInfo>& moduleInfos)
{
	// get process/thread info
	HANDLE hProcess = ::GetCurrentProcess();
	DWORD dwPid = ::GetCurrentProcessId();

	// initialize symbol loading
	::SymInitialize(hProcess, NULL, FALSE);

	// initialize module enumeration function
	HANDLE hth32 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	MODULEENTRY32 modEntry;
	BOOL more = Module32First(hth32, &modEntry);
	while( more )
	{
		// load symbols
		SymLoadModule64(hProcess, NULL, modEntry.szExePath, modEntry.szModule, 
					    reinterpret_cast<DWORD>(modEntry.modBaseAddr), modEntry.modBaseSize);

		// store the info
		SModuleInfo moduleInfo;
		moduleInfo.szModuleName = modEntry.szModule;
		moduleInfo.dwStartAddress = reinterpret_cast<DWORD>(modEntry.modBaseAddr);
		moduleInfo.dwSize = modEntry.modBaseSize;
		moduleInfos.push_back(moduleInfo);

		more = Module32Next(hth32, &modEntry);
	}

	// release handle on the current process
	CloseHandle(hth32);
}

