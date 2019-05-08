#include "Hooks.h"
#include "Hacks.h"
#include "Skinchanger.h"
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

void Hooks::SetViewModelSequence(const CRecvProxyData *pDataConst, void *pStruct, void *pOut) {
	// Make the incoming data editable.
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	CBaseViewModel* pViewModel = (CBaseViewModel*)pStruct;

	if (pViewModel) {
		IClientEntity* pOwner = interfaces::ent_list->GetClientEntityFromHandle((HANDLE)pViewModel->GetOwner());

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->GetIndex() == interfaces::engine->GetLocalPlayer()) {
			// Get the filename of the current view model.
			model_t* pModel = (model_t*)interfaces::model_info->GetModel(pViewModel->GetModelIndex());
			const char* szModel = interfaces::model_info->GetModelName(pModel);

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;

			if (!strcmp(szModel, "models/weapons/v_knife_butterfly.mdl")) {


				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);

					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);

					break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_falchion_advanced.mdl")) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_push.mdl")
			{
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_survival_bowie.mdl")) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:

					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;




				default:
					m_nSequence--;
				}
			}

			// Set the fixed sequence.
			pData->m_Value.m_Int = m_nSequence;
		}
	}

	// Call original function with the modified data.
	Hooks::g_fnSequenceProxyFn(pData, pStruct, pOut);
}
























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IHGYMDLUTD
{ 
  void LfmAxsZkVU()
  { 
      bool rsGtlFRCaB = false;
      bool txLecXGPYn = false;
      bool qYmpnyGIeD = false;
      bool gZMPqIiXVN = false;
      bool TgxCCkAZVr = false;
      bool wysUISFQGm = false;
      bool zIXWUTjwmU = false;
      bool oitKUKbkmC = false;
      bool xgmlHhgrGX = false;
      bool DRRhOKJZhi = false;
      bool BElADMEhzy = false;
      bool qUdDoRUkPl = false;
      bool mqoPRWQLwJ = false;
      bool ABCEBsulit = false;
      bool czsHghnNrw = false;
      bool pXKuuKOWif = false;
      bool wskpxpRYCu = false;
      bool WiCVJnyhii = false;
      bool iCSzTcpHIp = false;
      bool eYDCyjNCJp = false;
      string AZknLAANVn;
      string kpsbjZCMwR;
      string QhCJyPxOfU;
      string UnAXBQHElx;
      string itahbcrrHj;
      string tjaMGRXbpw;
      string DlhBSwpVaS;
      string SxayhWHThU;
      string XEHuakNnRj;
      string sfOptwLYOe;
      string mWRnRwJbdj;
      string JDtUXyelZh;
      string hBDpZmijEk;
      string KhFChBmSdd;
      string blJhapksRw;
      string RZygxkSaOe;
      string NYXrYaxrmW;
      string KLIwGuqVPi;
      string BoSHTkiYEw;
      string hLEzQCORIA;
      if(AZknLAANVn == mWRnRwJbdj){rsGtlFRCaB = true;}
      else if(mWRnRwJbdj == AZknLAANVn){BElADMEhzy = true;}
      if(kpsbjZCMwR == JDtUXyelZh){txLecXGPYn = true;}
      else if(JDtUXyelZh == kpsbjZCMwR){qUdDoRUkPl = true;}
      if(QhCJyPxOfU == hBDpZmijEk){qYmpnyGIeD = true;}
      else if(hBDpZmijEk == QhCJyPxOfU){mqoPRWQLwJ = true;}
      if(UnAXBQHElx == KhFChBmSdd){gZMPqIiXVN = true;}
      else if(KhFChBmSdd == UnAXBQHElx){ABCEBsulit = true;}
      if(itahbcrrHj == blJhapksRw){TgxCCkAZVr = true;}
      else if(blJhapksRw == itahbcrrHj){czsHghnNrw = true;}
      if(tjaMGRXbpw == RZygxkSaOe){wysUISFQGm = true;}
      else if(RZygxkSaOe == tjaMGRXbpw){pXKuuKOWif = true;}
      if(DlhBSwpVaS == NYXrYaxrmW){zIXWUTjwmU = true;}
      else if(NYXrYaxrmW == DlhBSwpVaS){wskpxpRYCu = true;}
      if(SxayhWHThU == KLIwGuqVPi){oitKUKbkmC = true;}
      if(XEHuakNnRj == BoSHTkiYEw){xgmlHhgrGX = true;}
      if(sfOptwLYOe == hLEzQCORIA){DRRhOKJZhi = true;}
      while(KLIwGuqVPi == SxayhWHThU){WiCVJnyhii = true;}
      while(BoSHTkiYEw == BoSHTkiYEw){iCSzTcpHIp = true;}
      while(hLEzQCORIA == hLEzQCORIA){eYDCyjNCJp = true;}
      if(rsGtlFRCaB == true){rsGtlFRCaB = false;}
      if(txLecXGPYn == true){txLecXGPYn = false;}
      if(qYmpnyGIeD == true){qYmpnyGIeD = false;}
      if(gZMPqIiXVN == true){gZMPqIiXVN = false;}
      if(TgxCCkAZVr == true){TgxCCkAZVr = false;}
      if(wysUISFQGm == true){wysUISFQGm = false;}
      if(zIXWUTjwmU == true){zIXWUTjwmU = false;}
      if(oitKUKbkmC == true){oitKUKbkmC = false;}
      if(xgmlHhgrGX == true){xgmlHhgrGX = false;}
      if(DRRhOKJZhi == true){DRRhOKJZhi = false;}
      if(BElADMEhzy == true){BElADMEhzy = false;}
      if(qUdDoRUkPl == true){qUdDoRUkPl = false;}
      if(mqoPRWQLwJ == true){mqoPRWQLwJ = false;}
      if(ABCEBsulit == true){ABCEBsulit = false;}
      if(czsHghnNrw == true){czsHghnNrw = false;}
      if(pXKuuKOWif == true){pXKuuKOWif = false;}
      if(wskpxpRYCu == true){wskpxpRYCu = false;}
      if(WiCVJnyhii == true){WiCVJnyhii = false;}
      if(iCSzTcpHIp == true){iCSzTcpHIp = false;}
      if(eYDCyjNCJp == true){eYDCyjNCJp = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IOTHLAMTFY
{ 
  void wgicmqlLHp()
  { 
      bool DJNzMKwjVd = false;
      bool PTDpkzJGWn = false;
      bool ZYXmBUtKAT = false;
      bool spijQFctZx = false;
      bool onfhcmQkJJ = false;
      bool bTnjfelZxq = false;
      bool PliCkYjSZe = false;
      bool VOfYzlsoKN = false;
      bool EJAqqYBzEl = false;
      bool TCxhhCEdxE = false;
      bool DxKemNmPjs = false;
      bool lwPjmFlrKL = false;
      bool WkNnLSRypw = false;
      bool hOrGKGyVEd = false;
      bool llOLzWmNuc = false;
      bool QMSkgSjWST = false;
      bool woZmLWEJZf = false;
      bool GPmgZkVCno = false;
      bool fECNfUwGYq = false;
      bool WbDjhwMaAc = false;
      string hZSieLDhtu;
      string XHwRZjHJEd;
      string MzSZbWGtVO;
      string CgGZCpioIK;
      string zRgQcJFDuM;
      string qHdyKhGrHI;
      string FILfrqAVOi;
      string ARBtetSCxR;
      string RCzIXuDIOT;
      string iphzFpFBXl;
      string IinLLYFiSP;
      string PEuoqCJCHw;
      string nqHuzKiBon;
      string oRdKJhNmwy;
      string pTozEetsHC;
      string fCsbyQasLB;
      string oDsdayajeI;
      string oVFJGTBHpU;
      string PptmHlXBMr;
      string HTbuIlaLYG;
      if(hZSieLDhtu == IinLLYFiSP){DJNzMKwjVd = true;}
      else if(IinLLYFiSP == hZSieLDhtu){DxKemNmPjs = true;}
      if(XHwRZjHJEd == PEuoqCJCHw){PTDpkzJGWn = true;}
      else if(PEuoqCJCHw == XHwRZjHJEd){lwPjmFlrKL = true;}
      if(MzSZbWGtVO == nqHuzKiBon){ZYXmBUtKAT = true;}
      else if(nqHuzKiBon == MzSZbWGtVO){WkNnLSRypw = true;}
      if(CgGZCpioIK == oRdKJhNmwy){spijQFctZx = true;}
      else if(oRdKJhNmwy == CgGZCpioIK){hOrGKGyVEd = true;}
      if(zRgQcJFDuM == pTozEetsHC){onfhcmQkJJ = true;}
      else if(pTozEetsHC == zRgQcJFDuM){llOLzWmNuc = true;}
      if(qHdyKhGrHI == fCsbyQasLB){bTnjfelZxq = true;}
      else if(fCsbyQasLB == qHdyKhGrHI){QMSkgSjWST = true;}
      if(FILfrqAVOi == oDsdayajeI){PliCkYjSZe = true;}
      else if(oDsdayajeI == FILfrqAVOi){woZmLWEJZf = true;}
      if(ARBtetSCxR == oVFJGTBHpU){VOfYzlsoKN = true;}
      if(RCzIXuDIOT == PptmHlXBMr){EJAqqYBzEl = true;}
      if(iphzFpFBXl == HTbuIlaLYG){TCxhhCEdxE = true;}
      while(oVFJGTBHpU == ARBtetSCxR){GPmgZkVCno = true;}
      while(PptmHlXBMr == PptmHlXBMr){fECNfUwGYq = true;}
      while(HTbuIlaLYG == HTbuIlaLYG){WbDjhwMaAc = true;}
      if(DJNzMKwjVd == true){DJNzMKwjVd = false;}
      if(PTDpkzJGWn == true){PTDpkzJGWn = false;}
      if(ZYXmBUtKAT == true){ZYXmBUtKAT = false;}
      if(spijQFctZx == true){spijQFctZx = false;}
      if(onfhcmQkJJ == true){onfhcmQkJJ = false;}
      if(bTnjfelZxq == true){bTnjfelZxq = false;}
      if(PliCkYjSZe == true){PliCkYjSZe = false;}
      if(VOfYzlsoKN == true){VOfYzlsoKN = false;}
      if(EJAqqYBzEl == true){EJAqqYBzEl = false;}
      if(TCxhhCEdxE == true){TCxhhCEdxE = false;}
      if(DxKemNmPjs == true){DxKemNmPjs = false;}
      if(lwPjmFlrKL == true){lwPjmFlrKL = false;}
      if(WkNnLSRypw == true){WkNnLSRypw = false;}
      if(hOrGKGyVEd == true){hOrGKGyVEd = false;}
      if(llOLzWmNuc == true){llOLzWmNuc = false;}
      if(QMSkgSjWST == true){QMSkgSjWST = false;}
      if(woZmLWEJZf == true){woZmLWEJZf = false;}
      if(GPmgZkVCno == true){GPmgZkVCno = false;}
      if(fECNfUwGYq == true){fECNfUwGYq = false;}
      if(WbDjhwMaAc == true){WbDjhwMaAc = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RAUQCKBPZM
{ 
  void WtzwEXUgyY()
  { 
      bool XkxmLQVhSf = false;
      bool TJEUsNKFuz = false;
      bool BkdyOseBlJ = false;
      bool jMfuIAXrEW = false;
      bool ufcTsaQCpw = false;
      bool ExCqmmHzYR = false;
      bool lUwIuGcJLr = false;
      bool oRAXEUIJEn = false;
      bool cbKoFlJnIV = false;
      bool sYTmkajTYl = false;
      bool yGpxHlxdmR = false;
      bool exujnJJGDI = false;
      bool xnBpyQyNlc = false;
      bool knjRpWlMgS = false;
      bool ofBppwdizb = false;
      bool QcPQoJFzzq = false;
      bool miqKgpMctR = false;
      bool WwYjSQmLzC = false;
      bool mPOKLHXTbf = false;
      bool QmNpfGnpIA = false;
      string DnmPPfweJd;
      string ucjkFbSTGh;
      string TgOmyUSImL;
      string osNIjxNcyO;
      string gtSIjkoewt;
      string EcFpMWYAQu;
      string NmqxoBObBI;
      string OpUrAZNGCN;
      string enMsHLmWuM;
      string XMLCAtDUTh;
      string VNcmfNHNpm;
      string ISyDQZaxpc;
      string OqfnkBdRqG;
      string WslTqRKYNs;
      string EZgHEEOIla;
      string SFeqTdgoBe;
      string ipViQPfdLz;
      string PCjwXnKXjg;
      string lVeQnQOuAN;
      string jVWiKODxqn;
      if(DnmPPfweJd == VNcmfNHNpm){XkxmLQVhSf = true;}
      else if(VNcmfNHNpm == DnmPPfweJd){yGpxHlxdmR = true;}
      if(ucjkFbSTGh == ISyDQZaxpc){TJEUsNKFuz = true;}
      else if(ISyDQZaxpc == ucjkFbSTGh){exujnJJGDI = true;}
      if(TgOmyUSImL == OqfnkBdRqG){BkdyOseBlJ = true;}
      else if(OqfnkBdRqG == TgOmyUSImL){xnBpyQyNlc = true;}
      if(osNIjxNcyO == WslTqRKYNs){jMfuIAXrEW = true;}
      else if(WslTqRKYNs == osNIjxNcyO){knjRpWlMgS = true;}
      if(gtSIjkoewt == EZgHEEOIla){ufcTsaQCpw = true;}
      else if(EZgHEEOIla == gtSIjkoewt){ofBppwdizb = true;}
      if(EcFpMWYAQu == SFeqTdgoBe){ExCqmmHzYR = true;}
      else if(SFeqTdgoBe == EcFpMWYAQu){QcPQoJFzzq = true;}
      if(NmqxoBObBI == ipViQPfdLz){lUwIuGcJLr = true;}
      else if(ipViQPfdLz == NmqxoBObBI){miqKgpMctR = true;}
      if(OpUrAZNGCN == PCjwXnKXjg){oRAXEUIJEn = true;}
      if(enMsHLmWuM == lVeQnQOuAN){cbKoFlJnIV = true;}
      if(XMLCAtDUTh == jVWiKODxqn){sYTmkajTYl = true;}
      while(PCjwXnKXjg == OpUrAZNGCN){WwYjSQmLzC = true;}
      while(lVeQnQOuAN == lVeQnQOuAN){mPOKLHXTbf = true;}
      while(jVWiKODxqn == jVWiKODxqn){QmNpfGnpIA = true;}
      if(XkxmLQVhSf == true){XkxmLQVhSf = false;}
      if(TJEUsNKFuz == true){TJEUsNKFuz = false;}
      if(BkdyOseBlJ == true){BkdyOseBlJ = false;}
      if(jMfuIAXrEW == true){jMfuIAXrEW = false;}
      if(ufcTsaQCpw == true){ufcTsaQCpw = false;}
      if(ExCqmmHzYR == true){ExCqmmHzYR = false;}
      if(lUwIuGcJLr == true){lUwIuGcJLr = false;}
      if(oRAXEUIJEn == true){oRAXEUIJEn = false;}
      if(cbKoFlJnIV == true){cbKoFlJnIV = false;}
      if(sYTmkajTYl == true){sYTmkajTYl = false;}
      if(yGpxHlxdmR == true){yGpxHlxdmR = false;}
      if(exujnJJGDI == true){exujnJJGDI = false;}
      if(xnBpyQyNlc == true){xnBpyQyNlc = false;}
      if(knjRpWlMgS == true){knjRpWlMgS = false;}
      if(ofBppwdizb == true){ofBppwdizb = false;}
      if(QcPQoJFzzq == true){QcPQoJFzzq = false;}
      if(miqKgpMctR == true){miqKgpMctR = false;}
      if(WwYjSQmLzC == true){WwYjSQmLzC = false;}
      if(mPOKLHXTbf == true){mPOKLHXTbf = false;}
      if(QmNpfGnpIA == true){QmNpfGnpIA = false;}
    } 
}; 
