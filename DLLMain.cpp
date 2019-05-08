

#include "DLLMain.h"
#include "Utilities.h"
#include "INJ/ReflectiveLoader.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"

#include "hitmarker.h"
#include "AntiLeak.h"
#include <d3d9.h>
#include <strstream>
#include <fstream>
#include "Skinchanger.h"
#include <iostream>
#include <ostream>
#include "CBulletListenerr.h"
#include "Dumping.h"

int missedLogHits[65];
template <int XORSTART, int BUFLEN, int XREFKILLER>

class Xor
{
private:
	Xor();
public:
	char s[BUFLEN];

	Xor(const char* xs);
	~Xor()
	{
		for (int i = 0; i < BUFLEN; i++) s[i] = 0;
	}
};

template <int XORSTART, int BUFLEN, int XREFKILLER>

Xor<XORSTART, BUFLEN, XREFKILLER>::Xor(const char* xs)
{
	int xvalue = XORSTART;
	int i = 0;
	for (; i < (BUFLEN - 1); i++)
	{
		s[i] = xs[i - XREFKILLER] ^ xvalue;
		xvalue += 1;
		xvalue %= 256;
	}
	s[BUFLEN - 1] = (2 * 2 - 3) - 1;
}

extern HINSTANCE hAppInstance;

UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

bool ErasePEHeader(HMODULE hModule)
{
	hModule = GetModuleHandle(NULL);
	if ((DWORD)hModule == 0) return 0;
	DWORD IMAGE_NT_HEADER = *(int*)((DWORD)hModule + 60);
	for (int i = 0; i<0x108; i++)
		*(BYTE*)(IMAGE_NT_HEADER + i) = 0;
	for (int i = 0; i<120; i++)
		*(BYTE*)((DWORD)hModule + i) = 0;
	return 1;
}

HINSTANCE HThisModule;
bool DoUnload;


bool is_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void create_folder(const char * path) {
	if (!CreateDirectory(path, NULL)) {
		return;
	}
}

void initialize_configs_file() {

	create_folder("spuri");
	create_folder("spuri\\cfg");

	if (!is_file_exist("spuri\\cfg\\spuri_configs.txt")) {
		std::ofstream("spuri\\cfg\\spuri_configs.txt");
	}
}

void CAntiLeak::ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0; 
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}



int InitialThread()
{
	PrintMetaHeader();
	initialize_configs_file();
	Offsets::Initialise();
	interfaces::Initialise(); 
	skinchanger.set_skins();
	NetVar.RetrieveClasses();
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	hitmarker::singleton()->initialize();
	Hacks::SetupHacks();
	options::SetupMenu();
	Hooks::Initialise();
	CBulletListener::singleton()->init();
	FLASHWINFO flash;
	flash.cbSize = sizeof(FLASHWINFO);
	flash.hwnd = interfaces::window;
	flash.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	flash.uCount = 0;
	flash.dwTimeout = 0;
	FlashWindowEx(&flash);
	while (DoUnload == false)
	{
		Sleep(1000);
	}
	Hooks::UndoHooks();
	Sleep(2000); 
	FreeLibraryAndExitThread(HThisModule, 0);
	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);
		Sleep(100);
		{
			{
				DisableThreadLibraryCalls(hModule);

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);

				return TRUE;
			}
		}
		return FALSE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}




