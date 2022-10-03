#pragma once

#ifndef __MEMORY_MODULE_HEADER
#define __MEMORY_MODULE_HEADER

typedef HMODULE HMEMORYMODULE;

typedef struct _MEMORYMODULE {
	/*
		---------------------------
		|xxxxxxxx    BaseAddress  |
		|...                      |
		|...                      |
		|...                      | --> IMAGE_DOS_HEADER
		|...                      | --> IMAGE_NT_HEADERS
		|...                      |
		|...                      |
		--------------------------
		struct MEMORYMODULE;
		... (align)
		codes
	*/
	ULONG64 Signature;

	DWORD SizeofHeaders;
	union {
		struct {
			//Status Flags
			BYTE initialized : 1;
			BYTE loadFromNtLoadDllMemory : 1;
			BYTE underUnload : 1;
			BYTE reservedStatusFlags : 5;

			BYTE cbFlagsReserved;

			//Load Flags
			WORD MappedDll : 1;
			WORD InsertInvertedFunctionTableEntry : 1;
			WORD TlsHandled : 1;
			WORD UseReferenceCount : 1;
			WORD reservedLoadFlags : 12;

		};
		DWORD dwFlags;
	};

	LPBYTE codeBase;						//codeBase == ImageBase
	PVOID lpReserved;

	HMODULE* hModulesList;					//Import module handles
	DWORD dwModulesCount;					//number of module handles
	DWORD dwReferenceCount;

	DWORD dwImageFileSize;
	//DWORD headers_align;				//headers_align == OptionalHeaders.BaseOfCode;

} MEMORYMODULE, * PMEMORYMODULE;


#define MEMORY_MODULE_SIGNATURE 0x00aabbcc11ffee00

#ifdef __cplusplus
extern "C" {
#endif

	NTSTATUS MemoryLoadLibrary(
		_Out_ HMEMORYMODULE* MemoryModuleHandle,
		_In_ LPCVOID data,
		_In_ DWORD size
	);

	NTSTATUS MemoryResolveImportTable(
		_In_ LPBYTE base,
		_In_ PIMAGE_NT_HEADERS lpNtHeaders,
		_In_ PMEMORYMODULE hMemoryModule
	);

	NTSTATUS MemorySetSectionProtection(
		_In_ LPBYTE base,
		_In_ PIMAGE_NT_HEADERS lpNtHeaders
	);

    BOOL MemoryFreeLibrary(HMEMORYMODULE);

	BOOL WINAPI IsValidMemoryModuleHandle(HMEMORYMODULE hModule);

	PMEMORYMODULE WINAPI MapMemoryModuleHandle(HMEMORYMODULE hModule);

#ifdef __cplusplus
}
#endif

#endif  // __MEMORY_MODULE_HEADER
