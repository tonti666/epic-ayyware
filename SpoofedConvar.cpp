#include "SDK.h"
#include "MiscClasses.h"

void ConVar::SetValue(const char* value)
{
	typedef void(__thiscall* OriginalFn)(void*, const char*);
	return  call_vfunc<OriginalFn>(this, 14)(this, value);
}

void ConVar::SetValue(float value)
{
	typedef void(__thiscall* OriginalFn)(void*, float);
	return call_vfunc<OriginalFn>(this, 15)(this, value);
}

void ConVar::SetValue(int value)
{
	typedef void(__thiscall* OriginalFn)(void*, int);
	return call_vfunc<OriginalFn>(this, 16)(this, value);
}
float ConVar::GetFloat(void) const {
	return pParent->fValue;
}
int ConVar::GetInt(void) const {
	return pParent->nValue;
}
bool ConVar::GetBool()
{
	return !!GetInt();
}
const char* ConVar::GetString(void) const {
	return pParent->pszDefaultValue;
}

Color ConVar::GetColor(void) const {
	unsigned char *pColorElement = ((unsigned char *)&pParent->nValue);
	return Color(pColorElement[0], pColorElement[1], pColorElement[2], pColorElement[3]);
}
void ConVar::SetValue(Color value)
{
	typedef void(__thiscall* OriginalFn)(void*, Color);
	return call_vfunc<OriginalFn>(this, 17)(this, value);
}

char* ConVar::GetName()
{
	typedef char*(__thiscall* OriginalFn)(void*);
	return call_vfunc<OriginalFn>(this, 5)(this);
}

char* ConVar::GetDefault()
{
	return pszDefaultValue;
}

template<typename T>
inline void MinspecCvar::SetValue(T value)
{
	m_pConVar->SetValue(T);
}
MinspecCvar::MinspecCvar(const char* szCVar, char* newname, float newvalue) : m_pConVar(nullptr)
{
	m_pConVar = interfaces::cvar->FindVar(szCVar);
	m_newvalue = newvalue;
	m_szReplacementName = newname;
	Spoof();
}

MinspecCvar::~MinspecCvar()
{
	if (ValidCvar())
	{
		interfaces::cvar->UnregisterConCommand(m_pConVar);
		m_pConVar->pszName = m_szOriginalName;
		m_pConVar->SetValue(m_OriginalValue);
		interfaces::cvar->RegisterConCommand(m_pConVar);
	}
}

bool MinspecCvar::ValidCvar()
{
	return m_pConVar != nullptr;
}
void MinspecCvar::Spoof()
{
	if (ValidCvar())
	{
		interfaces::cvar->UnregisterConCommand(m_pConVar);
		m_szOriginalName = m_pConVar->pszName;
		m_OriginalValue = m_pConVar->GetFloat();

		m_pConVar->pszName = m_szReplacementName;
		interfaces::cvar->RegisterConCommand(m_pConVar);
		m_pConVar->SetValue(m_newvalue);
	}
}

int MinspecCvar::GetInt()
{
	if (ValidCvar()) {
		return m_pConVar->GetInt();
	}
	return 0;
}

float MinspecCvar::GetFloat()
{
	if (ValidCvar()) {
		return m_pConVar->GetFloat();
	}
	return 0.0f;
}

