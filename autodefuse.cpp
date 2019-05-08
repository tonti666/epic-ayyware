#include "autodefuse.h"
void defususmaximus(CUserCmd* cmd) {
	if (!options::menu.MiscTab.AutoDefuse.GetState())
		return;
	IClientEntity* localplayer = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!localplayer || !localplayer->IsAlive())
		return;
	if (localplayer->team() != 3)
		return;
	CCSBomb* bomb = nullptr;
	for (int i = 1; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
	{
		IClientEntity* entity = interfaces::ent_list->get_client_entity(i);
		if (!entity)
			continue;
		if (entity->GetClientClass()->m_ClassID == 108)
		{
			bomb = (CCSBomb*)entity;
			break;
		}
		if (!bomb || bomb->IsBombDefused())
			return;
		float flBlow = bomb->GetC4BlowTime();
		auto local = hackManager.pLocal();
		float lifetime = flBlow - (interfaces::globals->interval_per_tick * local->GetTickBase());
		if (localplayer->HasDefuser() && lifetime > 5.5f)
			return;
		if (!localplayer->HasDefuser() && lifetime > 10.5f)
			return;
		if (bomb->IsBombDefused())
			return;
		float distance = localplayer->m_VecORIGIN().DistTo(bomb->GetAbsOrigin());
		if (distance <= 75.0f)
			cmd->buttons |= IN_USE;
	}
}
































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ONUABGNSEX
{ 
  void lkNOJGCmLR()
  { 
      bool NKzUDtIUFA = false;
      bool FqYwiQYQUA = false;
      bool fyywIpyprN = false;
      bool DobXNgPCfj = false;
      bool xhadWeERBA = false;
      bool gfwHhPLCLM = false;
      bool wScrxPPAiC = false;
      bool AWkjCxWUZd = false;
      bool lfoqgWsTox = false;
      bool UNpSmeEifG = false;
      bool PDhyzKuLFj = false;
      bool spchZQdkKZ = false;
      bool WuLlgmfSxW = false;
      bool smoSOixprW = false;
      bool bgOEEejIrV = false;
      bool BTYFjTrwRL = false;
      bool dcOhqFBJTq = false;
      bool SmsmyogGhC = false;
      bool CXZaVZOiht = false;
      bool pBYfszSfNy = false;
      string KTbOSRIBad;
      string xIkzRSojEj;
      string iZhjfObbzE;
      string ouhWzQoShT;
      string oWHhDIotti;
      string nUqfJjlqiD;
      string ogCVnCZNzA;
      string cVBktLSzgW;
      string ZhgQPTDCNP;
      string eyiddHUksA;
      string ibtsFSSGlx;
      string RlJMdHAkGU;
      string yWzjUelXGl;
      string phGgnymmKx;
      string BxQPysnWAW;
      string HidYBLbKXZ;
      string fuAzHiecXe;
      string TMaJSAGjlb;
      string qOWieGDgeb;
      string WcOjQFxQIO;
      if(KTbOSRIBad == ibtsFSSGlx){NKzUDtIUFA = true;}
      else if(ibtsFSSGlx == KTbOSRIBad){PDhyzKuLFj = true;}
      if(xIkzRSojEj == RlJMdHAkGU){FqYwiQYQUA = true;}
      else if(RlJMdHAkGU == xIkzRSojEj){spchZQdkKZ = true;}
      if(iZhjfObbzE == yWzjUelXGl){fyywIpyprN = true;}
      else if(yWzjUelXGl == iZhjfObbzE){WuLlgmfSxW = true;}
      if(ouhWzQoShT == phGgnymmKx){DobXNgPCfj = true;}
      else if(phGgnymmKx == ouhWzQoShT){smoSOixprW = true;}
      if(oWHhDIotti == BxQPysnWAW){xhadWeERBA = true;}
      else if(BxQPysnWAW == oWHhDIotti){bgOEEejIrV = true;}
      if(nUqfJjlqiD == HidYBLbKXZ){gfwHhPLCLM = true;}
      else if(HidYBLbKXZ == nUqfJjlqiD){BTYFjTrwRL = true;}
      if(ogCVnCZNzA == fuAzHiecXe){wScrxPPAiC = true;}
      else if(fuAzHiecXe == ogCVnCZNzA){dcOhqFBJTq = true;}
      if(cVBktLSzgW == TMaJSAGjlb){AWkjCxWUZd = true;}
      if(ZhgQPTDCNP == qOWieGDgeb){lfoqgWsTox = true;}
      if(eyiddHUksA == WcOjQFxQIO){UNpSmeEifG = true;}
      while(TMaJSAGjlb == cVBktLSzgW){SmsmyogGhC = true;}
      while(qOWieGDgeb == qOWieGDgeb){CXZaVZOiht = true;}
      while(WcOjQFxQIO == WcOjQFxQIO){pBYfszSfNy = true;}
      if(NKzUDtIUFA == true){NKzUDtIUFA = false;}
      if(FqYwiQYQUA == true){FqYwiQYQUA = false;}
      if(fyywIpyprN == true){fyywIpyprN = false;}
      if(DobXNgPCfj == true){DobXNgPCfj = false;}
      if(xhadWeERBA == true){xhadWeERBA = false;}
      if(gfwHhPLCLM == true){gfwHhPLCLM = false;}
      if(wScrxPPAiC == true){wScrxPPAiC = false;}
      if(AWkjCxWUZd == true){AWkjCxWUZd = false;}
      if(lfoqgWsTox == true){lfoqgWsTox = false;}
      if(UNpSmeEifG == true){UNpSmeEifG = false;}
      if(PDhyzKuLFj == true){PDhyzKuLFj = false;}
      if(spchZQdkKZ == true){spchZQdkKZ = false;}
      if(WuLlgmfSxW == true){WuLlgmfSxW = false;}
      if(smoSOixprW == true){smoSOixprW = false;}
      if(bgOEEejIrV == true){bgOEEejIrV = false;}
      if(BTYFjTrwRL == true){BTYFjTrwRL = false;}
      if(dcOhqFBJTq == true){dcOhqFBJTq = false;}
      if(SmsmyogGhC == true){SmsmyogGhC = false;}
      if(CXZaVZOiht == true){CXZaVZOiht = false;}
      if(pBYfszSfNy == true){pBYfszSfNy = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LSLQEDHEJA
{ 
  void ElbbjtPFxP()
  { 
      bool hiVHdzJbgR = false;
      bool CJZaDIIDVR = false;
      bool DSGdGRBOVb = false;
      bool QjKRqscToH = false;
      bool LWdPFcuXrh = false;
      bool gNrpdPbqBL = false;
      bool yuGFBRzIiJ = false;
      bool enOOFgnqYd = false;
      bool WdVmuCqUmb = false;
      bool QlRcIQQYNm = false;
      bool dlohDrzckx = false;
      bool akSXoUYSJz = false;
      bool adUGssVkFo = false;
      bool POckzjnnjC = false;
      bool DNjpAeStmn = false;
      bool qyOOMzRhoK = false;
      bool VNWugMIhXF = false;
      bool fkhGuMteGN = false;
      bool ZbxRxRzNYA = false;
      bool SGyYgopnIn = false;
      string tpFFuRzUCK;
      string uoYYTKCMXt;
      string kMIhLfdMef;
      string jhUoLgQuZn;
      string msWMgLZXZY;
      string UzorShhWtP;
      string nmpWZMTiNl;
      string sHmjHqouBA;
      string FFmFLkrcVc;
      string zsAKzADAmT;
      string qpKmeNMFka;
      string rmVtJizjKo;
      string TqNjlzGcjH;
      string noIgIPuzay;
      string YmrtkMXGBQ;
      string ASoxzAcZAe;
      string HiVcmDdnks;
      string mAJMQgySzZ;
      string bzaSHUzLhH;
      string mVyupoQOHh;
      if(tpFFuRzUCK == qpKmeNMFka){hiVHdzJbgR = true;}
      else if(qpKmeNMFka == tpFFuRzUCK){dlohDrzckx = true;}
      if(uoYYTKCMXt == rmVtJizjKo){CJZaDIIDVR = true;}
      else if(rmVtJizjKo == uoYYTKCMXt){akSXoUYSJz = true;}
      if(kMIhLfdMef == TqNjlzGcjH){DSGdGRBOVb = true;}
      else if(TqNjlzGcjH == kMIhLfdMef){adUGssVkFo = true;}
      if(jhUoLgQuZn == noIgIPuzay){QjKRqscToH = true;}
      else if(noIgIPuzay == jhUoLgQuZn){POckzjnnjC = true;}
      if(msWMgLZXZY == YmrtkMXGBQ){LWdPFcuXrh = true;}
      else if(YmrtkMXGBQ == msWMgLZXZY){DNjpAeStmn = true;}
      if(UzorShhWtP == ASoxzAcZAe){gNrpdPbqBL = true;}
      else if(ASoxzAcZAe == UzorShhWtP){qyOOMzRhoK = true;}
      if(nmpWZMTiNl == HiVcmDdnks){yuGFBRzIiJ = true;}
      else if(HiVcmDdnks == nmpWZMTiNl){VNWugMIhXF = true;}
      if(sHmjHqouBA == mAJMQgySzZ){enOOFgnqYd = true;}
      if(FFmFLkrcVc == bzaSHUzLhH){WdVmuCqUmb = true;}
      if(zsAKzADAmT == mVyupoQOHh){QlRcIQQYNm = true;}
      while(mAJMQgySzZ == sHmjHqouBA){fkhGuMteGN = true;}
      while(bzaSHUzLhH == bzaSHUzLhH){ZbxRxRzNYA = true;}
      while(mVyupoQOHh == mVyupoQOHh){SGyYgopnIn = true;}
      if(hiVHdzJbgR == true){hiVHdzJbgR = false;}
      if(CJZaDIIDVR == true){CJZaDIIDVR = false;}
      if(DSGdGRBOVb == true){DSGdGRBOVb = false;}
      if(QjKRqscToH == true){QjKRqscToH = false;}
      if(LWdPFcuXrh == true){LWdPFcuXrh = false;}
      if(gNrpdPbqBL == true){gNrpdPbqBL = false;}
      if(yuGFBRzIiJ == true){yuGFBRzIiJ = false;}
      if(enOOFgnqYd == true){enOOFgnqYd = false;}
      if(WdVmuCqUmb == true){WdVmuCqUmb = false;}
      if(QlRcIQQYNm == true){QlRcIQQYNm = false;}
      if(dlohDrzckx == true){dlohDrzckx = false;}
      if(akSXoUYSJz == true){akSXoUYSJz = false;}
      if(adUGssVkFo == true){adUGssVkFo = false;}
      if(POckzjnnjC == true){POckzjnnjC = false;}
      if(DNjpAeStmn == true){DNjpAeStmn = false;}
      if(qyOOMzRhoK == true){qyOOMzRhoK = false;}
      if(VNWugMIhXF == true){VNWugMIhXF = false;}
      if(fkhGuMteGN == true){fkhGuMteGN = false;}
      if(ZbxRxRzNYA == true){ZbxRxRzNYA = false;}
      if(SGyYgopnIn == true){SGyYgopnIn = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FCLTLGETAU
{ 
  void OWmIVPkcix()
  { 
      bool CzqpChXeTL = false;
      bool LREoomYKya = false;
      bool LgzmSKduIA = false;
      bool sDLyZTObNY = false;
      bool JiYtrZIjGP = false;
      bool VEDXXVzDWq = false;
      bool EbVSSHGGPU = false;
      bool XxwZQcHFur = false;
      bool BsoseozmaE = false;
      bool TLFVTgMGaj = false;
      bool TCTkqXHdGt = false;
      bool NsIdpKlcmd = false;
      bool ISeLBMraFi = false;
      bool QBeLYyYEBG = false;
      bool gUgfpZCbeJ = false;
      bool ujTwbBWIje = false;
      bool fgYXdMOwEr = false;
      bool khafwmlzUa = false;
      bool JQuwnghhjX = false;
      bool aQTWaAwIBc = false;
      string LTgDZdBFgl;
      string LlznHRnZhK;
      string thgKsgetau;
      string HIWWNuHFZu;
      string mnhbTsUDhp;
      string RKCTKclWKr;
      string zHdkeCPbaM;
      string JiAlWFAFqz;
      string MbMjdwFCLk;
      string IktgwfJwxQ;
      string drbUghKPwE;
      string bgfLfWttGh;
      string xMQpDYoSoq;
      string wDHZUTzqDw;
      string loGGIbFhBZ;
      string kOpPTRblRH;
      string sqaEIYYFqL;
      string XDqKwHFJhb;
      string SGRPGhNrFD;
      string rNZuCuOCyi;
      if(LTgDZdBFgl == drbUghKPwE){CzqpChXeTL = true;}
      else if(drbUghKPwE == LTgDZdBFgl){TCTkqXHdGt = true;}
      if(LlznHRnZhK == bgfLfWttGh){LREoomYKya = true;}
      else if(bgfLfWttGh == LlznHRnZhK){NsIdpKlcmd = true;}
      if(thgKsgetau == xMQpDYoSoq){LgzmSKduIA = true;}
      else if(xMQpDYoSoq == thgKsgetau){ISeLBMraFi = true;}
      if(HIWWNuHFZu == wDHZUTzqDw){sDLyZTObNY = true;}
      else if(wDHZUTzqDw == HIWWNuHFZu){QBeLYyYEBG = true;}
      if(mnhbTsUDhp == loGGIbFhBZ){JiYtrZIjGP = true;}
      else if(loGGIbFhBZ == mnhbTsUDhp){gUgfpZCbeJ = true;}
      if(RKCTKclWKr == kOpPTRblRH){VEDXXVzDWq = true;}
      else if(kOpPTRblRH == RKCTKclWKr){ujTwbBWIje = true;}
      if(zHdkeCPbaM == sqaEIYYFqL){EbVSSHGGPU = true;}
      else if(sqaEIYYFqL == zHdkeCPbaM){fgYXdMOwEr = true;}
      if(JiAlWFAFqz == XDqKwHFJhb){XxwZQcHFur = true;}
      if(MbMjdwFCLk == SGRPGhNrFD){BsoseozmaE = true;}
      if(IktgwfJwxQ == rNZuCuOCyi){TLFVTgMGaj = true;}
      while(XDqKwHFJhb == JiAlWFAFqz){khafwmlzUa = true;}
      while(SGRPGhNrFD == SGRPGhNrFD){JQuwnghhjX = true;}
      while(rNZuCuOCyi == rNZuCuOCyi){aQTWaAwIBc = true;}
      if(CzqpChXeTL == true){CzqpChXeTL = false;}
      if(LREoomYKya == true){LREoomYKya = false;}
      if(LgzmSKduIA == true){LgzmSKduIA = false;}
      if(sDLyZTObNY == true){sDLyZTObNY = false;}
      if(JiYtrZIjGP == true){JiYtrZIjGP = false;}
      if(VEDXXVzDWq == true){VEDXXVzDWq = false;}
      if(EbVSSHGGPU == true){EbVSSHGGPU = false;}
      if(XxwZQcHFur == true){XxwZQcHFur = false;}
      if(BsoseozmaE == true){BsoseozmaE = false;}
      if(TLFVTgMGaj == true){TLFVTgMGaj = false;}
      if(TCTkqXHdGt == true){TCTkqXHdGt = false;}
      if(NsIdpKlcmd == true){NsIdpKlcmd = false;}
      if(ISeLBMraFi == true){ISeLBMraFi = false;}
      if(QBeLYyYEBG == true){QBeLYyYEBG = false;}
      if(gUgfpZCbeJ == true){gUgfpZCbeJ = false;}
      if(ujTwbBWIje == true){ujTwbBWIje = false;}
      if(fgYXdMOwEr == true){fgYXdMOwEr = false;}
      if(khafwmlzUa == true){khafwmlzUa = false;}
      if(JQuwnghhjX == true){JQuwnghhjX = false;}
      if(aQTWaAwIBc == true){aQTWaAwIBc = false;}
    } 
}; 
