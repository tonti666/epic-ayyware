#include "Interfaces.h"
#include "glow_outline_effect.h"
#include "Utilities.h"
#include <d3d9.h>
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUISurfaceFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;
CreateInterface_t MatFactory = NULL;
CreateInterface_t InputSys = NULL;
CreateInterface_t PhysFactory = NULL;
CreateInterface_t StdFactory = NULL;
void interfaces::Initialise()
{
	EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	InputSys = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::InputSys, "CreateInterface");
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");
	char* CHLClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)Utilities::Memory::FindTextPattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)Utilities::Memory::FindTextPattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineClient0");
	char* ClientPredictionInterface = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VClientPrediction0");
	char* MatSystemInterfaceName = (char*)Utilities::Memory::FindTextPattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VModelInfoClient0");
	char* EngineTraceInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VPhysicsSurfaceProps0");
	char* VEngineCvarName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineCvar00");
	char* effectthing = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineEffects001");
	client = (IBaseClientDLL*)ClientFactory(CHLClientInterfaceName, NULL);
	engine = (IVEngineClient*)EngineFactory(EngineClientInterfaceName, NULL);
	panels = (IPanel*)VGUI2Factory(VGUI2PanelsInterfaceName, NULL);
	window = FindWindow("Valve001", NULL);
	surface = (ISurface*)VGUISurfaceFactory(VGUISurfaceInterfaceName, NULL);
	ent_list = (IClientEntityList*)ClientFactory(EntityListInterfaceName, NULL);
	DebugOverlay = (IVDebugOverlay*)EngineFactory(EngineDebugThingInterface, NULL);
	prediction_dword = (DWORD*)ClientFactory(ClientPredictionInterface, NULL);
	materialsystem = (CMaterialSystem*)MatFactory(MatSystemInterfaceName, NULL);
	render_view = (CVRenderView*)EngineFactory(EngineRenderViewInterface, NULL);
	model_render = (IVModelRender*)EngineFactory(EngineModelRenderInterface, NULL);
	model_info = (CModelInfo*)EngineFactory(EngineModelInfoInterface, NULL);
	trace = (IEngineTrace*)EngineFactory(EngineTraceInterfaceName, NULL);
	trace_2 = (Trace2*)EngineFactory(EngineTraceInterfaceName, NULL);
	m_iInputSys = (IInputSystem*)InputSys("InputSystemVersion001", NULL);
	phys_props = (IPhysicsSurfaceProps*)PhysFactory(PhysPropsInterfaces, NULL);
	cvar = (ICVar*)StdFactory(VEngineCvarName, NULL);
	event_manager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);
	g_pD3DDevice9 = **(IDirect3DDevice9***)(Utilities::Memory::FindPatternV2("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	effects = (IVEffects*)EngineFactory(effectthing, NULL);
	ClientMode = **(IClientModeShared***)((*(uintptr_t**)client)[10] + 0x5);
	globals = **reinterpret_cast< CGlobalVarsBase*** >((*reinterpret_cast< DWORD** >(client))[0] + 0x1B);
	g_pViewRenderBeams = *(IViewRenderBeams**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 1);
	PDWORD pdwClientVMT = *(PDWORD*)client;
	glow_manager = *(CGlowObjectManager**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
	client_state = **(CClientState***)(Utilities::Memory::FindPatternV2("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	g_ClientSideAnimationList = (CUtlVectorSimple*)*(DWORD*)(Utilities::Memory::FindPatternV2("client_panorama.dll", "A1 ? ? ? ? F6 44 F0 04 01 74 0B") + 1);
	pinput = *(CInput**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
}
namespace interfaces
{
	IBaseClientDLL* client;
	IVEngineClient* engine;
	IPanel* panels;
	IClientEntityList* ent_list;
	ISurface* surface;
	IVDebugOverlay* DebugOverlay;
	IClientModeShared *ClientMode;
	CGlobalVarsBase *globals;
	DWORD *prediction_dword;
	CGlowObjectManager* glow_manager;
	CMaterialSystem* materialsystem;
	CVRenderView* render_view;
	IVModelRender* model_render;
	IInputSystem* m_iInputSys;
	CModelInfo* model_info;
	IEngineTrace* trace;
	IPhysicsSurfaceProps* phys_props;
	CClientState* client_state;
	IVEffects* effects;
	Trace2 *trace_2;
	HWND window;
	ICVar *cvar;
	CInput* pinput;
	CUtlVectorSimple *g_ClientSideAnimationList;
	IDirect3DDevice9        *g_pD3DDevice9;
	IGameEventManager2 *event_manager;
	HANDLE __FNTHANDLE;
	IMoveHelper* movehelper;
	IPrediction *c_prediction;
	IGameMovement* gamemovement;
	IViewRenderBeams* g_pViewRenderBeams;
	CGameMovement * c_gamemove;
};





























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MIJKPGHGPG
{ 
  void nffkbaNhUJ()
  { 
      bool DzNmCiMdNr = false;
      bool lphtkrFFNQ = false;
      bool OZWyQLoRMU = false;
      bool FemqPozjJD = false;
      bool lDijKaWyWe = false;
      bool fAfeNdbOVq = false;
      bool stQrmHDqeu = false;
      bool sVuJdKpmmp = false;
      bool nqtgnEyHcl = false;
      bool NjNSKrlpxC = false;
      bool WfTblKFWAC = false;
      bool mnbfCBziEi = false;
      bool JuNshIIcyl = false;
      bool XApjSIXSud = false;
      bool zbHZyYIgPI = false;
      bool rClNsbSZtc = false;
      bool BbXOelyqNt = false;
      bool dgnZjgmmtI = false;
      bool AaNpsUObpp = false;
      bool oxYRcXEsuL = false;
      string XuPQVZbbZt;
      string pZzjMEkNkS;
      string ZKBMblBZaL;
      string olKrcGjCcO;
      string ItotqqaBMg;
      string IFRNfMtCoi;
      string YCBOEDOHHF;
      string UikVKeVGUk;
      string KAMsCiUxMP;
      string MgNspGsZpm;
      string bTsGMZUwDA;
      string qmhEXXoHMV;
      string iRtrsmVreD;
      string kuQgIpHGgb;
      string OXsLXZzIUS;
      string RlNTEKZBNY;
      string EcRzPuVAuh;
      string QhZjGLXiTS;
      string nMWLlygwcx;
      string QFsHWlGswb;
      if(XuPQVZbbZt == bTsGMZUwDA){DzNmCiMdNr = true;}
      else if(bTsGMZUwDA == XuPQVZbbZt){WfTblKFWAC = true;}
      if(pZzjMEkNkS == qmhEXXoHMV){lphtkrFFNQ = true;}
      else if(qmhEXXoHMV == pZzjMEkNkS){mnbfCBziEi = true;}
      if(ZKBMblBZaL == iRtrsmVreD){OZWyQLoRMU = true;}
      else if(iRtrsmVreD == ZKBMblBZaL){JuNshIIcyl = true;}
      if(olKrcGjCcO == kuQgIpHGgb){FemqPozjJD = true;}
      else if(kuQgIpHGgb == olKrcGjCcO){XApjSIXSud = true;}
      if(ItotqqaBMg == OXsLXZzIUS){lDijKaWyWe = true;}
      else if(OXsLXZzIUS == ItotqqaBMg){zbHZyYIgPI = true;}
      if(IFRNfMtCoi == RlNTEKZBNY){fAfeNdbOVq = true;}
      else if(RlNTEKZBNY == IFRNfMtCoi){rClNsbSZtc = true;}
      if(YCBOEDOHHF == EcRzPuVAuh){stQrmHDqeu = true;}
      else if(EcRzPuVAuh == YCBOEDOHHF){BbXOelyqNt = true;}
      if(UikVKeVGUk == QhZjGLXiTS){sVuJdKpmmp = true;}
      if(KAMsCiUxMP == nMWLlygwcx){nqtgnEyHcl = true;}
      if(MgNspGsZpm == QFsHWlGswb){NjNSKrlpxC = true;}
      while(QhZjGLXiTS == UikVKeVGUk){dgnZjgmmtI = true;}
      while(nMWLlygwcx == nMWLlygwcx){AaNpsUObpp = true;}
      while(QFsHWlGswb == QFsHWlGswb){oxYRcXEsuL = true;}
      if(DzNmCiMdNr == true){DzNmCiMdNr = false;}
      if(lphtkrFFNQ == true){lphtkrFFNQ = false;}
      if(OZWyQLoRMU == true){OZWyQLoRMU = false;}
      if(FemqPozjJD == true){FemqPozjJD = false;}
      if(lDijKaWyWe == true){lDijKaWyWe = false;}
      if(fAfeNdbOVq == true){fAfeNdbOVq = false;}
      if(stQrmHDqeu == true){stQrmHDqeu = false;}
      if(sVuJdKpmmp == true){sVuJdKpmmp = false;}
      if(nqtgnEyHcl == true){nqtgnEyHcl = false;}
      if(NjNSKrlpxC == true){NjNSKrlpxC = false;}
      if(WfTblKFWAC == true){WfTblKFWAC = false;}
      if(mnbfCBziEi == true){mnbfCBziEi = false;}
      if(JuNshIIcyl == true){JuNshIIcyl = false;}
      if(XApjSIXSud == true){XApjSIXSud = false;}
      if(zbHZyYIgPI == true){zbHZyYIgPI = false;}
      if(rClNsbSZtc == true){rClNsbSZtc = false;}
      if(BbXOelyqNt == true){BbXOelyqNt = false;}
      if(dgnZjgmmtI == true){dgnZjgmmtI = false;}
      if(AaNpsUObpp == true){AaNpsUObpp = false;}
      if(oxYRcXEsuL == true){oxYRcXEsuL = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DFLPYMCVDW
{ 
  void akZZjqLGCl()
  { 
      bool VmjgBAjZXF = false;
      bool WOhIxErzmk = false;
      bool HEOZqRLVfV = false;
      bool xGSdxhlZkA = false;
      bool EXHnkaCgqX = false;
      bool KQzLtQYJoi = false;
      bool oOtBHQwDLO = false;
      bool RRNmgjxaoP = false;
      bool PdeHKfGGjg = false;
      bool IbkBKocFfT = false;
      bool eyVbGsCLeJ = false;
      bool TyTYTKjmpb = false;
      bool KoTdFogLcq = false;
      bool GquVkQICAR = false;
      bool ctaZMKILgA = false;
      bool FpPEJoeqVR = false;
      bool MpinDcDynD = false;
      bool CglYyPOsRu = false;
      bool gUebkQbpMw = false;
      bool kQgiFmLmWj = false;
      string QiezEiLKfb;
      string AYwnWDFmFq;
      string JCWTiSgSEz;
      string cGkeHdYpBH;
      string QQaWbYhhwN;
      string eFaSANLKQl;
      string BrGfytmQgr;
      string eOkxarbArJ;
      string kzWpTWeSes;
      string SNZPGXKtXg;
      string kQADiQUcqH;
      string lLSGBBVQLb;
      string OUlmZQoVqD;
      string hgbKXTyTGD;
      string CBaWHGuSNE;
      string CtiwRQdNcS;
      string azlnZFjHhg;
      string CBJIYMsPfT;
      string VWHmAnyesH;
      string pkpncDPmxB;
      if(QiezEiLKfb == kQADiQUcqH){VmjgBAjZXF = true;}
      else if(kQADiQUcqH == QiezEiLKfb){eyVbGsCLeJ = true;}
      if(AYwnWDFmFq == lLSGBBVQLb){WOhIxErzmk = true;}
      else if(lLSGBBVQLb == AYwnWDFmFq){TyTYTKjmpb = true;}
      if(JCWTiSgSEz == OUlmZQoVqD){HEOZqRLVfV = true;}
      else if(OUlmZQoVqD == JCWTiSgSEz){KoTdFogLcq = true;}
      if(cGkeHdYpBH == hgbKXTyTGD){xGSdxhlZkA = true;}
      else if(hgbKXTyTGD == cGkeHdYpBH){GquVkQICAR = true;}
      if(QQaWbYhhwN == CBaWHGuSNE){EXHnkaCgqX = true;}
      else if(CBaWHGuSNE == QQaWbYhhwN){ctaZMKILgA = true;}
      if(eFaSANLKQl == CtiwRQdNcS){KQzLtQYJoi = true;}
      else if(CtiwRQdNcS == eFaSANLKQl){FpPEJoeqVR = true;}
      if(BrGfytmQgr == azlnZFjHhg){oOtBHQwDLO = true;}
      else if(azlnZFjHhg == BrGfytmQgr){MpinDcDynD = true;}
      if(eOkxarbArJ == CBJIYMsPfT){RRNmgjxaoP = true;}
      if(kzWpTWeSes == VWHmAnyesH){PdeHKfGGjg = true;}
      if(SNZPGXKtXg == pkpncDPmxB){IbkBKocFfT = true;}
      while(CBJIYMsPfT == eOkxarbArJ){CglYyPOsRu = true;}
      while(VWHmAnyesH == VWHmAnyesH){gUebkQbpMw = true;}
      while(pkpncDPmxB == pkpncDPmxB){kQgiFmLmWj = true;}
      if(VmjgBAjZXF == true){VmjgBAjZXF = false;}
      if(WOhIxErzmk == true){WOhIxErzmk = false;}
      if(HEOZqRLVfV == true){HEOZqRLVfV = false;}
      if(xGSdxhlZkA == true){xGSdxhlZkA = false;}
      if(EXHnkaCgqX == true){EXHnkaCgqX = false;}
      if(KQzLtQYJoi == true){KQzLtQYJoi = false;}
      if(oOtBHQwDLO == true){oOtBHQwDLO = false;}
      if(RRNmgjxaoP == true){RRNmgjxaoP = false;}
      if(PdeHKfGGjg == true){PdeHKfGGjg = false;}
      if(IbkBKocFfT == true){IbkBKocFfT = false;}
      if(eyVbGsCLeJ == true){eyVbGsCLeJ = false;}
      if(TyTYTKjmpb == true){TyTYTKjmpb = false;}
      if(KoTdFogLcq == true){KoTdFogLcq = false;}
      if(GquVkQICAR == true){GquVkQICAR = false;}
      if(ctaZMKILgA == true){ctaZMKILgA = false;}
      if(FpPEJoeqVR == true){FpPEJoeqVR = false;}
      if(MpinDcDynD == true){MpinDcDynD = false;}
      if(CglYyPOsRu == true){CglYyPOsRu = false;}
      if(gUebkQbpMw == true){gUebkQbpMw = false;}
      if(kQgiFmLmWj == true){kQgiFmLmWj = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XMKULBSWCQ
{ 
  void hmQoKJzniy()
  { 
      bool bxVWWbePji = false;
      bool zrOJziotLo = false;
      bool rSPzabIOCb = false;
      bool ZeqmpqrVXh = false;
      bool TKGnexAZsV = false;
      bool JAObhIhVwu = false;
      bool SPMIJUAaTI = false;
      bool pTWcUgLbTc = false;
      bool aiVfRYPPFg = false;
      bool JVZcOExyeK = false;
      bool wZFjfpdkGA = false;
      bool wFCSTjsgpj = false;
      bool EkVmzUssNQ = false;
      bool iNxIfBclAb = false;
      bool teIFjDSHIe = false;
      bool KQHtFLDSkL = false;
      bool wjWpQiIslS = false;
      bool HHfNoYcQCu = false;
      bool BlkiGRFCCU = false;
      bool xJAofJISnB = false;
      string gUqxfjUYYl;
      string eKTYhdsjMd;
      string fjnouRaYlB;
      string joJXHfDQIG;
      string WPETBGLAgo;
      string noFKpwgCCN;
      string JwlWBpAwRC;
      string acaGHPeUmn;
      string GZqwNtjzgK;
      string yANuxacgRK;
      string FPieOLNyDa;
      string RMPNHiEpJG;
      string GBOGDZOfJf;
      string lyCbipqLhC;
      string fkwFslkAmO;
      string VmXKEEAbbo;
      string xhoWnSKmdQ;
      string qsIHaVEINR;
      string oNruKzaqWe;
      string oPjsEyuZRZ;
      if(gUqxfjUYYl == FPieOLNyDa){bxVWWbePji = true;}
      else if(FPieOLNyDa == gUqxfjUYYl){wZFjfpdkGA = true;}
      if(eKTYhdsjMd == RMPNHiEpJG){zrOJziotLo = true;}
      else if(RMPNHiEpJG == eKTYhdsjMd){wFCSTjsgpj = true;}
      if(fjnouRaYlB == GBOGDZOfJf){rSPzabIOCb = true;}
      else if(GBOGDZOfJf == fjnouRaYlB){EkVmzUssNQ = true;}
      if(joJXHfDQIG == lyCbipqLhC){ZeqmpqrVXh = true;}
      else if(lyCbipqLhC == joJXHfDQIG){iNxIfBclAb = true;}
      if(WPETBGLAgo == fkwFslkAmO){TKGnexAZsV = true;}
      else if(fkwFslkAmO == WPETBGLAgo){teIFjDSHIe = true;}
      if(noFKpwgCCN == VmXKEEAbbo){JAObhIhVwu = true;}
      else if(VmXKEEAbbo == noFKpwgCCN){KQHtFLDSkL = true;}
      if(JwlWBpAwRC == xhoWnSKmdQ){SPMIJUAaTI = true;}
      else if(xhoWnSKmdQ == JwlWBpAwRC){wjWpQiIslS = true;}
      if(acaGHPeUmn == qsIHaVEINR){pTWcUgLbTc = true;}
      if(GZqwNtjzgK == oNruKzaqWe){aiVfRYPPFg = true;}
      if(yANuxacgRK == oPjsEyuZRZ){JVZcOExyeK = true;}
      while(qsIHaVEINR == acaGHPeUmn){HHfNoYcQCu = true;}
      while(oNruKzaqWe == oNruKzaqWe){BlkiGRFCCU = true;}
      while(oPjsEyuZRZ == oPjsEyuZRZ){xJAofJISnB = true;}
      if(bxVWWbePji == true){bxVWWbePji = false;}
      if(zrOJziotLo == true){zrOJziotLo = false;}
      if(rSPzabIOCb == true){rSPzabIOCb = false;}
      if(ZeqmpqrVXh == true){ZeqmpqrVXh = false;}
      if(TKGnexAZsV == true){TKGnexAZsV = false;}
      if(JAObhIhVwu == true){JAObhIhVwu = false;}
      if(SPMIJUAaTI == true){SPMIJUAaTI = false;}
      if(pTWcUgLbTc == true){pTWcUgLbTc = false;}
      if(aiVfRYPPFg == true){aiVfRYPPFg = false;}
      if(JVZcOExyeK == true){JVZcOExyeK = false;}
      if(wZFjfpdkGA == true){wZFjfpdkGA = false;}
      if(wFCSTjsgpj == true){wFCSTjsgpj = false;}
      if(EkVmzUssNQ == true){EkVmzUssNQ = false;}
      if(iNxIfBclAb == true){iNxIfBclAb = false;}
      if(teIFjDSHIe == true){teIFjDSHIe = false;}
      if(KQHtFLDSkL == true){KQHtFLDSkL = false;}
      if(wjWpQiIslS == true){wjWpQiIslS = false;}
      if(HHfNoYcQCu == true){HHfNoYcQCu = false;}
      if(BlkiGRFCCU == true){BlkiGRFCCU = false;}
      if(xJAofJISnB == true){xJAofJISnB = false;}
    } 
}; 
