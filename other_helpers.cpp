#include "antiaim.h"

void NormalizeNum(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

float antiaim_helper::fov_ent(Vector ViewOffSet, Vector View, IClientEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = GetHitboxPosition(entity, hitbox);
	AngleVectors(Angles, &Forward);
	VectorSubtract(AimPos, Origin, Delta);
	NormalizeNum(Delta, Delta);
	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int antiaim_helper::closest()
{
	int index = -1;
	float lowest_fov = 180.f; // maybe??

	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	if (!local_player->IsAlive())
		return -1;

	Vector local_position = local_player->GetAbsOrigin() + local_player->GetViewOffset();
	Vector angles;
	interfaces::engine->get_viewangles(angles);
	for (int i = 1; i <= interfaces::globals->maxClients; i++)
	{
		IClientEntity *entity = interfaces::ent_list->get_client_entity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->team() == local_player->team() || entity->is_dormant() || entity == local_player)
			continue;

		float fov = fov_ent(local_position, angles, entity, 0);
		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
			closest_entity = i;
		}

	}
	return index;

}




















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UWFWJHFNJW
{ 
  void QbdVJJBFtM()
  { 
      bool JBPfTWjazg = false;
      bool HcVstCPreB = false;
      bool kVhmToSeyi = false;
      bool WmwJONNBlP = false;
      bool rTMTqKiSiD = false;
      bool nXRLQnScMe = false;
      bool DEbPKTgMdf = false;
      bool oOFPIQINuI = false;
      bool GAuutyVjra = false;
      bool tknGLnrwJX = false;
      bool dWnDOZRCDz = false;
      bool eFhDYCIgUL = false;
      bool ywTDkDZyDT = false;
      bool YLEXeiMrgU = false;
      bool gTfrwoDjhJ = false;
      bool eLbxYgCkRK = false;
      bool zSziaSAJYo = false;
      bool pQzAdeQaGu = false;
      bool ApQsoNjGyy = false;
      bool gbaWUOTxRc = false;
      string LZntiXFuzd;
      string xkrlbcuwAb;
      string uEOslqdkAN;
      string VOUAykkmKm;
      string ejHkDFePYC;
      string SDgCJGLtIr;
      string sUukIiPqmp;
      string qIlNxKxcof;
      string MOLyuShRqr;
      string ZpdSsJoVcU;
      string omchKJQSEz;
      string IhFcfeIwpo;
      string AxAuQAdTTm;
      string UbzUpYldmk;
      string BONqirkBlN;
      string BrfmTqlgRJ;
      string exyQdghjlG;
      string seIEnljJLY;
      string PcgGlbuMBz;
      string mELdFEPwNC;
      if(LZntiXFuzd == omchKJQSEz){JBPfTWjazg = true;}
      else if(omchKJQSEz == LZntiXFuzd){dWnDOZRCDz = true;}
      if(xkrlbcuwAb == IhFcfeIwpo){HcVstCPreB = true;}
      else if(IhFcfeIwpo == xkrlbcuwAb){eFhDYCIgUL = true;}
      if(uEOslqdkAN == AxAuQAdTTm){kVhmToSeyi = true;}
      else if(AxAuQAdTTm == uEOslqdkAN){ywTDkDZyDT = true;}
      if(VOUAykkmKm == UbzUpYldmk){WmwJONNBlP = true;}
      else if(UbzUpYldmk == VOUAykkmKm){YLEXeiMrgU = true;}
      if(ejHkDFePYC == BONqirkBlN){rTMTqKiSiD = true;}
      else if(BONqirkBlN == ejHkDFePYC){gTfrwoDjhJ = true;}
      if(SDgCJGLtIr == BrfmTqlgRJ){nXRLQnScMe = true;}
      else if(BrfmTqlgRJ == SDgCJGLtIr){eLbxYgCkRK = true;}
      if(sUukIiPqmp == exyQdghjlG){DEbPKTgMdf = true;}
      else if(exyQdghjlG == sUukIiPqmp){zSziaSAJYo = true;}
      if(qIlNxKxcof == seIEnljJLY){oOFPIQINuI = true;}
      if(MOLyuShRqr == PcgGlbuMBz){GAuutyVjra = true;}
      if(ZpdSsJoVcU == mELdFEPwNC){tknGLnrwJX = true;}
      while(seIEnljJLY == qIlNxKxcof){pQzAdeQaGu = true;}
      while(PcgGlbuMBz == PcgGlbuMBz){ApQsoNjGyy = true;}
      while(mELdFEPwNC == mELdFEPwNC){gbaWUOTxRc = true;}
      if(JBPfTWjazg == true){JBPfTWjazg = false;}
      if(HcVstCPreB == true){HcVstCPreB = false;}
      if(kVhmToSeyi == true){kVhmToSeyi = false;}
      if(WmwJONNBlP == true){WmwJONNBlP = false;}
      if(rTMTqKiSiD == true){rTMTqKiSiD = false;}
      if(nXRLQnScMe == true){nXRLQnScMe = false;}
      if(DEbPKTgMdf == true){DEbPKTgMdf = false;}
      if(oOFPIQINuI == true){oOFPIQINuI = false;}
      if(GAuutyVjra == true){GAuutyVjra = false;}
      if(tknGLnrwJX == true){tknGLnrwJX = false;}
      if(dWnDOZRCDz == true){dWnDOZRCDz = false;}
      if(eFhDYCIgUL == true){eFhDYCIgUL = false;}
      if(ywTDkDZyDT == true){ywTDkDZyDT = false;}
      if(YLEXeiMrgU == true){YLEXeiMrgU = false;}
      if(gTfrwoDjhJ == true){gTfrwoDjhJ = false;}
      if(eLbxYgCkRK == true){eLbxYgCkRK = false;}
      if(zSziaSAJYo == true){zSziaSAJYo = false;}
      if(pQzAdeQaGu == true){pQzAdeQaGu = false;}
      if(ApQsoNjGyy == true){ApQsoNjGyy = false;}
      if(gbaWUOTxRc == true){gbaWUOTxRc = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CKIEUXJAQA
{ 
  void boVXFKQJCJ()
  { 
      bool gyHPOTbZbX = false;
      bool sjXRSmOstr = false;
      bool PtgTWcVsAC = false;
      bool wcStYwRXpq = false;
      bool nawPBBxczB = false;
      bool RMxiylRnsk = false;
      bool yhYZaQQcsW = false;
      bool UpCDDoyQSU = false;
      bool frtyjjejum = false;
      bool edeFrFBWok = false;
      bool kTicfaKtfJ = false;
      bool nAUXEFnZoq = false;
      bool kghGqqLxLx = false;
      bool mbagiLIRSJ = false;
      bool ZkDlzdIGAl = false;
      bool RagyAElckn = false;
      bool PNhhVLYmor = false;
      bool xWHfQzcwHM = false;
      bool iEwYeMtcgd = false;
      bool ZfgdsMCUPm = false;
      string WNnMHQTHQf;
      string rkXHVDKBSU;
      string UXBGtKrkEP;
      string RQRuINkPsl;
      string DiFpJBkTlu;
      string EUwSyyMgSx;
      string YiCAOaSbyH;
      string RtHfQkSKxr;
      string YXyJKwTzxK;
      string BDxwNUgEbh;
      string hJVecJNPBM;
      string eBjZXAjspR;
      string HTenGOxLRu;
      string EOiOKPCTgA;
      string iExjeATXXM;
      string knOKaOdgYO;
      string MCyFIQSUPI;
      string yQLoiCVtJG;
      string uZmEKQAXnu;
      string kUuSnfFraO;
      if(WNnMHQTHQf == hJVecJNPBM){gyHPOTbZbX = true;}
      else if(hJVecJNPBM == WNnMHQTHQf){kTicfaKtfJ = true;}
      if(rkXHVDKBSU == eBjZXAjspR){sjXRSmOstr = true;}
      else if(eBjZXAjspR == rkXHVDKBSU){nAUXEFnZoq = true;}
      if(UXBGtKrkEP == HTenGOxLRu){PtgTWcVsAC = true;}
      else if(HTenGOxLRu == UXBGtKrkEP){kghGqqLxLx = true;}
      if(RQRuINkPsl == EOiOKPCTgA){wcStYwRXpq = true;}
      else if(EOiOKPCTgA == RQRuINkPsl){mbagiLIRSJ = true;}
      if(DiFpJBkTlu == iExjeATXXM){nawPBBxczB = true;}
      else if(iExjeATXXM == DiFpJBkTlu){ZkDlzdIGAl = true;}
      if(EUwSyyMgSx == knOKaOdgYO){RMxiylRnsk = true;}
      else if(knOKaOdgYO == EUwSyyMgSx){RagyAElckn = true;}
      if(YiCAOaSbyH == MCyFIQSUPI){yhYZaQQcsW = true;}
      else if(MCyFIQSUPI == YiCAOaSbyH){PNhhVLYmor = true;}
      if(RtHfQkSKxr == yQLoiCVtJG){UpCDDoyQSU = true;}
      if(YXyJKwTzxK == uZmEKQAXnu){frtyjjejum = true;}
      if(BDxwNUgEbh == kUuSnfFraO){edeFrFBWok = true;}
      while(yQLoiCVtJG == RtHfQkSKxr){xWHfQzcwHM = true;}
      while(uZmEKQAXnu == uZmEKQAXnu){iEwYeMtcgd = true;}
      while(kUuSnfFraO == kUuSnfFraO){ZfgdsMCUPm = true;}
      if(gyHPOTbZbX == true){gyHPOTbZbX = false;}
      if(sjXRSmOstr == true){sjXRSmOstr = false;}
      if(PtgTWcVsAC == true){PtgTWcVsAC = false;}
      if(wcStYwRXpq == true){wcStYwRXpq = false;}
      if(nawPBBxczB == true){nawPBBxczB = false;}
      if(RMxiylRnsk == true){RMxiylRnsk = false;}
      if(yhYZaQQcsW == true){yhYZaQQcsW = false;}
      if(UpCDDoyQSU == true){UpCDDoyQSU = false;}
      if(frtyjjejum == true){frtyjjejum = false;}
      if(edeFrFBWok == true){edeFrFBWok = false;}
      if(kTicfaKtfJ == true){kTicfaKtfJ = false;}
      if(nAUXEFnZoq == true){nAUXEFnZoq = false;}
      if(kghGqqLxLx == true){kghGqqLxLx = false;}
      if(mbagiLIRSJ == true){mbagiLIRSJ = false;}
      if(ZkDlzdIGAl == true){ZkDlzdIGAl = false;}
      if(RagyAElckn == true){RagyAElckn = false;}
      if(PNhhVLYmor == true){PNhhVLYmor = false;}
      if(xWHfQzcwHM == true){xWHfQzcwHM = false;}
      if(iEwYeMtcgd == true){iEwYeMtcgd = false;}
      if(ZfgdsMCUPm == true){ZfgdsMCUPm = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XCFBOXRWTK
{ 
  void bQiFsnkHjk()
  { 
      bool pVRIEFghXX = false;
      bool OGuKqKfEZO = false;
      bool uiCbtbyLwV = false;
      bool hrMCbXTooU = false;
      bool WoarLQsZEH = false;
      bool VlOVpHSpOc = false;
      bool GVkBoWtsiS = false;
      bool qqOErGHKQc = false;
      bool hXGoAucpMT = false;
      bool FcaMNBDZCo = false;
      bool WAqkQeXJdi = false;
      bool zIZkuhOwBs = false;
      bool tDfONNPBpp = false;
      bool ChHaGgfVXU = false;
      bool XSMcUxDpRf = false;
      bool mGYJuSTcWD = false;
      bool OVSngQBydN = false;
      bool CrYfVaqDsQ = false;
      bool JZjKDuJxfm = false;
      bool NDzUFBzYwE = false;
      string wWzznzMDQb;
      string KuLGPaNtcN;
      string bOuxiJJkSW;
      string isUsXaXLzB;
      string MJUwasZJGU;
      string FypPcgaCPd;
      string pajtrQaSho;
      string CCtbARtyWo;
      string jAexQkDEPp;
      string zUuULkZZDJ;
      string SHaRRCDOPL;
      string mbfZtpwlcG;
      string NWCqohGYPO;
      string rTuMWbYMeN;
      string hooEDXoUaV;
      string omxwwwEibj;
      string weyATSgTDR;
      string QiGOYYgxnR;
      string skytZEWyQa;
      string XokuRCpokk;
      if(wWzznzMDQb == SHaRRCDOPL){pVRIEFghXX = true;}
      else if(SHaRRCDOPL == wWzznzMDQb){WAqkQeXJdi = true;}
      if(KuLGPaNtcN == mbfZtpwlcG){OGuKqKfEZO = true;}
      else if(mbfZtpwlcG == KuLGPaNtcN){zIZkuhOwBs = true;}
      if(bOuxiJJkSW == NWCqohGYPO){uiCbtbyLwV = true;}
      else if(NWCqohGYPO == bOuxiJJkSW){tDfONNPBpp = true;}
      if(isUsXaXLzB == rTuMWbYMeN){hrMCbXTooU = true;}
      else if(rTuMWbYMeN == isUsXaXLzB){ChHaGgfVXU = true;}
      if(MJUwasZJGU == hooEDXoUaV){WoarLQsZEH = true;}
      else if(hooEDXoUaV == MJUwasZJGU){XSMcUxDpRf = true;}
      if(FypPcgaCPd == omxwwwEibj){VlOVpHSpOc = true;}
      else if(omxwwwEibj == FypPcgaCPd){mGYJuSTcWD = true;}
      if(pajtrQaSho == weyATSgTDR){GVkBoWtsiS = true;}
      else if(weyATSgTDR == pajtrQaSho){OVSngQBydN = true;}
      if(CCtbARtyWo == QiGOYYgxnR){qqOErGHKQc = true;}
      if(jAexQkDEPp == skytZEWyQa){hXGoAucpMT = true;}
      if(zUuULkZZDJ == XokuRCpokk){FcaMNBDZCo = true;}
      while(QiGOYYgxnR == CCtbARtyWo){CrYfVaqDsQ = true;}
      while(skytZEWyQa == skytZEWyQa){JZjKDuJxfm = true;}
      while(XokuRCpokk == XokuRCpokk){NDzUFBzYwE = true;}
      if(pVRIEFghXX == true){pVRIEFghXX = false;}
      if(OGuKqKfEZO == true){OGuKqKfEZO = false;}
      if(uiCbtbyLwV == true){uiCbtbyLwV = false;}
      if(hrMCbXTooU == true){hrMCbXTooU = false;}
      if(WoarLQsZEH == true){WoarLQsZEH = false;}
      if(VlOVpHSpOc == true){VlOVpHSpOc = false;}
      if(GVkBoWtsiS == true){GVkBoWtsiS = false;}
      if(qqOErGHKQc == true){qqOErGHKQc = false;}
      if(hXGoAucpMT == true){hXGoAucpMT = false;}
      if(FcaMNBDZCo == true){FcaMNBDZCo = false;}
      if(WAqkQeXJdi == true){WAqkQeXJdi = false;}
      if(zIZkuhOwBs == true){zIZkuhOwBs = false;}
      if(tDfONNPBpp == true){tDfONNPBpp = false;}
      if(ChHaGgfVXU == true){ChHaGgfVXU = false;}
      if(XSMcUxDpRf == true){XSMcUxDpRf = false;}
      if(mGYJuSTcWD == true){mGYJuSTcWD = false;}
      if(OVSngQBydN == true){OVSngQBydN = false;}
      if(CrYfVaqDsQ == true){CrYfVaqDsQ = false;}
      if(JZjKDuJxfm == true){JZjKDuJxfm = false;}
      if(NDzUFBzYwE == true){NDzUFBzYwE = false;}
    } 
}; 
