//===============================================================================================//
// Your mom is a mirror.tk paste
// POSSIBILITY OF SUCH DAMAGE.
//===============================================================================================//
#include "ReflectiveLoader.h"
//===============================================================================================//
// Our loader will set this to a pseudo correct HINSTANCE/HMODULE value
HINSTANCE hAppInstance = NULL;
//===============================================================================================//
#pragma intrinsic( _ReturnAddress )
// This function can not be inlined by the compiler or we will not get the address we expect. Ideally 
// this code will be compiled with the /O2 and /Ob1 switches. Bonus points if we could take advantage of 
// RIP relative addressing in this instance but I dont believe we can do so with the compiler intrinsics 
// available (and no inline asm available under x64).
__declspec(noinline) ULONG_PTR caller( VOID ) { return (ULONG_PTR)_ReturnAddress(); }
//===============================================================================================//
// Note 1: If you want to have your own DllMain, define REFLECTIVEDLLINJECTION_CUSTOM_DLLMAIN,  

//         otherwise the DllMain at the end of this file will be used.

// Note 2: If you are injecting the DLL via LoadRemoteLibraryR, define REFLECTIVEDLLINJECTION_VIA_LOADREMOTELIBRARYR,
//         otherwise it is assumed you are calling the ReflectiveLoader via a stub.

// This is our position independent reflective DLL loader/injector

#define REFLECTIVEDLLINJECTION_CUSTOM_DLLMAIN

