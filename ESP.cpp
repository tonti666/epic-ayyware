#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include <stdarg.h>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <random>
#include <iostream>
#include <iomanip>
#include <random>
#include "RageBot.h"
#include "Autowall.h"
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <stdlib.h>
#include "position_adjust.h"
#include "Hooks.h"
#include "experimental.h"
#include "math.h"
#include "Vector2.h"
#include "GrenadePrediction.h"
#include <sstream> 
#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif
#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#else
#define XorStr( s ) ( s )
#endif
float lineLBY;
float lineLBY2;
float lineRealAngle;
float lineFakeAngle;
float lby2;
float lspeed;
float pitchmeme;
CAimbot rageXbot;

float inaccuracy;
void CEsp::Init()
{
	BombCarrier = nullptr;
}
void CEsp::Move(CUserCmd *pCmd, bool &bSendPacket)
{
}
bool screen_transformx(const Vector& point, Vector& screen)
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
bool world_to_screenx(const Vector &origin, Vector &screen)
{
	if (!screen_transformx(origin, screen)) {
		int iScreenWidth, iScreenHeight;
		interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
		screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
		screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;
		return true;
	}
	return false;
}


void CEsp::Draw()
{
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	IClientEntity *pLocal = hackManager.pLocal();

	if (options::menu.visuals_tab.deadesp.GetState())
	{
		if (pLocal->IsAlive())
		{
			//empty
		}
		else

			for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
			{

				IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
				player_info_t pinfo;
				if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
				{
					if (options::menu.visuals_tab.OtherRadar.GetState())
					{
						DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
						*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
					}
					if (options::menu.visuals_tab.show_players.GetState() && interfaces::engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
					{
						DrawPlayer(pEntity, pinfo);
					}


					if (options::menu.visuals_tab.SpecList.GetState())
					{
						SpecList();
					}

					ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();
					if (options::menu.visuals_tab.FiltersNades.GetState() && strstr(cClass->m_pNetworkName, "Projectile"))
					{
						DrawThrowable(pEntity);
					}
					if (options::menu.visuals_tab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
					{
						DrawDrop(pEntity, cClass);
					}

				

					if (options::menu.visuals_tab.FiltersC4.GetState())
					{
						if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
							DrawBombus(pEntity);
						if (cClass->m_ClassID == (int)CSGOClassID::CC4)
							DrawBomb(pEntity, cClass);
					}
				}
			}

	}
	else
		for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
		{

			IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
			player_info_t pinfo;
			if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
			{
				if (options::menu.visuals_tab.OtherRadar.GetState())
				{
					DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
					*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
				}
				if (options::menu.visuals_tab.show_players.GetState() && interfaces::engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
				{
					DrawPlayer(pEntity, pinfo);
				}

				if (options::menu.visuals_tab.SpecList.GetState())
				{
					SpecList();
				}

				ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();
				if (options::menu.visuals_tab.FiltersNades.GetState() && strstr(cClass->m_pNetworkName, "Projectile"))
				{
					DrawThrowable(pEntity);
				}
				if (options::menu.visuals_tab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
				{
					DrawDrop(pEntity, cClass);
				}

				if (options::menu.visuals_tab.OtherNoFlash.GetState())
				{
					float alp = options::menu.visuals_tab.flashAlpha.GetValue() / 255;
					DWORD m_flFlashMaxAlpha = NetVar.GetNetVar(0xFE79FB98);
					*(float*)((DWORD)pLocal + m_flFlashMaxAlpha) = alp;
				}


				if (options::menu.visuals_tab.FiltersC4.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
						DrawBombus(pEntity);
					if (cClass->m_ClassID == (int)CSGOClassID::CC4)
						DrawBomb(pEntity, cClass);
				}
			}
		}




	if (options::menu.visuals_tab.GrenadePred.GetState())
	{
	//	grenade_prediction::instance().Paint();
	}

		
}
float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];
inline float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;
		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}
		flDamage = flNew;
	}
	return flDamage;
}
void CEsp::DrawBombus(IClientEntity* pEntity) {
	BombCarrier = nullptr;
	auto entity = pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)entity;
	float flBlow = Bomb->GetC4BlowTime();
	auto local = hackManager.pLocal();
	float lifetime = flBlow - (interfaces::globals->interval_per_tick * local->GetTickBase());
	int width = 0;
	int height = 0;
	interfaces::engine->GetScreenSize(width, height);
	if (world_to_screenx(vOrig, vScreen))
	{
		Render::nonamegey(vScreen.x, vScreen.y, "c4", Render::Fonts::esp, Color(255, 255, 255));
	}
	if (interfaces::engine->IsConnected() && interfaces::engine->IsInGame())
	{
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			int boomval = (lifetime * 300) / 40;
			Render::gradient_horizontal(0, 0, 12, boomval, Color(options::menu.ColorsTab.bomb_timer.GetValue()), Color(10, 10, 10, 55));
			Render::textT(0, boomval, Render::Fonts::esp, Color(255, 255, 255), "%.1f", lifetime);
			Render::textT(5, height / 2 - 140, Render::Fonts::niggerbomb, Color(250, 10, 50, 255), "%.1fs", lifetime);
		}
	}
	if (interfaces::engine->IsConnected() && interfaces::engine->IsInGame())
	{
		float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
		float a = 450.7f;
		float b = 75.68f;
		float c = 789.2f;
		float d = ((flDistance - b) / c);
		float flDamage = a * exp(-d * d);
		damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));
		sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "");
		if (lifetime > 0.01f && !Bomb->IsBombDefused() && local->IsAlive())
		{
			if (damage >= local->GetHealth() && lifetime > 0.01f)
			{
				Render::textT(5, height / 2 - 120, Render::Fonts::niggerbomb, Color(255, 10, 10, 255), "deadly");
			}
			else {
				if (lifetime > 0.01f && local->IsAlive()) {
					std::string gey;
					gey += "-";
					gey += std::to_string((int)(damage));
					gey += "HP";
					Render::textT(5, height / 2 - 120, Render::Fonts::niggerbomb, Color(216, 215, 164, 255), gey.c_str());
				}
			}
		}
	}

	if (interfaces::engine->IsConnected() && interfaces::engine->IsInGame())
	{
		if (Bomb->GetBombDefuser() > 0)
		{
			//IClientEntity *pDefuser = Interfaces::EntList->GetClientEntity(Bomb->GetBombDefuser());
			float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * interfaces::globals->interval_per_tick);
			//float maxdefuse = pDefuser->HasDefuser() ? 5.0f : 10.f;
			if (countdown > 0.01f)
			{
				if (lifetime > countdown)
				{
					Render::textT(5, height / 2 - 100, Render::Fonts::niggerbomb, Color(0, 140, 255, 255), "defuse: %.1f", countdown);
				}
			}
		}
	}
}

bool World2Screen(const Vector &origin, Vector &screen)
{
	if (!screen_transformx(origin, screen)) {
		int iScreenWidth, iScreenHeight;
		interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
		screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
		screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;
		return true;
	}
	return false;
}
bool isOnScreen(Vector origin, Vector &screen)
{
	if (!World2Screen(origin, screen)) return false;
	int iScreenWidth, iScreenHeight;
	interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
	bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
	return xOk && yOk;
}
Vector CalcAngle_vec(Vector src, Vector dst)
{
	Vector ret;
	VectorAngles(dst - src, ret);
	return ret;
}

void CEsp::DrawWeapon(IClientEntity* pEntity, Box size)
{
	IClientEntity* pWeapon = interfaces::ent_list->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (options::menu.visuals_tab.OptionsHealth.GetIndex() > 2 || options::menu.visuals_tab.OptionsArmor.GetIndex() > 2)
		{
			RECT nameSize = Render::GetTextSize(Render::Fonts::esp, pWeapon->GetWeaponName());
			Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 13,
				Color(pEntity->team() == hackManager.pLocal()->team() ? options::menu.ColorsTab.Weaponsteam.GetValue() : options::menu.ColorsTab.Weapons.GetValue()), Render::Fonts::esp, pWeapon->GetWeaponName());
		}

		else
		{
			RECT nameSize = Render::GetTextSize(Render::Fonts::esp, pWeapon->GetWeaponName());
			Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 5,
				Color(pEntity->team() == hackManager.pLocal()->team() ? options::menu.ColorsTab.Weaponsteam.GetValue() : options::menu.ColorsTab.Weapons.GetValue()), Render::Fonts::esp, pWeapon->GetWeaponName());
		}
	}
}

