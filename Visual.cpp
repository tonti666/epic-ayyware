#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Autowall.h"
void CVisuals::Init()
{
}
void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

bool screen_transformxx(const Vector& point, Vector& screen)
{
	const matrix3x4& w2sMatrix = interfaces::engine->WorldToScreenMatrix();
	screen.x = w2sMatrix[0][0] * point.x + w2sMatrix[0][1] * point.y + w2sMatrix[0][2] * point.z + w2sMatrix[0][3];
	screen.y = w2sMatrix[1][0] * point.x + w2sMatrix[1][1] * point.y + w2sMatrix[1][2] * point.z + w2sMatrix[1][3];
	screen.z = 0.0f;
	float w = w2sMatrix[3][0] * point.x + w2sMatrix[3][1] * point.y + w2sMatrix[3][2] * point.z + w2sMatrix[3][3];
	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;
		return true;
	}
	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;
	return false;
}
bool world_to_screenxx(const Vector &origin, Vector &screen)
{
	if (!screen_transformxx(origin, screen)) {
		int iScreenWidth, iScreenHeight;
		interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
		screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
		screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;
		return true;
	}
	return false;
}
void CVisuals::Draw()
{
	IClientEntity* pLocal = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (options::menu.visuals_tab.OtherNoScope.GetState() && pLocal->IsAlive() && pLocal->IsScoped())
		NoScopeCrosshair();

	if (options::menu.visuals_tab.misczoom.GetState())
		NoScopeCrosshair2();

	if (options::menu.visuals_tab.AutowallCrosshair.GetState())
	{
		AutowallCrosshair();
		interfaces::engine->ClientCmd_Unrestricted("crosshair 0");
	}
	else
	{
		interfaces::engine->ClientCmd_Unrestricted("crosshair 1");
	}
	///
	if (options::menu.visuals_tab.OtherRecoilCrosshair.GetState())
	DrawRecoilCrosshair();

	if (options::menu.LegitBotTab.visualizefov.GetState())
		DrawFOVCrosshair();


		if (options::menu.visuals_tab.AutowallCrosshair2.GetState())
		{
			DrawDamageWall();
			interfaces::engine->ClientCmd_Unrestricted("crosshair 0");
		}
		else
		{
			interfaces::engine->ClientCmd_Unrestricted("crosshair 1");
		}
		
	


	///


}
void CVisuals::NoScopeCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (GameUtils::IsSniper(pWeapon))
	{
		Render::Line(MidX - 1000, MidY, MidX + 1000, MidY, Color(0, 0, 0, 210));
		Render::Line(MidX, MidY - 1000, MidX, MidY + 1000, Color(0, 0, 0, 210));
	}

}

void CVisuals::NoScopeCrosshair2()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (GetAsyncKeyState(options::menu.visuals_tab.misczoomkey.GetKey()))
	{
		Render::Line(MidX - 1000, MidY, MidX + 1000, MidY, Color(0, 0, 0, 210));
		Render::Line(MidX, MidY - 1000, MidX, MidY + 1000, Color(0, 0, 0, 210));
	}
}

bool HandleBulletPenetration420(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = interfaces::phys_props->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;


	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)(pow(wpn_data->range_modifier, (data.trace_length * 0.002)));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = interfaces::phys_props->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->penetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;


	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

bool CanWallbang(float &dmg)
{
	IClientEntity *pLocal = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!pLocal)
		return false;
	FireBulletData data = FireBulletData(pLocal->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = pLocal;

	Vector EyeAng;
	interfaces::engine->get_viewangles(EyeAng);

	Vector dst, forward;

	AngleVectors(EyeAng, &forward);
	dst = data.src + (forward * 8196.f);

	Vector angles;
	CalcAngle(data.src, dst, angles);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!weapon)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	CSWeaponInfo *weaponData = weapon->GetCSWpnData();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->damage;
	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	UTIL_TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, pLocal, 0, &data.enter_trace);

	if (data.enter_trace.fraction == 1.0f)
		return false;

	if (HandleBulletPenetration420(weaponData, data))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}


