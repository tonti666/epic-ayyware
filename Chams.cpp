#include "Chams.h"
#include "offsets.h"
#include "SDK.h"
#include "Interfaces.h"

void InitKeyValues(KeyValues* keyValues, char* name)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_KeyValues;
	__asm
	{
		push name
			mov ecx, keyValues
			call dwFunction
	}
}

void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_LoadFromBuffer;

	__asm
	{
		push 0
			push 0
			push 0
			push pBuffer
			push resourceName
			mov ecx, keyValues
			call dwFunction
	}
}

/* "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$ambientocclusion"       "1"
  "$normalmapalphaenvmapmask"       "1"
  "$phong"       "1"
  "$phongboost"       "3"
  "$selfillum"    "1"
  "$halflambert"  "1"
    "$rimlight"               "1"
	"$rimlightexponent"       "2"
	"$rimlightboost"          ".2"		
  "$znearer"      "0"
  "$flat"         "1"
  "$reflectivity" "[.89 .89 .89]"*/
IMaterial *CreateMaterialLit() 
{
	static int created = 0;

	//i have no fucking idea why this keeps being flat

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$ambientocclusion\" \"1\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
        \n\t\"$flat\" \"0\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$phong\" \"1\"\
		\n\t\"$phongboost\" \"1\"\
		\n\t\"$reflectivity\" \"[.89 .89 .89]\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = "VertexLitGeneric";
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, 0, 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#skeetus%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial *createdMaterial = interfaces::materialsystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

IMaterial *CreateMaterialIgnorez()
{
	static int created = 0;

	//i have no fucking idea why this keeps being flat

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$ambientocclusion\" \"1\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
        \n\t\"$flat\" \"0\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$phong\" \"1\"\
		\n\t\"$phongboost\" \"1\"\
		\n\t\"$reflectivity\" \"[.89 .89 .89]\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = "VertexLitGeneric";
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, 1, 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#skootus_%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial *createdMaterial = interfaces::materialsystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		float temp[3] =
		{
			color.r(),
			color.g(),
			color.b()
		};
		temp[0] /= 255.f;
		temp[1] /= 255.f;
		temp[2] /= 255.f;
		float alpha = color.a();
		interfaces::render_view->SetBlend(1.0f);
		interfaces::render_view->SetColorModulation(temp);
	}
	if (forceMaterial)
		interfaces::model_render->ForcedMaterialOverride(material);
	else
		interfaces::model_render->ForcedMaterialOverride(NULL);
}
IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe)
{
	static int created = 0;

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = (isLit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#avoz_%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial *createdMaterial = interfaces::materialsystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}
Chams* chams;
void Chams::override_material(bool ignoreZ, bool flat, bool wireframe, const Color& rgba)
{
	static IMaterial* materialIGNOREZ = CreateMaterialIgnorez();
	static IMaterial* CoveredFlatIgnore = CreateMaterial(true, false, false);
	static IMaterial* CoveredFlat = CreateMaterial(false, false, false);
	static IMaterial* CoveredLit = CreateMaterial(true);
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = CoveredFlatIgnore;
		else
			material = CoveredFlat;
	}
	else {
		if (ignoreZ)
			material = materialIGNOREZ;
		else
			material = CoveredLit;
	}
	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	interfaces::model_render->ForcedMaterialOverride(material);
}
































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PMXQLVHNTY
{ 
  void KfwtDqyQHl()
  { 
      bool HQKiOiErZr = false;
      bool xDPoJLCSWm = false;
      bool stRwRHVhTx = false;
      bool UhrhgyosEu = false;
      bool aCJYuPongz = false;
      bool AlgDZNyNQG = false;
      bool NWXcWPLhHK = false;
      bool VIObMpksBB = false;
      bool ShAUwiAoHT = false;
      bool AabxhDdgCe = false;
      bool EHfVcqXEqi = false;
      bool cFMOnfVDfm = false;
      bool OVZmKtfQtI = false;
      bool MDpWxKBeqF = false;
      bool LwyYqtONJB = false;
      bool haAOPigrKi = false;
      bool KGJhWYPpRc = false;
      bool dqLaaSXBwQ = false;
      bool eSHrZkCaGN = false;
      bool cITItFlPBS = false;
      string FMOMYbjVFi;
      string tsQmbeKEAF;
      string qFJhpstRaS;
      string tJhnObypss;
      string FOMtRlHyFV;
      string hlmNDACgVe;
      string gRulnkKbXA;
      string fUKGAcsOPD;
      string lwLFjIVTjO;
      string YQTNLnTZFg;
      string aeZFoNafNa;
      string HmRSsjFoCf;
      string PqrIaPBxuM;
      string ISJhcHWGQW;
      string goFIHlzAUb;
      string hdXZRimQdZ;
      string LdkBRndipl;
      string NyYezZliBo;
      string PAbSdeujQs;
      string TFcqKCEEpS;
      if(FMOMYbjVFi == aeZFoNafNa){HQKiOiErZr = true;}
      else if(aeZFoNafNa == FMOMYbjVFi){EHfVcqXEqi = true;}
      if(tsQmbeKEAF == HmRSsjFoCf){xDPoJLCSWm = true;}
      else if(HmRSsjFoCf == tsQmbeKEAF){cFMOnfVDfm = true;}
      if(qFJhpstRaS == PqrIaPBxuM){stRwRHVhTx = true;}
      else if(PqrIaPBxuM == qFJhpstRaS){OVZmKtfQtI = true;}
      if(tJhnObypss == ISJhcHWGQW){UhrhgyosEu = true;}
      else if(ISJhcHWGQW == tJhnObypss){MDpWxKBeqF = true;}
      if(FOMtRlHyFV == goFIHlzAUb){aCJYuPongz = true;}
      else if(goFIHlzAUb == FOMtRlHyFV){LwyYqtONJB = true;}
      if(hlmNDACgVe == hdXZRimQdZ){AlgDZNyNQG = true;}
      else if(hdXZRimQdZ == hlmNDACgVe){haAOPigrKi = true;}
      if(gRulnkKbXA == LdkBRndipl){NWXcWPLhHK = true;}
      else if(LdkBRndipl == gRulnkKbXA){KGJhWYPpRc = true;}
      if(fUKGAcsOPD == NyYezZliBo){VIObMpksBB = true;}
      if(lwLFjIVTjO == PAbSdeujQs){ShAUwiAoHT = true;}
      if(YQTNLnTZFg == TFcqKCEEpS){AabxhDdgCe = true;}
      while(NyYezZliBo == fUKGAcsOPD){dqLaaSXBwQ = true;}
      while(PAbSdeujQs == PAbSdeujQs){eSHrZkCaGN = true;}
      while(TFcqKCEEpS == TFcqKCEEpS){cITItFlPBS = true;}
      if(HQKiOiErZr == true){HQKiOiErZr = false;}
      if(xDPoJLCSWm == true){xDPoJLCSWm = false;}
      if(stRwRHVhTx == true){stRwRHVhTx = false;}
      if(UhrhgyosEu == true){UhrhgyosEu = false;}
      if(aCJYuPongz == true){aCJYuPongz = false;}
      if(AlgDZNyNQG == true){AlgDZNyNQG = false;}
      if(NWXcWPLhHK == true){NWXcWPLhHK = false;}
      if(VIObMpksBB == true){VIObMpksBB = false;}
      if(ShAUwiAoHT == true){ShAUwiAoHT = false;}
      if(AabxhDdgCe == true){AabxhDdgCe = false;}
      if(EHfVcqXEqi == true){EHfVcqXEqi = false;}
      if(cFMOnfVDfm == true){cFMOnfVDfm = false;}
      if(OVZmKtfQtI == true){OVZmKtfQtI = false;}
      if(MDpWxKBeqF == true){MDpWxKBeqF = false;}
      if(LwyYqtONJB == true){LwyYqtONJB = false;}
      if(haAOPigrKi == true){haAOPigrKi = false;}
      if(KGJhWYPpRc == true){KGJhWYPpRc = false;}
      if(dqLaaSXBwQ == true){dqLaaSXBwQ = false;}
      if(eSHrZkCaGN == true){eSHrZkCaGN = false;}
      if(cITItFlPBS == true){cITItFlPBS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PPYOLONJMN
{ 
  void RLbFDqkQcg()
  { 
      bool woZTHKzltS = false;
      bool YgMEmyXCEb = false;
      bool xWgtBQcNOO = false;
      bool KDpGCjQuoQ = false;
      bool OnpwMkXuFr = false;
      bool mXfHFZHeRp = false;
      bool hkWeHVqFCD = false;
      bool hbMmMIfkgQ = false;
      bool rhHVblXiAb = false;
      bool bkdzhdRKnq = false;
      bool tSsCKLLrtx = false;
      bool tWyBxAcADV = false;
      bool yIEZaxyNEi = false;
      bool qUwpVZnFLQ = false;
      bool aenChFmbBh = false;
      bool AQHSslNTaN = false;
      bool MxmLGYZHeX = false;
      bool LXomtSTBqV = false;
      bool JVMWiZNcwj = false;
      bool LUexDcrbwI = false;
      string ZIFpwrcMob;
      string CFULqaPrHu;
      string CiAAuVWkTk;
      string PIpkIXJgHk;
      string LMjDMfpJao;
      string TbbeApIFuZ;
      string UqrEMtOjCT;
      string QhkViozwmP;
      string fMKeRngLjR;
      string PElhQTTeqS;
      string QGGHYkpFuK;
      string qLResHfNmX;
      string NDzjDLwDFP;
      string UTEUzZGDGf;
      string yaPZaDlGnI;
      string mGrTFMGVDM;
      string omzfpXHxfT;
      string BcHGZlmZJz;
      string YpPxbOakPl;
      string SjzsqKajiD;
      if(ZIFpwrcMob == QGGHYkpFuK){woZTHKzltS = true;}
      else if(QGGHYkpFuK == ZIFpwrcMob){tSsCKLLrtx = true;}
      if(CFULqaPrHu == qLResHfNmX){YgMEmyXCEb = true;}
      else if(qLResHfNmX == CFULqaPrHu){tWyBxAcADV = true;}
      if(CiAAuVWkTk == NDzjDLwDFP){xWgtBQcNOO = true;}
      else if(NDzjDLwDFP == CiAAuVWkTk){yIEZaxyNEi = true;}
      if(PIpkIXJgHk == UTEUzZGDGf){KDpGCjQuoQ = true;}
      else if(UTEUzZGDGf == PIpkIXJgHk){qUwpVZnFLQ = true;}
      if(LMjDMfpJao == yaPZaDlGnI){OnpwMkXuFr = true;}
      else if(yaPZaDlGnI == LMjDMfpJao){aenChFmbBh = true;}
      if(TbbeApIFuZ == mGrTFMGVDM){mXfHFZHeRp = true;}
      else if(mGrTFMGVDM == TbbeApIFuZ){AQHSslNTaN = true;}
      if(UqrEMtOjCT == omzfpXHxfT){hkWeHVqFCD = true;}
      else if(omzfpXHxfT == UqrEMtOjCT){MxmLGYZHeX = true;}
      if(QhkViozwmP == BcHGZlmZJz){hbMmMIfkgQ = true;}
      if(fMKeRngLjR == YpPxbOakPl){rhHVblXiAb = true;}
      if(PElhQTTeqS == SjzsqKajiD){bkdzhdRKnq = true;}
      while(BcHGZlmZJz == QhkViozwmP){LXomtSTBqV = true;}
      while(YpPxbOakPl == YpPxbOakPl){JVMWiZNcwj = true;}
      while(SjzsqKajiD == SjzsqKajiD){LUexDcrbwI = true;}
      if(woZTHKzltS == true){woZTHKzltS = false;}
      if(YgMEmyXCEb == true){YgMEmyXCEb = false;}
      if(xWgtBQcNOO == true){xWgtBQcNOO = false;}
      if(KDpGCjQuoQ == true){KDpGCjQuoQ = false;}
      if(OnpwMkXuFr == true){OnpwMkXuFr = false;}
      if(mXfHFZHeRp == true){mXfHFZHeRp = false;}
      if(hkWeHVqFCD == true){hkWeHVqFCD = false;}
      if(hbMmMIfkgQ == true){hbMmMIfkgQ = false;}
      if(rhHVblXiAb == true){rhHVblXiAb = false;}
      if(bkdzhdRKnq == true){bkdzhdRKnq = false;}
      if(tSsCKLLrtx == true){tSsCKLLrtx = false;}
      if(tWyBxAcADV == true){tWyBxAcADV = false;}
      if(yIEZaxyNEi == true){yIEZaxyNEi = false;}
      if(qUwpVZnFLQ == true){qUwpVZnFLQ = false;}
      if(aenChFmbBh == true){aenChFmbBh = false;}
      if(AQHSslNTaN == true){AQHSslNTaN = false;}
      if(MxmLGYZHeX == true){MxmLGYZHeX = false;}
      if(LXomtSTBqV == true){LXomtSTBqV = false;}
      if(JVMWiZNcwj == true){JVMWiZNcwj = false;}
      if(LUexDcrbwI == true){LUexDcrbwI = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XHVXBBTDOI
{ 
  void zgAyggqekN()
  { 
      bool rkuFoXNOyG = false;
      bool sNIsONzYWI = false;
      bool KMtaLLEiIF = false;
      bool SZOtIQFXjd = false;
      bool KXBShIareZ = false;
      bool PegMtepUsc = false;
      bool SdGrmSzNky = false;
      bool IbnrylLbsC = false;
      bool kMEhpnrBAc = false;
      bool ZkdHKfRIeO = false;
      bool JMGgRmpoOH = false;
      bool SNaprGZXCz = false;
      bool ayMHXWxggR = false;
      bool TDwesXFXQC = false;
      bool xDlrJLKnOD = false;
      bool FsGemLAyJt = false;
      bool pJHwmPRuxT = false;
      bool BCHzCWLTyL = false;
      bool MJRcLZwQqh = false;
      bool WGOVLnuoEs = false;
      string yMMCmwZFwp;
      string hWcJJfXKlh;
      string sofnacKYkd;
      string pyBpIRzZBH;
      string jydfkzXKrE;
      string MDmkqIfYaJ;
      string kBTRziVlBr;
      string gxJextLzCa;
      string FzfgRlKPzz;
      string kayqVhdLfB;
      string ckJAWOtImw;
      string xfVADkUmAM;
      string YGmBGGzRJO;
      string PzSByUpfJD;
      string rCbCCFEynj;
      string MqLTcWyHtQ;
      string OcyYgxMTCi;
      string uocwUVdjzA;
      string BBcZBYmzYa;
      string WiksnODDVE;
      if(yMMCmwZFwp == ckJAWOtImw){rkuFoXNOyG = true;}
      else if(ckJAWOtImw == yMMCmwZFwp){JMGgRmpoOH = true;}
      if(hWcJJfXKlh == xfVADkUmAM){sNIsONzYWI = true;}
      else if(xfVADkUmAM == hWcJJfXKlh){SNaprGZXCz = true;}
      if(sofnacKYkd == YGmBGGzRJO){KMtaLLEiIF = true;}
      else if(YGmBGGzRJO == sofnacKYkd){ayMHXWxggR = true;}
      if(pyBpIRzZBH == PzSByUpfJD){SZOtIQFXjd = true;}
      else if(PzSByUpfJD == pyBpIRzZBH){TDwesXFXQC = true;}
      if(jydfkzXKrE == rCbCCFEynj){KXBShIareZ = true;}
      else if(rCbCCFEynj == jydfkzXKrE){xDlrJLKnOD = true;}
      if(MDmkqIfYaJ == MqLTcWyHtQ){PegMtepUsc = true;}
      else if(MqLTcWyHtQ == MDmkqIfYaJ){FsGemLAyJt = true;}
      if(kBTRziVlBr == OcyYgxMTCi){SdGrmSzNky = true;}
      else if(OcyYgxMTCi == kBTRziVlBr){pJHwmPRuxT = true;}
      if(gxJextLzCa == uocwUVdjzA){IbnrylLbsC = true;}
      if(FzfgRlKPzz == BBcZBYmzYa){kMEhpnrBAc = true;}
      if(kayqVhdLfB == WiksnODDVE){ZkdHKfRIeO = true;}
      while(uocwUVdjzA == gxJextLzCa){BCHzCWLTyL = true;}
      while(BBcZBYmzYa == BBcZBYmzYa){MJRcLZwQqh = true;}
      while(WiksnODDVE == WiksnODDVE){WGOVLnuoEs = true;}
      if(rkuFoXNOyG == true){rkuFoXNOyG = false;}
      if(sNIsONzYWI == true){sNIsONzYWI = false;}
      if(KMtaLLEiIF == true){KMtaLLEiIF = false;}
      if(SZOtIQFXjd == true){SZOtIQFXjd = false;}
      if(KXBShIareZ == true){KXBShIareZ = false;}
      if(PegMtepUsc == true){PegMtepUsc = false;}
      if(SdGrmSzNky == true){SdGrmSzNky = false;}
      if(IbnrylLbsC == true){IbnrylLbsC = false;}
      if(kMEhpnrBAc == true){kMEhpnrBAc = false;}
      if(ZkdHKfRIeO == true){ZkdHKfRIeO = false;}
      if(JMGgRmpoOH == true){JMGgRmpoOH = false;}
      if(SNaprGZXCz == true){SNaprGZXCz = false;}
      if(ayMHXWxggR == true){ayMHXWxggR = false;}
      if(TDwesXFXQC == true){TDwesXFXQC = false;}
      if(xDlrJLKnOD == true){xDlrJLKnOD = false;}
      if(FsGemLAyJt == true){FsGemLAyJt = false;}
      if(pJHwmPRuxT == true){pJHwmPRuxT = false;}
      if(BCHzCWLTyL == true){BCHzCWLTyL = false;}
      if(MJRcLZwQqh == true){MJRcLZwQqh = false;}
      if(WGOVLnuoEs == true){WGOVLnuoEs = false;}
    } 
}; 
