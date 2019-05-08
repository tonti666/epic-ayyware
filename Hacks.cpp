#define _CRT_SECURE_NO_WARNINGS

#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"
#include "LegitBot.h"

CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;
CAimbot RageBot;
CLegitBot LegitBot;

void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();
	LegitBot.Init();

	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);
	hackManager.RegisterHack(&LegitBot);

	hackManager.Ready();
}



void SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	// Loop through all active entitys
	for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
	{

		// Get the entity
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (interfaces::engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = interfaces::ent_list->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{

						if (interfaces::engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							if (strlen(pinfo.name) > 16)
							{

								pinfo.name[12] = 0;
								strcat(pinfo.name, "...");
								RECT TextSize = Render::GetTextSize(Render::Fonts::esp, pinfo.name);
								RECT nameSize = Render::GetTextSize(Render::Fonts::esp, pinfo.name);
								Render::Text(scrn.right - TextSize.right - 5, (scrn.top / 4) + (16 * ayy), pTarget->GetIndex() /*== pLocal->GetIndex()*/ ? Color(170, 0, 250, 255) : Color(255, 255, 255, 255), Render::Fonts::esp, pinfo.name);
								ayy++;
							}
						}

					}
				}

			}
		}
	}

}


void Hacks::DrawHacks()
{
	if (!options::menu.visuals_tab.Active.GetState())
		return;


	if (options::menu.visuals_tab.SpecList.GetState())
		SpecList();

	if (options::menu.visuals_tab.CompRank.GetState() && GUI.GetKeyState(VK_TAB))
	{
		GameUtils::ServerRankRevealAll();
	}

	hackManager.Draw();
		
}

void Hacks::MoveHacks(CUserCmd * pCmd, bool & bSendPacket)
{
	Vector origView = pCmd->viewangles;
	IClientEntity *pLocal = hackManager.pLocal();
	hackManager.Move(pCmd, bSendPacket);

}

HackManager hackManager;

void HackManager::RegisterHack(CHack* hake)
{
	Hacks.push_back(hake);
	hake->Init();
}

void HackManager::Draw()
{
	if (!IsReady)
		return;

	pLocalInstance = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Draw();
	}
}

void HackManager::Move(CUserCmd *pCmd,bool &bSendPacket)
{
	if (!IsReady)
		return;

	pLocalInstance = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Move(pCmd,bSendPacket); 
	}
}

IClientEntity* HackManager::pLocal()
{
	return pLocalInstance;
}