void CEsp::DrawAmmo(IClientEntity* pEntity, Box size)
{
	C_BaseCombatWeapon* pWeapon = pEntity->GetWeapon2();
	IClientEntity* pLocal = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!pEntity || pEntity == nullptr || !pLocal || !pLocal->IsAlive())
		return;

	if (!pWeapon)
		return;

	if (pWeapon->isZeus27() || pWeapon->IsMiscGAY())
		return;

	CSWeaponInfo* weapInfo = pWeapon->GetCSWpnData();

	if (weapInfo == nullptr)
		return;

	Color arc = Color(options::menu.ColorsTab.Ammo.GetValue());
	int ammoyes = pWeapon->GetAmmoInClip() * (size.w) / weapInfo->max_clip;
	Render::outlineyeti(size.x - 1, size.y + size.h + 1, size.w + 2, 4, Color(21, 21, 21, 255));
	Render::rect(size.x, size.y + size.h + 2, size.w, 2, Color(51, 51, 51, 255));
	Render::rect(size.x, size.y + size.h + 2, ammoyes, 2, arc);
}

void CEsp::ammo_text(IClientEntity* pEntity, Box size)
{
	C_BaseCombatWeapon* pWeapon = pEntity->GetWeapon2();
	IClientEntity* pLocal = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!pEntity || pEntity == nullptr || !pLocal || !pLocal->IsAlive())
		return;

	if (!pWeapon)
		return;

	if (pWeapon->isZeus27() || pWeapon->IsMiscGAY())
		return;
	bool text_side = options::menu.visuals_tab.OptionsName.GetIndex() > 1;
	CSWeaponInfo* weapInfo = pWeapon->GetCSWpnData();

	if (weapInfo == nullptr)
		return;

	Color arc = Color(options::menu.ColorsTab.Ammo.GetValue());

	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = hackManager.pLocal()->GetOrigin();
	float dist = DistanceTo(vecOriginLocal, vecOrigin) / 3;

	static RECT Size = Render::GetTextSize(Render::Fonts::esp, "Hi");
	char ammoBuffer[512];
	sprintf_s(ammoBuffer, "%d bullets", pWeapon->GetAmmoInClip());
	Render::Text(size.x + size.w + 5, (size.y + (text_side ? 33 : 14)) + (1 * Size.bottom) - dist, Color(255, 255, 255, 255), Render::Fonts::esp, ammoBuffer);

}

void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	Box box;
	ESPBox esp_box;
	Color Color;

	//Vector origin;


	Vector max = pEntity->GetCollideable()->OBBMaxs();

	Vector pos, pos3D;
	Vector top, top3D;
	pos3D = pEntity->GetOrigin();
	top3D = pos3D + Vector(0, 0, max.z);

	bool teammate = pEntity->team() == hackManager.pLocal()->team();

	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	if (!GameUtils::World2Screen(pos3D, pos) || !GameUtils::World2Screen(top3D, top))
		return;

	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (local_player == nullptr)
		return;

	if (options::menu.visuals_tab.OffscreenESP.GetState())
	{
		DrawFovArrows(pEntity->GetOrigin());
	}
	//if (!teammate && options::menu.visuals_tab.OffscreenESP.GetState())
		//enemy_out_of_range(pEntity);

	if (get_box(pEntity, box, options::menu.visuals_tab.Active.GetState()) && (!teammate))
	{

		if (options::menu.visuals_tab.OptionsBox.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsBox.GetIndex())
			{
			case 1: default_box(box, options::menu.ColorsTab.BoxCol.GetValue());
				break;
			case 2: bracket_box(box, options::menu.ColorsTab.BoxCol.GetValue());
				break;
			case 3: corner_box(box, options::menu.ColorsTab.BoxCol.GetValue(), pEntity);
				break;
			case 4: default_box2(pEntity, box, Color);
				break;
			case 5: default_box3(pEntity, box, Color);
				break;

			}
		}

		if (options::menu.visuals_tab.OptionsName.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsName.GetIndex())
			{
			case 1: DrawName(pinfo, box, pEntity);
				break;
			case 2: name_side(pinfo, box, pEntity);
				break;
			}
		}

		if (options::menu.visuals_tab.OptionsHealth.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsHealth.GetIndex())
			{
			case 1: hp_default(pEntity, box);
				break;
			case 2: hp_battery(pEntity, box);
				break;
			case 3: hp_bottom(pEntity, box);
				break;
			}

			if (options::menu.visuals_tab.OptionsInfo.GetState())
			{
				hp_text(pEntity, box);
			}
		}

		if (options::menu.visuals_tab.OptionsArmor.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsArmor.GetIndex())
			{
			case 1: armor_default(pEntity, box);
				break;
			case 2: armor_battery(pEntity, box);
				break;
			case 3: armor_bottom(pEntity, box);
				break;
			}

			if (options::menu.visuals_tab.OptionsInfo.GetState())
			{
				ar_text(pEntity, box);
			}
		}

		if (options::menu.visuals_tab.OptionsInfo.GetState() || options::menu.visuals_tab.OptionsWeapone.GetState())
		{
			DrawInfo(pEntity, box);
			DrawInfo2(pEntity, box);
		}

		if (options::menu.visuals_tab.OptionsSkeleton.GetState())
			DrawSkeleton(pEntity);

		if (options::menu.visuals_tab.Weapons.GetState()) {
			DrawWeapon(pEntity, box);
		}



		if (options::menu.visuals_tab.Ammo.GetState())
		{
			DrawAmmo(pEntity, box);

			if (options::menu.visuals_tab.OptionsInfo.GetState())
			{
				ammo_text(pEntity, box);
			}
		}
	}
	if (get_box(pEntity, box, options::menu.visuals_tab.Active.GetState()) && options::menu.visuals_tab.show_team.GetState() || options::menu.MiscTab.dangerzone.GetState() && (teammate))
	{
		if (options::menu.visuals_tab.OptionsBox.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsBox.GetIndex())
			{
			case 1: default_box(box, options::menu.ColorsTab.BoxCol.GetValue());
				break;
			case 2: bracket_box(box, options::menu.ColorsTab.BoxCol.GetValue());
				break;
			case 3: corner_box(box, options::menu.ColorsTab.BoxCol.GetValue(), pEntity);
				break;
			case 4: default_box2(pEntity, box, Color);
				break;
			case 5: default_box3(pEntity, box, Color);
				break;

			}
		}

		if (options::menu.visuals_tab.OptionsName.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsName.GetIndex())
			{
			case 1: DrawName(pinfo, box, pEntity);
				break;
			case 2: name_side(pinfo, box, pEntity);
				break;
			}
		}

		if (options::menu.visuals_tab.OptionsHealth.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsHealth.GetIndex())
			{
			case 1: hp_default(pEntity, box);
				break;
			case 2: hp_battery(pEntity, box);
				break;
			case 3: hp_bottom(pEntity, box);
				break;
			}

			if (options::menu.visuals_tab.OptionsInfo.GetState())
			{
				hp_text(pEntity, box);
			}
		}


		if (options::menu.visuals_tab.OptionsArmor.GetIndex() != 0)
		{
			switch (options::menu.visuals_tab.OptionsArmor.GetIndex())
			{
			case 1: armor_default(pEntity, box);
				break;
			case 2: armor_battery(pEntity, box);
				break;
			case 3: armor_bottom(pEntity, box);
				break;
			}

			if (options::menu.visuals_tab.OptionsInfo.GetState())
			{
				ar_text(pEntity, box);
			}
		}

		if (options::menu.visuals_tab.OptionsInfo.GetState() || options::menu.visuals_tab.OptionsWeapone.GetState())
		{
			DrawInfo(pEntity, box);
			
		}

		if (options::menu.visuals_tab.OptionsSkeleton.GetState())
			DrawSkeleton_team(pEntity);

		if (options::menu.visuals_tab.Weapons.GetState()) {
			DrawWeapon(pEntity, box);
		}



		//if (options::menu.visuals_tab.GrenadePred.GetState())
		//	NewNadePred();

		if (options::menu.visuals_tab.Ammo.GetState())
		{
			DrawAmmo(pEntity, box);

			if (options::menu.visuals_tab.OptionsInfo.GetState())
			{
				ammo_text(pEntity, box);
			}
		}

		//if (options::menu.visuals_tab.OffscreenESP.GetState())
		//	direction_arrow(origin);

		

	
	}


}
float dot_product_t(const float* a, const float* b) {
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}
#define rad_pi 57.295779513082f
#define pi 3.14159265358979323846f
#define rad(a) a * 0.01745329251
#define deg(a) a * 57.295779513082
float degrees_to_radians(const float deg)
{
	return deg * (pi / 180.f);
}
float radians_to_degrees(const float rad)
{
	return rad * rad_pi;
}

