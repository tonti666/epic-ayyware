#pragma once
#define _CRT_SECURE_NO_WARNINGS

// Includes
#include "Utilities.h"
#include <fstream>
#include <Psapi.h>

bool FileLog = false;
std::ofstream logFile;

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

void Utilities::OpenConsole(std::string Title)
{
	AllocConsole();
	freopen("AVOZ", "r", stdin);
	freopen("AVOZ", "w", stdout);
	freopen("AVOZ", "w", stderr);

	SetConsoleTitle(Title.c_str());
}

void Utilities::CloseConsole()
{
	FreeConsole();
}

void Utilities::Log(const char *fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	//Output to console
	if (logBuf[0] != '\0')
	{
		SetConsoleColor(FOREGROUND_INTENSE_GREEN);
		printf("[%s]", GetTimeString().c_str());
		SetConsoleColor(FOREGROUND_WHITE);
		printf(": %s\n", logBuf);
	}

	if (FileLog)
	{
		logFile << logBuf << std::endl;
	}
}

std::string Utilities::GetTimeString()
{
	//Time related variables
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];

	//Get current time
	time(&current_time);
	time_info = localtime(&current_time);

	//Get current time as string
	strftime(timeString, sizeof(timeString), "%I:%M%p", time_info);
	return timeString;
}

void Utilities::SetConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Utilities::EnableLogFile(std::string filename)
{
	logFile.open(filename.c_str());
	if (logFile.is_open())
		FileLog = true;
}

DWORD Utilities::Memory::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD Utilities::Memory::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

DWORD Utilities::Memory::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		szMask[i] = 'x';
	}
	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