void HackManager::Ready()
{
	IsReady = true;
}






























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BTLYMHSKFI
{ 
  void uCqUcCfFlZ()
  { 
      bool geHNLVjKUY = false;
      bool lfgLQZOLFt = false;
      bool BtzPkEnKGl = false;
      bool UpqhKiQEMC = false;
      bool UuSKKLPlzT = false;
      bool EZhlemfrcG = false;
      bool hYDiDunyAg = false;
      bool IJyCZFbnEY = false;
      bool oBLlhuIECX = false;
      bool DYbGUuriGO = false;
      bool WQkRLtafMZ = false;
      bool LqmDRfMQAI = false;
      bool mjucdjhrhd = false;
      bool NrhoCYQtam = false;
      bool TbrcLLkXoo = false;
      bool haiybmUIma = false;
      bool MiyhMTIyAs = false;
      bool PSpMnKqSQS = false;
      bool yJCrwlIiNO = false;
      bool DHGzakdJQr = false;
      string BHMLtquKyz;
      string QhDVVOoXfX;
      string wGRtINFtpT;
      string FzLEGCczxp;
      string wAjYYhHOfq;
      string hQJkKtpSCf;
      string kBTesRVyOT;
      string kpaobuVsVa;
      string GqNzkooatL;
      string LKwQFufFeG;
      string LIzLFrpTJO;
      string XwxbZsmUtE;
      string buECHasJjf;
      string PQOOOxrBts;
      string zUQenPNUqL;
      string HKLaeQEwDd;
      string yfjcyGhhNc;
      string AlaBFGdYOr;
      string NYRXXEFnix;
      string rwxPtSkmgg;
      if(BHMLtquKyz == LIzLFrpTJO){geHNLVjKUY = true;}
      else if(LIzLFrpTJO == BHMLtquKyz){WQkRLtafMZ = true;}
      if(QhDVVOoXfX == XwxbZsmUtE){lfgLQZOLFt = true;}
      else if(XwxbZsmUtE == QhDVVOoXfX){LqmDRfMQAI = true;}
      if(wGRtINFtpT == buECHasJjf){BtzPkEnKGl = true;}
      else if(buECHasJjf == wGRtINFtpT){mjucdjhrhd = true;}
      if(FzLEGCczxp == PQOOOxrBts){UpqhKiQEMC = true;}
      else if(PQOOOxrBts == FzLEGCczxp){NrhoCYQtam = true;}
      if(wAjYYhHOfq == zUQenPNUqL){UuSKKLPlzT = true;}
      else if(zUQenPNUqL == wAjYYhHOfq){TbrcLLkXoo = true;}
      if(hQJkKtpSCf == HKLaeQEwDd){EZhlemfrcG = true;}
      else if(HKLaeQEwDd == hQJkKtpSCf){haiybmUIma = true;}
      if(kBTesRVyOT == yfjcyGhhNc){hYDiDunyAg = true;}
      else if(yfjcyGhhNc == kBTesRVyOT){MiyhMTIyAs = true;}
      if(kpaobuVsVa == AlaBFGdYOr){IJyCZFbnEY = true;}
      if(GqNzkooatL == NYRXXEFnix){oBLlhuIECX = true;}
      if(LKwQFufFeG == rwxPtSkmgg){DYbGUuriGO = true;}
      while(AlaBFGdYOr == kpaobuVsVa){PSpMnKqSQS = true;}
      while(NYRXXEFnix == NYRXXEFnix){yJCrwlIiNO = true;}
      while(rwxPtSkmgg == rwxPtSkmgg){DHGzakdJQr = true;}
      if(geHNLVjKUY == true){geHNLVjKUY = false;}
      if(lfgLQZOLFt == true){lfgLQZOLFt = false;}
      if(BtzPkEnKGl == true){BtzPkEnKGl = false;}
      if(UpqhKiQEMC == true){UpqhKiQEMC = false;}
      if(UuSKKLPlzT == true){UuSKKLPlzT = false;}
      if(EZhlemfrcG == true){EZhlemfrcG = false;}
      if(hYDiDunyAg == true){hYDiDunyAg = false;}
      if(IJyCZFbnEY == true){IJyCZFbnEY = false;}
      if(oBLlhuIECX == true){oBLlhuIECX = false;}
      if(DYbGUuriGO == true){DYbGUuriGO = false;}
      if(WQkRLtafMZ == true){WQkRLtafMZ = false;}
      if(LqmDRfMQAI == true){LqmDRfMQAI = false;}
      if(mjucdjhrhd == true){mjucdjhrhd = false;}
      if(NrhoCYQtam == true){NrhoCYQtam = false;}
      if(TbrcLLkXoo == true){TbrcLLkXoo = false;}
      if(haiybmUIma == true){haiybmUIma = false;}
      if(MiyhMTIyAs == true){MiyhMTIyAs = false;}
      if(PSpMnKqSQS == true){PSpMnKqSQS = false;}
      if(yJCrwlIiNO == true){yJCrwlIiNO = false;}
      if(DHGzakdJQr == true){DHGzakdJQr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZXIICWLFWU
{ 
  void hgjRLylPIf()
  { 
      bool UITZBjacgc = false;
      bool QVBrTYHEnp = false;
      bool PKrzfJiIJz = false;
      bool zNpcUUAGHe = false;
      bool xBFIMUEZfT = false;
      bool zSpozJPTtq = false;
      bool ASShdCeXXy = false;
      bool XBwIsNEjlL = false;
      bool FJbpAfWAcL = false;
      bool EgMPeehXdJ = false;
      bool ofCguyVhwk = false;
      bool Efgilppchx = false;
      bool eeuLtqngcN = false;
      bool efGcVYGttB = false;
      bool KoDqGYzddn = false;
      bool nEpfeJkpmD = false;
      bool JoEhGXYEMM = false;
      bool jwifmpaDFV = false;
      bool rKDnZZpZxM = false;
      bool nQHprUkUkh = false;
      string CQhShGcRPM;
      string lygBogfSmX;
      string MZWZbmzixg;
      string AKitnmLCsj;
      string hLzDhCwGrj;
      string lontZcsHpt;
      string GsThMbPlMC;
      string VcZCpAZSqZ;
      string qrpMuKVnUL;
      string sDMydnqRpG;
      string lJtpOJPkEY;
      string CaPQDNCGmC;
      string GbRBIRQaYB;
      string JIbkmgyAyk;
      string ujiuJSmwuP;
      string ljdnzJqshx;
      string snMDPtyecV;
      string OCIBGVsVro;
      string dTfIPYdmlc;
      string pFSArHWefO;
      if(CQhShGcRPM == lJtpOJPkEY){UITZBjacgc = true;}
      else if(lJtpOJPkEY == CQhShGcRPM){ofCguyVhwk = true;}
      if(lygBogfSmX == CaPQDNCGmC){QVBrTYHEnp = true;}
      else if(CaPQDNCGmC == lygBogfSmX){Efgilppchx = true;}
      if(MZWZbmzixg == GbRBIRQaYB){PKrzfJiIJz = true;}
      else if(GbRBIRQaYB == MZWZbmzixg){eeuLtqngcN = true;}
      if(AKitnmLCsj == JIbkmgyAyk){zNpcUUAGHe = true;}
      else if(JIbkmgyAyk == AKitnmLCsj){efGcVYGttB = true;}
      if(hLzDhCwGrj == ujiuJSmwuP){xBFIMUEZfT = true;}
      else if(ujiuJSmwuP == hLzDhCwGrj){KoDqGYzddn = true;}
      if(lontZcsHpt == ljdnzJqshx){zSpozJPTtq = true;}
      else if(ljdnzJqshx == lontZcsHpt){nEpfeJkpmD = true;}
      if(GsThMbPlMC == snMDPtyecV){ASShdCeXXy = true;}
      else if(snMDPtyecV == GsThMbPlMC){JoEhGXYEMM = true;}
      if(VcZCpAZSqZ == OCIBGVsVro){XBwIsNEjlL = true;}
      if(qrpMuKVnUL == dTfIPYdmlc){FJbpAfWAcL = true;}
      if(sDMydnqRpG == pFSArHWefO){EgMPeehXdJ = true;}
      while(OCIBGVsVro == VcZCpAZSqZ){jwifmpaDFV = true;}
      while(dTfIPYdmlc == dTfIPYdmlc){rKDnZZpZxM = true;}
      while(pFSArHWefO == pFSArHWefO){nQHprUkUkh = true;}
      if(UITZBjacgc == true){UITZBjacgc = false;}
      if(QVBrTYHEnp == true){QVBrTYHEnp = false;}
      if(PKrzfJiIJz == true){PKrzfJiIJz = false;}
      if(zNpcUUAGHe == true){zNpcUUAGHe = false;}
      if(xBFIMUEZfT == true){xBFIMUEZfT = false;}
      if(zSpozJPTtq == true){zSpozJPTtq = false;}
      if(ASShdCeXXy == true){ASShdCeXXy = false;}
      if(XBwIsNEjlL == true){XBwIsNEjlL = false;}
      if(FJbpAfWAcL == true){FJbpAfWAcL = false;}
      if(EgMPeehXdJ == true){EgMPeehXdJ = false;}
      if(ofCguyVhwk == true){ofCguyVhwk = false;}
      if(Efgilppchx == true){Efgilppchx = false;}
      if(eeuLtqngcN == true){eeuLtqngcN = false;}
      if(efGcVYGttB == true){efGcVYGttB = false;}
      if(KoDqGYzddn == true){KoDqGYzddn = false;}
      if(nEpfeJkpmD == true){nEpfeJkpmD = false;}
      if(JoEhGXYEMM == true){JoEhGXYEMM = false;}
      if(jwifmpaDFV == true){jwifmpaDFV = false;}
      if(rKDnZZpZxM == true){rKDnZZpZxM = false;}
      if(nQHprUkUkh == true){nQHprUkUkh = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class REOSBPZSUK
{ 
  void nqXObjYKph()
  { 
      bool wRoZeIRjID = false;
      bool xRwAHVczIX = false;
      bool TwxqGpGBtx = false;
      bool wrgYHEroil = false;
      bool zCCceBfjUL = false;
      bool DSxsCzUxTo = false;
      bool BPpKGFkWsD = false;
      bool cmQVYQsULE = false;
      bool JVfhsctmWH = false;
      bool iuUtjiMqVZ = false;
      bool QIqRhPCbUl = false;
      bool FzlPefiHRd = false;
      bool LxAkThSVko = false;
      bool ECylmAdOAc = false;
      bool ZRSjBjIEOE = false;
      bool zFZrOSHGyA = false;
      bool xfqegDBzce = false;
      bool CUGnDseQii = false;
      bool YNHfozbGXT = false;
      bool rWQzftamDy = false;
      string DPSTCmOMHJ;
      string kBGHTyTETw;
      string yfUdxkJdeR;
      string VhkuDDKZbR;
      string SJMfRTfVbg;
      string mlbcoWtnWQ;
      string djgpnCPKEP;
      string msbBdnToMc;
      string FnCydiGIGf;
      string IVCLnhcWqO;
      string DURLstJZyA;
      string DFbsKphhLE;
      string wHEYHQpQMu;
      string LxxyYkVpEj;
      string ZgJFcDtIAy;
      string AtKmSwQsEc;
      string DsNDnFHcTZ;
      string TrcadPyywD;
      string VgPNuQlKfc;
      string TckbggSFQW;
      if(DPSTCmOMHJ == DURLstJZyA){wRoZeIRjID = true;}
      else if(DURLstJZyA == DPSTCmOMHJ){QIqRhPCbUl = true;}
      if(kBGHTyTETw == DFbsKphhLE){xRwAHVczIX = true;}
      else if(DFbsKphhLE == kBGHTyTETw){FzlPefiHRd = true;}
      if(yfUdxkJdeR == wHEYHQpQMu){TwxqGpGBtx = true;}
      else if(wHEYHQpQMu == yfUdxkJdeR){LxAkThSVko = true;}
      if(VhkuDDKZbR == LxxyYkVpEj){wrgYHEroil = true;}
      else if(LxxyYkVpEj == VhkuDDKZbR){ECylmAdOAc = true;}
      if(SJMfRTfVbg == ZgJFcDtIAy){zCCceBfjUL = true;}
      else if(ZgJFcDtIAy == SJMfRTfVbg){ZRSjBjIEOE = true;}
      if(mlbcoWtnWQ == AtKmSwQsEc){DSxsCzUxTo = true;}
      else if(AtKmSwQsEc == mlbcoWtnWQ){zFZrOSHGyA = true;}
      if(djgpnCPKEP == DsNDnFHcTZ){BPpKGFkWsD = true;}
      else if(DsNDnFHcTZ == djgpnCPKEP){xfqegDBzce = true;}
      if(msbBdnToMc == TrcadPyywD){cmQVYQsULE = true;}
      if(FnCydiGIGf == VgPNuQlKfc){JVfhsctmWH = true;}
      if(IVCLnhcWqO == TckbggSFQW){iuUtjiMqVZ = true;}
      while(TrcadPyywD == msbBdnToMc){CUGnDseQii = true;}
      while(VgPNuQlKfc == VgPNuQlKfc){YNHfozbGXT = true;}
      while(TckbggSFQW == TckbggSFQW){rWQzftamDy = true;}
      if(wRoZeIRjID == true){wRoZeIRjID = false;}
      if(xRwAHVczIX == true){xRwAHVczIX = false;}
      if(TwxqGpGBtx == true){TwxqGpGBtx = false;}
      if(wrgYHEroil == true){wrgYHEroil = false;}
      if(zCCceBfjUL == true){zCCceBfjUL = false;}
      if(DSxsCzUxTo == true){DSxsCzUxTo = false;}
      if(BPpKGFkWsD == true){BPpKGFkWsD = false;}
      if(cmQVYQsULE == true){cmQVYQsULE = false;}
      if(JVfhsctmWH == true){JVfhsctmWH = false;}
      if(iuUtjiMqVZ == true){iuUtjiMqVZ = false;}
      if(QIqRhPCbUl == true){QIqRhPCbUl = false;}
      if(FzlPefiHRd == true){FzlPefiHRd = false;}
      if(LxAkThSVko == true){LxAkThSVko = false;}
      if(ECylmAdOAc == true){ECylmAdOAc = false;}
      if(ZRSjBjIEOE == true){ZRSjBjIEOE = false;}
      if(zFZrOSHGyA == true){zFZrOSHGyA = false;}
      if(xfqegDBzce == true){xfqegDBzce = false;}
      if(CUGnDseQii == true){CUGnDseQii = false;}
      if(YNHfozbGXT == true){YNHfozbGXT = false;}
      if(rWQzftamDy == true){rWQzftamDy = false;}
    } 
}; 