void rotate_triangle(std::array<Vector2D, 3>& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points)
	{
		point -= points_center;
		const auto temp_x = point.x;
		const auto temp_y = point.y;
		const auto theta = degrees_to_radians(rotation);
		const auto c = cosf(theta);
		const auto s = sinf(theta);
		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;
		point += points_center;
	}
}
template<class T, class U>
inline T clampx(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
void VectorAngles(Vector &forward, QAngle &angles)
{
	Assert(s_bMathlibInitialized);
	float	tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / pi);
		if (yaw < 0)
			yaw += 360;
		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / pi);
		if (pitch < 0)
			pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}



void VectorAnglesXX(const Vector& forward, Vector &angles)
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / PI);
		if (yaw < 0)
			yaw += 360;
		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI);
		if (pitch < 0)
			pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
Vector CalcAngleXX(Vector src, Vector dst)
{
	Vector ret;
	VectorAnglesXX(dst - src, ret);
	return ret;
}

Vector CalcAngle69(Vector dst, Vector src)
{
	Vector angles;

	double delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta[1] * delta[1] + delta[2] * delta[2]);
	angles.x = (float)(atan(delta[2] / hyp) * 180.0 / 3.14159265);
	angles.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles.z = 0.0f;

	if (delta[0] >= 1.9)
	{
		angles.y += 180.0f;
	}

	return angles;
}
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
/*
void CEsp::flashbar()
{
	float flashed = hackManager.pLocal()->GetFlashDuration(); // your job to find out what this function is
	int posY = 10 + 29;
	if (flashed > 0.f) // checking if our current flash value is about 0.f
	{
		float ptc = flashed / 255.f; // getting a percentage of it in order todo a reverted progressbar
		Render::DrawFilledRect(500, 500, 500 + 500, 500 + 200, Color(0, 0, 0, 110)); // drawing a black filled rectangle beneath the actual bar
		Render::DrawFilledRect(size.x, posY - 3.5, size.x + (300 * ptc), posY + 3.5, Color(194, 244, 66, 110)); // actual bar itself calculating the current size via the ptc to achieve a reverted progressbar
		Render::DrawRect(size.x, posY - 3.5, size.x + 300, posY + 3.5, Color(0, 0, 0, 110)); // drawing outline 
	}
}*/

void CEsp::SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	// Loop through all active entitys
	for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (interfaces::engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = interfaces::ent_list->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{
						if (interfaces::engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							char buf[255]; sprintf_s(buf, "%s > %s", pinfo.name, pinfo2.name);
							RECT TextSize = Render::GetTextSize(Render::Fonts::esp, buf);
							
							Render::Text(scrn.right - TextSize.right - 4, (scrn.bottom / 2) + (16 * ayy), pTarget->GetIndex() == pLocal->GetIndex() ? Color(255, 255, 255, 220) : Color(255, 255, 255, 220), Render::Fonts::esp, buf);
							ayy++;
						}
					}
				}
			}
		}
	}
}
void vector_transform_a(const float *in1, const matrix3x4& in2, float *out) {
	out[0] = dot_product_t(in1, in2[0]) + in2[0][3];
	out[1] = dot_product_t(in1, in2[1]) + in2[1][3];
	out[2] = dot_product_t(in1, in2[2]) + in2[2][3];
}
inline void vector_transform_z(const Vector& in1, const matrix3x4 &in2, Vector &out) {
	vector_transform_a(&in1.x, in2, &out.x);
}
bool CEsp::get_box(IClientEntity* m_entity, Box& box, bool dynamic) {
	DWORD m_rgflCoordinateFrame = (DWORD)0x444; //(DWORD)0x470 - 0x30
	const matrix3x4& trnsf = *(matrix3x4*)((DWORD)m_entity + (DWORD)m_rgflCoordinateFrame);
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;
	vOrigin = m_entity->GetOrigin();
	min = m_entity->collisionProperty()->GetMins();
	max = m_entity->collisionProperty()->GetMaxs();
	if (!dynamic) {
		min += vOrigin;
		max += vOrigin;
	}
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };
	Vector vector_transformed[8];
	if (dynamic)
	{
		for (int i = 0; i < 8; i++)
		{
			vector_transform_z(points[i], trnsf, vector_transformed[i]);
			points[i] = vector_transformed[i];
		}
	}
	if (!Render::TransformScreen(points[3], flb) || !Render::TransformScreen(points[5], brt)
		|| !Render::TransformScreen(points[0], blb) || !Render::TransformScreen(points[4], frt)
		|| !Render::TransformScreen(points[2], frb) || !Render::TransformScreen(points[1], brb)
		|| !Render::TransformScreen(points[6], blt) || !Render::TransformScreen(points[7], flt))
		return false;
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;
	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	box.x = left;
	box.y = top;
	box.w = right - left;
	box.h = bottom - top;
	return true;
}
bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };
	if (!Render::TransformScreen(points[3], flb) || !Render::TransformScreen(points[5], brt)
		|| !Render::TransformScreen(points[0], blb) || !Render::TransformScreen(points[4], frt)
		|| !Render::TransformScreen(points[2], frb) || !Render::TransformScreen(points[1], brb)
		|| !Render::TransformScreen(points[6], blt) || !Render::TransformScreen(points[7], flt))
		return false;
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;
	return true;
}
Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->team();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)csgo_hitboxes::head);
	Color color;
	if (TeamNum == TEAM_CS_T)
	{
	}
	else
	{
	}
	return color;
}
std::string CleanItemName(std::string name)
{
	std::string Name = name;
	if (Name[0] == 'C')
		Name.erase(Name.begin());
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);
	return Name;
}
void CEsp::DrawGun(IClientEntity* pEntity, CEsp::ESPBox size)
{
	IClientEntity* pWeapon = interfaces::ent_list->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
	if (cClass)
	{
		std::string meme = CleanItemName(cClass->m_pNetworkName);
		RECT nameSize = Render::GetTextSize(Render::Fonts::esp, meme.c_str());
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 1,
			Color(255, 255, 255, 255), Render::Fonts::esp, meme.c_str());
	}
}
void CEsp::corner_box(Box size, Color color, IClientEntity* pEntity)
{
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.30f));
	Render::Clear(size.x, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x, size.y, 1, HorzLine, color);
	Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
}
void CEsp::FilledBox(CEsp::ESPBox size, Color color)
{
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.20f));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, 1, HorzLine, color);
	Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	CEsp::ESPBox box = size;
	Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(21, 21, 21, 150));
	Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(21, 21, 21, 150));
}
void CEsp::default_box(Box box, Color color)
{
	Render::Outline(box.x, box.y, box.w, box.h, color);
	Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(21, 21, 21, 150));
	Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(21, 21, 21, 150));
	Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(21, 21, 21, 150));
	Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(21, 21, 21, 150));
}