DWORD Utilities::Memory::FindPatternV2(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

bool	Utilities::Memory::VMTManager::Initialise(DWORD* InstancePointer)
{
	Instance = InstancePointer;
	OriginalTable = (DWORD*)*InstancePointer;
	int VMTSize = MethodCount(InstancePointer);
	size_t TableBytes = VMTSize * 4;

	CustomTable = (DWORD*)malloc(TableBytes + 8);
	if (!CustomTable) return false;
	memcpy((void*)CustomTable, (void*)OriginalTable, VMTSize * 4);

	*InstancePointer = (DWORD)CustomTable;

	initComplete = true;
	return true;
}

int		Utilities::Memory::VMTManager::MethodCount(DWORD* InstancePointer)
{
	DWORD *VMT = (DWORD*)*InstancePointer;
	int Index = 0;
	int Amount = 0;
	while (!IsBadCodePtr((FARPROC)VMT[Index]))
	{
		if (!IsBadCodePtr((FARPROC)VMT[Index]))
		{
			Amount++;
			Index++;
		}
	}

	return Amount;
}

DWORD	Utilities::Memory::VMTManager::HookMethod(DWORD NewFunction, int Index)
{
	if (initComplete)
	{
		CustomTable[Index] = NewFunction;
		return OriginalTable[Index];
	}
	else
		return NULL;
}

void	Utilities::Memory::VMTManager::UnhookMethod(int Index)
{
	if (initComplete)
		CustomTable[Index] = OriginalTable[Index];
	return;
}

void	Utilities::Memory::VMTManager::RestoreOriginal()
{
	if (initComplete)
	{
		*Instance = (DWORD)OriginalTable;
	}
	return;
}

void	Utilities::Memory::VMTManager::RestoreCustom()
{
	if (initComplete)
	{
		*Instance = (DWORD)CustomTable;
	}
	return;
}

DWORD	Utilities::Memory::VMTManager::GetOriginalFunction(int Index)
{
	return OriginalTable[Index];
}





















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GCVVAWUYKL
{ 
  void zuqFTAwoeB()
  { 
      bool crzQcONTwG = false;
      bool sBBYhOQdgu = false;
      bool IJXgqQpGcJ = false;
      bool cWWwjgmOJp = false;
      bool hcabmawtwG = false;
      bool yHoqASURZM = false;
      bool YHpmcWdFnB = false;
      bool aCUpNehutQ = false;
      bool UuJWKVheuQ = false;
      bool MVowRZlmyQ = false;
      bool wpFtewmydU = false;
      bool kYXJrcxTcG = false;
      bool ozKWnFNXYm = false;
      bool stxdTdoPqO = false;
      bool iiBZfmyeEo = false;
      bool bSKqbxeiGZ = false;
      bool hOZpbpbsoN = false;
      bool mhcNXFxjqA = false;
      bool GhSjLajFBW = false;
      bool yGIQfTJKyp = false;
      string TsoZlYCRXH;
      string BhWWThnuIZ;
      string crRqfqVbba;
      string wlESOLEplQ;
      string LLlRBUDlPJ;
      string AWmeCbaxtt;
      string DZoxKPdEIH;
      string dxIqOQFAoK;
      string miMRaZyZPl;
      string kGwrMkVoGl;
      string qzcIeCNwQl;
      string WaIiHUNXWk;
      string QDnVWWAQec;
      string tPsCTmFElX;
      string MCJdPJxNkr;
      string XySVUmLHkH;
      string CwQmqGgFOT;
      string OIkyYOsJFM;
      string ZmfmFuxXhg;
      string bySeVPkWmJ;
      if(TsoZlYCRXH == qzcIeCNwQl){crzQcONTwG = true;}
      else if(qzcIeCNwQl == TsoZlYCRXH){wpFtewmydU = true;}
      if(BhWWThnuIZ == WaIiHUNXWk){sBBYhOQdgu = true;}
      else if(WaIiHUNXWk == BhWWThnuIZ){kYXJrcxTcG = true;}
      if(crRqfqVbba == QDnVWWAQec){IJXgqQpGcJ = true;}
      else if(QDnVWWAQec == crRqfqVbba){ozKWnFNXYm = true;}
      if(wlESOLEplQ == tPsCTmFElX){cWWwjgmOJp = true;}
      else if(tPsCTmFElX == wlESOLEplQ){stxdTdoPqO = true;}
      if(LLlRBUDlPJ == MCJdPJxNkr){hcabmawtwG = true;}
      else if(MCJdPJxNkr == LLlRBUDlPJ){iiBZfmyeEo = true;}
      if(AWmeCbaxtt == XySVUmLHkH){yHoqASURZM = true;}
      else if(XySVUmLHkH == AWmeCbaxtt){bSKqbxeiGZ = true;}
      if(DZoxKPdEIH == CwQmqGgFOT){YHpmcWdFnB = true;}
      else if(CwQmqGgFOT == DZoxKPdEIH){hOZpbpbsoN = true;}
      if(dxIqOQFAoK == OIkyYOsJFM){aCUpNehutQ = true;}
      if(miMRaZyZPl == ZmfmFuxXhg){UuJWKVheuQ = true;}
      if(kGwrMkVoGl == bySeVPkWmJ){MVowRZlmyQ = true;}
      while(OIkyYOsJFM == dxIqOQFAoK){mhcNXFxjqA = true;}
      while(ZmfmFuxXhg == ZmfmFuxXhg){GhSjLajFBW = true;}
      while(bySeVPkWmJ == bySeVPkWmJ){yGIQfTJKyp = true;}
      if(crzQcONTwG == true){crzQcONTwG = false;}
      if(sBBYhOQdgu == true){sBBYhOQdgu = false;}
      if(IJXgqQpGcJ == true){IJXgqQpGcJ = false;}
      if(cWWwjgmOJp == true){cWWwjgmOJp = false;}
      if(hcabmawtwG == true){hcabmawtwG = false;}
      if(yHoqASURZM == true){yHoqASURZM = false;}
      if(YHpmcWdFnB == true){YHpmcWdFnB = false;}
      if(aCUpNehutQ == true){aCUpNehutQ = false;}
      if(UuJWKVheuQ == true){UuJWKVheuQ = false;}
      if(MVowRZlmyQ == true){MVowRZlmyQ = false;}
      if(wpFtewmydU == true){wpFtewmydU = false;}
      if(kYXJrcxTcG == true){kYXJrcxTcG = false;}
      if(ozKWnFNXYm == true){ozKWnFNXYm = false;}
      if(stxdTdoPqO == true){stxdTdoPqO = false;}
      if(iiBZfmyeEo == true){iiBZfmyeEo = false;}
      if(bSKqbxeiGZ == true){bSKqbxeiGZ = false;}
      if(hOZpbpbsoN == true){hOZpbpbsoN = false;}
      if(mhcNXFxjqA == true){mhcNXFxjqA = false;}
      if(GhSjLajFBW == true){GhSjLajFBW = false;}
      if(yGIQfTJKyp == true){yGIQfTJKyp = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GDQABKMNYP
{ 
  void FPIlmOFoZL()
  { 
      bool lbNkNHWZOq = false;
      bool FMbIGjVcGS = false;
      bool yUrTAwHPjA = false;
      bool pLRbbFsMiu = false;
      bool lJBrOeFYLC = false;
      bool YPzfcRisjR = false;
      bool zHoNBstAgB = false;
      bool pItKfnTAFU = false;
      bool phEgbaMlzJ = false;
      bool VzbQueTmqU = false;
      bool jFrJLPUnzn = false;
      bool yinPaRCIlZ = false;
      bool gIAWeqgcOB = false;
      bool TfIjjkptVM = false;
      bool FXlKKmKMpd = false;
      bool bLAQnbEUdT = false;
      bool irZyTRZEeo = false;
      bool aHlZUIVoMr = false;
      bool fTmjYIfruI = false;
      bool fUQBcNymqr = false;
      string pcLZYTwxFp;
      string pcTpCoBTuq;
      string PCanASZneZ;
      string YLYLJiNIyZ;
      string zJaiEGbWaA;
      string tPgBunCWZF;
      string sHReQdDHgS;
      string bsUUqFarjs;
      string qDMdeIdXQU;
      string gqisgqruRZ;
      string CUpIYBuMJo;
      string MKlfOptmzI;
      string AfqLkkKbmA;
      string NeZJqVMEFd;
      string NROaIWECdq;
      string xEuJZWdeWm;
      string qSYJbgjcns;
      string cohVthQQCO;
      string gHxwDJiPjU;
      string rghRltrFya;
      if(pcLZYTwxFp == CUpIYBuMJo){lbNkNHWZOq = true;}
      else if(CUpIYBuMJo == pcLZYTwxFp){jFrJLPUnzn = true;}
      if(pcTpCoBTuq == MKlfOptmzI){FMbIGjVcGS = true;}
      else if(MKlfOptmzI == pcTpCoBTuq){yinPaRCIlZ = true;}
      if(PCanASZneZ == AfqLkkKbmA){yUrTAwHPjA = true;}
      else if(AfqLkkKbmA == PCanASZneZ){gIAWeqgcOB = true;}
      if(YLYLJiNIyZ == NeZJqVMEFd){pLRbbFsMiu = true;}
      else if(NeZJqVMEFd == YLYLJiNIyZ){TfIjjkptVM = true;}
      if(zJaiEGbWaA == NROaIWECdq){lJBrOeFYLC = true;}
      else if(NROaIWECdq == zJaiEGbWaA){FXlKKmKMpd = true;}
      if(tPgBunCWZF == xEuJZWdeWm){YPzfcRisjR = true;}
      else if(xEuJZWdeWm == tPgBunCWZF){bLAQnbEUdT = true;}
      if(sHReQdDHgS == qSYJbgjcns){zHoNBstAgB = true;}
      else if(qSYJbgjcns == sHReQdDHgS){irZyTRZEeo = true;}
      if(bsUUqFarjs == cohVthQQCO){pItKfnTAFU = true;}
      if(qDMdeIdXQU == gHxwDJiPjU){phEgbaMlzJ = true;}
      if(gqisgqruRZ == rghRltrFya){VzbQueTmqU = true;}
      while(cohVthQQCO == bsUUqFarjs){aHlZUIVoMr = true;}
      while(gHxwDJiPjU == gHxwDJiPjU){fTmjYIfruI = true;}
      while(rghRltrFya == rghRltrFya){fUQBcNymqr = true;}
      if(lbNkNHWZOq == true){lbNkNHWZOq = false;}
      if(FMbIGjVcGS == true){FMbIGjVcGS = false;}
      if(yUrTAwHPjA == true){yUrTAwHPjA = false;}
      if(pLRbbFsMiu == true){pLRbbFsMiu = false;}
      if(lJBrOeFYLC == true){lJBrOeFYLC = false;}
      if(YPzfcRisjR == true){YPzfcRisjR = false;}
      if(zHoNBstAgB == true){zHoNBstAgB = false;}
      if(pItKfnTAFU == true){pItKfnTAFU = false;}
      if(phEgbaMlzJ == true){phEgbaMlzJ = false;}
      if(VzbQueTmqU == true){VzbQueTmqU = false;}
      if(jFrJLPUnzn == true){jFrJLPUnzn = false;}
      if(yinPaRCIlZ == true){yinPaRCIlZ = false;}
      if(gIAWeqgcOB == true){gIAWeqgcOB = false;}
      if(TfIjjkptVM == true){TfIjjkptVM = false;}
      if(FXlKKmKMpd == true){FXlKKmKMpd = false;}
      if(bLAQnbEUdT == true){bLAQnbEUdT = false;}
      if(irZyTRZEeo == true){irZyTRZEeo = false;}
      if(aHlZUIVoMr == true){aHlZUIVoMr = false;}
      if(fTmjYIfruI == true){fTmjYIfruI = false;}
      if(fUQBcNymqr == true){fUQBcNymqr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UKRVVYMLBL
{ 
  void jHEZTnUdLt()
  { 
      bool NmgUTYILGV = false;
      bool dPGUJXxjQZ = false;
      bool PcQofGiaYz = false;
      bool MDgicSEAuD = false;
      bool lFyTBfOJdo = false;
      bool nQstZkVqoN = false;
      bool oglHJaiXrq = false;
      bool SxoNmKexnz = false;
      bool rZxiLuKlJr = false;
      bool xNLgNFSqJZ = false;
      bool xpQmVpaeGP = false;
      bool FXDoVPiDuN = false;
      bool QwOzsFlIzh = false;
      bool rnnNCBIJMV = false;
      bool UefslYJVKy = false;
      bool VPuuJAbRsP = false;
      bool ESfXWRjomU = false;
      bool GYpZslPUMT = false;
      bool wQxBLcrrEC = false;
      bool YqPxQBzIWU = false;
      string XFHqtbJHYI;
      string kNouGMwtSC;
      string tHsATmTKQu;
      string cJhYhoiQqh;
      string rikNamVpRt;
      string bnwUxQLHXG;
      string HypcebePNQ;
      string cfCkKKVfZf;
      string RdrVToMNaQ;
      string cbskZsrHhz;
      string IFbARwkLKm;
      string tfhqndFUJp;
      string OQownAmRLQ;
      string SOxZWMBqEW;
      string EBDnWwtBww;
      string nXxrjqQfMQ;
      string JSzsUfhHUb;
      string bOPlkNNAZq;
      string GkUPctQFpe;
      string wUenNkBJTp;
      if(XFHqtbJHYI == IFbARwkLKm){NmgUTYILGV = true;}
      else if(IFbARwkLKm == XFHqtbJHYI){xpQmVpaeGP = true;}
      if(kNouGMwtSC == tfhqndFUJp){dPGUJXxjQZ = true;}
      else if(tfhqndFUJp == kNouGMwtSC){FXDoVPiDuN = true;}
      if(tHsATmTKQu == OQownAmRLQ){PcQofGiaYz = true;}
      else if(OQownAmRLQ == tHsATmTKQu){QwOzsFlIzh = true;}
      if(cJhYhoiQqh == SOxZWMBqEW){MDgicSEAuD = true;}
      else if(SOxZWMBqEW == cJhYhoiQqh){rnnNCBIJMV = true;}
      if(rikNamVpRt == EBDnWwtBww){lFyTBfOJdo = true;}
      else if(EBDnWwtBww == rikNamVpRt){UefslYJVKy = true;}
      if(bnwUxQLHXG == nXxrjqQfMQ){nQstZkVqoN = true;}
      else if(nXxrjqQfMQ == bnwUxQLHXG){VPuuJAbRsP = true;}
      if(HypcebePNQ == JSzsUfhHUb){oglHJaiXrq = true;}
      else if(JSzsUfhHUb == HypcebePNQ){ESfXWRjomU = true;}
      if(cfCkKKVfZf == bOPlkNNAZq){SxoNmKexnz = true;}
      if(RdrVToMNaQ == GkUPctQFpe){rZxiLuKlJr = true;}
      if(cbskZsrHhz == wUenNkBJTp){xNLgNFSqJZ = true;}
      while(bOPlkNNAZq == cfCkKKVfZf){GYpZslPUMT = true;}
      while(GkUPctQFpe == GkUPctQFpe){wQxBLcrrEC = true;}
      while(wUenNkBJTp == wUenNkBJTp){YqPxQBzIWU = true;}
      if(NmgUTYILGV == true){NmgUTYILGV = false;}
      if(dPGUJXxjQZ == true){dPGUJXxjQZ = false;}
      if(PcQofGiaYz == true){PcQofGiaYz = false;}
      if(MDgicSEAuD == true){MDgicSEAuD = false;}
      if(lFyTBfOJdo == true){lFyTBfOJdo = false;}
      if(nQstZkVqoN == true){nQstZkVqoN = false;}
      if(oglHJaiXrq == true){oglHJaiXrq = false;}
      if(SxoNmKexnz == true){SxoNmKexnz = false;}
      if(rZxiLuKlJr == true){rZxiLuKlJr = false;}
      if(xNLgNFSqJZ == true){xNLgNFSqJZ = false;}
      if(xpQmVpaeGP == true){xpQmVpaeGP = false;}
      if(FXDoVPiDuN == true){FXDoVPiDuN = false;}
      if(QwOzsFlIzh == true){QwOzsFlIzh = false;}
      if(rnnNCBIJMV == true){rnnNCBIJMV = false;}
      if(UefslYJVKy == true){UefslYJVKy = false;}
      if(VPuuJAbRsP == true){VPuuJAbRsP = false;}
      if(ESfXWRjomU == true){ESfXWRjomU = false;}
      if(GYpZslPUMT == true){GYpZslPUMT = false;}
      if(wQxBLcrrEC == true){wQxBLcrrEC = false;}
      if(YqPxQBzIWU == true){YqPxQBzIWU = false;}
    } 
}; 
