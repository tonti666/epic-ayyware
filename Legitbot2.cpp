#include "sdk.h"
//#include "backtrackmanager.h"
#include "Legitbot2.h"
//#include "Globals.h"
/*
//#include "GameUtils.h"
#include "Math.h"
#include <iostream>

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

CLegitBot* g_Legitbot = new CLegitBot();

//stiz to kurwa



void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
	HitBox = -1;
}

void CLegitBot::Draw()
{

}

static int CustomDelay = 0;
static int CustomBreak = 0;

void CLegitBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{

	if (!options::menu.LegitBotTab.Active.GetState())
		return;
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	static int CustomAimTime = 0;
	static int CustomAimStart = 0;
	if (options::menu.LegitBotTab.AimbotEnable.GetState())
	{
		if (StartAim > 0)
		{
			if (CustomAimStart < (StartAim * 333))
			{
				CustomAimStart++;
			}
			else
			{
				if (Aimtime > 0)
				{
					if (CustomAimTime < (Aimtime * 333))
					{

						DoAimbot(pCmd, bSendPacket);
						CustomAimTime++;
					}
					if (!GetAsyncKeyState(options::menu.LegitBotTab.AimbotKeyBind.GetKey()))
					{
						CustomAimTime = 0;
						CustomAimStart = 0;
					}
				}
				else
				{

					DoAimbot(pCmd, bSendPacket);
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}

			if (!GetAsyncKeyState(options::menu.LegitBotTab.AimbotKeyBind.GetKey()))
			{
				CustomAimStart = 0;
				CustomAimTime = 0;
			}
		}
		else
		{
			if (Aimtime > 0)
			{
				if (CustomAimTime < (Aimtime * 333))
				{
					DoAimbot(pCmd, bSendPacket);
					CustomAimTime++;
				}
				if (!GetAsyncKeyState(options::menu.LegitBotTab.AimbotKeyBind.GetKey()))
				{
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}
			else
			{
				DoAimbot(pCmd, bSendPacket);
				CustomAimTime = 0;
				CustomAimStart = 0;
			}
		}
	}
	//if (Menu.LegitBot.triggerboton && (GetAsyncKeyState(Menu.LegitBot.triggerbotkey)))
	//{

	//	DoTrigger(pCmd);
	//}

	SyncWeaponSettings();
}

void CLegitBot::SyncWeaponSettings()
{
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());


	if (!pWeapon)
		return;

	if (pWeapon->isPistol())
	{

		Speed = options::menu.LegitBotTab.WeaponPistSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponPistFoV.GetValue() * 2;
		PSilent = options::menu.LegitBotTab.WeaponPistPSilent.GetState();
		//RecoilControl = Menu.LegitBot.pis_rcs;

		switch (options::menu.LegitBotTab.WeaponPistHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;

			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;

			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;

			break;
		case 4:
			Multihitbox = true;
			break;
		}
		Aimtime = 0;
		StartAim = 0;
	}
	else if (pWeapon->m_bIsSniper())
	{
		Speed = options::menu.LegitBotTab.WeaponSnipSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponSnipFoV.GetValue() * 2;
		PSilent = options::menu.LegitBotTab.WeaponSnipPSilent.GetState();
		//RecoilControl = Menu.LegitBot.sni_rcs;

		switch (options::menu.LegitBotTab.WeaponSnipHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;

			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;

			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;

			break;
		case 4:
			Multihitbox = true;
			break;

		}
		Aimtime = 0;
		StartAim = 0;
	}
	else if (pWeapon->IsMiscGAY())
	{
		Speed = options::menu.LegitBotTab.WeaponMpSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponMpFoV.GetValue() * 2;
		//PSilent = Menu.LegitBot.smg_pSilent;
		RecoilControl = options::menu.LegitBotTab.WeaponMpRecoil.GetValue();

		switch (options::menu.LegitBotTab.WeaponMpHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;

			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;

			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;

			break;
		case 4:
			Multihitbox = true;
			break;

		}
		Aimtime = 0;
		StartAim = 0;
	}
	else
	{

		Speed = options::menu.LegitBotTab.WeaponMainSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponMainFoV.GetValue() * 2;
		//PSilent = Menu.LegitBot.def_pSilent;
		RecoilControl = options::menu.LegitBotTab.WeaponMainrcs.GetValue();

		switch (options::menu.LegitBotTab.WeaponMainHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;

			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;

			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;

			break;
		case 4:
			Multihitbox = true;
			break;

		}
		Aimtime = 0;
		StartAim = 0;
	}
}

void CLegitBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || pWeapon->IsMiscGAY())
		{
			return;
		}
		SyncWeaponSettings();
	}
	else
		return;

	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{

		pTarget = interfaces::ent_list->get_client_entity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			SyncWeaponSettings();
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; interfaces::engine->get_viewangles(View);
				if (pLocal->GetVelocity().Length() > 45.f);
				View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * RecoilControl;
				float nFoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (nFoV < FoV)
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		TargetID = GetTargetCrosshair();

		if (TargetID >= 0)
		{

			pTarget = interfaces::ent_list->get_client_entity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	SyncWeaponSettings();

	if (TargetID >= 0 && pTarget)
	{
		SyncWeaponSettings();

		if (!options::menu.LegitBotTab.legitresolver.GetState())
		{

			int Key = options::menu.LegitBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GetAsyncKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector AimPoint;

		if (Multihitbox)
		{

			AimPoint = GameUtils::get_hitbox_location(pTarget, besthitbox);
		}
		else
		{

			AimPoint = GameUtils::get_hitbox_location(pTarget, HitBox);
		}

		if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
		{
			if (options::menu.LegitBotTab.legitresolver.GetState() && !(pCmd->buttons & IN_ATTACK))
			{

				pCmd->buttons |= IN_ATTACK;
			}
		}
	}

}

bool CLegitBot::TargetMeetsTriggerRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex() && pEntity->GetIndex() < 65)
	{
		if (pEntity->team() != hackManager.pLocal()->team() || options::menu.LegitBotTab.AimbotFriendlyFire.GetState())
		{
			if (!pEntity->has_gungame_immunity())
			{

				return true;
			}
		}

	}

	return false;
}
/*
void CLegitBot::DoTrigger(CUserCmd *pCmd)
{
	CBaseEntity* LocalPlayer = csgo::LocalPlayer;

	auto LocalPlayerWeapon = csgo::MainWeapon;

	if (LocalPlayerWeapon) {
		if (LocalPlayerWeapon->GetLoadedAmmo() == 0)
			return;

		if (LocalPlayerWeapon->IsMiscWeapon())
			return;
	}
	else
		return;

	Vector ViewAngles = pCmd->viewangles;
	if (Menu.LegitBot.triggerrcs)
		ViewAngles += LocalPlayer->GetPunchAngle() * 2.0f;

	Vector CrosshairForward;
	Math::AngleVectors(ViewAngles, &CrosshairForward);
	CrosshairForward *= 8000.f;


	Vector TraceSource = LocalPlayer->GetEyePosition();
	Vector TraceDestination = TraceSource + CrosshairForward;

	Ray_t Ray;
	trace_t Trace;
	CTraceFilter Filter;

	Filter.pSkip1 = LocalPlayer;

	Ray.Init(TraceSource, TraceDestination);
	g_pEngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->isAlive())
		return;
	if (Trace.m_pEnt->GetHealth() <= 0 || Trace.m_pEnt->GetHealth() > 100)
		return;
	if (Trace.m_pEnt->IsProtected())
		return;

	if (!Menu.LegitBot.triggerteam) {
		if (LocalPlayer->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
			return;
	}

	if (Menu.LegitBot.triggersmokecheck)
	{

		typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

		static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternIDA("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

		if (GoesThroughSmokeFunction(csgo::LocalPlayer->GetEyePosition(), Trace.m_pEnt->GetBonePos(6)))
			return;
	}

	if ((Menu.LegitBot.headhitscan && Trace.hitgroup == HITGROUP_HEAD) ||
		(Menu.LegitBot.chesthitscan && Trace.hitgroup == HITGROUP_CHEST) ||
		(Menu.LegitBot.stomachhitscan && Trace.hitgroup == HITGROUP_STOMACH) ||
		(Menu.LegitBot.armshitscan && (Trace.hitgroup == HITGROUP_LEFTARM || Trace.hitgroup == HITGROUP_RIGHTARM)) ||
		(Menu.LegitBot.legshitscan && (Trace.hitgroup == HITGROUP_LEFTLEG || Trace.hitgroup == HITGROUP_RIGHTLEG))) {
		pCmd->buttons |= IN_ATTACK;
	}
}
*/
/*
bool CLegitBot::TargetMeetsRequirements(IClientEntity* pEntity)
{

	if (pEntity && pEntity->IsDormant() == false && pEntity->isValidPlayer() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		player_info_t pinfo;
		if (pEntity->isValidPlayer() && interfaces::engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->team() != hackManager.pLocal()->team() || options::menu.LegitBotTab.AimbotFriendlyFire.GetState())
			{

				if (options::menu.LegitBotTab.AimbotSmokeCheck.GetState()) {
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

					if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(8)))
						return false;
				}

				if (Multihitbox)
				{
					if (!pEntity->has_gungame_immunity())
					{
						if (GameUtils::isVisible(pEntity, 0) || GameUtils::isVisible(pEntity, 8) || GameUtils::isVisible(pEntity, 4) || GameUtils::isVisible(pEntity, 1) || GameUtils::isVisible(pEntity, 2) || GameUtils::isVisible(pEntity, 3) || GameUtils::isVisible(pEntity, 5) || GameUtils::isVisible(pEntity, 7) || GameUtils::isVisible(pEntity, 9) || GameUtils::isVisible(pEntity, 10) || GameUtils::isVisible(pEntity, 6))
							return true;
						else
							return false;
					}
				}
				else
				{
					if (!pEntity->has_gungame_immunity() && GameUtils::isVisible(pEntity, HitBox))
					{
						return true;
					}
				}

			}

		}
	}
	return false;
}

float Get3dDistance(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleVectors(Angles, &Forward);
	Vector AimPos = GameUtils::get_hitbox_location(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Normalize(Delta, Delta);

	float Distance = Get3dDistance(Origin, AimPos);

	float pitch = sin(Forward.x - Delta.x) * Distance;
	float yaw = sin(Forward.y - Delta.y) * Distance;
	float zaw = sin(Forward.z - Delta.z) * Distance;

	float mag = sqrt((pitch*pitch) + (yaw*yaw) + (zaw*zaw));
	return mag;
}

int CLegitBot::GetTargetCrosshair()
{
	SyncWeaponSettings();
	int target = -1;


	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; interfaces::engine->get_viewangles(View);
	View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);

		if (TargetMeetsRequirements(pEntity))
		{
			if (Multihitbox)
			{

				float fov1 = FovToPlayer(ViewOffset, View, pEntity, 0);
				float fov2 = FovToPlayer(ViewOffset, View, pEntity, 4);
				float fov3 = FovToPlayer(ViewOffset, View, pEntity, 6);

				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}

				if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 4;
				}

				if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 6;
				}

			}
			else
			{

				int NewHitBox = HitBox;
				if (NewHitBox >= 0)
				{
					float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
					if (fov < FoV)
					{
						FoV = fov;
						target = i;
					}
				}
			}
		}
	}

	return target;
}

bool ShouldFire()
{
	IClientEntity* pLocalEntity = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	C_BaseCombatWeapon* entwep = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());


	float flServerTime = (float)pLocalEntity->GetTickBase() * interfaces::globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}


bool CLegitBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{

	if (point.Length() == 0) return false;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (RecoilControl > 0)
	{

		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * RecoilControl;
			GameUtils::NormaliseViewAngle(angles);
		}
	}

	IsLocked = true;

	Vector shit = angles - pCmd->viewangles;
	bool v = false;
	GameUtils::NormaliseViewAngle(shit);
	if (shit.Length() > Speed)
	{
		Normalize(shit, shit);
		shit *= Speed;
	}
	else
	{
		v = true;

	}

	Vector ang = angles - pCmd->viewangles;
	bool f = false;

	if (ang.Length() > Speed)
	{
		Normalize(ang, ang);
		ang *= Speed;
	}
	else
	{
		f = true;
	}

	if (PSilent)
	{
		Vector Oldview = pCmd->viewangles;
		Vector qAimAngles = pCmd->viewangles;
		float Oldsidemove = pCmd->sidemove;
		float Oldforwardmove = pCmd->forwardmove;

		static int ChokedPackets = -1;

		if (ShouldFire() && ChokedPackets < 6)
		{
			bSendPacket = false;
			pCmd->viewangles += ang;
			pCmd->viewangles = angles;
			ChokedPackets++;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles = Oldview;
			pCmd->sidemove = Oldsidemove;
			pCmd->forwardmove = Oldforwardmove;
			ChokedPackets = -1;
		}

		pCmd->viewangles.z = 0;
	}
	else
	{
		pCmd->viewangles += ang;
		interfaces::engine->SetViewAngles(pCmd->viewangles);
	}
	return f;

	pCmd->viewangles += shit;
	interfaces::engine->SetViewAngles(pCmd->viewangles);

	return f;
}*/