const char* MinspecCvar::GetString()
{
	if (ValidCvar()) {
		return m_pConVar->GetString();
	}
	return nullptr;
}
SpoofedConvar::SpoofedConvar(const char* szCVar) {
	m_pOriginalCVar = interfaces::cvar->FindVar(szCVar);
	Spoof();
}
SpoofedConvar::SpoofedConvar(ConVar* pCVar) {
	m_pOriginalCVar = pCVar;
	Spoof();
}
SpoofedConvar::~SpoofedConvar() {
	if (IsSpoofed()) {
		DWORD dwOld;

		SetFlags(m_iOriginalFlags);
		SetString(m_szOriginalValue);

		VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, PAGE_READWRITE, &dwOld);
		strcpy((char*)m_pOriginalCVar->pszName, m_szOriginalName);
		VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, dwOld, &dwOld);

		//Unregister dummy cvar
		interfaces::cvar->UnregisterConCommand(m_pDummyCVar);
		free(m_pDummyCVar);
		m_pDummyCVar = NULL;
	}
}
bool SpoofedConvar::IsSpoofed() {
	return m_pDummyCVar != NULL;
}
void SpoofedConvar::Spoof() {
	if (!IsSpoofed() && m_pOriginalCVar) {
		//Save old name value and flags so we can restore the cvar lates if needed
		m_iOriginalFlags = m_pOriginalCVar->nFlags;
		strcpy(m_szOriginalName, m_pOriginalCVar->pszName);
		strcpy(m_szOriginalValue, m_pOriginalCVar->pszDefaultValue);

		sprintf_s(m_szDummyName, 128, "d_%s", m_szOriginalName);

		//Create the dummy cvar
		m_pDummyCVar = (ConVar*)malloc(sizeof(ConVar));
		if (!m_pDummyCVar) return;
		memcpy(m_pDummyCVar, m_pOriginalCVar, sizeof(ConVar));
		
		m_pDummyCVar->pNext = NULL;
		//Register it
		interfaces::cvar->RegisterConCommand(m_pDummyCVar);

		//Fix "write access violation" bullshit
		DWORD dwOld;
		VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, PAGE_READWRITE, &dwOld);

		//Rename the cvar
		strcpy((char*)m_pOriginalCVar->pszName, m_szDummyName);

		VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, dwOld, &dwOld);

		SetFlags(FCVAR_NONE);
	}
}
void SpoofedConvar::SetFlags(int flags) {
	if (IsSpoofed()) {
		m_pOriginalCVar->nFlags = flags;
	}
}
int SpoofedConvar::GetFlags() {
	return m_pOriginalCVar->nFlags;
}
void SpoofedConvar::SetInt(int iValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(iValue);
	}
}
void SpoofedConvar::SetBool(bool bValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(bValue);
	}
}
void SpoofedConvar::SetFloat(float flValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(flValue);
	}
}
void SpoofedConvar::SetString(const char* szValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(szValue);
	}
}




















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FOHRYRPQRM
{ 
  void fVtWfyelZs()
  { 
      bool usEbniwckb = false;
      bool TbhjpRboPC = false;
      bool wsQVzOmRCT = false;
      bool SQcxxhRUDI = false;
      bool bbGMNSReDH = false;
      bool jjIYZBbibZ = false;
      bool wdZQJDzklH = false;
      bool zGlGZTmEHg = false;
      bool wHNujhHDWw = false;
      bool FhscMxCoCK = false;
      bool SgsuHcxCyF = false;
      bool NXaudoQegb = false;
      bool uwNpCRNItc = false;
      bool RyCxNOORmG = false;
      bool jFEQhgGxzX = false;
      bool gWHaOWSCrp = false;
      bool OoIVnPVdhP = false;
      bool ZyraqpqxHV = false;
      bool KQJVuupahp = false;
      bool uLZwquxhcS = false;
      string mDTLtmkBzf;
      string JDkkGpGozh;
      string LTTcrKXuef;
      string jaOXFIDXbi;
      string WBOdBUyZAc;
      string AFrwQSipTi;
      string OpWTuhnMey;
      string liMPKJFpbX;
      string MxtBediSHl;
      string YMEsUgdthS;
      string fkuCpRsblG;
      string NsNuVRuVoR;
      string yVFsYZeTud;
      string fKHIraCrKM;
      string kGfBYfyIWD;
      string kHFQTJfCcQ;
      string mYgHZCkFDu;
      string OCLbfroDTr;
      string aVxqrXFqnh;
      string uAPsYLZYaW;
      if(mDTLtmkBzf == fkuCpRsblG){usEbniwckb = true;}
      else if(fkuCpRsblG == mDTLtmkBzf){SgsuHcxCyF = true;}
      if(JDkkGpGozh == NsNuVRuVoR){TbhjpRboPC = true;}
      else if(NsNuVRuVoR == JDkkGpGozh){NXaudoQegb = true;}
      if(LTTcrKXuef == yVFsYZeTud){wsQVzOmRCT = true;}
      else if(yVFsYZeTud == LTTcrKXuef){uwNpCRNItc = true;}
      if(jaOXFIDXbi == fKHIraCrKM){SQcxxhRUDI = true;}
      else if(fKHIraCrKM == jaOXFIDXbi){RyCxNOORmG = true;}
      if(WBOdBUyZAc == kGfBYfyIWD){bbGMNSReDH = true;}
      else if(kGfBYfyIWD == WBOdBUyZAc){jFEQhgGxzX = true;}
      if(AFrwQSipTi == kHFQTJfCcQ){jjIYZBbibZ = true;}
      else if(kHFQTJfCcQ == AFrwQSipTi){gWHaOWSCrp = true;}
      if(OpWTuhnMey == mYgHZCkFDu){wdZQJDzklH = true;}
      else if(mYgHZCkFDu == OpWTuhnMey){OoIVnPVdhP = true;}
      if(liMPKJFpbX == OCLbfroDTr){zGlGZTmEHg = true;}
      if(MxtBediSHl == aVxqrXFqnh){wHNujhHDWw = true;}
      if(YMEsUgdthS == uAPsYLZYaW){FhscMxCoCK = true;}
      while(OCLbfroDTr == liMPKJFpbX){ZyraqpqxHV = true;}
      while(aVxqrXFqnh == aVxqrXFqnh){KQJVuupahp = true;}
      while(uAPsYLZYaW == uAPsYLZYaW){uLZwquxhcS = true;}
      if(usEbniwckb == true){usEbniwckb = false;}
      if(TbhjpRboPC == true){TbhjpRboPC = false;}
      if(wsQVzOmRCT == true){wsQVzOmRCT = false;}
      if(SQcxxhRUDI == true){SQcxxhRUDI = false;}
      if(bbGMNSReDH == true){bbGMNSReDH = false;}
      if(jjIYZBbibZ == true){jjIYZBbibZ = false;}
      if(wdZQJDzklH == true){wdZQJDzklH = false;}
      if(zGlGZTmEHg == true){zGlGZTmEHg = false;}
      if(wHNujhHDWw == true){wHNujhHDWw = false;}
      if(FhscMxCoCK == true){FhscMxCoCK = false;}
      if(SgsuHcxCyF == true){SgsuHcxCyF = false;}
      if(NXaudoQegb == true){NXaudoQegb = false;}
      if(uwNpCRNItc == true){uwNpCRNItc = false;}
      if(RyCxNOORmG == true){RyCxNOORmG = false;}
      if(jFEQhgGxzX == true){jFEQhgGxzX = false;}
      if(gWHaOWSCrp == true){gWHaOWSCrp = false;}
      if(OoIVnPVdhP == true){OoIVnPVdhP = false;}
      if(ZyraqpqxHV == true){ZyraqpqxHV = false;}
      if(KQJVuupahp == true){KQJVuupahp = false;}
      if(uLZwquxhcS == true){uLZwquxhcS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GQCPFHLPSQ
{ 
  void obadGhAggn()
  { 
      bool SdQFmNkbBo = false;
      bool HPoePOgSTb = false;
      bool OBiKeodsjp = false;
      bool FPOYefUgPH = false;
      bool HPSEywGTuq = false;
      bool GUYrNQuFay = false;
      bool jqqLjrAPAj = false;
      bool tzCLUyieLh = false;
      bool iJysijpWKN = false;
      bool jOGRXPXGTG = false;
      bool wpTLxGSMWO = false;
      bool hQtsOasybA = false;
      bool BcFlAfsjnF = false;
      bool abKFkromuc = false;
      bool RdzufSUMCw = false;
      bool VZdTFSkgKR = false;
      bool NhYQnFGnHW = false;
      bool kGCziXcJGg = false;
      bool KfgqjAxpAS = false;
      bool GIbdOiEiGa = false;
      string EPoaTNANzy;
      string EchXuLUYRD;
      string NNcTLeYDwc;
      string BoANoxhinn;
      string symNCisJMT;
      string DodNTqlVOn;
      string owRVxPcTix;
      string cljcKPaPQb;
      string kOqbYMKlty;
      string LMTAzwfQPm;
      string GZaOpREADE;
      string PVFlDZFpxk;
      string EpZjlQRjdN;
      string iKzcVLmbkG;
      string HKhphoGgwZ;
      string qpSeiPzqcZ;
      string zTWpUFVcQq;
      string lcEqcsIjBx;
      string JkmDVrqUsG;
      string MwzLGmnrUc;
      if(EPoaTNANzy == GZaOpREADE){SdQFmNkbBo = true;}
      else if(GZaOpREADE == EPoaTNANzy){wpTLxGSMWO = true;}
      if(EchXuLUYRD == PVFlDZFpxk){HPoePOgSTb = true;}
      else if(PVFlDZFpxk == EchXuLUYRD){hQtsOasybA = true;}
      if(NNcTLeYDwc == EpZjlQRjdN){OBiKeodsjp = true;}
      else if(EpZjlQRjdN == NNcTLeYDwc){BcFlAfsjnF = true;}
      if(BoANoxhinn == iKzcVLmbkG){FPOYefUgPH = true;}
      else if(iKzcVLmbkG == BoANoxhinn){abKFkromuc = true;}
      if(symNCisJMT == HKhphoGgwZ){HPSEywGTuq = true;}
      else if(HKhphoGgwZ == symNCisJMT){RdzufSUMCw = true;}
      if(DodNTqlVOn == qpSeiPzqcZ){GUYrNQuFay = true;}
      else if(qpSeiPzqcZ == DodNTqlVOn){VZdTFSkgKR = true;}
      if(owRVxPcTix == zTWpUFVcQq){jqqLjrAPAj = true;}
      else if(zTWpUFVcQq == owRVxPcTix){NhYQnFGnHW = true;}
      if(cljcKPaPQb == lcEqcsIjBx){tzCLUyieLh = true;}
      if(kOqbYMKlty == JkmDVrqUsG){iJysijpWKN = true;}
      if(LMTAzwfQPm == MwzLGmnrUc){jOGRXPXGTG = true;}
      while(lcEqcsIjBx == cljcKPaPQb){kGCziXcJGg = true;}
      while(JkmDVrqUsG == JkmDVrqUsG){KfgqjAxpAS = true;}
      while(MwzLGmnrUc == MwzLGmnrUc){GIbdOiEiGa = true;}
      if(SdQFmNkbBo == true){SdQFmNkbBo = false;}
      if(HPoePOgSTb == true){HPoePOgSTb = false;}
      if(OBiKeodsjp == true){OBiKeodsjp = false;}
      if(FPOYefUgPH == true){FPOYefUgPH = false;}
      if(HPSEywGTuq == true){HPSEywGTuq = false;}
      if(GUYrNQuFay == true){GUYrNQuFay = false;}
      if(jqqLjrAPAj == true){jqqLjrAPAj = false;}
      if(tzCLUyieLh == true){tzCLUyieLh = false;}
      if(iJysijpWKN == true){iJysijpWKN = false;}
      if(jOGRXPXGTG == true){jOGRXPXGTG = false;}
      if(wpTLxGSMWO == true){wpTLxGSMWO = false;}
      if(hQtsOasybA == true){hQtsOasybA = false;}
      if(BcFlAfsjnF == true){BcFlAfsjnF = false;}
      if(abKFkromuc == true){abKFkromuc = false;}
      if(RdzufSUMCw == true){RdzufSUMCw = false;}
      if(VZdTFSkgKR == true){VZdTFSkgKR = false;}
      if(NhYQnFGnHW == true){NhYQnFGnHW = false;}
      if(kGCziXcJGg == true){kGCziXcJGg = false;}
      if(KfgqjAxpAS == true){KfgqjAxpAS = false;}
      if(GIbdOiEiGa == true){GIbdOiEiGa = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NSCVFOJBZD
{ 
  void ANfFgSqQwx()
  { 
      bool wVFVekfzcU = false;
      bool JdVwXAekMr = false;
      bool gBgjjutdpx = false;
      bool ToNOhQrjwg = false;
      bool UqWErXXYIS = false;
      bool emGjFuoVkj = false;
      bool pBJeAUQGNG = false;
      bool HtofWodbrM = false;
      bool WVVWSghdIm = false;
      bool aWyLpThrDW = false;
      bool HQKBLjyaMQ = false;
      bool QMcYNUemLT = false;
      bool llyaekucoC = false;
      bool GlXUEFKULh = false;
      bool hHQrWOsZyU = false;
      bool nufjkVSjVZ = false;
      bool oVkKJenITH = false;
      bool KKadkeUFEQ = false;
      bool rVdUNLFnji = false;
      bool layksMVGiq = false;
      string IMgZJqcuKb;
      string IclqCmpNIG;
      string myFEXxhPdf;
      string dfZVtpABej;
      string ZGOiWcugPN;
      string biidQJHbCL;
      string IMQexcCPxL;
      string mnhYfgjVhu;
      string CBAQEuwwEj;
      string iXzSbNpjlt;
      string ZejDZmAKxj;
      string YktHJGMAZC;
      string fGKMbPoPxS;
      string qAjnUWRwpQ;
      string tlyULxcrIU;
      string AoJjNSTWaK;
      string xkNesYBGlm;
      string oPJJHsIpBe;
      string mxGbRBqzZC;
      string CtVfTJYTbl;
      if(IMgZJqcuKb == ZejDZmAKxj){wVFVekfzcU = true;}
      else if(ZejDZmAKxj == IMgZJqcuKb){HQKBLjyaMQ = true;}
      if(IclqCmpNIG == YktHJGMAZC){JdVwXAekMr = true;}
      else if(YktHJGMAZC == IclqCmpNIG){QMcYNUemLT = true;}
      if(myFEXxhPdf == fGKMbPoPxS){gBgjjutdpx = true;}
      else if(fGKMbPoPxS == myFEXxhPdf){llyaekucoC = true;}
      if(dfZVtpABej == qAjnUWRwpQ){ToNOhQrjwg = true;}
      else if(qAjnUWRwpQ == dfZVtpABej){GlXUEFKULh = true;}
      if(ZGOiWcugPN == tlyULxcrIU){UqWErXXYIS = true;}
      else if(tlyULxcrIU == ZGOiWcugPN){hHQrWOsZyU = true;}
      if(biidQJHbCL == AoJjNSTWaK){emGjFuoVkj = true;}
      else if(AoJjNSTWaK == biidQJHbCL){nufjkVSjVZ = true;}
      if(IMQexcCPxL == xkNesYBGlm){pBJeAUQGNG = true;}
      else if(xkNesYBGlm == IMQexcCPxL){oVkKJenITH = true;}
      if(mnhYfgjVhu == oPJJHsIpBe){HtofWodbrM = true;}
      if(CBAQEuwwEj == mxGbRBqzZC){WVVWSghdIm = true;}
      if(iXzSbNpjlt == CtVfTJYTbl){aWyLpThrDW = true;}
      while(oPJJHsIpBe == mnhYfgjVhu){KKadkeUFEQ = true;}
      while(mxGbRBqzZC == mxGbRBqzZC){rVdUNLFnji = true;}
      while(CtVfTJYTbl == CtVfTJYTbl){layksMVGiq = true;}
      if(wVFVekfzcU == true){wVFVekfzcU = false;}
      if(JdVwXAekMr == true){JdVwXAekMr = false;}
      if(gBgjjutdpx == true){gBgjjutdpx = false;}
      if(ToNOhQrjwg == true){ToNOhQrjwg = false;}
      if(UqWErXXYIS == true){UqWErXXYIS = false;}
      if(emGjFuoVkj == true){emGjFuoVkj = false;}
      if(pBJeAUQGNG == true){pBJeAUQGNG = false;}
      if(HtofWodbrM == true){HtofWodbrM = false;}
      if(WVVWSghdIm == true){WVVWSghdIm = false;}
      if(aWyLpThrDW == true){aWyLpThrDW = false;}
      if(HQKBLjyaMQ == true){HQKBLjyaMQ = false;}
      if(QMcYNUemLT == true){QMcYNUemLT = false;}
      if(llyaekucoC == true){llyaekucoC = false;}
      if(GlXUEFKULh == true){GlXUEFKULh = false;}
      if(hHQrWOsZyU == true){hHQrWOsZyU = false;}
      if(nufjkVSjVZ == true){nufjkVSjVZ = false;}
      if(oVkKJenITH == true){oVkKJenITH = false;}
      if(KKadkeUFEQ == true){KKadkeUFEQ = false;}
      if(rVdUNLFnji == true){rVdUNLFnji = false;}
      if(layksMVGiq == true){layksMVGiq = false;}
    } 
}; 