void CEsp::default_box2(IClientEntity* pEntity, Box box, Color color)
{
	IMaterial * visible; IMaterial * invisible;
	int * colvis = (options::menu.ColorsTab.BoxCol.GetValue());
	int * colinvis = (options::menu.ColorsTab.BoxColinvis.GetValue());
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);


	if (IsVis)
	{
		Render::Outline(box.x, box.y, box.w, box.h, color);
		Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(colvis));
		Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(colvis));
		Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(colvis));
		Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(colvis));
	}

	if (!IsVis)
	{
		Render::Outline(box.x, box.y, box.w, box.h, color);
		Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(colinvis));
		Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(colinvis));
		Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(colinvis));
		Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(colinvis));
	}
}

void CEsp::default_box3(IClientEntity* pEntity, Box box, Color color)
{
	IMaterial * visible; IMaterial * invisible;
	int * colvis = (options::menu.ColorsTab.BoxCol.GetValue());
	int * colinvis = (options::menu.ColorsTab.BoxColinvis.GetValue());
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);


	if (IsVis)
	{
		Render::Outline(box.x, box.y, box.w, box.h, color);
		Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(colvis));
		Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(colvis));
		Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(colvis));
		Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(colvis));
	}

	if (!IsVis)
	{
		return;
	}
}



void CEsp::bracket_box(Box size, Color color)
{
	int VertLine = (((float)size.w) * (0.30f));
	int HorzLine = (((float)size.h) * (1.00f));
	Render::Clear(size.x, size.y, VertLine, 1.2, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1.2, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1.2, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1.2, color);
	Render::Clear(size.x, size.y, 1, HorzLine, color);
	Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
	Box box = size;
	Render::Outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(21, 21, 21, 150));
	Render::Outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(21, 21, 21, 150));
}
static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}
//converting like that will fuck the unicode characters lmao who made that
void CEsp::DrawName(player_info_t pinfo, Box size, IClientEntity* pEntity)
{
	if (!pEntity || !pEntity->IsAlive() || !hackManager.pLocal())
		return;
	wchar_t buffer[36];
	auto name = pinfo.name;
	if (MultiByteToWideChar(CP_UTF8, 0, pinfo.name, -1, buffer, 36) > 0)
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::esp, pinfo.name);
		Render::TEXTUNICODE(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 11, pinfo.name, Render::Fonts::esp, Color(options::menu.ColorsTab.NameCol.GetValue()));
	}
}

void CEsp::hp_battery(IClientEntity* pEntity, Box size)
{
	Box HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	// --  Main Bar -- //

	float flBoxes = std::ceil(pEntity->GetHealth() / 10);
	float flX = size.x - 7; float flY = size.y - 1;
	float flHeight = size.h / 10.f;
	float flHeight2 = size.h / 10.f;
	float flMultiplier = 10 / 360.f; flMultiplier *= flBoxes - 1;

	int rectHeight = flHeight * flBoxes + 1;

	Render::DrawRect(flX, flY, 4, size.h + 2, Color(60, 60, 60, 255));
	Render::Outline(flX, flY, 4, size.h + 2, Color(0, 0, 0, 255));

	Render::DrawRect(flX + 1, flY + size.h + (flMultiplier - rectHeight) + 1, 2, rectHeight, pEntity->IsDormant() ? Color(0, 0, 0, 140) : Color(0, 250, 90, 255));

	for (int i = 0; i < 10; i++) // 
		Render::Line(flX, flY + i * flHeight2, flX + 4, flY + i * flHeight2, Color(0, 0, 0, 255));
}


void CEsp::armor_battery(IClientEntity* pEntity, Box size)
{
	Box ArmorBar = size;
	ArmorBar.y += (ArmorBar.h + 6);
	ArmorBar.h = 4;

	float ArmorValue = pEntity->ArmorValue();
	float ArmorPerc = ArmorValue / 100.f;
	float Width = (size.w * ArmorPerc);
	ArmorBar.w = Width;

	// --  Main Bar -- //

	float flBoxes = std::ceil(pEntity->ArmorValue() / 10);
	float flX = size.x - (options::menu.visuals_tab.OptionsHealth.GetIndex() > 2 ? 7 : 12); float flY = size.y - 1;
	float flHeight = size.h / 10.f;
	float flHeight2 = size.h / 10.f;
	float flMultiplier = 10 / 360.f; flMultiplier *= flBoxes - 1;
	Color ColArmor = Color::FromHSB(flMultiplier, 1, 1);
	int rectHeight = flHeight * flBoxes + 1;


	Render::DrawRect(flX, flY, 4, size.h + 2, Color(60, 60, 60, 255));
	Render::Outline(flX, flY, 4, size.h + 2, Color(0, 0, 0, 255));

	Render::DrawRect(flX + 1, flY + size.h + (flMultiplier - rectHeight) + 1, 2, rectHeight, Color(0, 100, 250, 140));

	for (int i = 0; i < 10; i++) // 
		Render::Line(flX, flY + i * flHeight2, flX + 4, flY + i * flHeight2, Color(0, 0, 0, 255));


}

void CEsp::armor_default(IClientEntity* pEntity, Box size)
{
	Box box = size;
	int player_ar = pEntity->ArmorValue() > 100 ? 100 : pEntity->ArmorValue();
	if (player_ar)
	{
		if (player_ar > 100) {
			player_ar = 100;
		}
		int color[3] = { 0, 120, 250 };

		Render::Outline(box.x - 7, box.y - 1, 4, box.h + 2, Color(21, 21, 21, 255));
		int health_height = player_ar * box.h / 100;
		int add_space = box.h - health_height;
		Color hec = Color(color[0], color[1], color[2], 255);
		Render::rect(box.x - 11, box.y, 2, box.h, Color(21, 21, 21, 255));
		Render::rect(box.x - 11, box.y + add_space, 2, health_height, hec);

	}
} // DrawAmmo_vital(pEntity, box)

void CEsp::hp_default(IClientEntity* pEntity, Box size)
{
	Box box = size;
	int player_health = pEntity->GetHealth() > 100 ? 100 : pEntity->GetHealth();
	if (player_health) {

		int color[3] = { 0, 0, 0 };
		color[0] = 83; color[1] = 250; color[2] = 90;

		Render::Outline(box.x - 7, box.y - 1, 4, box.h + 2, Color(21, 21, 21, 255));
		int health_height = player_health * box.h / 100;
		int add_space = box.h - health_height;
		Color hec = Color(color[0], color[1], color[2], 255);
		Render::rect(box.x - 6, box.y, 2, box.h, Color(21, 21, 21, 255));
		Render::rect(box.x - 6, box.y + add_space, 2, health_height, hec);

	}
}

