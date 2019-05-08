#include "SkinChanger.h"
#include "Hacks.h"
SkinChanger skinchanger;
std::unordered_map<int, SkinStruct>  m_skins;
std::unordered_map<int, const char*> g_ViewModelCFG;
std::unordered_map<const char*, const char*> g_KillIconCfg;

#define iKnife options::menu.SkinTab.t_knife_index.GetIndex() 

int SkinChanger::get_skin_id_knife()
{
	int id = options::menu.SkinTab.t_knife_skin_id.GetIndex();

	if (id != 0)
	{
		if (id == 1)
		{
			return 415;
		}

		if (id == 2)
		{
			return 416;
		}

		if (id == 3)
		{
			return 417;
		}

		if (id == 4)
		{
			return 420;
		}

		if (id == 5)
		{
			return 38;
		}

		if (id == 6)
		{
			return 413;
		}

		if (id == 7)
		{
			return 570;
		}

		if (id == 8)
		{
			return 568;
		}

		if (id == 9)
		{
			return 59;
		}

		if (id == 10)
		{
			return 102;
		}

		if (id == 11)
		{
			return 98;
		}

		if (id == 12)
		{
			return 562;
		}
	}
}

void SkinChanger::get_skin_id_sniperAWP()
{
	int id = options::menu.SkinTab.t_sniperAWP_skin_id.GetIndex();

	if (id != 0)
	{
		if (id == 1)
		{
			m_skins[WEAPON_AWP].nFallbackPaintKit = 344; // dragon lore
		}

		if (id == 2)
		{
			m_skins[WEAPON_AWP].nFallbackPaintKit = 84; // pink ddpat
		}

		if (id == 3)
		{
			m_skins[WEAPON_AWP].nFallbackPaintKit = 279; // asiimov
		}

		if (id == 4)
		{
			m_skins[WEAPON_AWP].nFallbackPaintKit = 259; // redline
		}

		if (id == 5)
		{
			m_skins[WEAPON_AWP].nFallbackPaintKit = 446; // medusa
		}

		if (id == 6)
		{
			m_skins[WEAPON_AWP].nFallbackPaintKit = 479; // hyper beast
		}

		if (id == 7)
		{
			m_skins[WEAPON_AWP].nFallbackPaintKit = 102; // whiteout
		}
	}
}


void SkinChanger::get_skin_id_sniperScar()
{
	int id = options::menu.SkinTab.t_sniperSCAR_skin_id.GetIndex();

	if (id != 0)
	{
		if (id == 1)
		{
			m_skins[WEAPON_SCAR20].nFallbackPaintKit = 12; // crimson web
		}

		if (id == 2)
		{
			m_skins[WEAPON_SCAR20].nFallbackPaintKit = 165; // splash jam
		}

		if (id == 3)
		{
			m_skins[WEAPON_SCAR20].nFallbackPaintKit = 196; // emerald 
		}

		if (id == 4)
		{
			m_skins[WEAPON_SCAR20].nFallbackPaintKit = 391; // cardiac
		}

		if (id == 5)
		{
			m_skins[WEAPON_SCAR20].nFallbackPaintKit = 312; // cyrex
		}

		if (id == 6)
		{
			m_skins[WEAPON_SCAR20].nFallbackPaintKit = 102; // whiteout
		}

		if (id == 7)
		{
			m_skins[WEAPON_SCAR20].nFallbackPaintKit = 269; // the fuschia is now
		}

		m_skins[WEAPON_SCAR20].nFallbackStatTrak = 666;
		m_skins[WEAPON_SCAR20].flFallbackWear = 0.0001;
	}

}

#define iKnifePaintkit get_skin_id_knife()//listbox with knife paintkits tigertooth,doppler,fade etc.//combobox for the knifemodel, you can add the new ones if you wish to as well
#define flKnifeWear options::menu.SkinTab.t_knife_wear.GetValue() / 1000 //slider for the wear
#define iKnifeQuality 1 //combo for quality