#ifdef REFLECTIVEDLLINJECTION_VIA_LOADREMOTELIBRARYR
DLLEXPORT ULONG_PTR WINAPI ReflectiveLoader( LPVOID lpParameter )
#else
DLLEXPORT ULONG_PTR WINAPI ReflectiveLoader( VOID )
#endif
{
	// the functions we need
	LOADLIBRARYA pLoadLibraryA     = NULL;
	GETPROCADDRESS pGetProcAddress = NULL;
	VIRTUALALLOC pVirtualAlloc     = NULL;
	NTFLUSHINSTRUCTIONCACHE pNtFlushInstructionCache = NULL;

	USHORT usCounter;

	// the initial location of this image in memory
	ULONG_PTR uiLibraryAddress;
	// the kernels base address and later this images newly loaded base address
	ULONG_PTR uiBaseAddress;

	// variables for processing the kernels export table
	ULONG_PTR uiAddressArray;
	ULONG_PTR uiNameArray;
	ULONG_PTR uiExportDir;
	ULONG_PTR uiNameOrdinals;
	DWORD dwHashValue;

	// variables for loading this image
	ULONG_PTR uiHeaderValue;
	ULONG_PTR uiValueA;
	ULONG_PTR uiValueB;
	ULONG_PTR uiValueC;
	ULONG_PTR uiValueD;
	ULONG_PTR uiValueE;

	// STEP 0: calculate our images current base address

	// we will start searching backwards from our callers return address.
	uiLibraryAddress = caller();

	// loop through memory backwards searching for our images base address
	// we dont need SEH style search as we shouldnt generate any access violations with this
	while( TRUE )
	{
		if( ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_magic == IMAGE_DOS_SIGNATURE )
		{
			uiHeaderValue = ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew;
			// some x64 dll's can trigger a bogus signature (IMAGE_DOS_SIGNATURE == 'POP r10'),
			// we sanity check the e_lfanew with an upper threshold value of 1024 to avoid problems.
			if( uiHeaderValue >= sizeof(IMAGE_DOS_HEADER) && uiHeaderValue < 1024 )
			{
				uiHeaderValue += uiLibraryAddress;
				// break if we have found a valid MZ/PE header
				if( ((PIMAGE_NT_HEADERS)uiHeaderValue)->Signature == IMAGE_NT_SIGNATURE )
					break;
			}
		}
		uiLibraryAddress--;
	}

	// STEP 1: process the kernels exports for the functions our loader needs...

	// get the Process Enviroment Block
#ifdef WIN_X64
	uiBaseAddress = __readgsqword( 0x60 );
#else
#ifdef WIN_X86
	uiBaseAddress = __readfsdword( 0x30 );
#else WIN_ARM
	//uiBaseAddress = *(DWORD *)( (BYTE *)_MoveFromCoprocessor( 15, 0, 13, 0, 2 ) + 0x30 );
#endif
#endif

	// get the processes loaded modules. ref: http://msdn.microsoft.com/en-us/library/aa813708(VS.85).aspx
	uiBaseAddress = (ULONG_PTR)((_PPEB)uiBaseAddress)->pLdr;

	// get the first entry of the InMemoryOrder module list
	uiValueA = (ULONG_PTR)((PPEB_LDR_DATA)uiBaseAddress)->InMemoryOrderModuleList.Flink;
	while( uiValueA )
	{
		// get pointer to current modules name (unicode string)
		uiValueB = (ULONG_PTR)((PLDR_DATA_TABLE_ENTRY)uiValueA)->BaseDllName.pBuffer;
		// set bCounter to the length for the loop
		usCounter = ((PLDR_DATA_TABLE_ENTRY)uiValueA)->BaseDllName.Length;
		// clear uiValueC which will store the hash of the module name
		uiValueC = 0;

		// compute the hash of the module name...
		do
		{
			uiValueC = ror( (DWORD)uiValueC );
			// normalize to uppercase if the madule name is in lowercase
			if( *((BYTE *)uiValueB) >= 'a' )
				uiValueC += *((BYTE *)uiValueB) - 0x20;
			else
				uiValueC += *((BYTE *)uiValueB);
			uiValueB++;
		} while( --usCounter );

		// compare the hash with that of kernel32.dll
		if( (DWORD)uiValueC == KERNEL32DLL_HASH )
		{
			// get this modules base address
			uiBaseAddress = (ULONG_PTR)((PLDR_DATA_TABLE_ENTRY)uiValueA)->DllBase;

			// get the VA of the modules NT Header
			uiExportDir = uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew;

			// uiNameArray = the address of the modules export directory entry
			uiNameArray = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];

			// get the VA of the export directory
			uiExportDir = ( uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress );

			// get the VA for the array of name pointers
			uiNameArray = ( uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->AddressOfNames );
			
			// get the VA for the array of name ordinals
			uiNameOrdinals = ( uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->AddressOfNameOrdinals );

			usCounter = 3;

			// loop while we still have imports to find
			while( usCounter > 0 )
			{
				// compute the hash values for this function name
				dwHashValue = hash( (char *)( uiBaseAddress + DEREF_32( uiNameArray ) )  );
				
				// if we have found a function we want we get its virtual address
				if( dwHashValue == LOADLIBRARYA_HASH || dwHashValue == GETPROCADDRESS_HASH || dwHashValue == VIRTUALALLOC_HASH )
				{
					// get the VA for the array of addresses
					uiAddressArray = ( uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->AddressOfFunctions );

					// use this functions name ordinal as an index into the array of name pointers
					uiAddressArray += ( DEREF_16( uiNameOrdinals ) * sizeof(DWORD) );

					// store this functions VA
					if( dwHashValue == LOADLIBRARYA_HASH )
						pLoadLibraryA = (LOADLIBRARYA)( uiBaseAddress + DEREF_32( uiAddressArray ) );
					else if( dwHashValue == GETPROCADDRESS_HASH )
						pGetProcAddress = (GETPROCADDRESS)( uiBaseAddress + DEREF_32( uiAddressArray ) );
					else if( dwHashValue == VIRTUALALLOC_HASH )
						pVirtualAlloc = (VIRTUALALLOC)( uiBaseAddress + DEREF_32( uiAddressArray ) );
			
					// decrement our counter
					usCounter--;
				}

				// get the next exported function name
				uiNameArray += sizeof(DWORD);

				// get the next exported function name ordinal
				uiNameOrdinals += sizeof(WORD);
			}
		}
		else if( (DWORD)uiValueC == NTDLLDLL_HASH )
		{
			// get this modules base address
			uiBaseAddress = (ULONG_PTR)((PLDR_DATA_TABLE_ENTRY)uiValueA)->DllBase;

			// get the VA of the modules NT Header
			uiExportDir = uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew;

			// uiNameArray = the address of the modules export directory entry
			uiNameArray = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];

			// get the VA of the export directory
			uiExportDir = ( uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress );

			// get the VA for the array of name pointers
			uiNameArray = ( uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->AddressOfNames );
			
			// get the VA for the array of name ordinals
			uiNameOrdinals = ( uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->AddressOfNameOrdinals );

			usCounter = 1;

			// loop while we still have imports to find
			while( usCounter > 0 )
			{
				// compute the hash values for this function name
				dwHashValue = hash( (char *)( uiBaseAddress + DEREF_32( uiNameArray ) )  );
				
				// if we have found a function we want we get its virtual address
				if( dwHashValue == NTFLUSHINSTRUCTIONCACHE_HASH )
				{
					// get the VA for the array of addresses
					uiAddressArray = ( uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->AddressOfFunctions );

					// use this functions name ordinal as an index into the array of name pointers
					uiAddressArray += ( DEREF_16( uiNameOrdinals ) * sizeof(DWORD) );

					// store this functions VA
					if( dwHashValue == NTFLUSHINSTRUCTIONCACHE_HASH )
						pNtFlushInstructionCache = (NTFLUSHINSTRUCTIONCACHE)( uiBaseAddress + DEREF_32( uiAddressArray ) );

					// decrement our counter
					usCounter--;
				}

				// get the next exported function name
				uiNameArray += sizeof(DWORD);

				// get the next exported function name ordinal
				uiNameOrdinals += sizeof(WORD);
			}
		}

		// we stop searching when we have found everything we need.
		if( pLoadLibraryA && pGetProcAddress && pVirtualAlloc && pNtFlushInstructionCache )
			break;

		// get the next entry
		uiValueA = DEREF( uiValueA );
	}

	// STEP 2: load our image into a new permanent location in memory...

	// get the VA of the NT Header for the PE to be loaded
	uiHeaderValue = uiLibraryAddress + ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew;

	// allocate all the memory for the DLL to be loaded into. we can load at any address because we will  
	// relocate the image. Also zeros all memory and marks it as READ, WRITE and EXECUTE to avoid any problems.
	uiBaseAddress = (ULONG_PTR)pVirtualAlloc( NULL, ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SizeOfImage, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE );

	// we must now copy over the headers
	uiValueA = ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SizeOfHeaders;
	uiValueB = uiLibraryAddress;
	uiValueC = uiBaseAddress;

	while( uiValueA-- )
		*(BYTE *)uiValueC++ = *(BYTE *)uiValueB++;

	// STEP 3: load in all of our sections...

	// uiValueA = the VA of the first section
	uiValueA = ( (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader + ((PIMAGE_NT_HEADERS)uiHeaderValue)->FileHeader.SizeOfOptionalHeader );
	
	// itterate through all sections, loading them into memory.
	uiValueE = ((PIMAGE_NT_HEADERS)uiHeaderValue)->FileHeader.NumberOfSections;
	while( uiValueE-- )
	{
		// uiValueB is the VA for this section
		uiValueB = ( uiBaseAddress + ((PIMAGE_SECTION_HEADER)uiValueA)->VirtualAddress );

		// uiValueC if the VA for this sections data
		uiValueC = ( uiLibraryAddress + ((PIMAGE_SECTION_HEADER)uiValueA)->PointerToRawData );

		// copy the section over
		uiValueD = ((PIMAGE_SECTION_HEADER)uiValueA)->SizeOfRawData;

		while( uiValueD-- )
			*(BYTE *)uiValueB++ = *(BYTE *)uiValueC++;

		// get the VA of the next section
		uiValueA += sizeof( IMAGE_SECTION_HEADER );
	}

	// STEP 4: process our images import table...

	// uiValueB = the address of the import directory
	uiValueB = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ];
	
	// we assume their is an import table to process
	// uiValueC is the first entry in the import table
	uiValueC = ( uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiValueB)->VirtualAddress );
	
	// itterate through all imports
	while( ((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->Name )
	{
		// use LoadLibraryA to load the imported module into memory
		uiLibraryAddress = (ULONG_PTR)pLoadLibraryA( (LPCSTR)( uiBaseAddress + ((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->Name ) );

		// uiValueD = VA of the OriginalFirstThunk
		uiValueD = ( uiBaseAddress + ((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->OriginalFirstThunk );
	
		// uiValueA = VA of the IAT (via first thunk not origionalfirstthunk)
		uiValueA = ( uiBaseAddress + ((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->FirstThunk );

		// itterate through all imported functions, importing by ordinal if no name present
		while( DEREF(uiValueA) )
		{
			// sanity check uiValueD as some compilers only import by FirstThunk
			if( uiValueD && ((PIMAGE_THUNK_DATA)uiValueD)->u1.Ordinal & IMAGE_ORDINAL_FLAG )
			{
				// get the VA of the modules NT Header
				uiExportDir = uiLibraryAddress + ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew;

				// uiNameArray = the address of the modules export directory entry
				uiNameArray = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];

				// get the VA of the export directory
				uiExportDir = ( uiLibraryAddress + ((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress );

				// get the VA for the array of addresses
				uiAddressArray = ( uiLibraryAddress + ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->AddressOfFunctions );

				// use the import ordinal (- export ordinal base) as an index into the array of addresses
				uiAddressArray += ( ( IMAGE_ORDINAL( ((PIMAGE_THUNK_DATA)uiValueD)->u1.Ordinal ) - ((PIMAGE_EXPORT_DIRECTORY )uiExportDir)->Base ) * sizeof(DWORD) );

				// patch in the address for this imported function
				DEREF(uiValueA) = ( uiLibraryAddress + DEREF_32(uiAddressArray) );
			}
			else
			{
				// get the VA of this functions import by name struct
				uiValueB = ( uiBaseAddress + DEREF(uiValueA) );

				// use GetProcAddress and patch in the address for this imported function
				DEREF(uiValueA) = (ULONG_PTR)pGetProcAddress( (HMODULE)uiLibraryAddress, (LPCSTR)((PIMAGE_IMPORT_BY_NAME)uiValueB)->Name );
			}
			// get the next imported function
			uiValueA += sizeof( ULONG_PTR );
			if( uiValueD )
				uiValueD += sizeof( ULONG_PTR );
		}

		// get the next import
		uiValueC += sizeof( IMAGE_IMPORT_DESCRIPTOR );
	}

	// STEP 5: process all of our images relocations...

	// calculate the base address delta and perform relocations (even if we load at desired image base)
	uiLibraryAddress = uiBaseAddress - ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.ImageBase;

	// uiValueB = the address of the relocation directory
	uiValueB = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ];

	// check if their are any relocations present
	if( ((PIMAGE_DATA_DIRECTORY)uiValueB)->Size )
	{
		// uiValueC is now the first entry (IMAGE_BASE_RELOCATION)
		uiValueC = ( uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiValueB)->VirtualAddress );

		// and we itterate through all entries...
		while( ((PIMAGE_BASE_RELOCATION)uiValueC)->SizeOfBlock )
		{
			// uiValueA = the VA for this relocation block
			uiValueA = ( uiBaseAddress + ((PIMAGE_BASE_RELOCATION)uiValueC)->VirtualAddress );

			// uiValueB = number of entries in this relocation block
			uiValueB = ( ((PIMAGE_BASE_RELOCATION)uiValueC)->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION) ) / sizeof( IMAGE_RELOC );

			// uiValueD is now the first entry in the current relocation block
			uiValueD = uiValueC + sizeof(IMAGE_BASE_RELOCATION);

			// we itterate through all the entries in the current block...
			while( uiValueB-- )
			{
				// perform the relocation, skipping IMAGE_REL_BASED_ABSOLUTE as required.
				// we dont use a switch statement to avoid the compiler building a jump table
				// which would not be very position independent!
				if( ((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_DIR64 )
					*(ULONG_PTR *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += uiLibraryAddress;
				else if( ((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_HIGHLOW )
					*(DWORD *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += (DWORD)uiLibraryAddress;
#ifdef WIN_ARM
				// Note: On ARM, the compiler optimization /O2 seems to introduce an off by one issue, possibly a code gen bug. Using /O1 instead avoids this problem.
				else if( ((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_ARM_MOV32T )
				{	
					register DWORD dwInstruction;
					register DWORD dwAddress;
					register WORD wImm;
					// get the MOV.T instructions DWORD value (We add 4 to the offset to go past the first MOV.W which handles the low word)
					dwInstruction = *(DWORD *)( uiValueA + ((PIMAGE_RELOC)uiValueD)->offset + sizeof(DWORD) );
					// flip the words to get the instruction as expected
					dwInstruction = MAKELONG( HIWORD(dwInstruction), LOWORD(dwInstruction) );
					// sanity chack we are processing a MOV instruction...
					if( (dwInstruction & ARM_MOV_MASK) == ARM_MOVT )
					{
						// pull out the encoded 16bit value (the high portion of the address-to-relocate)
						wImm  = (WORD)( dwInstruction & 0x000000FF);
						wImm |= (WORD)((dwInstruction & 0x00007000) >> 4);
						wImm |= (WORD)((dwInstruction & 0x04000000) >> 15);
						wImm |= (WORD)((dwInstruction & 0x000F0000) >> 4);
						// apply the relocation to the target address
						dwAddress = ( (WORD)HIWORD(uiLibraryAddress) + wImm ) & 0xFFFF;
						// now create a new instruction with the same opcode and register param.
						dwInstruction  = (DWORD)( dwInstruction & ARM_MOV_MASK2 );
						// patch in the relocated address...
						dwInstruction |= (DWORD)(dwAddress & 0x00FF);
						dwInstruction |= (DWORD)(dwAddress & 0x0700) << 4;
						dwInstruction |= (DWORD)(dwAddress & 0x0800) << 15;
						dwInstruction |= (DWORD)(dwAddress & 0xF000) << 4;
						// now flip the instructions words and patch back into the code...
						*(DWORD *)( uiValueA + ((PIMAGE_RELOC)uiValueD)->offset + sizeof(DWORD) ) = MAKELONG( HIWORD(dwInstruction), LOWORD(dwInstruction) );
					}
				}
#endif
				else if( ((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_HIGH )
					*(WORD *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += HIWORD(uiLibraryAddress);
				else if( ((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_LOW )
					*(WORD *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += LOWORD(uiLibraryAddress);

				// get the next entry in the current relocation block
				uiValueD += sizeof( IMAGE_RELOC );
			}

			// get the next entry in the relocation directory
			uiValueC = uiValueC + ((PIMAGE_BASE_RELOCATION)uiValueC)->SizeOfBlock;
		}
	}

	// STEP 6: call our images entry point

	// uiValueA = the VA of our newly loaded DLL/EXE's entry point
	uiValueA = ( uiBaseAddress + ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.AddressOfEntryPoint );

	// We must flush the instruction cache to avoid stale code being used which was updated by our relocation processing.
	pNtFlushInstructionCache( (HANDLE)-1, NULL, 0 );

	// call our respective entry point, fudging our hInstance value
#ifdef REFLECTIVEDLLINJECTION_VIA_LOADREMOTELIBRARYR
	// if we are injecting a DLL via LoadRemoteLibraryR we call DllMain and pass in our parameter (via the DllMain lpReserved parameter)
	((DLLMAIN)uiValueA)( (HINSTANCE)uiBaseAddress, DLL_PROCESS_ATTACH, lpParameter );
#else
	// if we are injecting an DLL via a stub we call DllMain with no parameter
	((DLLMAIN)uiValueA)( (HINSTANCE)uiBaseAddress, DLL_PROCESS_ATTACH, NULL );
#endif

	// STEP 8: return our new entry point address so whatever called us can call DllMain() if needed.
	return uiValueA;
}
//===============================================================================================//
#ifndef REFLECTIVEDLLINJECTION_CUSTOM_DLLMAIN

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved )
{
    BOOL bReturnValue = TRUE;
	switch( dwReason ) 
    { 
		case DLL_QUERY_HMODULE:
			if( lpReserved != NULL )
				*(HMODULE *)lpReserved = hAppInstance;
			break;
		case DLL_PROCESS_ATTACH:
			hAppInstance = hinstDLL;
			break;
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
            break;
    }
	return bReturnValue;
}

#endif
//===============================================================================================//





















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FEPIOBJEYL
{ 
  void qUfoOAhbMQ()
  { 
      bool eLmqsOMOFP = false;
      bool xhsEfaEIHS = false;
      bool fGbkJyQndA = false;
      bool VZIjIEEoIC = false;
      bool ZwldReDTyH = false;
      bool UpaUyYiCgt = false;
      bool xYjEtXZJub = false;
      bool WOzErjTxSe = false;
      bool uxoVefymkj = false;
      bool UBIOEBLhjs = false;
      bool EtXVkGpWcO = false;
      bool EkiBzAiNYO = false;
      bool YlXLHUGkzq = false;
      bool XlNZZBYTjY = false;
      bool pbKWeiSGOE = false;
      bool SxIVTSVLNQ = false;
      bool AEWkqSXJay = false;
      bool bOcAXxaBfY = false;
      bool CzVuYZcrFH = false;
      bool eMFTYkEYfM = false;
      string tmzsXPpKCT;
      string lpqpcOMfPW;
      string dkdBetUHwm;
      string stYhHKgXHi;
      string rlGpGmfZdb;
      string mEhXPIJxNd;
      string uiNOZqQJhS;
      string hoYfdqemur;
      string OucUzoMBSz;
      string AjJdnJYqPP;
      string VVqLtyeCsX;
      string dVBVPzGsOj;
      string ePqiSxetqU;
      string SmXeVrERaB;
      string JJHtUdhmOo;
      string DebfkGeHsw;
      string SkLYFqyWAW;
      string uoSNZUIwOn;
      string PqfCRmKrtP;
      string MwXeiYyhnd;
      if(tmzsXPpKCT == VVqLtyeCsX){eLmqsOMOFP = true;}
      else if(VVqLtyeCsX == tmzsXPpKCT){EtXVkGpWcO = true;}
      if(lpqpcOMfPW == dVBVPzGsOj){xhsEfaEIHS = true;}
      else if(dVBVPzGsOj == lpqpcOMfPW){EkiBzAiNYO = true;}
      if(dkdBetUHwm == ePqiSxetqU){fGbkJyQndA = true;}
      else if(ePqiSxetqU == dkdBetUHwm){YlXLHUGkzq = true;}
      if(stYhHKgXHi == SmXeVrERaB){VZIjIEEoIC = true;}
      else if(SmXeVrERaB == stYhHKgXHi){XlNZZBYTjY = true;}
      if(rlGpGmfZdb == JJHtUdhmOo){ZwldReDTyH = true;}
      else if(JJHtUdhmOo == rlGpGmfZdb){pbKWeiSGOE = true;}
      if(mEhXPIJxNd == DebfkGeHsw){UpaUyYiCgt = true;}
      else if(DebfkGeHsw == mEhXPIJxNd){SxIVTSVLNQ = true;}
      if(uiNOZqQJhS == SkLYFqyWAW){xYjEtXZJub = true;}
      else if(SkLYFqyWAW == uiNOZqQJhS){AEWkqSXJay = true;}
      if(hoYfdqemur == uoSNZUIwOn){WOzErjTxSe = true;}
      if(OucUzoMBSz == PqfCRmKrtP){uxoVefymkj = true;}
      if(AjJdnJYqPP == MwXeiYyhnd){UBIOEBLhjs = true;}
      while(uoSNZUIwOn == hoYfdqemur){bOcAXxaBfY = true;}
      while(PqfCRmKrtP == PqfCRmKrtP){CzVuYZcrFH = true;}
      while(MwXeiYyhnd == MwXeiYyhnd){eMFTYkEYfM = true;}
      if(eLmqsOMOFP == true){eLmqsOMOFP = false;}
      if(xhsEfaEIHS == true){xhsEfaEIHS = false;}
      if(fGbkJyQndA == true){fGbkJyQndA = false;}
      if(VZIjIEEoIC == true){VZIjIEEoIC = false;}
      if(ZwldReDTyH == true){ZwldReDTyH = false;}
      if(UpaUyYiCgt == true){UpaUyYiCgt = false;}
      if(xYjEtXZJub == true){xYjEtXZJub = false;}
      if(WOzErjTxSe == true){WOzErjTxSe = false;}
      if(uxoVefymkj == true){uxoVefymkj = false;}
      if(UBIOEBLhjs == true){UBIOEBLhjs = false;}
      if(EtXVkGpWcO == true){EtXVkGpWcO = false;}
      if(EkiBzAiNYO == true){EkiBzAiNYO = false;}
      if(YlXLHUGkzq == true){YlXLHUGkzq = false;}
      if(XlNZZBYTjY == true){XlNZZBYTjY = false;}
      if(pbKWeiSGOE == true){pbKWeiSGOE = false;}
      if(SxIVTSVLNQ == true){SxIVTSVLNQ = false;}
      if(AEWkqSXJay == true){AEWkqSXJay = false;}
      if(bOcAXxaBfY == true){bOcAXxaBfY = false;}
      if(CzVuYZcrFH == true){CzVuYZcrFH = false;}
      if(eMFTYkEYfM == true){eMFTYkEYfM = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IBCOJHRPGE
{ 
  void nlAilFLlkh()
  { 
      bool wPoLcVJUGK = false;
      bool gmgLUMwVNo = false;
      bool VYfcOrirMP = false;
      bool iUmgidxhBn = false;
      bool liBUOIYxyU = false;
      bool blBJeWjLwk = false;
      bool lRHpbLAzcq = false;
      bool PNTYDlXLOG = false;
      bool SltrnwTdCL = false;
      bool tllQCUaKrC = false;
      bool HYUHFOSbFs = false;
      bool zXxpRzWPCe = false;
      bool rVxQwdbCWi = false;
      bool KMSLytQmlY = false;
      bool dqYJMVnwzC = false;
      bool xLINJkaEKb = false;
      bool lPFVhlFkqW = false;
      bool iiccABxnDa = false;
      bool ofDTkzDFBM = false;
      bool TMcKmOkCaZ = false;
      string eYbdafPGuc;
      string mPhTUllWTk;
      string ITjAraLUti;
      string zXLXjAwkDQ;
      string EbFeRrHMTa;
      string AbIciKnblz;
      string UuMLdOEKjT;
      string pATCgfOEiA;
      string SzAShXOhnY;
      string LTzDwIOsBC;
      string mxbzkAHGuD;
      string wjrkgUtscF;
      string AWWLHVAQFX;
      string RUDgMVWBhh;
      string PIlkbbbNWa;
      string OVQuBQZQqO;
      string zqAWlafLYK;
      string ysLuoqjlie;
      string tlteJwEHeu;
      string wGJShYIOzB;
      if(eYbdafPGuc == mxbzkAHGuD){wPoLcVJUGK = true;}
      else if(mxbzkAHGuD == eYbdafPGuc){HYUHFOSbFs = true;}
      if(mPhTUllWTk == wjrkgUtscF){gmgLUMwVNo = true;}
      else if(wjrkgUtscF == mPhTUllWTk){zXxpRzWPCe = true;}
      if(ITjAraLUti == AWWLHVAQFX){VYfcOrirMP = true;}
      else if(AWWLHVAQFX == ITjAraLUti){rVxQwdbCWi = true;}
      if(zXLXjAwkDQ == RUDgMVWBhh){iUmgidxhBn = true;}
      else if(RUDgMVWBhh == zXLXjAwkDQ){KMSLytQmlY = true;}
      if(EbFeRrHMTa == PIlkbbbNWa){liBUOIYxyU = true;}
      else if(PIlkbbbNWa == EbFeRrHMTa){dqYJMVnwzC = true;}
      if(AbIciKnblz == OVQuBQZQqO){blBJeWjLwk = true;}
      else if(OVQuBQZQqO == AbIciKnblz){xLINJkaEKb = true;}
      if(UuMLdOEKjT == zqAWlafLYK){lRHpbLAzcq = true;}
      else if(zqAWlafLYK == UuMLdOEKjT){lPFVhlFkqW = true;}
      if(pATCgfOEiA == ysLuoqjlie){PNTYDlXLOG = true;}
      if(SzAShXOhnY == tlteJwEHeu){SltrnwTdCL = true;}
      if(LTzDwIOsBC == wGJShYIOzB){tllQCUaKrC = true;}
      while(ysLuoqjlie == pATCgfOEiA){iiccABxnDa = true;}
      while(tlteJwEHeu == tlteJwEHeu){ofDTkzDFBM = true;}
      while(wGJShYIOzB == wGJShYIOzB){TMcKmOkCaZ = true;}
      if(wPoLcVJUGK == true){wPoLcVJUGK = false;}
      if(gmgLUMwVNo == true){gmgLUMwVNo = false;}
      if(VYfcOrirMP == true){VYfcOrirMP = false;}
      if(iUmgidxhBn == true){iUmgidxhBn = false;}
      if(liBUOIYxyU == true){liBUOIYxyU = false;}
      if(blBJeWjLwk == true){blBJeWjLwk = false;}
      if(lRHpbLAzcq == true){lRHpbLAzcq = false;}
      if(PNTYDlXLOG == true){PNTYDlXLOG = false;}
      if(SltrnwTdCL == true){SltrnwTdCL = false;}
      if(tllQCUaKrC == true){tllQCUaKrC = false;}
      if(HYUHFOSbFs == true){HYUHFOSbFs = false;}
      if(zXxpRzWPCe == true){zXxpRzWPCe = false;}
      if(rVxQwdbCWi == true){rVxQwdbCWi = false;}
      if(KMSLytQmlY == true){KMSLytQmlY = false;}
      if(dqYJMVnwzC == true){dqYJMVnwzC = false;}
      if(xLINJkaEKb == true){xLINJkaEKb = false;}
      if(lPFVhlFkqW == true){lPFVhlFkqW = false;}
      if(iiccABxnDa == true){iiccABxnDa = false;}
      if(ofDTkzDFBM == true){ofDTkzDFBM = false;}
      if(TMcKmOkCaZ == true){TMcKmOkCaZ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class EKASPJHLIP
{ 
  void lQHCfxczeo()
  { 
      bool TGTeKOXigu = false;
      bool FSdBSwhJPo = false;
      bool HUiXutOwBU = false;
      bool nBumyZzlYZ = false;
      bool MTNOQkpKYC = false;
      bool CTrLazWwjQ = false;
      bool zagMHEXpCr = false;
      bool xujpcnGTHK = false;
      bool DEWyRTJiMz = false;
      bool DDCIZkmAHM = false;
      bool NAHgFGiBim = false;
      bool rDnYMuEitm = false;
      bool tndDossaQr = false;
      bool AFMtxDwpsK = false;
      bool jHAYHAOcud = false;
      bool gEGTSYVEqL = false;
      bool NIkpllptbJ = false;
      bool yNoiCmmEwo = false;
      bool eQelLwzrbX = false;
      bool gryhxbVJmU = false;
      string yFSelkAIZl;
      string BSkkODjNyY;
      string rPBeYNOwDu;
      string SqCOaiEDUf;
      string zzdsIBIaye;
      string fnTHPiEmFX;
      string WDFeSJPVJe;
      string ygsRZrkHVX;
      string FmyYOdXtyr;
      string eRxxJutzay;
      string klRSfzuBhX;
      string xwQhSjFgPn;
      string lVWQAgiwId;
      string ylgWlSUerP;
      string iVLathAPeY;
      string pNilMPOASz;
      string QzsEZLOtuz;
      string OaoAEQJwWR;
      string gTRjnjDtKW;
      string DULrzjcHAi;
      if(yFSelkAIZl == klRSfzuBhX){TGTeKOXigu = true;}
      else if(klRSfzuBhX == yFSelkAIZl){NAHgFGiBim = true;}
      if(BSkkODjNyY == xwQhSjFgPn){FSdBSwhJPo = true;}
      else if(xwQhSjFgPn == BSkkODjNyY){rDnYMuEitm = true;}
      if(rPBeYNOwDu == lVWQAgiwId){HUiXutOwBU = true;}
      else if(lVWQAgiwId == rPBeYNOwDu){tndDossaQr = true;}
      if(SqCOaiEDUf == ylgWlSUerP){nBumyZzlYZ = true;}
      else if(ylgWlSUerP == SqCOaiEDUf){AFMtxDwpsK = true;}
      if(zzdsIBIaye == iVLathAPeY){MTNOQkpKYC = true;}
      else if(iVLathAPeY == zzdsIBIaye){jHAYHAOcud = true;}
      if(fnTHPiEmFX == pNilMPOASz){CTrLazWwjQ = true;}
      else if(pNilMPOASz == fnTHPiEmFX){gEGTSYVEqL = true;}
      if(WDFeSJPVJe == QzsEZLOtuz){zagMHEXpCr = true;}
      else if(QzsEZLOtuz == WDFeSJPVJe){NIkpllptbJ = true;}
      if(ygsRZrkHVX == OaoAEQJwWR){xujpcnGTHK = true;}
      if(FmyYOdXtyr == gTRjnjDtKW){DEWyRTJiMz = true;}
      if(eRxxJutzay == DULrzjcHAi){DDCIZkmAHM = true;}
      while(OaoAEQJwWR == ygsRZrkHVX){yNoiCmmEwo = true;}
      while(gTRjnjDtKW == gTRjnjDtKW){eQelLwzrbX = true;}
      while(DULrzjcHAi == DULrzjcHAi){gryhxbVJmU = true;}
      if(TGTeKOXigu == true){TGTeKOXigu = false;}
      if(FSdBSwhJPo == true){FSdBSwhJPo = false;}
      if(HUiXutOwBU == true){HUiXutOwBU = false;}
      if(nBumyZzlYZ == true){nBumyZzlYZ = false;}
      if(MTNOQkpKYC == true){MTNOQkpKYC = false;}
      if(CTrLazWwjQ == true){CTrLazWwjQ = false;}
      if(zagMHEXpCr == true){zagMHEXpCr = false;}
      if(xujpcnGTHK == true){xujpcnGTHK = false;}
      if(DEWyRTJiMz == true){DEWyRTJiMz = false;}
      if(DDCIZkmAHM == true){DDCIZkmAHM = false;}
      if(NAHgFGiBim == true){NAHgFGiBim = false;}
      if(rDnYMuEitm == true){rDnYMuEitm = false;}
      if(tndDossaQr == true){tndDossaQr = false;}
      if(AFMtxDwpsK == true){AFMtxDwpsK = false;}
      if(jHAYHAOcud == true){jHAYHAOcud = false;}
      if(gEGTSYVEqL == true){gEGTSYVEqL = false;}
      if(NIkpllptbJ == true){NIkpllptbJ = false;}
      if(yNoiCmmEwo == true){yNoiCmmEwo = false;}
      if(eQelLwzrbX == true){eQelLwzrbX = false;}
      if(gryhxbVJmU == true){gryhxbVJmU = false;}
    } 
}; 
