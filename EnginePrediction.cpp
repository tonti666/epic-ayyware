#include "EnginePrediction.h"
#include "NetVars.h"
#include "XorStr.hpp"
#include "MathFunctions.h"
#include "Hacks.h"
#include "EnginePrediction.h"
BYTE bMoveData[200];

void CPredictionSystem::StartPrediction(CUserCmd* pCmd) 
{
	static int flTickBase;

	if (pCmd)
	{
		if (pCmd->hasbeenpredicted)
			flTickBase = hackManager.pLocal()->GetTickBase();
		else
			++flTickBase;
	}

	static bool bInit = false;
	if (!bInit) {
		m_pPredictionRandomSeed = *(int**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
		bInit = true;
	}

//	*m_pPredictionRandomSeed = MD5_PseudoRandom(Hacks.CurrentCmd->command_number) & 0x7FFFFFFF; /* no thanks */

	m_flOldCurtime = interfaces::globals->curtime;
	m_flOldFrametime = interfaces::globals->frametime;

	if (!interfaces::c_gamemove)
		return;

	interfaces::globals->curtime = flTickBase * interfaces::globals->interval_per_tick;
	interfaces::globals->frametime = interfaces::globals->interval_per_tick;

	interfaces::c_gamemove->StartTrackPredictionErrors(hackManager.pLocal());

	memset(&m_MoveData, 0, sizeof(m_MoveData));

	interfaces::movehelper->SetHost(hackManager.pLocal());
	interfaces::c_prediction->SetupMove(hackManager.pLocal(), pCmd, interfaces::movehelper, &m_MoveData);
	interfaces::c_gamemove->ProcessMovement(hackManager.pLocal(), &m_MoveData);
	interfaces::c_prediction->FinishMove(hackManager.pLocal(), pCmd, &m_MoveData);
}
void CPredictionSystem::EndPrediction(CUserCmd* pCmd) 
{
	if (!interfaces::c_gamemove)
		return;

	interfaces::c_gamemove->FinishTrackPredictionErrors(hackManager.pLocal());
	interfaces::movehelper->SetHost(0);

	*m_pPredictionRandomSeed = -1;

	interfaces::globals->curtime = m_flOldCurtime;
	interfaces::globals->frametime = m_flOldFrametime;
}



































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HNXTUVAPGU
{ 
  void TaUIrGxrYd()
  { 
      bool cUPJfkMSqb = false;
      bool bHWfmREyZI = false;
      bool rJnxAhdXnf = false;
      bool PWaCNrSkhs = false;
      bool SFBcUMewfi = false;
      bool BjEEjSgSIy = false;
      bool GodKMEfURt = false;
      bool zTPgomHKbn = false;
      bool CgrFeeSVSu = false;
      bool LzbjldbqPQ = false;
      bool fJekjhTWOw = false;
      bool KVqUuZxXeg = false;
      bool lJCWizqfuL = false;
      bool LQtLIsCGVx = false;
      bool LwHVjpHHoL = false;
      bool gDLidVHHBj = false;
      bool pnbCktafMC = false;
      bool XpChVpCfms = false;
      bool rjqidHELgl = false;
      bool ktEkGWUEkp = false;
      string pPZHilStAU;
      string BAKeGYPjIU;
      string SPsCxASRmO;
      string tSFuQcKpwX;
      string zUkyCIAnXp;
      string SNcwUaVXKi;
      string SAGlZitPNd;
      string OYmjUxeuzo;
      string zdtwugoJZp;
      string GlfNEzNyXT;
      string NkZtbtZsaX;
      string waYUinGKps;
      string sjoXWbsZeV;
      string ifWeaOEzTB;
      string GwiBsfNKdQ;
      string rDHdZduCWA;
      string AANEnQDstU;
      string jGqomxBneq;
      string NiuOdfXsFx;
      string TCcSAGhdMc;
      if(pPZHilStAU == NkZtbtZsaX){cUPJfkMSqb = true;}
      else if(NkZtbtZsaX == pPZHilStAU){fJekjhTWOw = true;}
      if(BAKeGYPjIU == waYUinGKps){bHWfmREyZI = true;}
      else if(waYUinGKps == BAKeGYPjIU){KVqUuZxXeg = true;}
      if(SPsCxASRmO == sjoXWbsZeV){rJnxAhdXnf = true;}
      else if(sjoXWbsZeV == SPsCxASRmO){lJCWizqfuL = true;}
      if(tSFuQcKpwX == ifWeaOEzTB){PWaCNrSkhs = true;}
      else if(ifWeaOEzTB == tSFuQcKpwX){LQtLIsCGVx = true;}
      if(zUkyCIAnXp == GwiBsfNKdQ){SFBcUMewfi = true;}
      else if(GwiBsfNKdQ == zUkyCIAnXp){LwHVjpHHoL = true;}
      if(SNcwUaVXKi == rDHdZduCWA){BjEEjSgSIy = true;}
      else if(rDHdZduCWA == SNcwUaVXKi){gDLidVHHBj = true;}
      if(SAGlZitPNd == AANEnQDstU){GodKMEfURt = true;}
      else if(AANEnQDstU == SAGlZitPNd){pnbCktafMC = true;}
      if(OYmjUxeuzo == jGqomxBneq){zTPgomHKbn = true;}
      if(zdtwugoJZp == NiuOdfXsFx){CgrFeeSVSu = true;}
      if(GlfNEzNyXT == TCcSAGhdMc){LzbjldbqPQ = true;}
      while(jGqomxBneq == OYmjUxeuzo){XpChVpCfms = true;}
      while(NiuOdfXsFx == NiuOdfXsFx){rjqidHELgl = true;}
      while(TCcSAGhdMc == TCcSAGhdMc){ktEkGWUEkp = true;}
      if(cUPJfkMSqb == true){cUPJfkMSqb = false;}
      if(bHWfmREyZI == true){bHWfmREyZI = false;}
      if(rJnxAhdXnf == true){rJnxAhdXnf = false;}
      if(PWaCNrSkhs == true){PWaCNrSkhs = false;}
      if(SFBcUMewfi == true){SFBcUMewfi = false;}
      if(BjEEjSgSIy == true){BjEEjSgSIy = false;}
      if(GodKMEfURt == true){GodKMEfURt = false;}
      if(zTPgomHKbn == true){zTPgomHKbn = false;}
      if(CgrFeeSVSu == true){CgrFeeSVSu = false;}
      if(LzbjldbqPQ == true){LzbjldbqPQ = false;}
      if(fJekjhTWOw == true){fJekjhTWOw = false;}
      if(KVqUuZxXeg == true){KVqUuZxXeg = false;}
      if(lJCWizqfuL == true){lJCWizqfuL = false;}
      if(LQtLIsCGVx == true){LQtLIsCGVx = false;}
      if(LwHVjpHHoL == true){LwHVjpHHoL = false;}
      if(gDLidVHHBj == true){gDLidVHHBj = false;}
      if(pnbCktafMC == true){pnbCktafMC = false;}
      if(XpChVpCfms == true){XpChVpCfms = false;}
      if(rjqidHELgl == true){rjqidHELgl = false;}
      if(ktEkGWUEkp == true){ktEkGWUEkp = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WNTPMYDRVB
{ 
  void fHFzxNXlsy()
  { 
      bool WUaZtwbigR = false;
      bool SNNBsadDNj = false;
      bool zicBmqZHxd = false;
      bool EubsZbKMHd = false;
      bool OppNJdPXYV = false;
      bool NYpKXmqYlD = false;
      bool WUkCMWkTbR = false;
      bool QRujVJrOqJ = false;
      bool aBhaQfFQVl = false;
      bool jkaEHtEmBm = false;
      bool rwuHEwcFoO = false;
      bool ubgbbUlJlf = false;
      bool RUyphfpqcb = false;
      bool FFNEQdGaWT = false;
      bool CzbHqHDybl = false;
      bool HtAUUXyFZY = false;
      bool FlcNJoOcLy = false;
      bool NKmRdSrpzZ = false;
      bool brOpTKFsjr = false;
      bool fBBxOPVOyB = false;
      string rVzoeJMcCU;
      string FsaQZgbYiW;
      string TiiWKSdwYj;
      string ZmzpftrHWX;
      string iozbfFNcCC;
      string WJhxzJlcdL;
      string eQERtStjjf;
      string nTCKmdiGqF;
      string FBsVNPECqW;
      string HPcLsRkggM;
      string KcrVODHWJU;
      string ZelYGdWhEd;
      string NrsNXYpqRV;
      string sBZdtbIXHJ;
      string IgrVwbcEEP;
      string uCcCySjeyo;
      string MQFnaYFIde;
      string UzZtVgzWRw;
      string wklCZqYWeb;
      string qdZSxrbEuX;
      if(rVzoeJMcCU == KcrVODHWJU){WUaZtwbigR = true;}
      else if(KcrVODHWJU == rVzoeJMcCU){rwuHEwcFoO = true;}
      if(FsaQZgbYiW == ZelYGdWhEd){SNNBsadDNj = true;}
      else if(ZelYGdWhEd == FsaQZgbYiW){ubgbbUlJlf = true;}
      if(TiiWKSdwYj == NrsNXYpqRV){zicBmqZHxd = true;}
      else if(NrsNXYpqRV == TiiWKSdwYj){RUyphfpqcb = true;}
      if(ZmzpftrHWX == sBZdtbIXHJ){EubsZbKMHd = true;}
      else if(sBZdtbIXHJ == ZmzpftrHWX){FFNEQdGaWT = true;}
      if(iozbfFNcCC == IgrVwbcEEP){OppNJdPXYV = true;}
      else if(IgrVwbcEEP == iozbfFNcCC){CzbHqHDybl = true;}
      if(WJhxzJlcdL == uCcCySjeyo){NYpKXmqYlD = true;}
      else if(uCcCySjeyo == WJhxzJlcdL){HtAUUXyFZY = true;}
      if(eQERtStjjf == MQFnaYFIde){WUkCMWkTbR = true;}
      else if(MQFnaYFIde == eQERtStjjf){FlcNJoOcLy = true;}
      if(nTCKmdiGqF == UzZtVgzWRw){QRujVJrOqJ = true;}
      if(FBsVNPECqW == wklCZqYWeb){aBhaQfFQVl = true;}
      if(HPcLsRkggM == qdZSxrbEuX){jkaEHtEmBm = true;}
      while(UzZtVgzWRw == nTCKmdiGqF){NKmRdSrpzZ = true;}
      while(wklCZqYWeb == wklCZqYWeb){brOpTKFsjr = true;}
      while(qdZSxrbEuX == qdZSxrbEuX){fBBxOPVOyB = true;}
      if(WUaZtwbigR == true){WUaZtwbigR = false;}
      if(SNNBsadDNj == true){SNNBsadDNj = false;}
      if(zicBmqZHxd == true){zicBmqZHxd = false;}
      if(EubsZbKMHd == true){EubsZbKMHd = false;}
      if(OppNJdPXYV == true){OppNJdPXYV = false;}
      if(NYpKXmqYlD == true){NYpKXmqYlD = false;}
      if(WUkCMWkTbR == true){WUkCMWkTbR = false;}
      if(QRujVJrOqJ == true){QRujVJrOqJ = false;}
      if(aBhaQfFQVl == true){aBhaQfFQVl = false;}
      if(jkaEHtEmBm == true){jkaEHtEmBm = false;}
      if(rwuHEwcFoO == true){rwuHEwcFoO = false;}
      if(ubgbbUlJlf == true){ubgbbUlJlf = false;}
      if(RUyphfpqcb == true){RUyphfpqcb = false;}
      if(FFNEQdGaWT == true){FFNEQdGaWT = false;}
      if(CzbHqHDybl == true){CzbHqHDybl = false;}
      if(HtAUUXyFZY == true){HtAUUXyFZY = false;}
      if(FlcNJoOcLy == true){FlcNJoOcLy = false;}
      if(NKmRdSrpzZ == true){NKmRdSrpzZ = false;}
      if(brOpTKFsjr == true){brOpTKFsjr = false;}
      if(fBBxOPVOyB == true){fBBxOPVOyB = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ICPQKHWIHE
{ 
  void draPEEbBMa()
  { 
      bool oKTQlfxXoe = false;
      bool KVcIrPnORB = false;
      bool XDLFeeeIDK = false;
      bool QGrUWlYcIJ = false;
      bool ObGemnbPps = false;
      bool LtlWElzNlG = false;
      bool BoTdNDdIkG = false;
      bool qLGHlwxfSp = false;
      bool DPRJzVUUQk = false;
      bool lKpBIZHbQi = false;
      bool KSdjCzmdjI = false;
      bool nSuwClWKer = false;
      bool hIqzTfUFzM = false;
      bool oVPifkkhHz = false;
      bool ipjIxTPtyV = false;
      bool CwxYABbdPV = false;
      bool eheWSSDLPR = false;
      bool AnCfIrXiCA = false;
      bool QZGcKrYqhd = false;
      bool JHRlTALMod = false;
      string wiByyzoUoE;
      string MxcWEMlBKE;
      string blyBQcfZIV;
      string qYhZRTQbLG;
      string wSjHmFrcHP;
      string gYeoYJkzhU;
      string TPLTMnODEj;
      string uJpEHIBpOK;
      string kDRsoStsBH;
      string iRtpmgHQtz;
      string qoXWTWiahP;
      string ItkWjPxRet;
      string sVCxBdOzQZ;
      string BkuPTwftZR;
      string iSLlznfWFW;
      string jokAFVlaVp;
      string HiJHNWyUDn;
      string jDrNKZKQxM;
      string xrttpLzuJy;
      string JEXiJAJDYb;
      if(wiByyzoUoE == qoXWTWiahP){oKTQlfxXoe = true;}
      else if(qoXWTWiahP == wiByyzoUoE){KSdjCzmdjI = true;}
      if(MxcWEMlBKE == ItkWjPxRet){KVcIrPnORB = true;}
      else if(ItkWjPxRet == MxcWEMlBKE){nSuwClWKer = true;}
      if(blyBQcfZIV == sVCxBdOzQZ){XDLFeeeIDK = true;}
      else if(sVCxBdOzQZ == blyBQcfZIV){hIqzTfUFzM = true;}
      if(qYhZRTQbLG == BkuPTwftZR){QGrUWlYcIJ = true;}
      else if(BkuPTwftZR == qYhZRTQbLG){oVPifkkhHz = true;}
      if(wSjHmFrcHP == iSLlznfWFW){ObGemnbPps = true;}
      else if(iSLlznfWFW == wSjHmFrcHP){ipjIxTPtyV = true;}
      if(gYeoYJkzhU == jokAFVlaVp){LtlWElzNlG = true;}
      else if(jokAFVlaVp == gYeoYJkzhU){CwxYABbdPV = true;}
      if(TPLTMnODEj == HiJHNWyUDn){BoTdNDdIkG = true;}
      else if(HiJHNWyUDn == TPLTMnODEj){eheWSSDLPR = true;}
      if(uJpEHIBpOK == jDrNKZKQxM){qLGHlwxfSp = true;}
      if(kDRsoStsBH == xrttpLzuJy){DPRJzVUUQk = true;}
      if(iRtpmgHQtz == JEXiJAJDYb){lKpBIZHbQi = true;}
      while(jDrNKZKQxM == uJpEHIBpOK){AnCfIrXiCA = true;}
      while(xrttpLzuJy == xrttpLzuJy){QZGcKrYqhd = true;}
      while(JEXiJAJDYb == JEXiJAJDYb){JHRlTALMod = true;}
      if(oKTQlfxXoe == true){oKTQlfxXoe = false;}
      if(KVcIrPnORB == true){KVcIrPnORB = false;}
      if(XDLFeeeIDK == true){XDLFeeeIDK = false;}
      if(QGrUWlYcIJ == true){QGrUWlYcIJ = false;}
      if(ObGemnbPps == true){ObGemnbPps = false;}
      if(LtlWElzNlG == true){LtlWElzNlG = false;}
      if(BoTdNDdIkG == true){BoTdNDdIkG = false;}
      if(qLGHlwxfSp == true){qLGHlwxfSp = false;}
      if(DPRJzVUUQk == true){DPRJzVUUQk = false;}
      if(lKpBIZHbQi == true){lKpBIZHbQi = false;}
      if(KSdjCzmdjI == true){KSdjCzmdjI = false;}
      if(nSuwClWKer == true){nSuwClWKer = false;}
      if(hIqzTfUFzM == true){hIqzTfUFzM = false;}
      if(oVPifkkhHz == true){oVPifkkhHz = false;}
      if(ipjIxTPtyV == true){ipjIxTPtyV = false;}
      if(CwxYABbdPV == true){CwxYABbdPV = false;}
      if(eheWSSDLPR == true){eheWSSDLPR = false;}
      if(AnCfIrXiCA == true){AnCfIrXiCA = false;}
      if(QZGcKrYqhd == true){QZGcKrYqhd = false;}
      if(JHRlTALMod == true){JHRlTALMod = false;}
    } 
}; 