#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BJIDBGRONT
{ 
  void LLOiRUCjtm()
  { 
      bool KrZyPygrDZ = false;
      bool ZMABlLenOZ = false;
      bool FYEQUpiHpq = false;
      bool dingYAROYq = false;
      bool aolKQlwfze = false;
      bool DYhEKknIdY = false;
      bool OYBLwpYSck = false;
      bool sPOWzPmsAi = false;
      bool tCHLnEBiWm = false;
      bool MJkAbBmZOZ = false;
      bool NcDNkmKKFt = false;
      bool ytIteMRoXq = false;
      bool oowoYjdOux = false;
      bool cMeaVQytZA = false;
      bool tqdboSBpUP = false;
      bool jyBXjXfOoB = false;
      bool IuPHRlPgYO = false;
      bool NWPXwyLUAG = false;
      bool inseNdKGpx = false;
      bool jjEYqzbVJC = false;
      string GwSpnLgUDV;
      string wLUpBKaAnI;
      string tGUfGDkehF;
      string WHWNuiKSCL;
      string DYyajZPrOI;
      string jWxOkAblik;
      string aClxzonGtR;
      string AHxcFUHtwg;
      string mrQZVzJIbu;
      string HXiQkAlBbu;
      string kJxwHbYUIQ;
      string rgpCyhkLbz;
      string efdzKbYgrE;
      string iGRAuduTRt;
      string ZoALCZhgSC;
      string PaZbPFeylf;
      string DrcDglPoGT;
      string mRsgYpeRxj;
      string sqNooHgpkp;
      string ywhlphMwdP;
      if(GwSpnLgUDV == kJxwHbYUIQ){KrZyPygrDZ = true;}
      else if(kJxwHbYUIQ == GwSpnLgUDV){NcDNkmKKFt = true;}
      if(wLUpBKaAnI == rgpCyhkLbz){ZMABlLenOZ = true;}
      else if(rgpCyhkLbz == wLUpBKaAnI){ytIteMRoXq = true;}
      if(tGUfGDkehF == efdzKbYgrE){FYEQUpiHpq = true;}
      else if(efdzKbYgrE == tGUfGDkehF){oowoYjdOux = true;}
      if(WHWNuiKSCL == iGRAuduTRt){dingYAROYq = true;}
      else if(iGRAuduTRt == WHWNuiKSCL){cMeaVQytZA = true;}
      if(DYyajZPrOI == ZoALCZhgSC){aolKQlwfze = true;}
      else if(ZoALCZhgSC == DYyajZPrOI){tqdboSBpUP = true;}
      if(jWxOkAblik == PaZbPFeylf){DYhEKknIdY = true;}
      else if(PaZbPFeylf == jWxOkAblik){jyBXjXfOoB = true;}
      if(aClxzonGtR == DrcDglPoGT){OYBLwpYSck = true;}
      else if(DrcDglPoGT == aClxzonGtR){IuPHRlPgYO = true;}
      if(AHxcFUHtwg == mRsgYpeRxj){sPOWzPmsAi = true;}
      if(mrQZVzJIbu == sqNooHgpkp){tCHLnEBiWm = true;}
      if(HXiQkAlBbu == ywhlphMwdP){MJkAbBmZOZ = true;}
      while(mRsgYpeRxj == AHxcFUHtwg){NWPXwyLUAG = true;}
      while(sqNooHgpkp == sqNooHgpkp){inseNdKGpx = true;}
      while(ywhlphMwdP == ywhlphMwdP){jjEYqzbVJC = true;}
      if(KrZyPygrDZ == true){KrZyPygrDZ = false;}
      if(ZMABlLenOZ == true){ZMABlLenOZ = false;}
      if(FYEQUpiHpq == true){FYEQUpiHpq = false;}
      if(dingYAROYq == true){dingYAROYq = false;}
      if(aolKQlwfze == true){aolKQlwfze = false;}
      if(DYhEKknIdY == true){DYhEKknIdY = false;}
      if(OYBLwpYSck == true){OYBLwpYSck = false;}
      if(sPOWzPmsAi == true){sPOWzPmsAi = false;}
      if(tCHLnEBiWm == true){tCHLnEBiWm = false;}
      if(MJkAbBmZOZ == true){MJkAbBmZOZ = false;}
      if(NcDNkmKKFt == true){NcDNkmKKFt = false;}
      if(ytIteMRoXq == true){ytIteMRoXq = false;}
      if(oowoYjdOux == true){oowoYjdOux = false;}
      if(cMeaVQytZA == true){cMeaVQytZA = false;}
      if(tqdboSBpUP == true){tqdboSBpUP = false;}
      if(jyBXjXfOoB == true){jyBXjXfOoB = false;}
      if(IuPHRlPgYO == true){IuPHRlPgYO = false;}
      if(NWPXwyLUAG == true){NWPXwyLUAG = false;}
      if(inseNdKGpx == true){inseNdKGpx = false;}
      if(jjEYqzbVJC == true){jjEYqzbVJC = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XWWGBXTSEJ
{ 
  void yOtAGJZzDr()
  { 
      bool llWYdVZzVY = false;
      bool nDRUDszBTm = false;
      bool bUjHnuJjAD = false;
      bool oRJSaXtpjL = false;
      bool pNcOmMbYOH = false;
      bool bjEOtRCOki = false;
      bool MpyRiVszsf = false;
      bool zIQIwNqUUa = false;
      bool TkUrbFNuXI = false;
      bool UfFBzAOKeR = false;
      bool SbeiITSuRg = false;
      bool ZgtJPaVFpd = false;
      bool HybjKTCPJw = false;
      bool ygpCIqFkjI = false;
      bool lRcqimkulX = false;
      bool eITBJTAuXn = false;
      bool QDlUhsqqBX = false;
      bool VjVhrpVkBX = false;
      bool hHdlRoNosn = false;
      bool qKNuqhfHIo = false;
      string iwtwDjjmOX;
      string LCTNJnMLyU;
      string NDHSCJkbnD;
      string HkBBuOxqGI;
      string LJZlCyyaxY;
      string GLmRAfobtt;
      string zIyLQZBRRt;
      string pVEpJEcVPD;
      string KJdPxwyiBP;
      string ZVxLICLOVb;
      string xfICijGxIw;
      string DTLpiodiOT;
      string bRQBTUYrgG;
      string rNAnGeAbgM;
      string oAaFMGKGcS;
      string YbABAeOHhN;
      string LOPCDoTXjD;
      string oCCBeaqLoN;
      string KRPOhQNymb;
      string PssSBakYBW;
      if(iwtwDjjmOX == xfICijGxIw){llWYdVZzVY = true;}
      else if(xfICijGxIw == iwtwDjjmOX){SbeiITSuRg = true;}
      if(LCTNJnMLyU == DTLpiodiOT){nDRUDszBTm = true;}
      else if(DTLpiodiOT == LCTNJnMLyU){ZgtJPaVFpd = true;}
      if(NDHSCJkbnD == bRQBTUYrgG){bUjHnuJjAD = true;}
      else if(bRQBTUYrgG == NDHSCJkbnD){HybjKTCPJw = true;}
      if(HkBBuOxqGI == rNAnGeAbgM){oRJSaXtpjL = true;}
      else if(rNAnGeAbgM == HkBBuOxqGI){ygpCIqFkjI = true;}
      if(LJZlCyyaxY == oAaFMGKGcS){pNcOmMbYOH = true;}
      else if(oAaFMGKGcS == LJZlCyyaxY){lRcqimkulX = true;}
      if(GLmRAfobtt == YbABAeOHhN){bjEOtRCOki = true;}
      else if(YbABAeOHhN == GLmRAfobtt){eITBJTAuXn = true;}
      if(zIyLQZBRRt == LOPCDoTXjD){MpyRiVszsf = true;}
      else if(LOPCDoTXjD == zIyLQZBRRt){QDlUhsqqBX = true;}
      if(pVEpJEcVPD == oCCBeaqLoN){zIQIwNqUUa = true;}
      if(KJdPxwyiBP == KRPOhQNymb){TkUrbFNuXI = true;}
      if(ZVxLICLOVb == PssSBakYBW){UfFBzAOKeR = true;}
      while(oCCBeaqLoN == pVEpJEcVPD){VjVhrpVkBX = true;}
      while(KRPOhQNymb == KRPOhQNymb){hHdlRoNosn = true;}
      while(PssSBakYBW == PssSBakYBW){qKNuqhfHIo = true;}
      if(llWYdVZzVY == true){llWYdVZzVY = false;}
      if(nDRUDszBTm == true){nDRUDszBTm = false;}
      if(bUjHnuJjAD == true){bUjHnuJjAD = false;}
      if(oRJSaXtpjL == true){oRJSaXtpjL = false;}
      if(pNcOmMbYOH == true){pNcOmMbYOH = false;}
      if(bjEOtRCOki == true){bjEOtRCOki = false;}
      if(MpyRiVszsf == true){MpyRiVszsf = false;}
      if(zIQIwNqUUa == true){zIQIwNqUUa = false;}
      if(TkUrbFNuXI == true){TkUrbFNuXI = false;}
      if(UfFBzAOKeR == true){UfFBzAOKeR = false;}
      if(SbeiITSuRg == true){SbeiITSuRg = false;}
      if(ZgtJPaVFpd == true){ZgtJPaVFpd = false;}
      if(HybjKTCPJw == true){HybjKTCPJw = false;}
      if(ygpCIqFkjI == true){ygpCIqFkjI = false;}
      if(lRcqimkulX == true){lRcqimkulX = false;}
      if(eITBJTAuXn == true){eITBJTAuXn = false;}
      if(QDlUhsqqBX == true){QDlUhsqqBX = false;}
      if(VjVhrpVkBX == true){VjVhrpVkBX = false;}
      if(hHdlRoNosn == true){hHdlRoNosn = false;}
      if(qKNuqhfHIo == true){qKNuqhfHIo = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AVNNZBLNUP
{ 
  void NffGkPuFRa()
  { 
      bool zxLwsngLFx = false;
      bool AYBXtcNChe = false;
      bool qRonCIVzFr = false;
      bool VHsYVcciew = false;
      bool xTQysqACQy = false;
      bool PDYMIhIXsk = false;
      bool SgGMbrfbBQ = false;
      bool zeWkgXAptL = false;
      bool IThHcSifQn = false;
      bool TFotdQMDWA = false;
      bool LXAzykgmfj = false;
      bool gSdgbyyzjJ = false;
      bool JzLNxtobGP = false;
      bool LxSsAjSiIq = false;
      bool kwAfGAbISe = false;
      bool SMTNqMLmCE = false;
      bool wFfPYjAjgq = false;
      bool fPqrXNyfGw = false;
      bool SfJaxycEBn = false;
      bool YffEufPkGW = false;
      string UksUAdozny;
      string zXjZCGJcwG;
      string sgjfQjpDtR;
      string yGWRHXEoYd;
      string lnoTaENpgh;
      string WpWIknAMDP;
      string mlEVAsBRRe;
      string CUDSUFrQEM;
      string VAVMuIjNfT;
      string WqkJpziSSh;
      string DuXCfBlDne;
      string wjIqslPqZr;
      string InlxIKitcU;
      string VYxWJZbTgs;
      string VgZAXlitPD;
      string jqraZOJasU;
      string SraSJXmkhT;
      string KuKgVbOkZV;
      string aoeYkdfVwt;
      string QBZLacFGqA;
      if(UksUAdozny == DuXCfBlDne){zxLwsngLFx = true;}
      else if(DuXCfBlDne == UksUAdozny){LXAzykgmfj = true;}
      if(zXjZCGJcwG == wjIqslPqZr){AYBXtcNChe = true;}
      else if(wjIqslPqZr == zXjZCGJcwG){gSdgbyyzjJ = true;}
      if(sgjfQjpDtR == InlxIKitcU){qRonCIVzFr = true;}
      else if(InlxIKitcU == sgjfQjpDtR){JzLNxtobGP = true;}
      if(yGWRHXEoYd == VYxWJZbTgs){VHsYVcciew = true;}
      else if(VYxWJZbTgs == yGWRHXEoYd){LxSsAjSiIq = true;}
      if(lnoTaENpgh == VgZAXlitPD){xTQysqACQy = true;}
      else if(VgZAXlitPD == lnoTaENpgh){kwAfGAbISe = true;}
      if(WpWIknAMDP == jqraZOJasU){PDYMIhIXsk = true;}
      else if(jqraZOJasU == WpWIknAMDP){SMTNqMLmCE = true;}
      if(mlEVAsBRRe == SraSJXmkhT){SgGMbrfbBQ = true;}
      else if(SraSJXmkhT == mlEVAsBRRe){wFfPYjAjgq = true;}
      if(CUDSUFrQEM == KuKgVbOkZV){zeWkgXAptL = true;}
      if(VAVMuIjNfT == aoeYkdfVwt){IThHcSifQn = true;}
      if(WqkJpziSSh == QBZLacFGqA){TFotdQMDWA = true;}
      while(KuKgVbOkZV == CUDSUFrQEM){fPqrXNyfGw = true;}
      while(aoeYkdfVwt == aoeYkdfVwt){SfJaxycEBn = true;}
      while(QBZLacFGqA == QBZLacFGqA){YffEufPkGW = true;}
      if(zxLwsngLFx == true){zxLwsngLFx = false;}
      if(AYBXtcNChe == true){AYBXtcNChe = false;}
      if(qRonCIVzFr == true){qRonCIVzFr = false;}
      if(VHsYVcciew == true){VHsYVcciew = false;}
      if(xTQysqACQy == true){xTQysqACQy = false;}
      if(PDYMIhIXsk == true){PDYMIhIXsk = false;}
      if(SgGMbrfbBQ == true){SgGMbrfbBQ = false;}
      if(zeWkgXAptL == true){zeWkgXAptL = false;}
      if(IThHcSifQn == true){IThHcSifQn = false;}
      if(TFotdQMDWA == true){TFotdQMDWA = false;}
      if(LXAzykgmfj == true){LXAzykgmfj = false;}
      if(gSdgbyyzjJ == true){gSdgbyyzjJ = false;}
      if(JzLNxtobGP == true){JzLNxtobGP = false;}
      if(LxSsAjSiIq == true){LxSsAjSiIq = false;}
      if(kwAfGAbISe == true){kwAfGAbISe = false;}
      if(SMTNqMLmCE == true){SMTNqMLmCE = false;}
      if(wFfPYjAjgq == true){wFfPYjAjgq = false;}
      if(fPqrXNyfGw == true){fPqrXNyfGw = false;}
      if(SfJaxycEBn == true){SfJaxycEBn = false;}
      if(YffEufPkGW == true){YffEufPkGW = false;}
    } 
}; 

