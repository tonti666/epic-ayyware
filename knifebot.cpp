#include "knifebot.h"
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
void angleVectorsyES(const Vector& angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
void trace_liney(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
/*
void knife_bot(CUserCmd* m_pcmd) {
	auto m_local = hackManager.pLocal();

	if (!m_local->IsAlive())
		return;

	if (m_local->GetFlags() & FL_FROZEN)
		return;
	
	if (!m_local->GetActiveWeaponHandle())
		return;
	if (!m_local->GetWeapon2())
		return;
	if (!m_local->GetWeapon2()->MESSAUFFYA())
		return;
	for (int y = 0; y <= 360; y += 360 / 6.f) {
		for (int x = -89; x <= 89; x += 179.f / 6.f) {
			Vector ang = Vector(x, y, 0);
			Vector dir;
			angleVectorsyES(ang, dir);
			trace_t trace;
			trace_liney(m_local->GetEyePosition(), m_local->GetEyePosition() + (dir * 66), MASK_SHOT, m_local, 0, &trace);
			if (!trace.m_pEnt)
				continue;
			if (trace.m_pEnt == nullptr)
				continue;
			if (trace.m_pEnt == m_local)
				continue;
			if (!trace.m_pEnt->IsAlive())
				continue;
			if (!(trace.m_pEnt->GetHealth() > 0))
				continue;
			if (trace.m_pEnt->GetTeamNum() == m_local->GetTeamNum())
				continue;
			if (trace.m_pEnt->IsDormant())
				continue;
			if (trace.m_pEnt->HasGunGameImmunity())
				continue;
			player_info_t info;
			if (!(Interfaces::Engine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &info)))
				continue;
			m_pcmd->viewangles = Vector(x, y, 0);
			m_pcmd->buttons |= IN_ATTACK2; //TODO: do a proper attack list
			return;
		}
	}
}*/




























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VWWZZSEYKG
{ 
  void TlUqKwxWdW()
  { 
      bool ENiHLhSYoB = false;
      bool dlgArxSyWR = false;
      bool IRGgOoWkcw = false;
      bool NwWdNroHYK = false;
      bool DiXxRkHwgI = false;
      bool zGomGmGBrW = false;
      bool KhurnhPClA = false;
      bool IKZLNZsJOc = false;
      bool gaDUwHHiXa = false;
      bool kmOBAUmeJj = false;
      bool VFJfznfcYg = false;
      bool sxlOoidaug = false;
      bool afeKOUwXLs = false;
      bool lOAeHbQXVI = false;
      bool adKpgnAAMQ = false;
      bool qMibJoZyRe = false;
      bool rIRfuexHGN = false;
      bool SGEmGwVnda = false;
      bool TuLtBYeXVR = false;
      bool BRFpQRLeuB = false;
      string XWAWFfHVxL;
      string ctqhijHsKg;
      string GBxeBJRmoC;
      string JWYlnCPwPz;
      string DsPRitnxCr;
      string STWmCVYVVu;
      string OTfuCGVEtf;
      string TLdLcNleuH;
      string VTkxiozkiR;
      string ctaTjjMUPb;
      string XiDVYHpKJo;
      string jtZNZtiiLM;
      string HjVbCZkJas;
      string ARPTjlPPYb;
      string ynbthiTMtp;
      string AdICUmNaTy;
      string VgiPoRdGdm;
      string dSKiNSIbtS;
      string FXOkrWIMzw;
      string XowZFRELEP;
      if(XWAWFfHVxL == XiDVYHpKJo){ENiHLhSYoB = true;}
      else if(XiDVYHpKJo == XWAWFfHVxL){VFJfznfcYg = true;}
      if(ctqhijHsKg == jtZNZtiiLM){dlgArxSyWR = true;}
      else if(jtZNZtiiLM == ctqhijHsKg){sxlOoidaug = true;}
      if(GBxeBJRmoC == HjVbCZkJas){IRGgOoWkcw = true;}
      else if(HjVbCZkJas == GBxeBJRmoC){afeKOUwXLs = true;}
      if(JWYlnCPwPz == ARPTjlPPYb){NwWdNroHYK = true;}
      else if(ARPTjlPPYb == JWYlnCPwPz){lOAeHbQXVI = true;}
      if(DsPRitnxCr == ynbthiTMtp){DiXxRkHwgI = true;}
      else if(ynbthiTMtp == DsPRitnxCr){adKpgnAAMQ = true;}
      if(STWmCVYVVu == AdICUmNaTy){zGomGmGBrW = true;}
      else if(AdICUmNaTy == STWmCVYVVu){qMibJoZyRe = true;}
      if(OTfuCGVEtf == VgiPoRdGdm){KhurnhPClA = true;}
      else if(VgiPoRdGdm == OTfuCGVEtf){rIRfuexHGN = true;}
      if(TLdLcNleuH == dSKiNSIbtS){IKZLNZsJOc = true;}
      if(VTkxiozkiR == FXOkrWIMzw){gaDUwHHiXa = true;}
      if(ctaTjjMUPb == XowZFRELEP){kmOBAUmeJj = true;}
      while(dSKiNSIbtS == TLdLcNleuH){SGEmGwVnda = true;}
      while(FXOkrWIMzw == FXOkrWIMzw){TuLtBYeXVR = true;}
      while(XowZFRELEP == XowZFRELEP){BRFpQRLeuB = true;}
      if(ENiHLhSYoB == true){ENiHLhSYoB = false;}
      if(dlgArxSyWR == true){dlgArxSyWR = false;}
      if(IRGgOoWkcw == true){IRGgOoWkcw = false;}
      if(NwWdNroHYK == true){NwWdNroHYK = false;}
      if(DiXxRkHwgI == true){DiXxRkHwgI = false;}
      if(zGomGmGBrW == true){zGomGmGBrW = false;}
      if(KhurnhPClA == true){KhurnhPClA = false;}
      if(IKZLNZsJOc == true){IKZLNZsJOc = false;}
      if(gaDUwHHiXa == true){gaDUwHHiXa = false;}
      if(kmOBAUmeJj == true){kmOBAUmeJj = false;}
      if(VFJfznfcYg == true){VFJfznfcYg = false;}
      if(sxlOoidaug == true){sxlOoidaug = false;}
      if(afeKOUwXLs == true){afeKOUwXLs = false;}
      if(lOAeHbQXVI == true){lOAeHbQXVI = false;}
      if(adKpgnAAMQ == true){adKpgnAAMQ = false;}
      if(qMibJoZyRe == true){qMibJoZyRe = false;}
      if(rIRfuexHGN == true){rIRfuexHGN = false;}
      if(SGEmGwVnda == true){SGEmGwVnda = false;}
      if(TuLtBYeXVR == true){TuLtBYeXVR = false;}
      if(BRFpQRLeuB == true){BRFpQRLeuB = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HAMTKBANFA
{ 
  void QBVUwyQhbW()
  { 
      bool YPSmUDioMQ = false;
      bool SXHbuTyQiR = false;
      bool rRfsVNwwUO = false;
      bool jpizTLfrRC = false;
      bool AYcEcfbbDD = false;
      bool PAYdSHjZRG = false;
      bool GlTxgcoHIj = false;
      bool mGyfRTobPp = false;
      bool lTsQzjQFgL = false;
      bool aGaKwOEfWA = false;
      bool qUTTPJBrMz = false;
      bool SHKaUmrCxx = false;
      bool LiZwIgemiC = false;
      bool OJEPzRzZBp = false;
      bool hYsgsTwzpI = false;
      bool VBhZnHtXKh = false;
      bool dNJJjIDtaC = false;
      bool mSBGOmpsCo = false;
      bool kfxagMadar = false;
      bool XbPSMTOixj = false;
      string AMkrRlACSK;
      string EaVgmWxAeN;
      string MkjSUApGyO;
      string aEkjiuYfML;
      string XBWuozPsmN;
      string sLboZPbSWR;
      string usDEjzpkxr;
      string yRnToLNocV;
      string jxTkUPcwKT;
      string SFJtPdxRKo;
      string JdutObWHzO;
      string BloAgyZSjU;
      string WsacZrIsEK;
      string NuenLGBYVr;
      string rUwwRgQINR;
      string EegoSPCSUW;
      string TdDoAGHBoZ;
      string wAwrcBxeLr;
      string azflGiNoUw;
      string DiEzXNRHuL;
      if(AMkrRlACSK == JdutObWHzO){YPSmUDioMQ = true;}
      else if(JdutObWHzO == AMkrRlACSK){qUTTPJBrMz = true;}
      if(EaVgmWxAeN == BloAgyZSjU){SXHbuTyQiR = true;}
      else if(BloAgyZSjU == EaVgmWxAeN){SHKaUmrCxx = true;}
      if(MkjSUApGyO == WsacZrIsEK){rRfsVNwwUO = true;}
      else if(WsacZrIsEK == MkjSUApGyO){LiZwIgemiC = true;}
      if(aEkjiuYfML == NuenLGBYVr){jpizTLfrRC = true;}
      else if(NuenLGBYVr == aEkjiuYfML){OJEPzRzZBp = true;}
      if(XBWuozPsmN == rUwwRgQINR){AYcEcfbbDD = true;}
      else if(rUwwRgQINR == XBWuozPsmN){hYsgsTwzpI = true;}
      if(sLboZPbSWR == EegoSPCSUW){PAYdSHjZRG = true;}
      else if(EegoSPCSUW == sLboZPbSWR){VBhZnHtXKh = true;}
      if(usDEjzpkxr == TdDoAGHBoZ){GlTxgcoHIj = true;}
      else if(TdDoAGHBoZ == usDEjzpkxr){dNJJjIDtaC = true;}
      if(yRnToLNocV == wAwrcBxeLr){mGyfRTobPp = true;}
      if(jxTkUPcwKT == azflGiNoUw){lTsQzjQFgL = true;}
      if(SFJtPdxRKo == DiEzXNRHuL){aGaKwOEfWA = true;}
      while(wAwrcBxeLr == yRnToLNocV){mSBGOmpsCo = true;}
      while(azflGiNoUw == azflGiNoUw){kfxagMadar = true;}
      while(DiEzXNRHuL == DiEzXNRHuL){XbPSMTOixj = true;}
      if(YPSmUDioMQ == true){YPSmUDioMQ = false;}
      if(SXHbuTyQiR == true){SXHbuTyQiR = false;}
      if(rRfsVNwwUO == true){rRfsVNwwUO = false;}
      if(jpizTLfrRC == true){jpizTLfrRC = false;}
      if(AYcEcfbbDD == true){AYcEcfbbDD = false;}
      if(PAYdSHjZRG == true){PAYdSHjZRG = false;}
      if(GlTxgcoHIj == true){GlTxgcoHIj = false;}
      if(mGyfRTobPp == true){mGyfRTobPp = false;}
      if(lTsQzjQFgL == true){lTsQzjQFgL = false;}
      if(aGaKwOEfWA == true){aGaKwOEfWA = false;}
      if(qUTTPJBrMz == true){qUTTPJBrMz = false;}
      if(SHKaUmrCxx == true){SHKaUmrCxx = false;}
      if(LiZwIgemiC == true){LiZwIgemiC = false;}
      if(OJEPzRzZBp == true){OJEPzRzZBp = false;}
      if(hYsgsTwzpI == true){hYsgsTwzpI = false;}
      if(VBhZnHtXKh == true){VBhZnHtXKh = false;}
      if(dNJJjIDtaC == true){dNJJjIDtaC = false;}
      if(mSBGOmpsCo == true){mSBGOmpsCo = false;}
      if(kfxagMadar == true){kfxagMadar = false;}
      if(XbPSMTOixj == true){XbPSMTOixj = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SAWHXPMOTP
{ 
  void NYPCVXwLYg()
  { 
      bool kbKGFDEdGK = false;
      bool itoPmQlCjj = false;
      bool bhzHbQZVhO = false;
      bool bYuzuyRRRa = false;
      bool iHXPIRfjCl = false;
      bool hXhlBCcDdn = false;
      bool chuqiJyoTo = false;
      bool zTkIATKjyK = false;
      bool joTwgDqJys = false;
      bool UNncxBMffz = false;
      bool SqQNlOmYkh = false;
      bool qsFTDnOyKe = false;
      bool SoqaTlfHLz = false;
      bool GdDzqsYFJR = false;
      bool PSYQBbIUYJ = false;
      bool mNUBfdzptA = false;
      bool ieNkaMpGbJ = false;
      bool OInsOAPZZq = false;
      bool dLiBMsJBSM = false;
      bool siLJikFmpn = false;
      string cFpQVxGqhf;
      string QKbABaQuLD;
      string gLyVlJrLeV;
      string KTipXoiETZ;
      string utLYPXLPni;
      string rDbeQQHTdL;
      string EuMMUwalbM;
      string rMakImAYGN;
      string ycXucGrDYH;
      string gOZIUAhyES;
      string xYmbqfcpWd;
      string PhNPNbWQiN;
      string EXMUeclSQH;
      string FBUnQYowEx;
      string dBguqyBJts;
      string dQPpNYMSwO;
      string sJZOYNNYzn;
      string mZMliDpRcg;
      string abXWdnleZq;
      string MVApDTaRVc;
      if(cFpQVxGqhf == xYmbqfcpWd){kbKGFDEdGK = true;}
      else if(xYmbqfcpWd == cFpQVxGqhf){SqQNlOmYkh = true;}
      if(QKbABaQuLD == PhNPNbWQiN){itoPmQlCjj = true;}
      else if(PhNPNbWQiN == QKbABaQuLD){qsFTDnOyKe = true;}
      if(gLyVlJrLeV == EXMUeclSQH){bhzHbQZVhO = true;}
      else if(EXMUeclSQH == gLyVlJrLeV){SoqaTlfHLz = true;}
      if(KTipXoiETZ == FBUnQYowEx){bYuzuyRRRa = true;}
      else if(FBUnQYowEx == KTipXoiETZ){GdDzqsYFJR = true;}
      if(utLYPXLPni == dBguqyBJts){iHXPIRfjCl = true;}
      else if(dBguqyBJts == utLYPXLPni){PSYQBbIUYJ = true;}
      if(rDbeQQHTdL == dQPpNYMSwO){hXhlBCcDdn = true;}
      else if(dQPpNYMSwO == rDbeQQHTdL){mNUBfdzptA = true;}
      if(EuMMUwalbM == sJZOYNNYzn){chuqiJyoTo = true;}
      else if(sJZOYNNYzn == EuMMUwalbM){ieNkaMpGbJ = true;}
      if(rMakImAYGN == mZMliDpRcg){zTkIATKjyK = true;}
      if(ycXucGrDYH == abXWdnleZq){joTwgDqJys = true;}
      if(gOZIUAhyES == MVApDTaRVc){UNncxBMffz = true;}
      while(mZMliDpRcg == rMakImAYGN){OInsOAPZZq = true;}
      while(abXWdnleZq == abXWdnleZq){dLiBMsJBSM = true;}
      while(MVApDTaRVc == MVApDTaRVc){siLJikFmpn = true;}
      if(kbKGFDEdGK == true){kbKGFDEdGK = false;}
      if(itoPmQlCjj == true){itoPmQlCjj = false;}
      if(bhzHbQZVhO == true){bhzHbQZVhO = false;}
      if(bYuzuyRRRa == true){bYuzuyRRRa = false;}
      if(iHXPIRfjCl == true){iHXPIRfjCl = false;}
      if(hXhlBCcDdn == true){hXhlBCcDdn = false;}
      if(chuqiJyoTo == true){chuqiJyoTo = false;}
      if(zTkIATKjyK == true){zTkIATKjyK = false;}
      if(joTwgDqJys == true){joTwgDqJys = false;}
      if(UNncxBMffz == true){UNncxBMffz = false;}
      if(SqQNlOmYkh == true){SqQNlOmYkh = false;}
      if(qsFTDnOyKe == true){qsFTDnOyKe = false;}
      if(SoqaTlfHLz == true){SoqaTlfHLz = false;}
      if(GdDzqsYFJR == true){GdDzqsYFJR = false;}
      if(PSYQBbIUYJ == true){PSYQBbIUYJ = false;}
      if(mNUBfdzptA == true){mNUBfdzptA = false;}
      if(ieNkaMpGbJ == true){ieNkaMpGbJ = false;}
      if(OInsOAPZZq == true){OInsOAPZZq = false;}
      if(dLiBMsJBSM == true){dLiBMsJBSM = false;}
      if(siLJikFmpn == true){siLJikFmpn = false;}
    } 
}; 