void NewNadePred()
{
	IClientEntity *pLocal = hackManager.pLocal();
	IClientEntity *pEntity = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos;
	Vector vStart;
	Vector vThrow;
	Vector vThrow2;
	int iCollisions = 0;

	Vector vViewAngles;
	interfaces::engine->get_viewangles(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	AngleVectors(vThrow, &vThrow2);

	Vector vEye = pEntity->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pEntity->GetVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + pEntity->GetVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + pEntity->GetVelocity()[2]; // casualhacker for da magic calc help

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;

		CTraceFilter loc;
		loc.pSkip = pEntity;

		ray.Init(vStart, vPos);
		interfaces::trace->TraceRay(ray, MASK_SOLID, &loc, &tr);

		if (tr.DidHit()) // if the trace hits a surface
		{
			//float proj = DotProduct(throwvec, tr.plane.normal);
			vThrow2 = tr.plane.normal * -2.0f *DotProduct(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2 * tr.fraction*fStep;

			fTime += (fStep * (1 - tr.fraction));
		} // end of bounce

		Vector vOutStart, vOutEnd;
		if (GameUtils::IsGrenade(pWeapon))
		{
			if (Render::WorldToScreen(vStart, vOutStart), Render::WorldToScreen(vPos, vOutEnd))
			{
				Render::Line(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, Color(0, 150, 255, 255));
			}

			vStart = vPos;
			vThrow2.z -= fGravity * tr.fraction*fStep;
		}
	}
}

float CEsp::DistanceTo(Vector vecSrc, Vector vecDst)
{
	Vector vDelta = vecDst - vecSrc;

	float fDistance = ::sqrtf((vDelta.Length()));

	if (fDistance < 1.0f)
		return 1.0f;

	return fDistance;
}
void CEsp::hp_bottom(IClientEntity* pEntity, Box size)
{
	Color arc = Color(10, 230, 75, 255);
	int hp = pEntity->GetHealth() * (size.w) / 100;

	Render::outlineyeti(size.x - 1, size.y + size.h + 5, size.w + 2, 4, Color(21, 21, 21, 255));
	Render::rect(size.x, size.y + size.h + 6, size.w, 2, Color(51, 51, 51, 255));
	Render::rect(size.x, size.y + size.h + 6, hp, 2, arc);
	//	Render::Text((size.x + 1) + hp, (size.y + 3.5 )+ size.h, Color(255, 255, 255), Render::Fonts::smallassfont, std::to_string(pEntity->GetHealth()).c_str());
}
void CEsp::hp_text(IClientEntity* pEntity, Box size)
{
	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = hackManager.pLocal()->GetOrigin();

	bool text_side = options::menu.visuals_tab.OptionsName.GetIndex() > 1;

	float dist = DistanceTo(vecOriginLocal, vecOrigin) / 3;
	static RECT Size = Render::GetTextSize(Render::Fonts::esp, "Hi");
	char HPBuffer[512];
	sprintf_s(HPBuffer, "%d health", pEntity->GetHealth());
	Render::Text(size.x + size.w + 5, (size.y + (text_side ? 1 : 0)) + (1 * Size.bottom) - dist, Color(10, 255, 170, 255), Render::Fonts::esp, HPBuffer);

}

void CEsp::ar_text(IClientEntity* pEntity, Box size)
{
	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = hackManager.pLocal()->GetOrigin();

	bool text_side = options::menu.visuals_tab.OptionsName.GetIndex() > 1;

	float dist = DistanceTo(vecOriginLocal, vecOrigin) / 3;
	static RECT Size = Render::GetTextSize(Render::Fonts::esp, "Hi");
	char armorbuffer[512];
	sprintf_s(armorbuffer, "%d armor", pEntity->ArmorValue());
	Render::Text(size.x + size.w + 5, (size.y + (text_side ? 21.5 : 7)) + (1 * Size.bottom) - dist, Color(10, 170, 255, 255), Render::Fonts::esp, armorbuffer);

}

void CEsp::name_side(player_info_t pinfo, Box size, IClientEntity* pEntity)
{
	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = hackManager.pLocal()->GetOrigin();
	auto name = pinfo.name;
	float dist = DistanceTo(vecOriginLocal, vecOrigin) / 3;
	static RECT Size = Render::GetTextSize(Render::Fonts::esp, "Hi");
	wchar_t buffer[36];

	if (MultiByteToWideChar(CP_UTF8, 0, pinfo.name, -1, buffer, 36) > 0)
	{
		Render::TEXTUNICODE(size.x + size.w + 5, size.y + (1 * Size.bottom) - dist, pinfo.name, Render::Fonts::esp, Color(options::menu.ColorsTab.NameCol.GetValue()));
	}
}

void CEsp::armor_bottom(IClientEntity* pEntity, Box size)
{
	Color arc = Color(10, 100, 230, 255);
	int hp = pEntity->ArmorValue() * (size.w) / 100;

	if (options::menu.visuals_tab.OptionsHealth.GetIndex() < 3)
	{
		Render::outlineyeti(size.x - 1, size.y + size.h + 5, size.w + 2, 4, Color(21, 21, 21, 255));
		Render::rect(size.x, size.y + size.h + 6, size.w, 2, Color(51, 51, 51, 255));
		Render::rect(size.x, size.y + size.h + 6, hp, 2, arc);
	}
	else
	{
		Render::outlineyeti(size.x - 1, size.y + size.h + 9, size.w + 2, 4, Color(21, 21, 21, 255));
		Render::rect(size.x, size.y + size.h + 10, size.w, 2, Color(51, 51, 51, 255));
		Render::rect(size.x, size.y + size.h + 10, hp, 2, arc);
	}
	//	Render::Text(size.x + hp, (size.y + 7 ) + size.h, Color(255, 255, 255), Render::Fonts::smallassfont, std::to_string(pEntity->GetHealth()).c_str());
}


void CEsp::DrawInfo(IClientEntity* pEntity, Box size)
{
	IClientEntity *g_LocalPlayer = hackManager.pLocal();
	if (!g_LocalPlayer)
		return;
	std::vector<std::pair<std::string, Color>> stored_info;
	auto entity = pEntity;
	static float old_simtime[65];

	if (entity->GetFlashDuration() > 0)
		stored_info.push_back(std::pair<std::string, Color>("Flashed", Color(250, 250, 250, 250)));
	if (BombCarrier == entity)
		stored_info.push_back(std::pair<std::string, Color>(" Bomb", Color(250, 250, 250, 220)));

	int i = 0;
	for (auto Text : stored_info)
	{
		RECT TextSize = Render::GetTextSize(Render::Fonts::esp, Text.first.c_str());
		Render::Text(size.x + size.w + 1, (size.y + 30) + i - ((TextSize.bottom / 2) - 5), Text.second, Render::Fonts::esp, Text.first.c_str());
		i += 8;
	}

}

void CEsp::DrawInfo2(IClientEntity* pEntity, Box size)
{
	RECT defSize = Render::GetTextSize(Render::Fonts::esp, "");
	std::vector<std::string> Info;

	// Player Weapon ESP
	IClientEntity* pWeapon = interfaces::ent_list->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	static RECT Size = Render::GetTextSize(Render::Fonts::Default, "Hi");
	RECT named = Render::GetTextSize(Render::Fonts::esp, "Scoped");

	if (options::menu.visuals_tab.OptionsInfo.GetState())
	{
		char hp[50];
		sprintf_s(hp, sizeof(hp), "%i", pEntity->ArmorValue());
		RECT nameSize = Render::GetTextSize(Render::Fonts::esp, hp);

		if (options::menu.visuals_tab.OptionsHealth.GetIndex() > 2 || options::menu.visuals_tab.OptionsArmor.GetIndex() > 2)
		{
			if (pEntity->ArmorValue() > 0)
			{
				if (pEntity->HasHelmet())
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 23, Color(255, 255, 255, 255), Render::Fonts::esp, "HK");
				else
					Render::Text(size.x + (size.w / 2) - ((nameSize.right / 2)), size.y + size.h + 23, Color(210, 210, 210, 255), Render::Fonts::esp, "Kev");
			}
		}

		else
		{
			if (pEntity->ArmorValue() > 0)
			{
				if (pEntity->HasHelmet())
					Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 15, Color(255, 255, 255, 255), Render::Fonts::esp, "HK");
				else
					Render::Text(size.x + (size.w / 2) - ((nameSize.right / 2)), size.y + size.h + 15, Color(210, 210, 210, 255), Render::Fonts::esp, "Kev");
			}
		}
	}
	if (options::menu.visuals_tab.OptionsInfo.GetState())
	{
		if (pEntity->IsScoped())
		{
			Render::Text(size.x + (size.w / 2) - (named.right / 2), size.y - 20, Color(250, 250, 250, 255), Render::Fonts::esp, "Scoped");
		}
	}


	int i = 0;
	for (auto Text : Info)
	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::esp, Text.c_str());
		i++;
	}
}



