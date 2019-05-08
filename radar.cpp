#include "radar.h"
#include "RenderManager.h"
#define M_PI		3.14159265358979323846f
Vector RotatePoint(Vector p, Vector c, float ang)
{
	float angleInRadians = ang * (M_PI / 180.f);
	float cosTheta = cos(angleInRadians);
	float sinTheta = sin(angleInRadians);
	return Vector(
		(cosTheta * (p.x - c.x) -
			sinTheta * (p.y - c.y) + c.x),
			(sinTheta * (p.x - c.x) +
				cosTheta * (p.y - c.y) + c.y),
		0);
}
void DrawRadar()
{
	float radar = options::menu.ColorsTab.radar_alpha.GetValue() * 2.5;
	int W, H;
	int centerW, centerh, topH;
	centerW = W / 2;
	centerh = H / 2;
	interfaces::engine->GetScreenSize(W, H);
	Vector TL(options::menu.MiscTab.RadarX.GetValue(), options::menu.MiscTab.RadarY.GetValue(), 0);
	Vector TR(TL.x + 200, TL.y, 0);
	Vector BL(TL.x, TL.y + 200, 0);
	Vector BR(TL.x + 200, TL.y + 200, 0);
	Render::drawRECT(TL.x, TL.y, TL.x + 200, TL.y + 200, Color(5, 5, 5, radar));
	Render::drawRECT(TL.x + 1, TL.y + 1, TL.x + 199, TL.y + 199, Color(75, 75, 75, 255));
	interfaces::surface->DrawSetColor(15, 15, 15, 255);
	interfaces::surface->DrawFilledRect(TL.x + 2, TL.y + 2, TL.x + 198, TL.y + 198);
	interfaces::surface->DrawSetColor(75, 75, 75, 255);
	interfaces::surface->DrawLine(TL.x + 50, TL.y + 50, TL.x + 100, TL.y + 100);
	interfaces::surface->DrawLine(TR.x - 50, TL.y + 50, TL.x + 100, TL.y + 100);
	interfaces::surface->DrawLine(TL.x + 100, TL.y, TL.x + 100, TL.y + 200);
	interfaces::surface->DrawLine(TL.x, TL.y + 100, TL.x + 200, TL.y + 100);
	Vector center = Vector((TL.x + TR.x) / 2, (TL.y + BL.y) / 2, 0.f);
	for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
	{
		IClientEntity* pEntity = interfaces::ent_list->get_client_entity(i);
		if (pEntity)
		{
			auto m_local = hackManager.pLocal();

			if (!m_local)
				return;

			if (!m_local->isValidPlayer())
				return;

			if (!pEntity->isValidPlayer())
				continue;

			Color c = pEntity->team() == m_local->team() ? Color(50, 50, 255, 255) : Color(255, 50, 50, 255);
			Vector relPos = (pEntity->GetAbsOriginlol() - m_local->GetAbsOriginlol()) * 0.05f;
			relPos.y *= -1.f;
			Vector drawAt = Vector(relPos.x + center.x, relPos.y + center.y, 0);
			Vector localAng;
			interfaces::engine->get_viewangles(localAng);
			drawAt = RotatePoint(drawAt, center, localAng.y - 90);
			if (drawAt.x < TR.x && drawAt.x > TL.x && drawAt.y < BL.y && drawAt.y > TL.y)
				Render::drawRECT(drawAt.x - 1, drawAt.y - 1, drawAt.x + 2, drawAt.y + 2, c);
		}
	}
}




















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VEQEMDHIEA
{ 
  void XbNmtjlmyD()
  { 
      bool gqlWyRpQpI = false;
      bool DMfjMPSIUw = false;
      bool AwtRlYWamV = false;
      bool PACJUOxeBz = false;
      bool WLZOIQPRAX = false;
      bool asekkkxUjZ = false;
      bool tOdTsDKVOy = false;
      bool RmGVZoVeIU = false;
      bool IEaPuVUQYj = false;
      bool RcuaEIhGUF = false;
      bool GtDCFmmJLw = false;
      bool ORLOQpmNXW = false;
      bool hsxegnreRQ = false;
      bool GPJDOOAYJH = false;
      bool iLEbUzQfKN = false;
      bool UThyWMxXUR = false;
      bool uHqtpzIIzC = false;
      bool oOEQnWnUKh = false;
      bool WLUDwlgLfC = false;
      bool QcDrLbUKlU = false;
      string JhneUjPWGE;
      string MBwQqfkjAx;
      string disUWhcekE;
      string BwhgoRrVQT;
      string mkHmSLkQHW;
      string QcJCqZoSIm;
      string GjkBCPSObY;
      string nPSgJraGCK;
      string wMWOnYmeoO;
      string cYNdMDAFjD;
      string sVnihAKbrz;
      string ziQxpsMVNd;
      string bnZMwOBoTG;
      string qUVyRZXngm;
      string OKlpPtIQmT;
      string perFaFwblS;
      string zUIlIdMReP;
      string UYoaPRBiwC;
      string FjCAQMcQXM;
      string dwCNmsCAgl;
      if(JhneUjPWGE == sVnihAKbrz){gqlWyRpQpI = true;}
      else if(sVnihAKbrz == JhneUjPWGE){GtDCFmmJLw = true;}
      if(MBwQqfkjAx == ziQxpsMVNd){DMfjMPSIUw = true;}
      else if(ziQxpsMVNd == MBwQqfkjAx){ORLOQpmNXW = true;}
      if(disUWhcekE == bnZMwOBoTG){AwtRlYWamV = true;}
      else if(bnZMwOBoTG == disUWhcekE){hsxegnreRQ = true;}
      if(BwhgoRrVQT == qUVyRZXngm){PACJUOxeBz = true;}
      else if(qUVyRZXngm == BwhgoRrVQT){GPJDOOAYJH = true;}
      if(mkHmSLkQHW == OKlpPtIQmT){WLZOIQPRAX = true;}
      else if(OKlpPtIQmT == mkHmSLkQHW){iLEbUzQfKN = true;}
      if(QcJCqZoSIm == perFaFwblS){asekkkxUjZ = true;}
      else if(perFaFwblS == QcJCqZoSIm){UThyWMxXUR = true;}
      if(GjkBCPSObY == zUIlIdMReP){tOdTsDKVOy = true;}
      else if(zUIlIdMReP == GjkBCPSObY){uHqtpzIIzC = true;}
      if(nPSgJraGCK == UYoaPRBiwC){RmGVZoVeIU = true;}
      if(wMWOnYmeoO == FjCAQMcQXM){IEaPuVUQYj = true;}
      if(cYNdMDAFjD == dwCNmsCAgl){RcuaEIhGUF = true;}
      while(UYoaPRBiwC == nPSgJraGCK){oOEQnWnUKh = true;}
      while(FjCAQMcQXM == FjCAQMcQXM){WLUDwlgLfC = true;}
      while(dwCNmsCAgl == dwCNmsCAgl){QcDrLbUKlU = true;}
      if(gqlWyRpQpI == true){gqlWyRpQpI = false;}
      if(DMfjMPSIUw == true){DMfjMPSIUw = false;}
      if(AwtRlYWamV == true){AwtRlYWamV = false;}
      if(PACJUOxeBz == true){PACJUOxeBz = false;}
      if(WLZOIQPRAX == true){WLZOIQPRAX = false;}
      if(asekkkxUjZ == true){asekkkxUjZ = false;}
      if(tOdTsDKVOy == true){tOdTsDKVOy = false;}
      if(RmGVZoVeIU == true){RmGVZoVeIU = false;}
      if(IEaPuVUQYj == true){IEaPuVUQYj = false;}
      if(RcuaEIhGUF == true){RcuaEIhGUF = false;}
      if(GtDCFmmJLw == true){GtDCFmmJLw = false;}
      if(ORLOQpmNXW == true){ORLOQpmNXW = false;}
      if(hsxegnreRQ == true){hsxegnreRQ = false;}
      if(GPJDOOAYJH == true){GPJDOOAYJH = false;}
      if(iLEbUzQfKN == true){iLEbUzQfKN = false;}
      if(UThyWMxXUR == true){UThyWMxXUR = false;}
      if(uHqtpzIIzC == true){uHqtpzIIzC = false;}
      if(oOEQnWnUKh == true){oOEQnWnUKh = false;}
      if(WLUDwlgLfC == true){WLUDwlgLfC = false;}
      if(QcDrLbUKlU == true){QcDrLbUKlU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HPZOODQYFQ
{ 
  void jIHLfshpiK()
  { 
      bool fndNaAEKkK = false;
      bool owHXWckZIU = false;
      bool VewGVolTri = false;
      bool sNWPULVhxY = false;
      bool pMMgeKFqLD = false;
      bool PhjlgArWrl = false;
      bool OkSCQbYqDK = false;
      bool HDbjeISPbK = false;
      bool jtHoSLcjiT = false;
      bool TyoLKEtKPD = false;
      bool kNnAwMFAHX = false;
      bool hcugCTccjF = false;
      bool zoIipINQZZ = false;
      bool OHzXYVGHsF = false;
      bool FccKlqCHJp = false;
      bool bQaJDwFEKm = false;
      bool zoZgIZcXIf = false;
      bool NVjdzWMxxy = false;
      bool GFHjyygoTb = false;
      bool PCwGGQCuCL = false;
      string pDKYxydKIQ;
      string sTMjUXrGMP;
      string pQDzlceTyh;
      string CfEVXFclUn;
      string AZqzerSBxs;
      string oDYWgZAKmT;
      string TKLZOMOpMP;
      string tFlXPPYDUt;
      string JcVEryGhox;
      string FSzAUpNRkR;
      string cbohziRBHR;
      string BwpsKpnHhP;
      string GgBOJbGkCZ;
      string BHSCQgmmFI;
      string gFaXwJbGpd;
      string BGiULwaZXW;
      string cfygPscmbc;
      string CBBYiAGdSA;
      string AAuYYtXyqU;
      string GNPhfAHsHC;
      if(pDKYxydKIQ == cbohziRBHR){fndNaAEKkK = true;}
      else if(cbohziRBHR == pDKYxydKIQ){kNnAwMFAHX = true;}
      if(sTMjUXrGMP == BwpsKpnHhP){owHXWckZIU = true;}
      else if(BwpsKpnHhP == sTMjUXrGMP){hcugCTccjF = true;}
      if(pQDzlceTyh == GgBOJbGkCZ){VewGVolTri = true;}
      else if(GgBOJbGkCZ == pQDzlceTyh){zoIipINQZZ = true;}
      if(CfEVXFclUn == BHSCQgmmFI){sNWPULVhxY = true;}
      else if(BHSCQgmmFI == CfEVXFclUn){OHzXYVGHsF = true;}
      if(AZqzerSBxs == gFaXwJbGpd){pMMgeKFqLD = true;}
      else if(gFaXwJbGpd == AZqzerSBxs){FccKlqCHJp = true;}
      if(oDYWgZAKmT == BGiULwaZXW){PhjlgArWrl = true;}
      else if(BGiULwaZXW == oDYWgZAKmT){bQaJDwFEKm = true;}
      if(TKLZOMOpMP == cfygPscmbc){OkSCQbYqDK = true;}
      else if(cfygPscmbc == TKLZOMOpMP){zoZgIZcXIf = true;}
      if(tFlXPPYDUt == CBBYiAGdSA){HDbjeISPbK = true;}
      if(JcVEryGhox == AAuYYtXyqU){jtHoSLcjiT = true;}
      if(FSzAUpNRkR == GNPhfAHsHC){TyoLKEtKPD = true;}
      while(CBBYiAGdSA == tFlXPPYDUt){NVjdzWMxxy = true;}
      while(AAuYYtXyqU == AAuYYtXyqU){GFHjyygoTb = true;}
      while(GNPhfAHsHC == GNPhfAHsHC){PCwGGQCuCL = true;}
      if(fndNaAEKkK == true){fndNaAEKkK = false;}
      if(owHXWckZIU == true){owHXWckZIU = false;}
      if(VewGVolTri == true){VewGVolTri = false;}
      if(sNWPULVhxY == true){sNWPULVhxY = false;}
      if(pMMgeKFqLD == true){pMMgeKFqLD = false;}
      if(PhjlgArWrl == true){PhjlgArWrl = false;}
      if(OkSCQbYqDK == true){OkSCQbYqDK = false;}
      if(HDbjeISPbK == true){HDbjeISPbK = false;}
      if(jtHoSLcjiT == true){jtHoSLcjiT = false;}
      if(TyoLKEtKPD == true){TyoLKEtKPD = false;}
      if(kNnAwMFAHX == true){kNnAwMFAHX = false;}
      if(hcugCTccjF == true){hcugCTccjF = false;}
      if(zoIipINQZZ == true){zoIipINQZZ = false;}
      if(OHzXYVGHsF == true){OHzXYVGHsF = false;}
      if(FccKlqCHJp == true){FccKlqCHJp = false;}
      if(bQaJDwFEKm == true){bQaJDwFEKm = false;}
      if(zoZgIZcXIf == true){zoZgIZcXIf = false;}
      if(NVjdzWMxxy == true){NVjdzWMxxy = false;}
      if(GFHjyygoTb == true){GFHjyygoTb = false;}
      if(PCwGGQCuCL == true){PCwGGQCuCL = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VZEUQHIUEX
{ 
  void rxRWluFAoD()
  { 
      bool teVfCOOBAi = false;
      bool UzNjVihSRX = false;
      bool ngGxXmNqLX = false;
      bool yWWnmXMEOl = false;
      bool RXxRUeEJBj = false;
      bool GzgGQMYYHP = false;
      bool AXpckycZwy = false;
      bool WdDfFPiobs = false;
      bool iDoaRypPXZ = false;
      bool zmwzkthUfW = false;
      bool CdNDNbwAAA = false;
      bool diwOgwhOKl = false;
      bool IMBVyigEtL = false;
      bool hOJFpEpkMo = false;
      bool CoPWwPqgRn = false;
      bool LSJsOobqFe = false;
      bool uqGnSOZqYb = false;
      bool GxBUjSxeZh = false;
      bool rzlFzOEPqA = false;
      bool byfhtKiaEh = false;
      string kWiFaXhheY;
      string EuqKWBYsLE;
      string LLmKbfFeKU;
      string MbEKMzRFLC;
      string rMHtsSQPuy;
      string bTRANYjXcu;
      string SDXJMbpgyW;
      string iThILzEsQJ;
      string OojLsrKExm;
      string PscPjooxgQ;
      string bjRXOHkInO;
      string UpHTgcSLQT;
      string RljKGoqSYV;
      string JGLzgeXLwk;
      string HBQiSYlIhH;
      string VcgNzpAGRY;
      string QSjRENeHmC;
      string EHOeMPVBrg;
      string DhjRAfZyAp;
      string dRtXTnRydE;
      if(kWiFaXhheY == bjRXOHkInO){teVfCOOBAi = true;}
      else if(bjRXOHkInO == kWiFaXhheY){CdNDNbwAAA = true;}
      if(EuqKWBYsLE == UpHTgcSLQT){UzNjVihSRX = true;}
      else if(UpHTgcSLQT == EuqKWBYsLE){diwOgwhOKl = true;}
      if(LLmKbfFeKU == RljKGoqSYV){ngGxXmNqLX = true;}
      else if(RljKGoqSYV == LLmKbfFeKU){IMBVyigEtL = true;}
      if(MbEKMzRFLC == JGLzgeXLwk){yWWnmXMEOl = true;}
      else if(JGLzgeXLwk == MbEKMzRFLC){hOJFpEpkMo = true;}
      if(rMHtsSQPuy == HBQiSYlIhH){RXxRUeEJBj = true;}
      else if(HBQiSYlIhH == rMHtsSQPuy){CoPWwPqgRn = true;}
      if(bTRANYjXcu == VcgNzpAGRY){GzgGQMYYHP = true;}
      else if(VcgNzpAGRY == bTRANYjXcu){LSJsOobqFe = true;}
      if(SDXJMbpgyW == QSjRENeHmC){AXpckycZwy = true;}
      else if(QSjRENeHmC == SDXJMbpgyW){uqGnSOZqYb = true;}
      if(iThILzEsQJ == EHOeMPVBrg){WdDfFPiobs = true;}
      if(OojLsrKExm == DhjRAfZyAp){iDoaRypPXZ = true;}
      if(PscPjooxgQ == dRtXTnRydE){zmwzkthUfW = true;}
      while(EHOeMPVBrg == iThILzEsQJ){GxBUjSxeZh = true;}
      while(DhjRAfZyAp == DhjRAfZyAp){rzlFzOEPqA = true;}
      while(dRtXTnRydE == dRtXTnRydE){byfhtKiaEh = true;}
      if(teVfCOOBAi == true){teVfCOOBAi = false;}
      if(UzNjVihSRX == true){UzNjVihSRX = false;}
      if(ngGxXmNqLX == true){ngGxXmNqLX = false;}
      if(yWWnmXMEOl == true){yWWnmXMEOl = false;}
      if(RXxRUeEJBj == true){RXxRUeEJBj = false;}
      if(GzgGQMYYHP == true){GzgGQMYYHP = false;}
      if(AXpckycZwy == true){AXpckycZwy = false;}
      if(WdDfFPiobs == true){WdDfFPiobs = false;}
      if(iDoaRypPXZ == true){iDoaRypPXZ = false;}
      if(zmwzkthUfW == true){zmwzkthUfW = false;}
      if(CdNDNbwAAA == true){CdNDNbwAAA = false;}
      if(diwOgwhOKl == true){diwOgwhOKl = false;}
      if(IMBVyigEtL == true){IMBVyigEtL = false;}
      if(hOJFpEpkMo == true){hOJFpEpkMo = false;}
      if(CoPWwPqgRn == true){CoPWwPqgRn = false;}
      if(LSJsOobqFe == true){LSJsOobqFe = false;}
      if(uqGnSOZqYb == true){uqGnSOZqYb = false;}
      if(GxBUjSxeZh == true){GxBUjSxeZh = false;}
      if(rzlFzOEPqA == true){rzlFzOEPqA = false;}
      if(byfhtKiaEh == true){byfhtKiaEh = false;}
    } 
}; 
