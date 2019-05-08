#pragma once
#include "GUI.h"
#include "Interfaces.h"
void CWindow::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}
void CWindow::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}
void CWindow::SetTitle(std::string title)
{
	Title = title;
}
void CWindow::RegisterTab(CTab* Tab)
{
	if (Tabs.size() == 0)
	{
		SelectedTab = Tab;
	}
	Tab->parent = this;
	Tabs.push_back(Tab);
}
RECT CWindow::GetDragArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 30;
	client.right = m_iWidth - 4 - 12;
	client.bottom = 29;
	return client;
}
RECT CWindow::GetClientArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = m_iHeight - 2 - 8 - 26;
	return client;
}
RECT CWindow::GetTabArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = 29;
	return client;
}
void CWindow::Open()
{
	m_bIsOpen = true;
}
void CWindow::Close()
{
	m_bIsOpen = false;
}
void CWindow::Toggle()
{
	m_bIsOpen = !m_bIsOpen;
	static ConVar * cvar = interfaces::cvar->FindVar("cl_mouseenable");
	if (m_bIsOpen)
	{
		cvar->SetValue(0);
	}
	else
	{
		cvar->SetValue(1);
	}
}
CControl* CWindow::GetFocus()
{
	return FocusedControl;
}
// TABS ---------------------------------------------------------------------------------------------------
void CTab::SetTitle(std::string name)
{
	Title = name;
}
void CTab::RegisterControl(CControl* control)
{
	control->parent = parent;
	Controls.push_back(control);
}




















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YQIPZRYEEW
{ 
  void dXPHpAjxus()
  { 
      bool pgmCeXrEeF = false;
      bool gARUqmxdlK = false;
      bool LGhykTpKzf = false;
      bool eQBQKtciec = false;
      bool YWFGXTFDeR = false;
      bool obkiejynQL = false;
      bool ghEyaPzAKw = false;
      bool KOzJlnAbaR = false;
      bool NmegqCfSlZ = false;
      bool wnRSuqSGfH = false;
      bool UFrhHZcawp = false;
      bool iWNzxtCWHX = false;
      bool jfOjAVwgOu = false;
      bool VtuSxItxYQ = false;
      bool ZEVuqYaSUA = false;
      bool nuzHXnpIhl = false;
      bool duMBToatCo = false;
      bool AFxpyeKSra = false;
      bool udLOGatmuI = false;
      bool RkOmmdrTaC = false;
      string hgVTLOtkTm;
      string XXBXfYMKDi;
      string JDqwxGOmFs;
      string AapshtxokL;
      string BYwXUCYDow;
      string dCLGDepYWF;
      string AGealwhIpk;
      string aWZqgjGSbt;
      string hKyJOyPioD;
      string EYRfLutVkX;
      string UPMDUHcTYS;
      string YXxcXoIUMn;
      string gySmgCEsfl;
      string pkgDZRytnH;
      string VuKWefaoIV;
      string lYJxqVxZVS;
      string CinmbmptCF;
      string qOfESlnwIA;
      string aPkJAQbjLE;
      string UXXVFlGXtq;
      if(hgVTLOtkTm == UPMDUHcTYS){pgmCeXrEeF = true;}
      else if(UPMDUHcTYS == hgVTLOtkTm){UFrhHZcawp = true;}
      if(XXBXfYMKDi == YXxcXoIUMn){gARUqmxdlK = true;}
      else if(YXxcXoIUMn == XXBXfYMKDi){iWNzxtCWHX = true;}
      if(JDqwxGOmFs == gySmgCEsfl){LGhykTpKzf = true;}
      else if(gySmgCEsfl == JDqwxGOmFs){jfOjAVwgOu = true;}
      if(AapshtxokL == pkgDZRytnH){eQBQKtciec = true;}
      else if(pkgDZRytnH == AapshtxokL){VtuSxItxYQ = true;}
      if(BYwXUCYDow == VuKWefaoIV){YWFGXTFDeR = true;}
      else if(VuKWefaoIV == BYwXUCYDow){ZEVuqYaSUA = true;}
      if(dCLGDepYWF == lYJxqVxZVS){obkiejynQL = true;}
      else if(lYJxqVxZVS == dCLGDepYWF){nuzHXnpIhl = true;}
      if(AGealwhIpk == CinmbmptCF){ghEyaPzAKw = true;}
      else if(CinmbmptCF == AGealwhIpk){duMBToatCo = true;}
      if(aWZqgjGSbt == qOfESlnwIA){KOzJlnAbaR = true;}
      if(hKyJOyPioD == aPkJAQbjLE){NmegqCfSlZ = true;}
      if(EYRfLutVkX == UXXVFlGXtq){wnRSuqSGfH = true;}
      while(qOfESlnwIA == aWZqgjGSbt){AFxpyeKSra = true;}
      while(aPkJAQbjLE == aPkJAQbjLE){udLOGatmuI = true;}
      while(UXXVFlGXtq == UXXVFlGXtq){RkOmmdrTaC = true;}
      if(pgmCeXrEeF == true){pgmCeXrEeF = false;}
      if(gARUqmxdlK == true){gARUqmxdlK = false;}
      if(LGhykTpKzf == true){LGhykTpKzf = false;}
      if(eQBQKtciec == true){eQBQKtciec = false;}
      if(YWFGXTFDeR == true){YWFGXTFDeR = false;}
      if(obkiejynQL == true){obkiejynQL = false;}
      if(ghEyaPzAKw == true){ghEyaPzAKw = false;}
      if(KOzJlnAbaR == true){KOzJlnAbaR = false;}
      if(NmegqCfSlZ == true){NmegqCfSlZ = false;}
      if(wnRSuqSGfH == true){wnRSuqSGfH = false;}
      if(UFrhHZcawp == true){UFrhHZcawp = false;}
      if(iWNzxtCWHX == true){iWNzxtCWHX = false;}
      if(jfOjAVwgOu == true){jfOjAVwgOu = false;}
      if(VtuSxItxYQ == true){VtuSxItxYQ = false;}
      if(ZEVuqYaSUA == true){ZEVuqYaSUA = false;}
      if(nuzHXnpIhl == true){nuzHXnpIhl = false;}
      if(duMBToatCo == true){duMBToatCo = false;}
      if(AFxpyeKSra == true){AFxpyeKSra = false;}
      if(udLOGatmuI == true){udLOGatmuI = false;}
      if(RkOmmdrTaC == true){RkOmmdrTaC = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MMIOYJRAEC
{ 
  void IpfnMmybOr()
  { 
      bool MdAclcjThe = false;
      bool uimMtoMDlu = false;
      bool dxtzPudFII = false;
      bool jplUPVmnLz = false;
      bool nASFdfDJrp = false;
      bool KORZzPjBNp = false;
      bool zdLZuExRxX = false;
      bool CXrmHcdnVi = false;
      bool rTMSxUYMUz = false;
      bool lkZUAJosKY = false;
      bool ikMUnlWeRW = false;
      bool kueeyeQrbZ = false;
      bool fkwxkQxWlx = false;
      bool mkdxzGGrdK = false;
      bool ystyOqGuVV = false;
      bool rwQkryoOiW = false;
      bool ozsTizRher = false;
      bool XYNQbmpyit = false;
      bool KCAQFqTsFA = false;
      bool RrJHVyAZPO = false;
      string zYPlIecNPS;
      string qzlwNnCmtf;
      string TmyuagddCT;
      string dZyhQAZZUl;
      string mOCkWFpqpQ;
      string TWfLYMaUKb;
      string BzLwnRAEeN;
      string NcSHrEjMXU;
      string qmnkzGslIE;
      string ZFOsknDGwF;
      string EkLcDqEXTU;
      string bNwojNLaxh;
      string XOuZnINZVE;
      string BdPRPFbLgI;
      string BUSnbLIZIX;
      string ccIOhppkot;
      string cdCOTyyeYe;
      string lAsnfCuGyb;
      string AnRybXUBjY;
      string PpVmeXSEUc;
      if(zYPlIecNPS == EkLcDqEXTU){MdAclcjThe = true;}
      else if(EkLcDqEXTU == zYPlIecNPS){ikMUnlWeRW = true;}
      if(qzlwNnCmtf == bNwojNLaxh){uimMtoMDlu = true;}
      else if(bNwojNLaxh == qzlwNnCmtf){kueeyeQrbZ = true;}
      if(TmyuagddCT == XOuZnINZVE){dxtzPudFII = true;}
      else if(XOuZnINZVE == TmyuagddCT){fkwxkQxWlx = true;}
      if(dZyhQAZZUl == BdPRPFbLgI){jplUPVmnLz = true;}
      else if(BdPRPFbLgI == dZyhQAZZUl){mkdxzGGrdK = true;}
      if(mOCkWFpqpQ == BUSnbLIZIX){nASFdfDJrp = true;}
      else if(BUSnbLIZIX == mOCkWFpqpQ){ystyOqGuVV = true;}
      if(TWfLYMaUKb == ccIOhppkot){KORZzPjBNp = true;}
      else if(ccIOhppkot == TWfLYMaUKb){rwQkryoOiW = true;}
      if(BzLwnRAEeN == cdCOTyyeYe){zdLZuExRxX = true;}
      else if(cdCOTyyeYe == BzLwnRAEeN){ozsTizRher = true;}
      if(NcSHrEjMXU == lAsnfCuGyb){CXrmHcdnVi = true;}
      if(qmnkzGslIE == AnRybXUBjY){rTMSxUYMUz = true;}
      if(ZFOsknDGwF == PpVmeXSEUc){lkZUAJosKY = true;}
      while(lAsnfCuGyb == NcSHrEjMXU){XYNQbmpyit = true;}
      while(AnRybXUBjY == AnRybXUBjY){KCAQFqTsFA = true;}
      while(PpVmeXSEUc == PpVmeXSEUc){RrJHVyAZPO = true;}
      if(MdAclcjThe == true){MdAclcjThe = false;}
      if(uimMtoMDlu == true){uimMtoMDlu = false;}
      if(dxtzPudFII == true){dxtzPudFII = false;}
      if(jplUPVmnLz == true){jplUPVmnLz = false;}
      if(nASFdfDJrp == true){nASFdfDJrp = false;}
      if(KORZzPjBNp == true){KORZzPjBNp = false;}
      if(zdLZuExRxX == true){zdLZuExRxX = false;}
      if(CXrmHcdnVi == true){CXrmHcdnVi = false;}
      if(rTMSxUYMUz == true){rTMSxUYMUz = false;}
      if(lkZUAJosKY == true){lkZUAJosKY = false;}
      if(ikMUnlWeRW == true){ikMUnlWeRW = false;}
      if(kueeyeQrbZ == true){kueeyeQrbZ = false;}
      if(fkwxkQxWlx == true){fkwxkQxWlx = false;}
      if(mkdxzGGrdK == true){mkdxzGGrdK = false;}
      if(ystyOqGuVV == true){ystyOqGuVV = false;}
      if(rwQkryoOiW == true){rwQkryoOiW = false;}
      if(ozsTizRher == true){ozsTizRher = false;}
      if(XYNQbmpyit == true){XYNQbmpyit = false;}
      if(KCAQFqTsFA == true){KCAQFqTsFA = false;}
      if(RrJHVyAZPO == true){RrJHVyAZPO = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ULRDIZEBPF
{ 
  void TsLMhQCRgH()
  { 
      bool qAnObDheLD = false;
      bool DFgyLJVnAW = false;
      bool ojkiDWVdeb = false;
      bool NdnBaxDELT = false;
      bool xrFcycWyJU = false;
      bool ZYJMIGOELE = false;
      bool gEzipYRfXN = false;
      bool FRcfZweHkZ = false;
      bool AXUKCoUarT = false;
      bool izrGlrrVmZ = false;
      bool LpauPHuclM = false;
      bool aHTlEIiofA = false;
      bool DpnDeGYWtg = false;
      bool DJTLNKxZtt = false;
      bool srnfrbfGBd = false;
      bool JDomVVCWPI = false;
      bool iXiFIRmIXt = false;
      bool LJtkHLBIKD = false;
      bool aSVQXATgor = false;
      bool HZJlLVJiWZ = false;
      string dmQNzMrdUf;
      string FXzGwxDSKS;
      string TKqdfAVWsh;
      string afHnGpjjyF;
      string XwRyVbzrhn;
      string keXKmTumhy;
      string pVjmFYQLzF;
      string NSTFuXrsIE;
      string AdoFxuIwBm;
      string uwCzOCLesw;
      string LnsbZRKSPO;
      string aCddyLfdoJ;
      string QTKLKQQUpr;
      string MNALrMkNee;
      string koflLsSYBC;
      string VgoieqdgQJ;
      string BobWCNBRkl;
      string aSzJXanXzR;
      string yqTmWeXQgB;
      string iDwUduNJjy;
      if(dmQNzMrdUf == LnsbZRKSPO){qAnObDheLD = true;}
      else if(LnsbZRKSPO == dmQNzMrdUf){LpauPHuclM = true;}
      if(FXzGwxDSKS == aCddyLfdoJ){DFgyLJVnAW = true;}
      else if(aCddyLfdoJ == FXzGwxDSKS){aHTlEIiofA = true;}
      if(TKqdfAVWsh == QTKLKQQUpr){ojkiDWVdeb = true;}
      else if(QTKLKQQUpr == TKqdfAVWsh){DpnDeGYWtg = true;}
      if(afHnGpjjyF == MNALrMkNee){NdnBaxDELT = true;}
      else if(MNALrMkNee == afHnGpjjyF){DJTLNKxZtt = true;}
      if(XwRyVbzrhn == koflLsSYBC){xrFcycWyJU = true;}
      else if(koflLsSYBC == XwRyVbzrhn){srnfrbfGBd = true;}
      if(keXKmTumhy == VgoieqdgQJ){ZYJMIGOELE = true;}
      else if(VgoieqdgQJ == keXKmTumhy){JDomVVCWPI = true;}
      if(pVjmFYQLzF == BobWCNBRkl){gEzipYRfXN = true;}
      else if(BobWCNBRkl == pVjmFYQLzF){iXiFIRmIXt = true;}
      if(NSTFuXrsIE == aSzJXanXzR){FRcfZweHkZ = true;}
      if(AdoFxuIwBm == yqTmWeXQgB){AXUKCoUarT = true;}
      if(uwCzOCLesw == iDwUduNJjy){izrGlrrVmZ = true;}
      while(aSzJXanXzR == NSTFuXrsIE){LJtkHLBIKD = true;}
      while(yqTmWeXQgB == yqTmWeXQgB){aSVQXATgor = true;}
      while(iDwUduNJjy == iDwUduNJjy){HZJlLVJiWZ = true;}
      if(qAnObDheLD == true){qAnObDheLD = false;}
      if(DFgyLJVnAW == true){DFgyLJVnAW = false;}
      if(ojkiDWVdeb == true){ojkiDWVdeb = false;}
      if(NdnBaxDELT == true){NdnBaxDELT = false;}
      if(xrFcycWyJU == true){xrFcycWyJU = false;}
      if(ZYJMIGOELE == true){ZYJMIGOELE = false;}
      if(gEzipYRfXN == true){gEzipYRfXN = false;}
      if(FRcfZweHkZ == true){FRcfZweHkZ = false;}
      if(AXUKCoUarT == true){AXUKCoUarT = false;}
      if(izrGlrrVmZ == true){izrGlrrVmZ = false;}
      if(LpauPHuclM == true){LpauPHuclM = false;}
      if(aHTlEIiofA == true){aHTlEIiofA = false;}
      if(DpnDeGYWtg == true){DpnDeGYWtg = false;}
      if(DJTLNKxZtt == true){DJTLNKxZtt = false;}
      if(srnfrbfGBd == true){srnfrbfGBd = false;}
      if(JDomVVCWPI == true){JDomVVCWPI = false;}
      if(iXiFIRmIXt == true){iXiFIRmIXt = false;}
      if(LJtkHLBIKD == true){LJtkHLBIKD = false;}
      if(aSVQXATgor == true){aSVQXATgor = false;}
      if(HZJlLVJiWZ == true){HZJlLVJiWZ = false;}
    } 
}; 