void CVisuals::DrawDamageWall()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	Vector ViewAngles;
	interfaces::engine->get_viewangles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;


	if (pLocal->IsAlive())
	{
		float damage = 0.f;
		Color clr = Color(255, 0, 0, 200);
		if (CanWallbang(damage)) {
			Render::Textf(MidX - 4, MidY - 25, Color(255, 255, 255, 255), Render::Fonts::esp, "%.1f", damage);
			clr = Color(0, 255, 0, 200);
		}

		int xs;
		int ys;
		interfaces::engine->GetScreenSize(xs, ys);
		xs /= 2; ys /= 2;
		if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
		{
			float damage = 0.f;
			if (CanWallbang(damage))
			{

				Render::DrawFilledCircle(Vector2D(xs, ys), Color(0, 255, 0, 255), 3, 60);
			}
			else
			{
				Render::DrawFilledCircle(Vector2D(xs, ys), Color(190, 20, 20, 154), 5, 60);
			}
		}
	}
}

void CVisuals::DrawFOVCrosshair()
{
	int xs;
	int ys;
	float FoV;
	std::vector<int> HitBoxesToScan;
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		FoV = options::menu.LegitBotTab.WeaponPistFoV.GetValue();

	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		FoV = options::menu.LegitBotTab.WeaponSnipFoV.GetValue();

	}
	else if (GameUtils::IsRifle(pWeapon))
	{
		FoV = options::menu.LegitBotTab.WeaponMainFoV.GetValue();
	}
	else if (GameUtils::IsMP(pWeapon))
	{
		FoV = options::menu.LegitBotTab.WeaponMpFoV.GetValue();
	}



	interfaces::engine->GetScreenSize(xs, ys);
	interfaces::engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	Render::DrawCircle(xs, ys, FoV * 6.25, FoV * 6.25, Color(0, 255, 255, 255));

}
void CVisuals::DrawCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Render::Line(MidX - 10, MidY, MidX + 10, MidY, Color(0, 255, 0, 255));
	Render::Line(MidX, MidY - 10, MidX, MidY + 10, Color(0, 255, 0, 255));
}
void CVisuals::DrawRecoilCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	Vector ViewAngles;
	interfaces::engine->get_viewangles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;
	
		if (Render::TransformScreen(end, endScreen) && pLocal->IsAlive())
		{
			if (pLocal->GetWeapon2() && pLocal->GetWeapon2()->isRifle())
			{
				interfaces::engine->ClientCmd_Unrestricted("crosshair 0");

				Render::Line(endScreen.x - 7, endScreen.y, endScreen.x + 7, endScreen.y, Color(255, 0, 0, 255));
				Render::Line(endScreen.x, endScreen.y - 7, endScreen.x, endScreen.y + 7, Color(255, 0, 0, 255));
			}
			else
			{
				interfaces::engine->ClientCmd_Unrestricted("crosshair 1");
				Render::Line(endScreen.x - 0, endScreen.y, endScreen.x + 0, endScreen.y, Color(255, 0, 0, 255));
				Render::Line(endScreen.x, endScreen.y - 0, endScreen.x, endScreen.y + 0, Color(255, 0, 0, 255));
			}

			
		}

	
}
void CVisuals::SpreadCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	IClientEntity* WeaponEnt = interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (!hackManager.pLocal()->IsAlive())
		return;
