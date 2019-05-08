#include "lin_extp.h"

template<class T, class U>
T fine(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
void LinearExtrapolations::run()
{

	auto m_local = hackManager.pLocal();
	if (m_local && m_local->IsAlive()) {
		for (int i = 1; i < interfaces::globals->maxClients; i++)
		{
			auto m_entity = interfaces::ent_list->get_client_entity(i);
			if (!m_entity || m_entity == m_local || m_entity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !m_entity->IsAlive())
				continue;

			float simtime_delta = m_entity->GetSimulationTime() - m_entity->m_flOldSimulationTime();
			int choked_ticks = fine(TIME_TO_TICKS(simtime_delta), 1, 15);
			Vector lastOrig;

			if (lastOrig.Length() != m_entity->GetOrigin().Length())
				lastOrig = m_entity->GetOrigin();

			float delta_distance = (m_entity->GetOrigin() - lastOrig).LengthSqr();
			if (delta_distance > 4096.f)
			{
				Vector velocity_per_tick = m_entity->GetVelocity() * interfaces::globals->interval_per_tick;
				auto new_origin = m_entity->GetOrigin() + (velocity_per_tick * choked_ticks);
				m_entity->SetAbsOriginal(new_origin);
			}
		}
	}

}






























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PXRWYCKNPR
{ 
  void zSzrYtHyim()
  { 
      bool yTBPNorAlu = false;
      bool AVJQiPcadw = false;
      bool kWfHhpKKJJ = false;
      bool rxcdmXFqTR = false;
      bool DXCFnZOKjr = false;
      bool BmAFAbnshX = false;
      bool ehpbDCrfdK = false;
      bool JkHMrMUZTH = false;
      bool azZRHFxWcK = false;
      bool iMIumVhZCk = false;
      bool higZhQfnNn = false;
      bool GOZZweIYUy = false;
      bool FVCSaxbLYV = false;
      bool euXhZqSAjq = false;
      bool YJIoDUliyI = false;
      bool NJEEfLfNyj = false;
      bool nTOqILqrAE = false;
      bool MHodYwMNOF = false;
      bool LsMQIVpSCN = false;
      bool LEBzCFtRfq = false;
      string dfryXUxFCG;
      string YUFLYghnhQ;
      string xXRbtaaDQz;
      string FHmbDuBQSi;
      string mjRcuEDraA;
      string sGRzXGNQEx;
      string ttoDARgdIc;
      string eQVzdMFofo;
      string WhcPGyomEx;
      string hkMUosuObd;
      string XZjyUbjSVK;
      string BXjOaLRYwC;
      string btYLusEBKj;
      string RqNCRwxeue;
      string NVQtuJBxtc;
      string bVRZBcAYCa;
      string WiIlrJKgwd;
      string OmdRnFcePl;
      string rHGhHwYnBn;
      string wYbTHeNeCl;
      if(dfryXUxFCG == XZjyUbjSVK){yTBPNorAlu = true;}
      else if(XZjyUbjSVK == dfryXUxFCG){higZhQfnNn = true;}
      if(YUFLYghnhQ == BXjOaLRYwC){AVJQiPcadw = true;}
      else if(BXjOaLRYwC == YUFLYghnhQ){GOZZweIYUy = true;}
      if(xXRbtaaDQz == btYLusEBKj){kWfHhpKKJJ = true;}
      else if(btYLusEBKj == xXRbtaaDQz){FVCSaxbLYV = true;}
      if(FHmbDuBQSi == RqNCRwxeue){rxcdmXFqTR = true;}
      else if(RqNCRwxeue == FHmbDuBQSi){euXhZqSAjq = true;}
      if(mjRcuEDraA == NVQtuJBxtc){DXCFnZOKjr = true;}
      else if(NVQtuJBxtc == mjRcuEDraA){YJIoDUliyI = true;}
      if(sGRzXGNQEx == bVRZBcAYCa){BmAFAbnshX = true;}
      else if(bVRZBcAYCa == sGRzXGNQEx){NJEEfLfNyj = true;}
      if(ttoDARgdIc == WiIlrJKgwd){ehpbDCrfdK = true;}
      else if(WiIlrJKgwd == ttoDARgdIc){nTOqILqrAE = true;}
      if(eQVzdMFofo == OmdRnFcePl){JkHMrMUZTH = true;}
      if(WhcPGyomEx == rHGhHwYnBn){azZRHFxWcK = true;}
      if(hkMUosuObd == wYbTHeNeCl){iMIumVhZCk = true;}
      while(OmdRnFcePl == eQVzdMFofo){MHodYwMNOF = true;}
      while(rHGhHwYnBn == rHGhHwYnBn){LsMQIVpSCN = true;}
      while(wYbTHeNeCl == wYbTHeNeCl){LEBzCFtRfq = true;}
      if(yTBPNorAlu == true){yTBPNorAlu = false;}
      if(AVJQiPcadw == true){AVJQiPcadw = false;}
      if(kWfHhpKKJJ == true){kWfHhpKKJJ = false;}
      if(rxcdmXFqTR == true){rxcdmXFqTR = false;}
      if(DXCFnZOKjr == true){DXCFnZOKjr = false;}
      if(BmAFAbnshX == true){BmAFAbnshX = false;}
      if(ehpbDCrfdK == true){ehpbDCrfdK = false;}
      if(JkHMrMUZTH == true){JkHMrMUZTH = false;}
      if(azZRHFxWcK == true){azZRHFxWcK = false;}
      if(iMIumVhZCk == true){iMIumVhZCk = false;}
      if(higZhQfnNn == true){higZhQfnNn = false;}
      if(GOZZweIYUy == true){GOZZweIYUy = false;}
      if(FVCSaxbLYV == true){FVCSaxbLYV = false;}
      if(euXhZqSAjq == true){euXhZqSAjq = false;}
      if(YJIoDUliyI == true){YJIoDUliyI = false;}
      if(NJEEfLfNyj == true){NJEEfLfNyj = false;}
      if(nTOqILqrAE == true){nTOqILqrAE = false;}
      if(MHodYwMNOF == true){MHodYwMNOF = false;}
      if(LsMQIVpSCN == true){LsMQIVpSCN = false;}
      if(LEBzCFtRfq == true){LEBzCFtRfq = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HJDHNLEKIS
{ 
  void HwpkRDCSwR()
  { 
      bool cMumSSkClV = false;
      bool sPClXPogbq = false;
      bool jIEPCsLoQx = false;
      bool fNaQsHAbbS = false;
      bool IfDSqCnMJF = false;
      bool FgQWWUIekZ = false;
      bool GulopfokrQ = false;
      bool ONFIhDElXA = false;
      bool FzfKqyxDtY = false;
      bool zUpCLMtObL = false;
      bool NDTrqqaHnS = false;
      bool PWHkTHObxq = false;
      bool hkpTRsWDwU = false;
      bool JpJLlCemSA = false;
      bool XoPaJcNFhE = false;
      bool xRRkccMAZx = false;
      bool UXKSJZMuyR = false;
      bool cbIwTsVPJo = false;
      bool fQVrWqYYhJ = false;
      bool peMVezdwRj = false;
      string SRWDbZmfYj;
      string tXBFesBWul;
      string EuRNOYhHpS;
      string VqYeoqNYBt;
      string awOgFNMqsx;
      string gwZntXAInC;
      string qfpFOCyWQm;
      string ecVAKHgjIB;
      string LqNrlYQjjQ;
      string pFjyCuFIRC;
      string OpZFREaZdl;
      string pPzmpARxTo;
      string kNlzxZrWxP;
      string BXRFlVQHZg;
      string yqGyZaFwHC;
      string cIocQADaIr;
      string LDbNjSxsBU;
      string iKXlqtGnYW;
      string TwOulsiook;
      string RpyhVtOrNH;
      if(SRWDbZmfYj == OpZFREaZdl){cMumSSkClV = true;}
      else if(OpZFREaZdl == SRWDbZmfYj){NDTrqqaHnS = true;}
      if(tXBFesBWul == pPzmpARxTo){sPClXPogbq = true;}
      else if(pPzmpARxTo == tXBFesBWul){PWHkTHObxq = true;}
      if(EuRNOYhHpS == kNlzxZrWxP){jIEPCsLoQx = true;}
      else if(kNlzxZrWxP == EuRNOYhHpS){hkpTRsWDwU = true;}
      if(VqYeoqNYBt == BXRFlVQHZg){fNaQsHAbbS = true;}
      else if(BXRFlVQHZg == VqYeoqNYBt){JpJLlCemSA = true;}
      if(awOgFNMqsx == yqGyZaFwHC){IfDSqCnMJF = true;}
      else if(yqGyZaFwHC == awOgFNMqsx){XoPaJcNFhE = true;}
      if(gwZntXAInC == cIocQADaIr){FgQWWUIekZ = true;}
      else if(cIocQADaIr == gwZntXAInC){xRRkccMAZx = true;}
      if(qfpFOCyWQm == LDbNjSxsBU){GulopfokrQ = true;}
      else if(LDbNjSxsBU == qfpFOCyWQm){UXKSJZMuyR = true;}
      if(ecVAKHgjIB == iKXlqtGnYW){ONFIhDElXA = true;}
      if(LqNrlYQjjQ == TwOulsiook){FzfKqyxDtY = true;}
      if(pFjyCuFIRC == RpyhVtOrNH){zUpCLMtObL = true;}
      while(iKXlqtGnYW == ecVAKHgjIB){cbIwTsVPJo = true;}
      while(TwOulsiook == TwOulsiook){fQVrWqYYhJ = true;}
      while(RpyhVtOrNH == RpyhVtOrNH){peMVezdwRj = true;}
      if(cMumSSkClV == true){cMumSSkClV = false;}
      if(sPClXPogbq == true){sPClXPogbq = false;}
      if(jIEPCsLoQx == true){jIEPCsLoQx = false;}
      if(fNaQsHAbbS == true){fNaQsHAbbS = false;}
      if(IfDSqCnMJF == true){IfDSqCnMJF = false;}
      if(FgQWWUIekZ == true){FgQWWUIekZ = false;}
      if(GulopfokrQ == true){GulopfokrQ = false;}
      if(ONFIhDElXA == true){ONFIhDElXA = false;}
      if(FzfKqyxDtY == true){FzfKqyxDtY = false;}
      if(zUpCLMtObL == true){zUpCLMtObL = false;}
      if(NDTrqqaHnS == true){NDTrqqaHnS = false;}
      if(PWHkTHObxq == true){PWHkTHObxq = false;}
      if(hkpTRsWDwU == true){hkpTRsWDwU = false;}
      if(JpJLlCemSA == true){JpJLlCemSA = false;}
      if(XoPaJcNFhE == true){XoPaJcNFhE = false;}
      if(xRRkccMAZx == true){xRRkccMAZx = false;}
      if(UXKSJZMuyR == true){UXKSJZMuyR = false;}
      if(cbIwTsVPJo == true){cbIwTsVPJo = false;}
      if(fQVrWqYYhJ == true){fQVrWqYYhJ = false;}
      if(peMVezdwRj == true){peMVezdwRj = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MWDTBDNZBT
{ 
  void EBqQYkeCbS()
  { 
      bool eQVwwyRQlH = false;
      bool yBgiFEgJZN = false;
      bool wRkzYTxdWl = false;
      bool BtlzJccPJV = false;
      bool sOaTfgUuVm = false;
      bool nNEDOMfVXb = false;
      bool AnszGEOgZb = false;
      bool bRAEUhpRTq = false;
      bool yBHjynohkx = false;
      bool wGrgUEUaNb = false;
      bool RnVsKWMAig = false;
      bool VNCfczgJBS = false;
      bool OgucfVYSiP = false;
      bool MOEqZKfOYa = false;
      bool aozEaiEMEV = false;
      bool XLYuMkrsNo = false;
      bool ysQSTGJKFR = false;
      bool SRkaqZgqBw = false;
      bool QMONqBpJdo = false;
      bool RmBicgYfkT = false;
      string xHFpKuzQos;
      string HGknLsJonM;
      string JGrirewspf;
      string uUzqQOVdbh;
      string LaCQgYsMTs;
      string gVsnqqPWye;
      string zFZJxmGtXx;
      string WeMGAiDaBS;
      string liZwyYaKzZ;
      string yzfwhGArSs;
      string RzoWWCJmmI;
      string mnUesPfdLX;
      string MutxyXyPXt;
      string hLIIAltDZw;
      string PmXqlKdADS;
      string aWftgVEqXz;
      string DtPsZbfMdG;
      string VcEPIYXDKn;
      string syMFTLeNae;
      string hCearVXdRK;
      if(xHFpKuzQos == RzoWWCJmmI){eQVwwyRQlH = true;}
      else if(RzoWWCJmmI == xHFpKuzQos){RnVsKWMAig = true;}
      if(HGknLsJonM == mnUesPfdLX){yBgiFEgJZN = true;}
      else if(mnUesPfdLX == HGknLsJonM){VNCfczgJBS = true;}
      if(JGrirewspf == MutxyXyPXt){wRkzYTxdWl = true;}
      else if(MutxyXyPXt == JGrirewspf){OgucfVYSiP = true;}
      if(uUzqQOVdbh == hLIIAltDZw){BtlzJccPJV = true;}
      else if(hLIIAltDZw == uUzqQOVdbh){MOEqZKfOYa = true;}
      if(LaCQgYsMTs == PmXqlKdADS){sOaTfgUuVm = true;}
      else if(PmXqlKdADS == LaCQgYsMTs){aozEaiEMEV = true;}
      if(gVsnqqPWye == aWftgVEqXz){nNEDOMfVXb = true;}
      else if(aWftgVEqXz == gVsnqqPWye){XLYuMkrsNo = true;}
      if(zFZJxmGtXx == DtPsZbfMdG){AnszGEOgZb = true;}
      else if(DtPsZbfMdG == zFZJxmGtXx){ysQSTGJKFR = true;}
      if(WeMGAiDaBS == VcEPIYXDKn){bRAEUhpRTq = true;}
      if(liZwyYaKzZ == syMFTLeNae){yBHjynohkx = true;}
      if(yzfwhGArSs == hCearVXdRK){wGrgUEUaNb = true;}
      while(VcEPIYXDKn == WeMGAiDaBS){SRkaqZgqBw = true;}
      while(syMFTLeNae == syMFTLeNae){QMONqBpJdo = true;}
      while(hCearVXdRK == hCearVXdRK){RmBicgYfkT = true;}
      if(eQVwwyRQlH == true){eQVwwyRQlH = false;}
      if(yBgiFEgJZN == true){yBgiFEgJZN = false;}
      if(wRkzYTxdWl == true){wRkzYTxdWl = false;}
      if(BtlzJccPJV == true){BtlzJccPJV = false;}
      if(sOaTfgUuVm == true){sOaTfgUuVm = false;}
      if(nNEDOMfVXb == true){nNEDOMfVXb = false;}
      if(AnszGEOgZb == true){AnszGEOgZb = false;}
      if(bRAEUhpRTq == true){bRAEUhpRTq = false;}
      if(yBHjynohkx == true){yBHjynohkx = false;}
      if(wGrgUEUaNb == true){wGrgUEUaNb = false;}
      if(RnVsKWMAig == true){RnVsKWMAig = false;}
      if(VNCfczgJBS == true){VNCfczgJBS = false;}
      if(OgucfVYSiP == true){OgucfVYSiP = false;}
      if(MOEqZKfOYa == true){MOEqZKfOYa = false;}
      if(aozEaiEMEV == true){aozEaiEMEV = false;}
      if(XLYuMkrsNo == true){XLYuMkrsNo = false;}
      if(ysQSTGJKFR == true){ysQSTGJKFR = false;}
      if(SRkaqZgqBw == true){SRkaqZgqBw = false;}
      if(QMONqBpJdo == true){QMONqBpJdo = false;}
      if(RmBicgYfkT == true){RmBicgYfkT = false;}
    } 
}; 
