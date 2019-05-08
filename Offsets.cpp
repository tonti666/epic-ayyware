#include "Offsets.h"
#include "UTIL Functions.h"

#define strenc( s ) ( s )

void Offsets::Initialise()
{
	// Modules
	Modules::Client = Utilities::Memory::WaitOnModuleHandle("client_panorama.dll");
	Modules::Engine = Utilities::Memory::WaitOnModuleHandle("engine.dll");
	Modules::VGUI2 = Utilities::Memory::WaitOnModuleHandle("vgui2.dll");
	Modules::VGUISurface = Utilities::Memory::WaitOnModuleHandle("vguimatsurface.dll");
	Modules::Material = Utilities::Memory::WaitOnModuleHandle("materialsystem.dll");
	Modules::VPhysics = Utilities::Memory::WaitOnModuleHandle("vphysics.dll");
	Modules::Stdlib = Utilities::Memory::WaitOnModuleHandle("vstdlib.dll");
	Modules::InputSys = Utilities::Memory::WaitOnModuleHandle("inputsystem.dll");

#pragma region VTables
	VMT::CHL_GetAllClasses = 8;

	VMT::Engine_GetScreenSize = 5;
	VMT::Engine_GetPlayerInfo = 8;
	VMT::Engine_GetLocalPlayer = 12;
	VMT::Engine_Time = 14;
	VMT::Engine_GetViewAngles = 18;
	VMT::Engine_SetViewAngles = 19;
	VMT::Engine_GetMaxClients = 20;
	VMT::Engine_IsConnected = 27;
	VMT::Engine_IsInGame = 26;
	VMT::Engine_WorldToScreenMatrix = 37;
	VMT::Engine_ClientCmd_Unrestricted = 114;

	VMT::Panel_GetName = 36;
	VMT::Panel_PaintTraverse = 41;

	VMT::Surface_DrawSetColorA = 14;
	VMT::Surface_DrawSetColorB = 15;
	VMT::Surface_DrawFilledRect = 16;
	VMT::Surface_DrawOutlinedRect = 18;
	VMT::Surface_DrawLine = 19;
	VMT::Surface_DrawSetTextFont = 23;
	VMT::Surface_DrawSetTextColorA = 24;
	VMT::Surface_DrawSetTextColorB = 25;
	VMT::Surface_DrawSetTextPos = 26;
	VMT::Surface_DrawPrintText = 28;
	VMT::Surface_DrawSetTextureRGBA = 37;
	VMT::Surface_DrawSetTexture = 38;
	VMT::Surface_CreateNewTextureID  = 43;
	VMT::Surface_FontCreate = 71;
	VMT::Surface_SetFontGlyphSet = 72;
	VMT::Surface_GetTextSize = 79;
	VMT::Surface_DrawOutlinedCircle = 103;
	VMT::Surface_SurfaceGetCursorPos = 66;
	VMT::Surface_DrawTexturedPolygon = 106;

	VMT::Material_GetName = 0;
	VMT::Material_SetMaterialVarFlag = 30;
	VMT::Material_GetMaterialVarFlag = 31;
	VMT::Material_AlphaModulate = 28;
	VMT::Material_ColorModulate = 29;
	VMT::Material_IncrementReferenceCount = 14;
	
	VMT::MaterialSystem_FindMaterial = 84;
	VMT::MaterialSystem_CreateMaterial = 83;
	
	VMT::ModelRender_ForcedMaterialOverride = 1;
	VMT::ModelRender_DrawModelExecute = 21;
	
	VMT::ModelInfo_GetModelName = 3;
	VMT::ModelInfo_GetStudiomodel = 30;

	VMT::RenderView_SetBlend = 4;
	VMT::RenderView_SetColorModulation = 6;

#pragma endregion Contains the VTable Indexs


	Functions::KeyValues_KeyValues = Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 51 33 C0 C7 45");

	Functions::KeyValues_LoadFromBuffer = Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");

	Functions::dwCalcPlayerView =  Utilities::Memory::FindPattern("client_panorama.dll", (PBYTE)"\x84\xC0\x75\x08\x57\x8B\xCE\xE8\x00\x00\x00\x00\x8B\x06", "xxxxxxxx????xx");

	Functions::GetSeqActivity = GameUtils::FindPattern1(strenc("client_panorama.dll"), strenc("55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));
	Functions::dwGetPlayerCompRank = GameUtils::FindPattern1(strenc("client_panorama.dll"), strenc("55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D"));

	Functions::IsReadySig = GameUtils::FindPattern1(strenc("client_panorama.dll"), strenc("55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B 8E"));

	Functions::dwIsReady = GameUtils::FindPattern1(strenc("client_panorama.dll"), strenc("55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B BE"));

	Utilities::Log("Offsets/Indexes Up to Date");
}

namespace Offsets
{
	namespace Modules
	{
		DWORD Client;
		DWORD Engine;
		DWORD VGUI2;
		DWORD VGUISurface;
		DWORD Material;
		DWORD VPhysics;
		DWORD Stdlib;
		DWORD InputSys;
	};

	namespace VMT
	{
		DWORD CHL_GetAllClasses;

		DWORD Engine_GetScreenSize;
		DWORD Engine_GetPlayerInfo;
		DWORD Engine_GetLocalPlayer;
		DWORD Engine_Time;
		DWORD Engine_GetViewAngles;
		DWORD Engine_SetViewAngles;
		DWORD Engine_GetMaxClients;
		DWORD Engine_IsConnected;
		DWORD Engine_IsInGame;
		DWORD Engine_WorldToScreenMatrix;
		DWORD Engine_GetNetChannelInfo;
		DWORD Engine_ClientCmd_Unrestricted;

		DWORD Panel_GetName;
		DWORD Panel_PaintTraverse;

		DWORD Surface_DrawSetColorA;
		DWORD Surface_DrawSetColorB;
		DWORD Surface_DrawFilledRect;
		DWORD Surface_DrawOutlinedRect;
		DWORD Surface_DrawLine;
		DWORD Surface_DrawSetTextFont;
		DWORD Surface_DrawSetTextColorA;
		DWORD Surface_DrawSetTextColorB;
		DWORD Surface_DrawSetTextPos;
		DWORD Surface_DrawPrintText;
		DWORD Surface_DrawSetTextureRGBA;
		DWORD Surface_DrawSetTexture;
		DWORD Surface_CreateNewTextureID;
		DWORD Surface_FontCreate;
		DWORD Surface_SetFontGlyphSet;
		DWORD Surface_GetTextSize;
		DWORD Surface_DrawOutlinedCircle;
		DWORD Surface_SurfaceGetCursorPos;
		DWORD Surface_DrawTexturedPolygon;

		DWORD Material_GetName;
		DWORD Material_SetMaterialVarFlag;
		DWORD Material_GetMaterialVarFlag;
		DWORD Material_AlphaModulate;
		DWORD Material_ColorModulate;
		DWORD Material_IncrementReferenceCount;

		DWORD MaterialSystem_FindMaterial;
		DWORD MaterialSystem_CreateMaterial;

		DWORD ModelRender_ForcedMaterialOverride;
		DWORD ModelRender_DrawModelExecute;

		DWORD ModelInfo_GetModelName;
		DWORD ModelInfo_GetStudiomodel;

		DWORD RenderView_SetBlend;
		DWORD RenderView_SetColorModulation;

		DWORD Weapon_GetSpread;
	};

	namespace Functions
	{
		DWORD KeyValues_KeyValues;
		DWORD KeyValues_LoadFromBuffer;
		DWORD dwCalcPlayerView;
		DWORD dwGetPlayerCompRank;
		DWORD dwIsReady;
		DWORD GetWeaponData;
		DWORD IsReadySig;
		DWORD GetSeqActivity;
	};
};




















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FMQWFSQDHA
{ 
  void pXCBeOAgeF()
  { 
      bool YbOiatFQYL = false;
      bool zskEBsUMTR = false;
      bool LnDfFuEVbi = false;
      bool NHsuflJHFp = false;
      bool fiScRBAyEZ = false;
      bool ndyTrjYhce = false;
      bool pjopyqApXO = false;
      bool jNHclcUZND = false;
      bool uFQCujJYDu = false;
      bool NadZAwtcea = false;
      bool BJappaWUhP = false;
      bool OyUZijSmuN = false;
      bool gambBIigbp = false;
      bool hrumCleaMT = false;
      bool MnqmfOGEqQ = false;
      bool cGgQsZWyng = false;
      bool rpsqgWDgWx = false;
      bool FraaBsjzwm = false;
      bool CdpLMaLdsb = false;
      bool OglebWuzGJ = false;
      string JYoybMAfBz;
      string MgJhdgqVuh;
      string oarcwqiAQd;
      string bEUcaZeEpr;
      string WsRCPibJVy;
      string oSlERifMCk;
      string dzOWWIMgHD;
      string BmfDLwIoNN;
      string PYeRoISPLq;
      string tSBFNWdufG;
      string VuBRxCmisV;
      string ZpyoUEUwhN;
      string owVQJhkaon;
      string SNJGqUWMUl;
      string VjWoQETMjI;
      string CgMRQYYkIO;
      string RMsEDMfryA;
      string hHbwrFZfzE;
      string NXqfkkAroR;
      string ulDFTJqRlE;
      if(JYoybMAfBz == VuBRxCmisV){YbOiatFQYL = true;}
      else if(VuBRxCmisV == JYoybMAfBz){BJappaWUhP = true;}
      if(MgJhdgqVuh == ZpyoUEUwhN){zskEBsUMTR = true;}
      else if(ZpyoUEUwhN == MgJhdgqVuh){OyUZijSmuN = true;}
      if(oarcwqiAQd == owVQJhkaon){LnDfFuEVbi = true;}
      else if(owVQJhkaon == oarcwqiAQd){gambBIigbp = true;}
      if(bEUcaZeEpr == SNJGqUWMUl){NHsuflJHFp = true;}
      else if(SNJGqUWMUl == bEUcaZeEpr){hrumCleaMT = true;}
      if(WsRCPibJVy == VjWoQETMjI){fiScRBAyEZ = true;}
      else if(VjWoQETMjI == WsRCPibJVy){MnqmfOGEqQ = true;}
      if(oSlERifMCk == CgMRQYYkIO){ndyTrjYhce = true;}
      else if(CgMRQYYkIO == oSlERifMCk){cGgQsZWyng = true;}
      if(dzOWWIMgHD == RMsEDMfryA){pjopyqApXO = true;}
      else if(RMsEDMfryA == dzOWWIMgHD){rpsqgWDgWx = true;}
      if(BmfDLwIoNN == hHbwrFZfzE){jNHclcUZND = true;}
      if(PYeRoISPLq == NXqfkkAroR){uFQCujJYDu = true;}
      if(tSBFNWdufG == ulDFTJqRlE){NadZAwtcea = true;}
      while(hHbwrFZfzE == BmfDLwIoNN){FraaBsjzwm = true;}
      while(NXqfkkAroR == NXqfkkAroR){CdpLMaLdsb = true;}
      while(ulDFTJqRlE == ulDFTJqRlE){OglebWuzGJ = true;}
      if(YbOiatFQYL == true){YbOiatFQYL = false;}
      if(zskEBsUMTR == true){zskEBsUMTR = false;}
      if(LnDfFuEVbi == true){LnDfFuEVbi = false;}
      if(NHsuflJHFp == true){NHsuflJHFp = false;}
      if(fiScRBAyEZ == true){fiScRBAyEZ = false;}
      if(ndyTrjYhce == true){ndyTrjYhce = false;}
      if(pjopyqApXO == true){pjopyqApXO = false;}
      if(jNHclcUZND == true){jNHclcUZND = false;}
      if(uFQCujJYDu == true){uFQCujJYDu = false;}
      if(NadZAwtcea == true){NadZAwtcea = false;}
      if(BJappaWUhP == true){BJappaWUhP = false;}
      if(OyUZijSmuN == true){OyUZijSmuN = false;}
      if(gambBIigbp == true){gambBIigbp = false;}
      if(hrumCleaMT == true){hrumCleaMT = false;}
      if(MnqmfOGEqQ == true){MnqmfOGEqQ = false;}
      if(cGgQsZWyng == true){cGgQsZWyng = false;}
      if(rpsqgWDgWx == true){rpsqgWDgWx = false;}
      if(FraaBsjzwm == true){FraaBsjzwm = false;}
      if(CdpLMaLdsb == true){CdpLMaLdsb = false;}
      if(OglebWuzGJ == true){OglebWuzGJ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OMOABMVKLA
{ 
  void lIbIyPWOmS()
  { 
      bool OXWFJqCEVg = false;
      bool VOsOqKTtcQ = false;
      bool cfbBqfnSQs = false;
      bool aXncHMRlwK = false;
      bool kbXiJdqGxQ = false;
      bool wFNqriuUOd = false;
      bool CjnbGyZQHd = false;
      bool shySfAHsrM = false;
      bool ohUAxVKxjX = false;
      bool QxXRIAnwcL = false;
      bool EayTWNGtLq = false;
      bool bCbBoTOmej = false;
      bool mbfiMwKfJe = false;
      bool MKQfINiBci = false;
      bool OPlswOUVcn = false;
      bool lAQrYbuPHZ = false;
      bool DZkfeQlusa = false;
      bool kNKuUWplmy = false;
      bool bnDVsLtXHf = false;
      bool STtCeoeFOe = false;
      string iNOjUGPQra;
      string QuSGkgmsDD;
      string eyTtxLRxAG;
      string UKGwGjApkF;
      string iyCZnKoVwI;
      string nAdCiktUlJ;
      string nRQlGGnVAA;
      string fwHxTfusFm;
      string VwBrSTfENV;
      string oeDbRQhQSh;
      string VSVubqrfhC;
      string oAPzmqPXTl;
      string ZjIdKArffD;
      string yeaXQDaFPa;
      string bzVLfoPEoZ;
      string BKxfHkKqkb;
      string zLAeEKZrhi;
      string PJHrKZOwyV;
      string knqFBmgtPf;
      string rjUnbQzpHV;
      if(iNOjUGPQra == VSVubqrfhC){OXWFJqCEVg = true;}
      else if(VSVubqrfhC == iNOjUGPQra){EayTWNGtLq = true;}
      if(QuSGkgmsDD == oAPzmqPXTl){VOsOqKTtcQ = true;}
      else if(oAPzmqPXTl == QuSGkgmsDD){bCbBoTOmej = true;}
      if(eyTtxLRxAG == ZjIdKArffD){cfbBqfnSQs = true;}
      else if(ZjIdKArffD == eyTtxLRxAG){mbfiMwKfJe = true;}
      if(UKGwGjApkF == yeaXQDaFPa){aXncHMRlwK = true;}
      else if(yeaXQDaFPa == UKGwGjApkF){MKQfINiBci = true;}
      if(iyCZnKoVwI == bzVLfoPEoZ){kbXiJdqGxQ = true;}
      else if(bzVLfoPEoZ == iyCZnKoVwI){OPlswOUVcn = true;}
      if(nAdCiktUlJ == BKxfHkKqkb){wFNqriuUOd = true;}
      else if(BKxfHkKqkb == nAdCiktUlJ){lAQrYbuPHZ = true;}
      if(nRQlGGnVAA == zLAeEKZrhi){CjnbGyZQHd = true;}
      else if(zLAeEKZrhi == nRQlGGnVAA){DZkfeQlusa = true;}
      if(fwHxTfusFm == PJHrKZOwyV){shySfAHsrM = true;}
      if(VwBrSTfENV == knqFBmgtPf){ohUAxVKxjX = true;}
      if(oeDbRQhQSh == rjUnbQzpHV){QxXRIAnwcL = true;}
      while(PJHrKZOwyV == fwHxTfusFm){kNKuUWplmy = true;}
      while(knqFBmgtPf == knqFBmgtPf){bnDVsLtXHf = true;}
      while(rjUnbQzpHV == rjUnbQzpHV){STtCeoeFOe = true;}
      if(OXWFJqCEVg == true){OXWFJqCEVg = false;}
      if(VOsOqKTtcQ == true){VOsOqKTtcQ = false;}
      if(cfbBqfnSQs == true){cfbBqfnSQs = false;}
      if(aXncHMRlwK == true){aXncHMRlwK = false;}
      if(kbXiJdqGxQ == true){kbXiJdqGxQ = false;}
      if(wFNqriuUOd == true){wFNqriuUOd = false;}
      if(CjnbGyZQHd == true){CjnbGyZQHd = false;}
      if(shySfAHsrM == true){shySfAHsrM = false;}
      if(ohUAxVKxjX == true){ohUAxVKxjX = false;}
      if(QxXRIAnwcL == true){QxXRIAnwcL = false;}
      if(EayTWNGtLq == true){EayTWNGtLq = false;}
      if(bCbBoTOmej == true){bCbBoTOmej = false;}
      if(mbfiMwKfJe == true){mbfiMwKfJe = false;}
      if(MKQfINiBci == true){MKQfINiBci = false;}
      if(OPlswOUVcn == true){OPlswOUVcn = false;}
      if(lAQrYbuPHZ == true){lAQrYbuPHZ = false;}
      if(DZkfeQlusa == true){DZkfeQlusa = false;}
      if(kNKuUWplmy == true){kNKuUWplmy = false;}
      if(bnDVsLtXHf == true){bnDVsLtXHf = false;}
      if(STtCeoeFOe == true){STtCeoeFOe = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YLFTAMGSMS
{ 
  void okPWwTrTdF()
  { 
      bool oSkzLfkbcw = false;
      bool geQTUbTIes = false;
      bool XbAZRymsqF = false;
      bool KjGETrrwIJ = false;
      bool BsDUoQKjNi = false;
      bool WdWquMRLgN = false;
      bool srjWlwYMzb = false;
      bool jWAUqdXekA = false;
      bool nCeIhfMOJG = false;
      bool afPuQIIfir = false;
      bool ryklyAdBzQ = false;
      bool omafYpNbnF = false;
      bool iJCsBEHEEh = false;
      bool VrwrHpRrJh = false;
      bool BKPsRnFiWZ = false;
      bool ufqcJiuqjt = false;
      bool rZxQtwphXi = false;
      bool YUSGEXtJaN = false;
      bool yYhdmoAThY = false;
      bool uZIPpbNyyU = false;
      string xlDSzZBNcq;
      string qHonUdAWln;
      string tCqANZxOea;
      string IXrkzERXTD;
      string gmCKdsfLzm;
      string scUnmttCFZ;
      string bmhCiSPDnQ;
      string LcDiGGAgWT;
      string slPqhfDExF;
      string KPStbtFzbK;
      string dbWibPYlgg;
      string sMekfoXqUC;
      string PKGiGwKJHM;
      string BAFMoRPgOR;
      string GRXPLxkgNl;
      string WNeYtTUncf;
      string FPdUfsHRuO;
      string ZqbDwympWI;
      string LcuUomUfRV;
      string waEiLqmCjS;
      if(xlDSzZBNcq == dbWibPYlgg){oSkzLfkbcw = true;}
      else if(dbWibPYlgg == xlDSzZBNcq){ryklyAdBzQ = true;}
      if(qHonUdAWln == sMekfoXqUC){geQTUbTIes = true;}
      else if(sMekfoXqUC == qHonUdAWln){omafYpNbnF = true;}
      if(tCqANZxOea == PKGiGwKJHM){XbAZRymsqF = true;}
      else if(PKGiGwKJHM == tCqANZxOea){iJCsBEHEEh = true;}
      if(IXrkzERXTD == BAFMoRPgOR){KjGETrrwIJ = true;}
      else if(BAFMoRPgOR == IXrkzERXTD){VrwrHpRrJh = true;}
      if(gmCKdsfLzm == GRXPLxkgNl){BsDUoQKjNi = true;}
      else if(GRXPLxkgNl == gmCKdsfLzm){BKPsRnFiWZ = true;}
      if(scUnmttCFZ == WNeYtTUncf){WdWquMRLgN = true;}
      else if(WNeYtTUncf == scUnmttCFZ){ufqcJiuqjt = true;}
      if(bmhCiSPDnQ == FPdUfsHRuO){srjWlwYMzb = true;}
      else if(FPdUfsHRuO == bmhCiSPDnQ){rZxQtwphXi = true;}
      if(LcDiGGAgWT == ZqbDwympWI){jWAUqdXekA = true;}
      if(slPqhfDExF == LcuUomUfRV){nCeIhfMOJG = true;}
      if(KPStbtFzbK == waEiLqmCjS){afPuQIIfir = true;}
      while(ZqbDwympWI == LcDiGGAgWT){YUSGEXtJaN = true;}
      while(LcuUomUfRV == LcuUomUfRV){yYhdmoAThY = true;}
      while(waEiLqmCjS == waEiLqmCjS){uZIPpbNyyU = true;}
      if(oSkzLfkbcw == true){oSkzLfkbcw = false;}
      if(geQTUbTIes == true){geQTUbTIes = false;}
      if(XbAZRymsqF == true){XbAZRymsqF = false;}
      if(KjGETrrwIJ == true){KjGETrrwIJ = false;}
      if(BsDUoQKjNi == true){BsDUoQKjNi = false;}
      if(WdWquMRLgN == true){WdWquMRLgN = false;}
      if(srjWlwYMzb == true){srjWlwYMzb = false;}
      if(jWAUqdXekA == true){jWAUqdXekA = false;}
      if(nCeIhfMOJG == true){nCeIhfMOJG = false;}
      if(afPuQIIfir == true){afPuQIIfir = false;}
      if(ryklyAdBzQ == true){ryklyAdBzQ = false;}
      if(omafYpNbnF == true){omafYpNbnF = false;}
      if(iJCsBEHEEh == true){iJCsBEHEEh = false;}
      if(VrwrHpRrJh == true){VrwrHpRrJh = false;}
      if(BKPsRnFiWZ == true){BKPsRnFiWZ = false;}
      if(ufqcJiuqjt == true){ufqcJiuqjt = false;}
      if(rZxQtwphXi == true){rZxQtwphXi = false;}
      if(YUSGEXtJaN == true){YUSGEXtJaN = false;}
      if(yYhdmoAThY == true){yYhdmoAThY = false;}
      if(uZIPpbNyyU == true){uZIPpbNyyU = false;}
    } 
}; 