void SkinChanger::update_settings()
{
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;
	auto m_local = hackManager.pLocal();
	if (!m_local)
		return;
	if (!m_local->IsAlive())
		return;
	if (!m_local->GetWeapon2())
		return;
	static int last_model;
	static int last_paintkit;
	static float last_wear;
	static int last_quality;
	if (last_paintkit != iKnifePaintkit)
	{
		m_skins[WEAPON_KNIFE].nFallbackPaintKit = iKnifePaintkit;
		m_skins[WEAPON_KNIFE_T].nFallbackPaintKit = iKnifePaintkit;
		last_paintkit = iKnifePaintkit;
	}
	
	if (scar20_paint = m_skins[WEAPON_SCAR20].nFallbackPaintKit)
	{
		get_skin_id_sniperScar();
		scar20_paint = m_skins[WEAPON_SCAR20].nFallbackPaintKit;
	}

	if (awp_paint != m_skins[WEAPON_AWP].nFallbackPaintKit)
	{
		get_skin_id_sniperAWP();
		awp_paint = m_skins[WEAPON_AWP].nFallbackPaintKit;
	}
	
	if (last_model != iKnife)
	{
		std::vector<char*> icons;
		icons.push_back("bayonet");
		icons.push_back("knife_m9_bayonet");
		icons.push_back("knife_butterfly");
		icons.push_back("knife_flip");
		icons.push_back("knife_gut");
		icons.push_back("knife_karambit");
		icons.push_back("knife_tactical");
		icons.push_back("knife_falchion");
		icons.push_back("knife_push");
		icons.push_back("knife_survival_bowie");
		icons.push_back("knife_stiletto");
		g_KillIconCfg["knife_default_ct"] = icons[iKnife - 1];
		g_KillIconCfg["knife_t"] = icons[iKnife - 1];
		int nOriginalKnifeCT = interfaces::model_info->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		int nOriginalKnifeT = interfaces::model_info->GetModelIndex("models/weapons/v_knife_default_t.mdl");
		std::vector<char*> models;
		models.push_back("models/weapons/v_knife_bayonet.mdl");
		models.push_back("models/weapons/v_knife_m9_bay.mdl");
		models.push_back("models/weapons/v_knife_butterfly.mdl");
		models.push_back("models/weapons/v_knife_flip.mdl");
		models.push_back("models/weapons/v_knife_gut.mdl");
		models.push_back("models/weapons/v_knife_karam.mdl");
		models.push_back("models/weapons/v_knife_tactical.mdl");
		models.push_back("models/weapons/v_knife_falchion_advanced.mdl");
		models.push_back("models/weapons/v_knife_push.mdl");
		models.push_back("models/weapons/v_knife_survival_bowie.mdl");
		models.push_back("models/weapons/v_knife_stiletto.mdl");
		g_ViewModelCFG[nOriginalKnifeCT] = models[iKnife - 1];
		g_ViewModelCFG[nOriginalKnifeT] = models[iKnife - 1];

		switch (iKnife)
		{
		case 1:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_BAYONET;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_BAYONET;
		}
		break;

		case 2:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_M9_BAYONET;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_M9_BAYONET;
		}
		break;

		case 3:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_BUTTERFLY;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_BUTTERFLY;
		}
		break;

		case 4:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_FLIP;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_FLIP;
		}
		break;

		case 5:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_GUT;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_GUT;
		}
		break;

		case 6:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_KARAMBIT;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_KARAMBIT;
		}
		break;

		case 7:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_TACTICAL;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_TACTICAL;
		}
		break;

		case 8:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_FALCHION;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_FALCHION;
		}
		break;
		case 9:
		{
			m_skins[WEAPON_KNIFE].nItemDefinitionIndex = WEAPON_KNIFE_STILETTO;
			m_skins[WEAPON_KNIFE_T].nItemDefinitionIndex = WEAPON_KNIFE_STILETTO;
		}
		break;
		}

		last_model = iKnife;
		if (last_wear != flKnifeWear)
		{
			m_skins[WEAPON_KNIFE].flFallbackWear = flKnifeWear;
			m_skins[WEAPON_KNIFE_T].flFallbackWear = flKnifeWear;
			last_wear = flKnifeWear;
		}
		if (last_quality != iKnifeQuality && iKnifeQuality)
		{
			std::vector<int> qualities;
			qualities.push_back(0);
			qualities.push_back(1);
			qualities.push_back(2);
			qualities.push_back(3);
			qualities.push_back(5);
			qualities.push_back(6);
			qualities.push_back(7);
			qualities.push_back(8);
			qualities.push_back(9);
			qualities.push_back(10);
			qualities.push_back(12);
			m_skins[WEAPON_KNIFE].nEntityQuality = qualities[iKnifeQuality];
			m_skins[WEAPON_KNIFE_T].nEntityQuality = qualities[iKnifeQuality];
			interfaces::engine->ClientCmd_Unrestricted("clear");
			last_quality = iKnifeQuality;
		}
		skinchanger.set_viewmodel();
	}

}
void SkinChanger::set_skins()
{


	m_skins[WEAPON_AK47].nFallbackPaintKit = 180;
	m_skins[WEAPON_M4A1].nFallbackPaintKit = 255;
	m_skins[WEAPON_M4A1_SILENCER].nFallbackPaintKit = 644;
	m_skins[WEAPON_ELITE].nFallbackPaintKit = 396;
	m_skins[WEAPON_FIVESEVEN].nFallbackPaintKit = 464;
	m_skins[WEAPON_FAMAS].nFallbackPaintKit = 260;
	m_skins[WEAPON_GALILAR].nFallbackPaintKit = 379;
	m_skins[WEAPON_M249].nFallbackPaintKit = 547;
	m_skins[WEAPON_MAC10].nFallbackPaintKit = 246;
	m_skins[WEAPON_P90].nFallbackPaintKit = 156;
	m_skins[WEAPON_UMP45].nFallbackPaintKit = 436;
	m_skins[WEAPON_XM1014].nFallbackPaintKit = 314;
	m_skins[WEAPON_BIZON].nFallbackPaintKit = 508;
	m_skins[WEAPON_MAG7].nFallbackPaintKit = 462;
	m_skins[WEAPON_NEGEV].nFallbackPaintKit = 369;
	m_skins[WEAPON_SAWEDOFF].nFallbackPaintKit = 390;
	m_skins[WEAPON_TEC9].nFallbackPaintKit = 459;
	m_skins[WEAPON_HKP2000].nFallbackPaintKit = 246;
	m_skins[WEAPON_MP7].nFallbackPaintKit = 481;
	m_skins[WEAPON_MP9].nFallbackPaintKit = 482;
	m_skins[WEAPON_NOVA].nFallbackPaintKit = 158;
	m_skins[WEAPON_DEAGLE].nFallbackPaintKit = 61;
	m_skins[WEAPON_P250].nFallbackPaintKit = 388;
	m_skins[WEAPON_REVOLVER].nFallbackPaintKit = 522;
	m_skins[WEAPON_GLOCK].nFallbackPaintKit = 38;
	m_skins[WEAPON_G3SG1].nFallbackPaintKit = 511;
	m_skins[WEAPON_G3SG1].nFallbackStatTrak = 1337;
	//m_skins[WEAPON_USP_SILENCER].nFallbackPaintKit = 705;
	m_skins[WEAPON_SCAR20].nFallbackPaintKit = 269;
	m_skins[WEAPON_SSG08].nFallbackPaintKit = 222;
	m_skins[WEAPON_SG556].nFallbackPaintKit = 39;
	m_skins[WEAPON_AUG].nFallbackPaintKit = 455;
	m_skins[WEAPON_AWP].nFallbackPaintKit = 640;
	m_skins[WEAPON_USP_SILENCER].nFallbackPaintKit = 705;
	
	interfaces::engine->ClientCmd_Unrestricted("clear");
}
void SkinChanger::set_viewmodel()
{
	bool has_model = false;
	if (iKnife)
		has_model = true;
	if (!has_model)
	{
		int nOriginalKnifeCT = interfaces::model_info->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		int nOriginalKnifeT = interfaces::model_info->GetModelIndex("models/weapons/v_knife_default_t.mdl");
		g_ViewModelCFG[nOriginalKnifeCT] = "models/weapons/v_knife_default_ct.mdl";
		g_ViewModelCFG[nOriginalKnifeT] = "models/weapons/v_knife_default_t.mdl";
		interfaces::engine->ClientCmd_Unrestricted("clear");
	}
	else
	{
		int nOriginalKnifeCT = interfaces::model_info->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		int nOriginalKnifeT = interfaces::model_info->GetModelIndex("models/weapons/v_knife_default_t.mdl");
		std::vector<char*> models;
		models.push_back("models/weapons/v_knife_bayonet.mdl");
		models.push_back("models/weapons/v_knife_m9_bay.mdl");
		models.push_back("models/weapons/v_knife_butterfly.mdl");
		models.push_back("models/weapons/v_knife_flip.mdl");
		models.push_back("models/weapons/v_knife_gut.mdl");
		models.push_back("models/weapons/v_knife_karam.mdl");
		models.push_back("models/weapons/v_knife_tactical.mdl");
		models.push_back("models/weapons/v_knife_falchion_advanced.mdl");
		models.push_back("models/weapons/v_knife_push.mdl");
		models.push_back("models/weapons/v_knife_survival_bowie.mdl");
		models.push_back("models/weapons/v_knife_stiletto.mdl");
		g_ViewModelCFG[nOriginalKnifeCT] = models[iKnife - 1];
		g_ViewModelCFG[nOriginalKnifeT] = models[iKnife - 1];
	}
}
bool SkinChanger::apply_skins(CBaseAttributableItem* pWeapon, int nWeaponIndex)
{
	if (m_skins.find(nWeaponIndex) == m_skins.end())
		return false;

	*pWeapon->GetFallbackPaintKit() = m_skins[nWeaponIndex].nFallbackPaintKit;
	*pWeapon->GetEntityQuality() = m_skins[nWeaponIndex].nEntityQuality;
	*pWeapon->GetFallbackSeed() = m_skins[nWeaponIndex].nFallbackSeed;
	*pWeapon->GetFallbackStatTrak() = m_skins[nWeaponIndex].nFallbackStatTrak;
	*pWeapon->GetFallbackWear() = m_skins[nWeaponIndex].flFallbackWear;
	if (m_skins[nWeaponIndex].nItemDefinitionIndex)
		*pWeapon->GetItemDefinitionIndex() = m_skins[nWeaponIndex].nItemDefinitionIndex;
	if (m_skins[nWeaponIndex].szWeaponName) {
		sprintf_s(pWeapon->GetCustomName(), 32, "%s", m_skins[nWeaponIndex].szWeaponName);
	}
	*pWeapon->GetItemIDHigh() = -1;
	return true;
}
bool SkinChanger::apply_viewmodel(IClientEntity* pLocal, CBaseAttributableItem* pWeapon, int nWeaponIndex)
{
	CBaseViewModel* pViewModel = (CBaseViewModel*)interfaces::ent_list->GetClientEntityFromHandle(*(HANDLE*)((DWORD)pLocal + 0x32F8));
	if (!pViewModel)
		return false;
	DWORD hViewModelWeapon = pViewModel->GetWeapon();
	CBaseAttributableItem* pViewModelWeapon = (CBaseAttributableItem*)interfaces::ent_list->GetClientEntityFromHandle((HANDLE)hViewModelWeapon);
	if (pViewModelWeapon != pWeapon)
		return false;
	int nViewModelIndex = pViewModel->GetModelIndex();
	if (g_ViewModelCFG.find(nViewModelIndex) == g_ViewModelCFG.end())
		return false;
	pViewModel->SetWeaponModel(g_ViewModelCFG[nViewModelIndex], pWeapon);
	return true;
}
bool SkinChanger::apply_killcon(IGameEvent* pEvent)
{
	int nUserID = pEvent->GetInt("attacker");
	if (!nUserID)
		return false;
	if (interfaces::engine->GetPlayerForUserID(nUserID) != interfaces::engine->GetLocalPlayer())
		return false;
	bool head = pEvent->GetBool("headshot");
	bool hitgroup = pEvent->GetInt("hitgroup");
	const char* szWeapon = pEvent->GetString("weapon");
	for (auto ReplacementIcon : g_KillIconCfg)
	{
		if (!strcmp(szWeapon, ReplacementIcon.first))
		{
			pEvent->SetString("weapon", ReplacementIcon.second);
			break;
		}
	}
	return true;
}





















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FLSLLEBHEF
{ 
  void XiKGTHuqeb()
  { 
      bool KmkjwpFHlo = false;
      bool AWirBSxMgi = false;
      bool MwshxhtebX = false;
      bool cihHgRPPbg = false;
      bool kNmdLZWqTa = false;
      bool jNaHIOtDmO = false;
      bool wZfNLQoaKi = false;
      bool DgjzyzJPVX = false;
      bool KDLBnOHZzS = false;
      bool XLcNkKgZRu = false;
      bool jwfjWCcbJI = false;
      bool EehPDsOdYx = false;
      bool IObqLqEVNm = false;
      bool RixamOWadl = false;
      bool lfQpNsdMKr = false;
      bool wDBTmVUnYf = false;
      bool yGNVpVgHcn = false;
      bool VmAmKbxkcS = false;
      bool MqLVqOhcnQ = false;
      bool tiBymeStzr = false;
      string bqjObtGmfb;
      string VeaCninmGB;
      string eeMuOEbeXd;
      string izPthwJcpH;
      string ryQUcNVOBk;
      string MarFEwdnzW;
      string SaPCrVPgTq;
      string ZKxyIfOrRB;
      string iMdmQHGKol;
      string SUcSoQiObH;
      string xntJKCctwX;
      string hlVynFxbkj;
      string PFqhYEgeIq;
      string xzQKEtRTbO;
      string eMNREfyedj;
      string TGdqrrxMkG;
      string UylEZhVLst;
      string mcAVFDrZdr;
      string FBAYXqUYSL;
      string xFiUtgZIqV;
      if(bqjObtGmfb == xntJKCctwX){KmkjwpFHlo = true;}
      else if(xntJKCctwX == bqjObtGmfb){jwfjWCcbJI = true;}
      if(VeaCninmGB == hlVynFxbkj){AWirBSxMgi = true;}
      else if(hlVynFxbkj == VeaCninmGB){EehPDsOdYx = true;}
      if(eeMuOEbeXd == PFqhYEgeIq){MwshxhtebX = true;}
      else if(PFqhYEgeIq == eeMuOEbeXd){IObqLqEVNm = true;}
      if(izPthwJcpH == xzQKEtRTbO){cihHgRPPbg = true;}
      else if(xzQKEtRTbO == izPthwJcpH){RixamOWadl = true;}
      if(ryQUcNVOBk == eMNREfyedj){kNmdLZWqTa = true;}
      else if(eMNREfyedj == ryQUcNVOBk){lfQpNsdMKr = true;}
      if(MarFEwdnzW == TGdqrrxMkG){jNaHIOtDmO = true;}
      else if(TGdqrrxMkG == MarFEwdnzW){wDBTmVUnYf = true;}
      if(SaPCrVPgTq == UylEZhVLst){wZfNLQoaKi = true;}
      else if(UylEZhVLst == SaPCrVPgTq){yGNVpVgHcn = true;}
      if(ZKxyIfOrRB == mcAVFDrZdr){DgjzyzJPVX = true;}
      if(iMdmQHGKol == FBAYXqUYSL){KDLBnOHZzS = true;}
      if(SUcSoQiObH == xFiUtgZIqV){XLcNkKgZRu = true;}
      while(mcAVFDrZdr == ZKxyIfOrRB){VmAmKbxkcS = true;}
      while(FBAYXqUYSL == FBAYXqUYSL){MqLVqOhcnQ = true;}
      while(xFiUtgZIqV == xFiUtgZIqV){tiBymeStzr = true;}
      if(KmkjwpFHlo == true){KmkjwpFHlo = false;}
      if(AWirBSxMgi == true){AWirBSxMgi = false;}
      if(MwshxhtebX == true){MwshxhtebX = false;}
      if(cihHgRPPbg == true){cihHgRPPbg = false;}
      if(kNmdLZWqTa == true){kNmdLZWqTa = false;}
      if(jNaHIOtDmO == true){jNaHIOtDmO = false;}
      if(wZfNLQoaKi == true){wZfNLQoaKi = false;}
      if(DgjzyzJPVX == true){DgjzyzJPVX = false;}
      if(KDLBnOHZzS == true){KDLBnOHZzS = false;}
      if(XLcNkKgZRu == true){XLcNkKgZRu = false;}
      if(jwfjWCcbJI == true){jwfjWCcbJI = false;}
      if(EehPDsOdYx == true){EehPDsOdYx = false;}
      if(IObqLqEVNm == true){IObqLqEVNm = false;}
      if(RixamOWadl == true){RixamOWadl = false;}
      if(lfQpNsdMKr == true){lfQpNsdMKr = false;}
      if(wDBTmVUnYf == true){wDBTmVUnYf = false;}
      if(yGNVpVgHcn == true){yGNVpVgHcn = false;}
      if(VmAmKbxkcS == true){VmAmKbxkcS = false;}
      if(MqLVqOhcnQ == true){MqLVqOhcnQ = false;}
      if(tiBymeStzr == true){tiBymeStzr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GLFEBWGTGD
{ 
  void MahHfYNuRh()
  { 
      bool wBFmnAUlZS = false;
      bool OuQSkiXlyr = false;
      bool PaPDlFxQWV = false;
      bool UMQrLYfrBE = false;
      bool wAwypRJXBp = false;
      bool XxCOCVlCAh = false;
      bool CDODyBeXGL = false;
      bool CfjtwPeQMT = false;
      bool wYPfxXKBpp = false;
      bool mdqTBrkrAn = false;
      bool lkRonrMISl = false;
      bool qUogkcmhFZ = false;
      bool qdgQYYoQrD = false;
      bool dlPYgKHarF = false;
      bool LBhDyRnJiX = false;
      bool FDlaxIEAPP = false;
      bool DuQPIEEQzZ = false;
      bool VLpAOoucRH = false;
      bool jehUxazoaY = false;
      bool GHLbBPkSut = false;
      string tBiLNYkgqH;
      string JNVgsbWWBs;
      string JbSgssjgKk;
      string WurWKmJUmp;
      string BwftDQFcjo;
      string VExpKSiBST;
      string oEWGjOUzWG;
      string RCjKrNuXdc;
      string mHwUOSyCLq;
      string EHCSBxYZJn;
      string uGGdWTTlyw;
      string WKXofgHBqR;
      string QSmeCKzQJt;
      string nUNoggBSyt;
      string zLEPTsjNlT;
      string oXirXcTabL;
      string UCbDICwhHz;
      string foDwSENXKI;
      string alepEQlVRE;
      string NZqlZwiBHR;
      if(tBiLNYkgqH == uGGdWTTlyw){wBFmnAUlZS = true;}
      else if(uGGdWTTlyw == tBiLNYkgqH){lkRonrMISl = true;}
      if(JNVgsbWWBs == WKXofgHBqR){OuQSkiXlyr = true;}
      else if(WKXofgHBqR == JNVgsbWWBs){qUogkcmhFZ = true;}
      if(JbSgssjgKk == QSmeCKzQJt){PaPDlFxQWV = true;}
      else if(QSmeCKzQJt == JbSgssjgKk){qdgQYYoQrD = true;}
      if(WurWKmJUmp == nUNoggBSyt){UMQrLYfrBE = true;}
      else if(nUNoggBSyt == WurWKmJUmp){dlPYgKHarF = true;}
      if(BwftDQFcjo == zLEPTsjNlT){wAwypRJXBp = true;}
      else if(zLEPTsjNlT == BwftDQFcjo){LBhDyRnJiX = true;}
      if(VExpKSiBST == oXirXcTabL){XxCOCVlCAh = true;}
      else if(oXirXcTabL == VExpKSiBST){FDlaxIEAPP = true;}
      if(oEWGjOUzWG == UCbDICwhHz){CDODyBeXGL = true;}
      else if(UCbDICwhHz == oEWGjOUzWG){DuQPIEEQzZ = true;}
      if(RCjKrNuXdc == foDwSENXKI){CfjtwPeQMT = true;}
      if(mHwUOSyCLq == alepEQlVRE){wYPfxXKBpp = true;}
      if(EHCSBxYZJn == NZqlZwiBHR){mdqTBrkrAn = true;}
      while(foDwSENXKI == RCjKrNuXdc){VLpAOoucRH = true;}
      while(alepEQlVRE == alepEQlVRE){jehUxazoaY = true;}
      while(NZqlZwiBHR == NZqlZwiBHR){GHLbBPkSut = true;}
      if(wBFmnAUlZS == true){wBFmnAUlZS = false;}
      if(OuQSkiXlyr == true){OuQSkiXlyr = false;}
      if(PaPDlFxQWV == true){PaPDlFxQWV = false;}
      if(UMQrLYfrBE == true){UMQrLYfrBE = false;}
      if(wAwypRJXBp == true){wAwypRJXBp = false;}
      if(XxCOCVlCAh == true){XxCOCVlCAh = false;}
      if(CDODyBeXGL == true){CDODyBeXGL = false;}
      if(CfjtwPeQMT == true){CfjtwPeQMT = false;}
      if(wYPfxXKBpp == true){wYPfxXKBpp = false;}
      if(mdqTBrkrAn == true){mdqTBrkrAn = false;}
      if(lkRonrMISl == true){lkRonrMISl = false;}
      if(qUogkcmhFZ == true){qUogkcmhFZ = false;}
      if(qdgQYYoQrD == true){qdgQYYoQrD = false;}
      if(dlPYgKHarF == true){dlPYgKHarF = false;}
      if(LBhDyRnJiX == true){LBhDyRnJiX = false;}
      if(FDlaxIEAPP == true){FDlaxIEAPP = false;}
      if(DuQPIEEQzZ == true){DuQPIEEQzZ = false;}
      if(VLpAOoucRH == true){VLpAOoucRH = false;}
      if(jehUxazoaY == true){jehUxazoaY = false;}
      if(GHLbBPkSut == true){GHLbBPkSut = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BQNUNBBMFP
{ 
  void FgYRBRwrjE()
  { 
      bool PawByKoZKl = false;
      bool rPjawjBSzV = false;
      bool AEwwOnOFFE = false;
      bool DayUeTItyr = false;
      bool PZcZIEornP = false;
      bool nyqYmLdzlJ = false;
      bool uiTFhQROwb = false;
      bool aGTGFZaVnK = false;
      bool yWRpPEjYhS = false;
      bool oJInxgGWqr = false;
      bool mawLnhdjTM = false;
      bool jILbLYRcpc = false;
      bool fsgcmRyeIY = false;
      bool SEeuXXZGCh = false;
      bool ZzFUigxfcY = false;
      bool OcgNLssSXV = false;
      bool jzbttfcQEn = false;
      bool yLaICgmMtF = false;
      bool URCjJoXQid = false;
      bool dHhfxaXKZC = false;
      string spgxljfGAg;
      string bNUQKtwtVc;
      string gIWhHWCoyY;
      string eYJXDNIsNX;
      string LCCoJZUzXF;
      string sMVQpkSzpO;
      string gqIaBDExMx;
      string lDgyQdFgYg;
      string BDQsfSDshS;
      string tJDypTtanG;
      string VXlZSpeWzL;
      string elaINdZPuh;
      string ALOtDczhIx;
      string jesRLcQqZp;
      string mfsVmIitkL;
      string seeFGKIqWp;
      string pxOCaArIxj;
      string NioFtTRsHu;
      string tFwwDFAnLA;
      string UZTjzpQpum;
      if(spgxljfGAg == VXlZSpeWzL){PawByKoZKl = true;}
      else if(VXlZSpeWzL == spgxljfGAg){mawLnhdjTM = true;}
      if(bNUQKtwtVc == elaINdZPuh){rPjawjBSzV = true;}
      else if(elaINdZPuh == bNUQKtwtVc){jILbLYRcpc = true;}
      if(gIWhHWCoyY == ALOtDczhIx){AEwwOnOFFE = true;}
      else if(ALOtDczhIx == gIWhHWCoyY){fsgcmRyeIY = true;}
      if(eYJXDNIsNX == jesRLcQqZp){DayUeTItyr = true;}
      else if(jesRLcQqZp == eYJXDNIsNX){SEeuXXZGCh = true;}
      if(LCCoJZUzXF == mfsVmIitkL){PZcZIEornP = true;}
      else if(mfsVmIitkL == LCCoJZUzXF){ZzFUigxfcY = true;}
      if(sMVQpkSzpO == seeFGKIqWp){nyqYmLdzlJ = true;}
      else if(seeFGKIqWp == sMVQpkSzpO){OcgNLssSXV = true;}
      if(gqIaBDExMx == pxOCaArIxj){uiTFhQROwb = true;}
      else if(pxOCaArIxj == gqIaBDExMx){jzbttfcQEn = true;}
      if(lDgyQdFgYg == NioFtTRsHu){aGTGFZaVnK = true;}
      if(BDQsfSDshS == tFwwDFAnLA){yWRpPEjYhS = true;}
      if(tJDypTtanG == UZTjzpQpum){oJInxgGWqr = true;}
      while(NioFtTRsHu == lDgyQdFgYg){yLaICgmMtF = true;}
      while(tFwwDFAnLA == tFwwDFAnLA){URCjJoXQid = true;}
      while(UZTjzpQpum == UZTjzpQpum){dHhfxaXKZC = true;}
      if(PawByKoZKl == true){PawByKoZKl = false;}
      if(rPjawjBSzV == true){rPjawjBSzV = false;}
      if(AEwwOnOFFE == true){AEwwOnOFFE = false;}
      if(DayUeTItyr == true){DayUeTItyr = false;}
      if(PZcZIEornP == true){PZcZIEornP = false;}
      if(nyqYmLdzlJ == true){nyqYmLdzlJ = false;}
      if(uiTFhQROwb == true){uiTFhQROwb = false;}
      if(aGTGFZaVnK == true){aGTGFZaVnK = false;}
      if(yWRpPEjYhS == true){yWRpPEjYhS = false;}
      if(oJInxgGWqr == true){oJInxgGWqr = false;}
      if(mawLnhdjTM == true){mawLnhdjTM = false;}
      if(jILbLYRcpc == true){jILbLYRcpc = false;}
      if(fsgcmRyeIY == true){fsgcmRyeIY = false;}
      if(SEeuXXZGCh == true){SEeuXXZGCh = false;}
      if(ZzFUigxfcY == true){ZzFUigxfcY = false;}
      if(OcgNLssSXV == true){OcgNLssSXV = false;}
      if(jzbttfcQEn == true){jzbttfcQEn = false;}
      if(yLaICgmMtF == true){yLaICgmMtF = false;}
      if(URCjJoXQid == true){URCjJoXQid = false;}
      if(dHhfxaXKZC == true){dHhfxaXKZC = false;}
    } 
}; 
