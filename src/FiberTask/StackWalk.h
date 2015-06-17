
#pragma once

typedef std::vector<DWORD> TCallStack;


typedef struct 
{
	std::string szModuleName;
	DWORD dwStartAddress;
	DWORD dwSize;
} SModuleInfo;


DWORD TraceCallStack(TCallStack& stack);
void LoadSymbols(std::vector<SModuleInfo>& moduleInfos);