void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Color color;
	Box Box;
	IClientEntity* Weapon = (IClientEntity*)pEntity;

	IClientEntity* plr = interfaces::ent_list->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());

	if (Weapon && !plr)
	{
		if (get_box(pEntity, Box, options::menu.visuals_tab.FiltersWeapons.GetState()))
		{
			if (options::menu.visuals_tab.FiltersWeapons.GetState())
			{
				bracket_box(Box, Color(255, 255, 255, 255));
			}
			RECT TextSize = Render::GetTextSize(Render::Fonts::nameaiz, Weapon->GetWeaponName());
			Render::Text(Box.x + (Box.w / 2) - (TextSize.right / 2), Box.y + Box.h + 5, Color(255, 255, 255, 255), Render::Fonts::nameaiz, Weapon->GetWeaponName());
		}
	}
}

void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass)
{
	BombCarrier = nullptr;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;
	float flBlow = Bomb->GetC4BlowTime();
	float TimeRemaining = flBlow - (interfaces::globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
	char buffer[64];
	sprintf_s(buffer, "%.1fs", TimeRemaining);
	float TimeRemaining2;
	bool exploded = true;
	if (TimeRemaining < 0)
	{
		!exploded;
		TimeRemaining2 = 0;
	}
	else
	{
		exploded = true;
		TimeRemaining2 = TimeRemaining;
	}
	if (exploded)
	{
		sprintf_s(buffer, " bomb: %.1f", TimeRemaining2);
	}
	else
	{
		sprintf_s(buffer, " undefusable", TimeRemaining2);
	}
	//Render::Text(10, 45, Color(0, 255, 0, 255), Render::Fonts::Clock, buffer);
}
void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	BombCarrier = nullptr;
	C_BaseCombatWeapon *BombWeapon = (C_BaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (interfaces::ent_list->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = interfaces::model_info->GetStudiomodel(pEntity->GetModel());
	if (!pStudioHdr)
		return;
	Vector vParent, vChild, sParent, sChild;
	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);
		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);
			if (Render::TransformScreen(vParent, sParent) && Render::TransformScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(options::menu.ColorsTab.Skeleton.GetValue()[0], options::menu.ColorsTab.Skeleton.GetValue()[1], options::menu.ColorsTab.Skeleton.GetValue()[2], options::menu.ColorsTab.Skeleton.GetValue()[3]));
			}
		}
	}
}

void CEsp::DrawSkeleton_team(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = interfaces::model_info->GetStudiomodel(pEntity->GetModel());
	if (!pStudioHdr)
		return;
	Vector vParent, vChild, sParent, sChild;
	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);
		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);
			if (Render::TransformScreen(vParent, sParent) && Render::TransformScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(options::menu.ColorsTab.Skeletonteam.GetValue()[0], options::menu.ColorsTab.Skeletonteam.GetValue()[1], options::menu.ColorsTab.Skeletonteam.GetValue()[2], options::menu.ColorsTab.Skeletonteam.GetValue()[3]));
			}
		}
	}
}



bool IsOnScreen(Vector origin, Vector& screen)
{
	if (!GameUtils::World2Screen(origin, screen)) return false;
	int iScreenWidth, iScreenHeight;
	interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
	bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
	return xOk && yOk;
}

mstudiobbox_t* get_hitbox1(IClientEntity* pEntity, int hitbox_index)
{
	if (pEntity->IsDormant() || pEntity->GetHealth() <= 0)
		return NULL;

	const auto pModel = pEntity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = interfaces::model_info->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->pHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->pHitbox(hitbox_index);
}
/*
void CEsp::watemark() 
{

	static int fps, old_tick_count;

	if ((interfaces::globals->tickcount - old_tick_count) > 50) 
	{

		fps = static_cast<int>(1.f / interfaces::globals->frametime);

		old_tick_count = interfaces::globals->tickcount;

	}
	

	int screen_x, screen_y;

	interfaces::engine->GetScreenSize(screen_x, screen_y);

	static float bar_width = 226.8;

	std::stringstream ss;

	static float rainbow;

	rainbow += 0.0005f;

	if (rainbow > 1.f)

		rainbow = 0.f;



	auto net_channel = interfaces::engine->GetNetChannelInfo();



	std::string incoming = hackManager.pLocal() ? std::to_string((int)(net_channel->GetLatency(FLOW_INCOMING) * 1000)) : "0";

	std::string outgoing = hackManager.pLocal() ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : "0";

	ss << "tornadonight.xyz | " << " | incoming: " << incoming.c_str() << "ms" << " | outgoing: " << outgoing.c_str() << "ms" << " | " << __DATE__ << "";

	int x, y, w, h;

	int textsize = (Render::text_width(Render::Fonts::esp, ss.str().c_str()));

	Render::DrawFilledRect(screen_x - (textsize + 20), 6, textsize + 20, 18, Color(20, 20, 100, 255));

	Render::rect(screen_x - (textsize + 20), 6, textsize + 20, 18, Color(0, 0, 0, 255));

	Render::text_width(Render::Fonts::esp, screen_x - (textsize + 10), 9, Color(255, 255, 255, 200), HFONT_OFF_SCREEN, ss.str().c_str());

}*/


void DrawFovArrows(const Vector& origin)
{/*
	auto local_player = hackManager.pLocal();
	if (!local_player) return;
	if (pEntity->IsDormant()) return;

	Vector screenPos, client_viewangles;
	int screen_width = 0, screen_height = 0;
	float radius = 300.f;

	if (IsOnScreen(GetHitboxPosition(pEntity, (int)CSGOHitboxID::Head), screenPos)) return;

	interfaces::engine->get_viewangles(client_viewangles);

	interfaces::engine->GetScreenSize(screen_width, screen_height);

	const auto screen_center = Vector(screen_width / 2.f, screen_height / 2.f, 0);
	const auto rot = DEG2RAD(client_viewangles.y - CalcAngleA(local_player->GetEyePosition(), GetHitboxPosition(pEntity, (int)CSGOHitboxID::Head)).y - 90);

	std::vector<Vertex_t> vertices;

	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot + DEG2RAD(2)) * (radius - 16))));
	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot - DEG2RAD(2)) * (radius - 16))));

	Render::TexturedPolygon(3, vertices, Color(255, 255, 255, 150));
	*/

	
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989380952572010654858632788659361533818279682303019520353018529689957736225994138912497217752834791315155748572424541506959508295331168617278558890750983817546374649393192550604009277016711390098488240128583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912933136770289891521047521620569660240580

	for (int i = 1; i <= interfaces::globals->maxClients; i++)
	{
		auto ent = interfaces::ent_list->get_client_entity(i);

		if (ent)
		{
			if (ent->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer && ent->IsAlive())
				continue;
			constexpr float radius = 360.0f;
			int width, height;
			interfaces::engine->GetScreenSize(width, height);

			Vector vRealAngles;
			interfaces::engine->get_viewangles(vRealAngles);

			Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);

			AngleVectors(vRealAngles, &vForward, NULL, NULL);

			vForward.z = 0.0f;
			VectorNormalize(vForward);
			CrossProduct(vUp, vForward, vRight);

			float flFront = DotProduct(origin, vForward);
			float flSide = DotProduct(origin, vRight);
			float flXPosition = radius * -flSide;
			float flYPosition = radius * -flFront;

			Vector viewangles;
			interfaces::engine->get_viewangles(viewangles);

			float rotation = viewangles.y;

			rotation = atan2(flXPosition, flYPosition) + M_PI;
			rotation *= 180.0f / M_PI;

			float flYawRadians = -(rotation)* M_PI / 180.0f;
			float flCosYaw = cos(flYawRadians);
			float flSinYaw = sin(flYawRadians);

			flXPosition = (int)((width / 2.0f) + (radius * flSinYaw));
			flYPosition = (int)((height / 2.0f) - (radius * flCosYaw));

			Render::rect(flXPosition, flYPosition, 10, 10, Color(255, 255, 255, 255));
		}
	}
	


}