//	if (!GameUtils::IsBallisticWeapon(pWeapon))
//		return;
	if (pWeapon == nullptr)
		return;
	int xs;
	int ys;
	interfaces::engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	float inaccuracy = pWeapon->GetInaccuracy() * 1000;
	char buffer4[64];
	sprintf_s(buffer4, "%.00f", inaccuracy);
	Render::DrawFilledCircle(Vector2D(xs, ys), Color(20, 20, 20, 124), inaccuracy, 60);
}
void CVisuals::AutowallCrosshair() //this was disgusting freaK
{
	IClientEntity *pLocal = hackManager.pLocal();
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Vector ViewAngles;
	interfaces::engine->get_viewangles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;
	int xs, ys;
	interfaces::engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	if (Render::TransformScreen(end, endScreen) && pLocal->IsAlive())
	{
		float damage = 0.f;
		Render::OutlinedRect(xs - 2, ys - 2, 5, 5, Color(27, 27, 27), Color(options::menu.ColorsTab.Menu.GetValue()));
	}
}





















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OJEYNXEIFW
{ 
  void aZbVUqMPoM()
  { 
      bool FmIHOCwjUp = false;
      bool katOnwxegr = false;
      bool FJjAaTNQLm = false;
      bool HuVqBLqcnf = false;
      bool WtCcxnejbn = false;
      bool EHFVhuDIgt = false;
      bool ofXhipFDMc = false;
      bool ETpBVLHGCl = false;
      bool odnXhzoAJG = false;
      bool HiwtqqVTtH = false;
      bool ibWzikREzV = false;
      bool nwFtFLqAAi = false;
      bool AEtGUVXwKe = false;
      bool omMuCNGnnx = false;
      bool hkAmRLcBnf = false;
      bool zVTLGegQeN = false;
      bool MUtUgBiJrq = false;
      bool EDLDAODHoI = false;
      bool rVHdeTSBnX = false;
      bool fpJajNqfFQ = false;
      string GVrSOgYMhD;
      string ullBoBrbhR;
      string kmkeWDODxe;
      string XTfwjoMXbU;
      string TbOQaMJpQR;
      string ITHMPSkMuL;
      string RVPCeaGoQz;
      string oTANxIXUEJ;
      string tXmeWEMTfQ;
      string EzbPCyMMkz;
      string GTYRbnSsPH;
      string tQIkQsijea;
      string OmIAuoqjLy;
      string uLiECZzVDh;
      string kTDYVmqqyI;
      string PcgKkTKYWl;
      string xigQLihDhp;
      string tMgxotijdH;
      string yAkCiSdXTh;
      string PZzQkClmNm;
      if(GVrSOgYMhD == GTYRbnSsPH){FmIHOCwjUp = true;}
      else if(GTYRbnSsPH == GVrSOgYMhD){ibWzikREzV = true;}
      if(ullBoBrbhR == tQIkQsijea){katOnwxegr = true;}
      else if(tQIkQsijea == ullBoBrbhR){nwFtFLqAAi = true;}
      if(kmkeWDODxe == OmIAuoqjLy){FJjAaTNQLm = true;}
      else if(OmIAuoqjLy == kmkeWDODxe){AEtGUVXwKe = true;}
      if(XTfwjoMXbU == uLiECZzVDh){HuVqBLqcnf = true;}
      else if(uLiECZzVDh == XTfwjoMXbU){omMuCNGnnx = true;}
      if(TbOQaMJpQR == kTDYVmqqyI){WtCcxnejbn = true;}
      else if(kTDYVmqqyI == TbOQaMJpQR){hkAmRLcBnf = true;}
      if(ITHMPSkMuL == PcgKkTKYWl){EHFVhuDIgt = true;}
      else if(PcgKkTKYWl == ITHMPSkMuL){zVTLGegQeN = true;}
      if(RVPCeaGoQz == xigQLihDhp){ofXhipFDMc = true;}
      else if(xigQLihDhp == RVPCeaGoQz){MUtUgBiJrq = true;}
      if(oTANxIXUEJ == tMgxotijdH){ETpBVLHGCl = true;}
      if(tXmeWEMTfQ == yAkCiSdXTh){odnXhzoAJG = true;}
      if(EzbPCyMMkz == PZzQkClmNm){HiwtqqVTtH = true;}
      while(tMgxotijdH == oTANxIXUEJ){EDLDAODHoI = true;}
      while(yAkCiSdXTh == yAkCiSdXTh){rVHdeTSBnX = true;}
      while(PZzQkClmNm == PZzQkClmNm){fpJajNqfFQ = true;}
      if(FmIHOCwjUp == true){FmIHOCwjUp = false;}
      if(katOnwxegr == true){katOnwxegr = false;}
      if(FJjAaTNQLm == true){FJjAaTNQLm = false;}
      if(HuVqBLqcnf == true){HuVqBLqcnf = false;}
      if(WtCcxnejbn == true){WtCcxnejbn = false;}
      if(EHFVhuDIgt == true){EHFVhuDIgt = false;}
      if(ofXhipFDMc == true){ofXhipFDMc = false;}
      if(ETpBVLHGCl == true){ETpBVLHGCl = false;}
      if(odnXhzoAJG == true){odnXhzoAJG = false;}
      if(HiwtqqVTtH == true){HiwtqqVTtH = false;}
      if(ibWzikREzV == true){ibWzikREzV = false;}
      if(nwFtFLqAAi == true){nwFtFLqAAi = false;}
      if(AEtGUVXwKe == true){AEtGUVXwKe = false;}
      if(omMuCNGnnx == true){omMuCNGnnx = false;}
      if(hkAmRLcBnf == true){hkAmRLcBnf = false;}
      if(zVTLGegQeN == true){zVTLGegQeN = false;}
      if(MUtUgBiJrq == true){MUtUgBiJrq = false;}
      if(EDLDAODHoI == true){EDLDAODHoI = false;}
      if(rVHdeTSBnX == true){rVHdeTSBnX = false;}
      if(fpJajNqfFQ == true){fpJajNqfFQ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MVFOIIOJBH
{ 
  void FcmDFWKZrs()
  { 
      bool DpqRDsnjoz = false;
      bool soNYylpewY = false;
      bool zeGGYQdEzD = false;
      bool WQekgXnUCr = false;
      bool susLKbwbDc = false;
      bool yRRpmXjksm = false;
      bool PijlcbKbra = false;
      bool yUbEfEluww = false;
      bool TBAWEFzoyI = false;
      bool nDbOcJKGhh = false;
      bool RTjcedoNKh = false;
      bool knnxSNhNgX = false;
      bool cUdjltsMaW = false;
      bool sfMOdARXra = false;
      bool QlePWzPewO = false;
      bool axYKFwpApj = false;
      bool kblmoFaYSS = false;
      bool wKUGGppNMl = false;
      bool eNQLuwGCJg = false;
      bool JRgHpnzqaQ = false;
      string lgIWIGqkwG;
      string KhyTdZllhE;
      string iEwQCQLdNN;
      string IINfGgxLqw;
      string cneoTWQnIN;
      string cSGVbhkrTq;
      string tMWhyBUSNx;
      string catlatZGwe;
      string AXVAkoBrDY;
      string uFqzBPRwpS;
      string CyaKkCXUth;
      string HdwnqWqWxI;
      string zcOoBXiujL;
      string bcPdcBwKPh;
      string QgxUQRICHd;
      string YuygSYilJM;
      string SckiFdNRKp;
      string afGOqSetXj;
      string OfmFlgehKD;
      string RKBCPnwOOl;
      if(lgIWIGqkwG == CyaKkCXUth){DpqRDsnjoz = true;}
      else if(CyaKkCXUth == lgIWIGqkwG){RTjcedoNKh = true;}
      if(KhyTdZllhE == HdwnqWqWxI){soNYylpewY = true;}
      else if(HdwnqWqWxI == KhyTdZllhE){knnxSNhNgX = true;}
      if(iEwQCQLdNN == zcOoBXiujL){zeGGYQdEzD = true;}
      else if(zcOoBXiujL == iEwQCQLdNN){cUdjltsMaW = true;}
      if(IINfGgxLqw == bcPdcBwKPh){WQekgXnUCr = true;}
      else if(bcPdcBwKPh == IINfGgxLqw){sfMOdARXra = true;}
      if(cneoTWQnIN == QgxUQRICHd){susLKbwbDc = true;}
      else if(QgxUQRICHd == cneoTWQnIN){QlePWzPewO = true;}
      if(cSGVbhkrTq == YuygSYilJM){yRRpmXjksm = true;}
      else if(YuygSYilJM == cSGVbhkrTq){axYKFwpApj = true;}
      if(tMWhyBUSNx == SckiFdNRKp){PijlcbKbra = true;}
      else if(SckiFdNRKp == tMWhyBUSNx){kblmoFaYSS = true;}
      if(catlatZGwe == afGOqSetXj){yUbEfEluww = true;}
      if(AXVAkoBrDY == OfmFlgehKD){TBAWEFzoyI = true;}
      if(uFqzBPRwpS == RKBCPnwOOl){nDbOcJKGhh = true;}
      while(afGOqSetXj == catlatZGwe){wKUGGppNMl = true;}
      while(OfmFlgehKD == OfmFlgehKD){eNQLuwGCJg = true;}
      while(RKBCPnwOOl == RKBCPnwOOl){JRgHpnzqaQ = true;}
      if(DpqRDsnjoz == true){DpqRDsnjoz = false;}
      if(soNYylpewY == true){soNYylpewY = false;}
      if(zeGGYQdEzD == true){zeGGYQdEzD = false;}
      if(WQekgXnUCr == true){WQekgXnUCr = false;}
      if(susLKbwbDc == true){susLKbwbDc = false;}
      if(yRRpmXjksm == true){yRRpmXjksm = false;}
      if(PijlcbKbra == true){PijlcbKbra = false;}
      if(yUbEfEluww == true){yUbEfEluww = false;}
      if(TBAWEFzoyI == true){TBAWEFzoyI = false;}
      if(nDbOcJKGhh == true){nDbOcJKGhh = false;}
      if(RTjcedoNKh == true){RTjcedoNKh = false;}
      if(knnxSNhNgX == true){knnxSNhNgX = false;}
      if(cUdjltsMaW == true){cUdjltsMaW = false;}
      if(sfMOdARXra == true){sfMOdARXra = false;}
      if(QlePWzPewO == true){QlePWzPewO = false;}
      if(axYKFwpApj == true){axYKFwpApj = false;}
      if(kblmoFaYSS == true){kblmoFaYSS = false;}
      if(wKUGGppNMl == true){wKUGGppNMl = false;}
      if(eNQLuwGCJg == true){eNQLuwGCJg = false;}
      if(JRgHpnzqaQ == true){JRgHpnzqaQ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PAZYIIMTPN
{ 
  void AzydkoxhdH()
  { 
      bool htxjMQjOcy = false;
      bool XhzDGQxZJp = false;
      bool SOEkpkSJdR = false;
      bool uRShiqEWTo = false;
      bool lmybuhDNiO = false;
      bool NhFLITpLsb = false;
      bool xajKDFPRTP = false;
      bool MZFTEePbpI = false;
      bool gEZODNLGZP = false;
      bool BrdrrKDmXo = false;
      bool SlsTCnblHc = false;
      bool DNVjtrAUHp = false;
      bool tIqDNTwNbs = false;
      bool bcEAptKGMO = false;
      bool lZzwLmVsxz = false;
      bool KhehuJtpFh = false;
      bool rNKNBaJVtz = false;
      bool gtIrDTJuqa = false;
      bool xCQbitxVEf = false;
      bool AUBdWXOkuk = false;
      string ZrjkwOIsIq;
      string lHxrfagGJO;
      string DVpgRQmpcc;
      string fUwbuwjytb;
      string nfNKkbJLts;
      string GOTwqDCDSe;
      string bRPfLRLVxi;
      string qiMgaWIfwo;
      string GawiVRAzjq;
      string tfeOAVPhzS;
      string QnOKCWBmUT;
      string uVHcYAVfJs;
      string QLkFoKDxgF;
      string GagcYSMlgz;
      string PNdQwXYBpA;
      string zjYFCFCJUr;
      string SEGPNUoeEG;
      string nizJcDgUJR;
      string kBHfFWBaaT;
      string PDQofLHOCa;
      if(ZrjkwOIsIq == QnOKCWBmUT){htxjMQjOcy = true;}
      else if(QnOKCWBmUT == ZrjkwOIsIq){SlsTCnblHc = true;}
      if(lHxrfagGJO == uVHcYAVfJs){XhzDGQxZJp = true;}
      else if(uVHcYAVfJs == lHxrfagGJO){DNVjtrAUHp = true;}
      if(DVpgRQmpcc == QLkFoKDxgF){SOEkpkSJdR = true;}
      else if(QLkFoKDxgF == DVpgRQmpcc){tIqDNTwNbs = true;}
      if(fUwbuwjytb == GagcYSMlgz){uRShiqEWTo = true;}
      else if(GagcYSMlgz == fUwbuwjytb){bcEAptKGMO = true;}
      if(nfNKkbJLts == PNdQwXYBpA){lmybuhDNiO = true;}
      else if(PNdQwXYBpA == nfNKkbJLts){lZzwLmVsxz = true;}
      if(GOTwqDCDSe == zjYFCFCJUr){NhFLITpLsb = true;}
      else if(zjYFCFCJUr == GOTwqDCDSe){KhehuJtpFh = true;}
      if(bRPfLRLVxi == SEGPNUoeEG){xajKDFPRTP = true;}
      else if(SEGPNUoeEG == bRPfLRLVxi){rNKNBaJVtz = true;}
      if(qiMgaWIfwo == nizJcDgUJR){MZFTEePbpI = true;}
      if(GawiVRAzjq == kBHfFWBaaT){gEZODNLGZP = true;}
      if(tfeOAVPhzS == PDQofLHOCa){BrdrrKDmXo = true;}
      while(nizJcDgUJR == qiMgaWIfwo){gtIrDTJuqa = true;}
      while(kBHfFWBaaT == kBHfFWBaaT){xCQbitxVEf = true;}
      while(PDQofLHOCa == PDQofLHOCa){AUBdWXOkuk = true;}
      if(htxjMQjOcy == true){htxjMQjOcy = false;}
      if(XhzDGQxZJp == true){XhzDGQxZJp = false;}
      if(SOEkpkSJdR == true){SOEkpkSJdR = false;}
      if(uRShiqEWTo == true){uRShiqEWTo = false;}
      if(lmybuhDNiO == true){lmybuhDNiO = false;}
      if(NhFLITpLsb == true){NhFLITpLsb = false;}
      if(xajKDFPRTP == true){xajKDFPRTP = false;}
      if(MZFTEePbpI == true){MZFTEePbpI = false;}
      if(gEZODNLGZP == true){gEZODNLGZP = false;}
      if(BrdrrKDmXo == true){BrdrrKDmXo = false;}
      if(SlsTCnblHc == true){SlsTCnblHc = false;}
      if(DNVjtrAUHp == true){DNVjtrAUHp = false;}
      if(tIqDNTwNbs == true){tIqDNTwNbs = false;}
      if(bcEAptKGMO == true){bcEAptKGMO = false;}
      if(lZzwLmVsxz == true){lZzwLmVsxz = false;}
      if(KhehuJtpFh == true){KhehuJtpFh = false;}
      if(rNKNBaJVtz == true){rNKNBaJVtz = false;}
      if(gtIrDTJuqa == true){gtIrDTJuqa = false;}
      if(xCQbitxVEf == true){xCQbitxVEf = false;}
      if(AUBdWXOkuk == true){AUBdWXOkuk = false;}
    } 
}; 