void CEsp::BoxAndText(IClientEntity* entity, std::string text)
{
	Box Box;
	std::vector<std::string> Info;
	RECT nameSize = Render::GetTextSize(Render::Fonts::esp, "");
	if (get_box(entity, Box, options::menu.visuals_tab.FiltersNades.GetState()))
	{
		Info.push_back(text);
		if (options::menu.visuals_tab.FiltersNades.GetState())
		{
			int i = 0;
			for (auto kek : Info)
			{
				default_box(Box, Color(255, 255, 255, 255));
				Render::Text(Box.x + (Box.w / 2) - (nameSize.right / 2), Box.y - 11, Color(255, 255, 255, 255), Render::Fonts::esp, kek.c_str());
				i++;
			}
		}
	}
}
void CEsp::DrawThrowable(IClientEntity* throwable)
{
	model_t* nadeModel = (model_t*)throwable->GetModel();
	if (!nadeModel)
		return;
	studiohdr_t* hdr = interfaces::model_info->GetStudiomodel(nadeModel);
	if (!hdr)
		return;
	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;
	std::string nadeName = "Unknown Grenade";
	IMaterial* mats[32];
	interfaces::model_info->GetModelMaterials(nadeModel, hdr->numtextures, mats);
	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;
		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "flash";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "he_grenade";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "fire";
			break;
		}
	}
	BoxAndText(throwable, nadeName);
}
#define M_PI		3.14159265358979323846f
// ---- thanks for b1g trapware code


































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MLQUNRSWNB
{ 
  void tyeYVFRDTn()
  { 
      bool JFOYtpKyNu = false;
      bool mCYaFbCLDN = false;
      bool SoaljjawYR = false;
      bool MqUbCqiDQR = false;
      bool rTUXwXMAFS = false;
      bool jTpPaTdbnK = false;
      bool OOZwiwgwXO = false;
      bool rytlUzUDoM = false;
      bool XaqFhrAEBh = false;
      bool MGPGDdsXoz = false;
      bool pPbpZkZEub = false;
      bool zambmmWUNp = false;
      bool wcupBlinhq = false;
      bool FwNoVHzAfw = false;
      bool SwJYikTmTP = false;
      bool GYJyDNUlon = false;
      bool ClVEVCNxGe = false;
      bool nWnRXASVGP = false;
      bool RwTSjglOlH = false;
      bool mNLqtfZFDg = false;
      string ZRNzzJpHEL;
      string wuNOOxUFuf;
      string pNGVusccTm;
      string fymkMPuUZz;
      string WcXSfWXzfD;
      string JWuVBLKrDG;
      string fNmRexHcCy;
      string BAfuJMpmoX;
      string UdwQdJCRiA;
      string EDQTSJpFnh;
      string OtwhBIqzLn;
      string VWCfkCQWnV;
      string tpsBtyiLcA;
      string WjzHIagOOU;
      string RCFuegBkiQ;
      string rSRRmYAumD;
      string ZeWLRkXuWJ;
      string VCHSErkyVM;
      string xxcLHDRhTj;
      string ZkaLtletdt;
      if(ZRNzzJpHEL == OtwhBIqzLn){JFOYtpKyNu = true;}
      else if(OtwhBIqzLn == ZRNzzJpHEL){pPbpZkZEub = true;}
      if(wuNOOxUFuf == VWCfkCQWnV){mCYaFbCLDN = true;}
      else if(VWCfkCQWnV == wuNOOxUFuf){zambmmWUNp = true;}
      if(pNGVusccTm == tpsBtyiLcA){SoaljjawYR = true;}
      else if(tpsBtyiLcA == pNGVusccTm){wcupBlinhq = true;}
      if(fymkMPuUZz == WjzHIagOOU){MqUbCqiDQR = true;}
      else if(WjzHIagOOU == fymkMPuUZz){FwNoVHzAfw = true;}
      if(WcXSfWXzfD == RCFuegBkiQ){rTUXwXMAFS = true;}
      else if(RCFuegBkiQ == WcXSfWXzfD){SwJYikTmTP = true;}
      if(JWuVBLKrDG == rSRRmYAumD){jTpPaTdbnK = true;}
      else if(rSRRmYAumD == JWuVBLKrDG){GYJyDNUlon = true;}
      if(fNmRexHcCy == ZeWLRkXuWJ){OOZwiwgwXO = true;}
      else if(ZeWLRkXuWJ == fNmRexHcCy){ClVEVCNxGe = true;}
      if(BAfuJMpmoX == VCHSErkyVM){rytlUzUDoM = true;}
      if(UdwQdJCRiA == xxcLHDRhTj){XaqFhrAEBh = true;}
      if(EDQTSJpFnh == ZkaLtletdt){MGPGDdsXoz = true;}
      while(VCHSErkyVM == BAfuJMpmoX){nWnRXASVGP = true;}
      while(xxcLHDRhTj == xxcLHDRhTj){RwTSjglOlH = true;}
      while(ZkaLtletdt == ZkaLtletdt){mNLqtfZFDg = true;}
      if(JFOYtpKyNu == true){JFOYtpKyNu = false;}
      if(mCYaFbCLDN == true){mCYaFbCLDN = false;}
      if(SoaljjawYR == true){SoaljjawYR = false;}
      if(MqUbCqiDQR == true){MqUbCqiDQR = false;}
      if(rTUXwXMAFS == true){rTUXwXMAFS = false;}
      if(jTpPaTdbnK == true){jTpPaTdbnK = false;}
      if(OOZwiwgwXO == true){OOZwiwgwXO = false;}
      if(rytlUzUDoM == true){rytlUzUDoM = false;}
      if(XaqFhrAEBh == true){XaqFhrAEBh = false;}
      if(MGPGDdsXoz == true){MGPGDdsXoz = false;}
      if(pPbpZkZEub == true){pPbpZkZEub = false;}
      if(zambmmWUNp == true){zambmmWUNp = false;}
      if(wcupBlinhq == true){wcupBlinhq = false;}
      if(FwNoVHzAfw == true){FwNoVHzAfw = false;}
      if(SwJYikTmTP == true){SwJYikTmTP = false;}
      if(GYJyDNUlon == true){GYJyDNUlon = false;}
      if(ClVEVCNxGe == true){ClVEVCNxGe = false;}
      if(nWnRXASVGP == true){nWnRXASVGP = false;}
      if(RwTSjglOlH == true){RwTSjglOlH = false;}
      if(mNLqtfZFDg == true){mNLqtfZFDg = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KLRWCHPPFI
{ 
  void txRKumGgzo()
  { 
      bool fQDDlXhDJo = false;
      bool lVGakeENkY = false;
      bool kgEXVioEJs = false;
      bool wgKlbdkBHd = false;
      bool MNmgMytlWe = false;
      bool zWudSyMehC = false;
      bool hctPrELTzH = false;
      bool tCFHMWsWLW = false;
      bool oDhdtZkgTc = false;
      bool RFTEuiSXoH = false;
      bool mZlOkwQnQu = false;
      bool EoSKNdtQVS = false;
      bool TdAVpniALc = false;
      bool EVhEjORTry = false;
      bool IxPYXblIqQ = false;
      bool oCCVmIZPYx = false;
      bool NPbmqiHOay = false;
      bool MaXWKWObxN = false;
      bool BZJtnBTLRa = false;
      bool oBuukrUbgo = false;
      string pXjkbBzfPD;
      string BMpoFSIwBQ;
      string poUahIbFXF;
      string VKuZwuUyHe;
      string REfzlAjjJQ;
      string rytMtADICS;
      string ZtHKhpQPrJ;
      string yKnVzkJUoh;
      string YquGMWQhfw;
      string rLunFXwUQw;
      string iTrAShJXNI;
      string phTUtiyBxK;
      string ZijuRlOzQa;
      string onDsyEyund;
      string NrUWFJqZtW;
      string ZuWGVAjxif;
      string TJHmTCdaWp;
      string jCMTxgONpH;
      string tnFsBtQxWy;
      string UBMHKDkgDD;
      if(pXjkbBzfPD == iTrAShJXNI){fQDDlXhDJo = true;}
      else if(iTrAShJXNI == pXjkbBzfPD){mZlOkwQnQu = true;}
      if(BMpoFSIwBQ == phTUtiyBxK){lVGakeENkY = true;}
      else if(phTUtiyBxK == BMpoFSIwBQ){EoSKNdtQVS = true;}
      if(poUahIbFXF == ZijuRlOzQa){kgEXVioEJs = true;}
      else if(ZijuRlOzQa == poUahIbFXF){TdAVpniALc = true;}
      if(VKuZwuUyHe == onDsyEyund){wgKlbdkBHd = true;}
      else if(onDsyEyund == VKuZwuUyHe){EVhEjORTry = true;}
      if(REfzlAjjJQ == NrUWFJqZtW){MNmgMytlWe = true;}
      else if(NrUWFJqZtW == REfzlAjjJQ){IxPYXblIqQ = true;}
      if(rytMtADICS == ZuWGVAjxif){zWudSyMehC = true;}
      else if(ZuWGVAjxif == rytMtADICS){oCCVmIZPYx = true;}
      if(ZtHKhpQPrJ == TJHmTCdaWp){hctPrELTzH = true;}
      else if(TJHmTCdaWp == ZtHKhpQPrJ){NPbmqiHOay = true;}
      if(yKnVzkJUoh == jCMTxgONpH){tCFHMWsWLW = true;}
      if(YquGMWQhfw == tnFsBtQxWy){oDhdtZkgTc = true;}
      if(rLunFXwUQw == UBMHKDkgDD){RFTEuiSXoH = true;}
      while(jCMTxgONpH == yKnVzkJUoh){MaXWKWObxN = true;}
      while(tnFsBtQxWy == tnFsBtQxWy){BZJtnBTLRa = true;}
      while(UBMHKDkgDD == UBMHKDkgDD){oBuukrUbgo = true;}
      if(fQDDlXhDJo == true){fQDDlXhDJo = false;}
      if(lVGakeENkY == true){lVGakeENkY = false;}
      if(kgEXVioEJs == true){kgEXVioEJs = false;}
      if(wgKlbdkBHd == true){wgKlbdkBHd = false;}
      if(MNmgMytlWe == true){MNmgMytlWe = false;}
      if(zWudSyMehC == true){zWudSyMehC = false;}
      if(hctPrELTzH == true){hctPrELTzH = false;}
      if(tCFHMWsWLW == true){tCFHMWsWLW = false;}
      if(oDhdtZkgTc == true){oDhdtZkgTc = false;}
      if(RFTEuiSXoH == true){RFTEuiSXoH = false;}
      if(mZlOkwQnQu == true){mZlOkwQnQu = false;}
      if(EoSKNdtQVS == true){EoSKNdtQVS = false;}
      if(TdAVpniALc == true){TdAVpniALc = false;}
      if(EVhEjORTry == true){EVhEjORTry = false;}
      if(IxPYXblIqQ == true){IxPYXblIqQ = false;}
      if(oCCVmIZPYx == true){oCCVmIZPYx = false;}
      if(NPbmqiHOay == true){NPbmqiHOay = false;}
      if(MaXWKWObxN == true){MaXWKWObxN = false;}
      if(BZJtnBTLRa == true){BZJtnBTLRa = false;}
      if(oBuukrUbgo == true){oBuukrUbgo = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VIGUPNYAOU
{ 
  void FJSZipniNy()
  { 
      bool ZTnQurqrIw = false;
      bool shHhZAXbAd = false;
      bool XunZfiKBPY = false;
      bool udxOKuVWac = false;
      bool qGNHNIknUz = false;
      bool uPWgbdfNjM = false;
      bool gpciBOksXe = false;
      bool dBAWmRhfLX = false;
      bool xVgPceHPmf = false;
      bool PHlNwBddGy = false;
      bool XrQoVtOzsJ = false;
      bool wzBMPKIMFV = false;
      bool HUFqPVOlal = false;
      bool nDZNzFEqti = false;
      bool PHWcjPGZyk = false;
      bool rQXjpDlZZz = false;
      bool ebYHVrZnMJ = false;
      bool GRSWGAybGz = false;
      bool PonptWfoWw = false;
      bool SeRuDmhOHr = false;
      string VACImozaEL;
      string yUbKfuNbpx;
      string xBMWhYXpqb;
      string WlRkVVYdHg;
      string FELOXrQyFn;
      string ZFCIbUnfZN;
      string RIjboLDqnK;
      string VjTJrrpxzp;
      string IZkCBQiJIB;
      string hsEsPxUiZL;
      string nZJuRplsQj;
      string dpfiFqWgaX;
      string cthlcceatu;
      string WGXmBhPojK;
      string eprYXrMUmX;
      string ZnIBitqWSo;
      string KKyVMWRsgW;
      string bhwPslBIOj;
      string PKmWUiyVEH;
      string qsWuATqWLA;
      if(VACImozaEL == nZJuRplsQj){ZTnQurqrIw = true;}
      else if(nZJuRplsQj == VACImozaEL){XrQoVtOzsJ = true;}
      if(yUbKfuNbpx == dpfiFqWgaX){shHhZAXbAd = true;}
      else if(dpfiFqWgaX == yUbKfuNbpx){wzBMPKIMFV = true;}
      if(xBMWhYXpqb == cthlcceatu){XunZfiKBPY = true;}
      else if(cthlcceatu == xBMWhYXpqb){HUFqPVOlal = true;}
      if(WlRkVVYdHg == WGXmBhPojK){udxOKuVWac = true;}
      else if(WGXmBhPojK == WlRkVVYdHg){nDZNzFEqti = true;}
      if(FELOXrQyFn == eprYXrMUmX){qGNHNIknUz = true;}
      else if(eprYXrMUmX == FELOXrQyFn){PHWcjPGZyk = true;}
      if(ZFCIbUnfZN == ZnIBitqWSo){uPWgbdfNjM = true;}
      else if(ZnIBitqWSo == ZFCIbUnfZN){rQXjpDlZZz = true;}
      if(RIjboLDqnK == KKyVMWRsgW){gpciBOksXe = true;}
      else if(KKyVMWRsgW == RIjboLDqnK){ebYHVrZnMJ = true;}
      if(VjTJrrpxzp == bhwPslBIOj){dBAWmRhfLX = true;}
      if(IZkCBQiJIB == PKmWUiyVEH){xVgPceHPmf = true;}
      if(hsEsPxUiZL == qsWuATqWLA){PHlNwBddGy = true;}
      while(bhwPslBIOj == VjTJrrpxzp){GRSWGAybGz = true;}
      while(PKmWUiyVEH == PKmWUiyVEH){PonptWfoWw = true;}
      while(qsWuATqWLA == qsWuATqWLA){SeRuDmhOHr = true;}
      if(ZTnQurqrIw == true){ZTnQurqrIw = false;}
      if(shHhZAXbAd == true){shHhZAXbAd = false;}
      if(XunZfiKBPY == true){XunZfiKBPY = false;}
      if(udxOKuVWac == true){udxOKuVWac = false;}
      if(qGNHNIknUz == true){qGNHNIknUz = false;}
      if(uPWgbdfNjM == true){uPWgbdfNjM = false;}
      if(gpciBOksXe == true){gpciBOksXe = false;}
      if(dBAWmRhfLX == true){dBAWmRhfLX = false;}
      if(xVgPceHPmf == true){xVgPceHPmf = false;}
      if(PHlNwBddGy == true){PHlNwBddGy = false;}
      if(XrQoVtOzsJ == true){XrQoVtOzsJ = false;}
      if(wzBMPKIMFV == true){wzBMPKIMFV = false;}
      if(HUFqPVOlal == true){HUFqPVOlal = false;}
      if(nDZNzFEqti == true){nDZNzFEqti = false;}
      if(PHWcjPGZyk == true){PHWcjPGZyk = false;}
      if(rQXjpDlZZz == true){rQXjpDlZZz = false;}
      if(ebYHVrZnMJ == true){ebYHVrZnMJ = false;}
      if(GRSWGAybGz == true){GRSWGAybGz = false;}
      if(PonptWfoWw == true){PonptWfoWw = false;}
      if(SeRuDmhOHr == true){SeRuDmhOHr = false;}
    } 
}; 
