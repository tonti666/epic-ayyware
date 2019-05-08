#include "legitbot.h"

#include <iostream>
void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
}

void CLegitBot::Draw()
{

}



void CLegitBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	// Master switch
	if (!options::menu.LegitBotTab.Active.GetState())
		return;

	// Aimbot
	if (options::menu.LegitBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	if (options::menu.LegitBotTab.StandAloneRCS.GetState())
		StandAloneRCS(pLocal, pCmd, pWeapon);



	// Triggerbot
	//if (options::menu.LegitBotTab.TriggerEnable.GetState() && (!options::menu.LegitBotTab.TriggerKeyPress.GetState() || GUI.GetKeyState(options::menu.LegitBotTab.TriggerKeyBind.GetKey())))
	//	DoTrigger(pCmd);
}

void CLegitBot::SyncWeaponSettings()
{
	std::vector<int> HitBoxesToScan;
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());


	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		Speed = options::menu.LegitBotTab.WeaponPistSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponPistFoV.GetValue();
		//RecoilControl = options::menu.LegitBotTab.WeaponPistRecoil.GetState();
		PSilent = options::menu.LegitBotTab.WeaponPistPSilent.GetState();
		//Inacc = options::menu.LegitBotTab.WeaponPistInacc.GetValue();


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
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		Speed = options::menu.LegitBotTab.WeaponSnipSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponSnipFoV.GetValue();
		//RecoilControl = options::menu.LegitBotTab.WeaponSnipRecoil.GetState();
		PSilent = options::menu.LegitBotTab.WeaponSnipPSilent.GetState();
		//Inacc = options::menu.LegitBotTab.WeaponSnipInacc.GetValue();

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
	}
	else if (GameUtils::IsRifle(pWeapon))
	{
		Speed = options::menu.LegitBotTab.WeaponMainSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponMainFoV.GetValue();
		RecoilControl = options::menu.LegitBotTab.WeaponMainRecoil.GetValue();


		//PSilent = options::menu.LegitBotTab.WeaponMainPSilent.GetState();
		Inacc = options::menu.LegitBotTab.WeaponMainInacc.GetValue();

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
	}
	else if (GameUtils::IsMP(pWeapon))
	{
		Speed = options::menu.LegitBotTab.WeaponMpSpeed.GetValue() / 100;
		FoV = options::menu.LegitBotTab.WeaponMpFoV.GetValue();
		RecoilControl = options::menu.LegitBotTab.WeaponMpRecoil.GetValue();
		//PSilent = options::menu.LegitBotTab.WeaponMainPSilent.GetState();
		Inacc = options::menu.LegitBotTab.weaponmpinacc.GetValue();

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
	}
}

// Functionality
void CLegitBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;
	//IsLocked = false;

	// Don't aimbot with the knife..
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			//TargetID = 0;
			//pTarget = nullptr;
			//HitBox = -1;
			return;
		}
		SyncWeaponSettings();

	}
	else
		return;

	// Make sure we have a good target
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
				View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2;
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

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		//HitBox = (int)CSGOHitboxID::Head;//
		SyncWeaponSettings();

		// Key
		if (options::menu.LegitBotTab.AimbotKeyPress.GetState())
		{
			int Key = options::menu.LegitBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		//Vector AimPoint = GetHitboxPosition(pTarget, HitBox);

		if (Multihitbox)
		{
			AimPoint = GetHitboxPosition(pTarget, besthitbox);
		}
		else
		{
			AimPoint = GetHitboxPosition(pTarget, HitBox);
		}

		if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
		{

		}
	}

	// Auto Pistol

}

bool TargetMeetsTriggerRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && interfaces::engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->team() != hackManager.pLocal()->team() || options::menu.MiscTab.dangerzone.GetState())
			{
				{
					if (options::menu.LegitBotTab.AimbotSmokeCheck.GetState())
					{
						typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

						static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
						static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;


						if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(8)))
							return false;
					}
				}
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}
/*
void CLegitBot::DoTrigger(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	// Don't triggerbot with the knife..
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->get_client_entity(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
	}
	else
		return;

	// Triggerbot
	// Get the view with the recoil
	Vector ViewAngles;
	interfaces::engine->get_viewangles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	// Build a ray going fowards at that angle
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	// Get ray start / end
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	trace_t Trace;
	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt && 0 < Trace.hitgroup <= 7) // hitbox not hitgroup
	{
		if (TargetMeetsTriggerRequirements(Trace.m_pEnt) && !time < options::menu.LegitBotTab.TriggerDelay.GetValue())
		{
			float time = 0;
			time++;
			float delay = options::menu.LegitBotTab.TriggerDelay.GetValue() / 1000.f;

			if ((time * 64) < delay)
			{
				return;
			}
			else
			{
				pCmd->buttons |= IN_ATTACK;
				time = 0;
			}
		}
	}

	// Auto Pistol
	if (GameUtils::IsPistol(pWeapon) && options::menu.LegitBotTab.AimbotAutoPistol.GetState())
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			static bool WasFiring = false;
			WasFiring = !WasFiring;

			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
	}
}
*/
bool CLegitBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && interfaces::engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->team() != hackManager.pLocal()->team() || options::menu.MiscTab.dangerzone.GetState())

			{
				if (options::menu.LegitBotTab.AimbotSmokeCheck.GetState())
				{
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;


					if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(8)))
						return false;
				}

				if (Multihitbox)
				{
					if (!pEntity->HasGunGameImmunity())
					{
						if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, 0) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 8) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 4 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 1 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 2 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 3 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 5 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 7 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 9 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 10 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 6))))))))))
							return true;
						else
							return false;
					}
				}
				else
				{
					if (!pEntity->HasGunGameImmunity() && GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBox))
					{
						return true;
					}
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

void ClampAngles2(Vector& v)
{
	if (v.x > 89.0f && v.x <= 180.0f)
		v.x = 89.0f;
	if (v.x > 180.0f)
		v.x = v.x - 360.0f;
	if (v.x < -89.0f)
		v.x = -89.0f;
	v.y = fmodf(v.y + 180, 360) - 180;
	v.z = 0;
}

void CLegitBot::StandAloneRCS(IClientEntity* pLocal, CUserCmd* pCmd, C_BaseCombatWeapon* pWeapon)
{
	static Vector vOldPunch = { 0.0f, 0.0f, 0.0f };
	Vector vNewPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();

	int amountxdxd = options::menu.LegitBotTab.StandAloneRCSamount.GetValue();


	vNewPunch *= amountxdxd;

	vNewPunch -= vOldPunch;
	vNewPunch *= 0.1f;
	vNewPunch += vOldPunch;

	Vector vFinal = pCmd->viewangles - (vNewPunch - vOldPunch);

	NormalizeVector(vFinal);
	ClampAngles2(vFinal);

	if (pWeapon->isRifle())
		interfaces::engine->SetViewAngles(vFinal);


	vOldPunch = vNewPunch;

}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	Vector out[9];

	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}



int CLegitBot::GetTargetCrosshair()
{
	// Target selection
	SyncWeaponSettings();
	int target = -1;
	float minFoV = FoV;

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
				float fov2 = FovToPlayer(ViewOffset, View, pEntity, 1);
				float fov3 = FovToPlayer(ViewOffset, View, pEntity, 2);
				float fov4 = FovToPlayer(ViewOffset, View, pEntity, 3);
				float fov5 = FovToPlayer(ViewOffset, View, pEntity, 4);


				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV && fov2 < FoV && fov1 < FoV || fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV || fov5 < FoV && fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}

				if (fov2 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV && fov2 < FoV && fov1 < FoV || fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV || fov5 < FoV && fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 1;
				}

				if (fov3 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV && fov2 < FoV && fov1 < FoV || fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV || fov5 < FoV && fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 2;
				}

				if (fov4 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV && fov2 < FoV && fov1 < FoV || fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV || fov5 < FoV && fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV)
				{
					FoV = fov4;
					target = i;
					besthitbox = 3;
				}

				if (fov5 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV && fov2 < FoV && fov1 < FoV || fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV || fov5 < FoV && fov4 < FoV && fov3 < FoV && fov2 < FoV && fov1 < FoV)
				{
					FoV = fov5;
					target = i;
					besthitbox = 4;
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
	// Get the full angles
	if (point.Length() == 0) return false;

	Vector angles;
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	static Vector Inaccuracy;
	static Vector Inaccuracy1;

	if (timeSoFar > 0.2)
	{
		Inaccuracy.Init(-50 + rand() % 100, -50 + rand() % 100, -50 + rand() % 100);
		Inaccuracy.NormalizeInPlace();
		Inaccuracy *= Inacc;
		Inaccuracy1.Init(-50 + rand() % 100, -50 + rand() % 100, -50 + rand() % 100);
		Inaccuracy1.NormalizeInPlace();
		Inaccuracy1 *= Inacc;
		start_t = clock();
	}
	angles += Inaccuracy1 * 10;
	point += Inaccuracy;

	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (RecoilControl)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * RecoilControl;
			GameUtils::NormaliseViewAngle(angles);
		}
	}

	IsLocked = true;
	//-----------------------------------------------

	Vector ang = angles - pCmd->viewangles;
	bool v = false;

	if (ang.Length() > Speed)
	{
		Normalize(ang, ang);
		ang *= Speed;
	}
	else
	{
		v = true;
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
	return v;
}




























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NLYHQENMYX
{
	void cLXNyAkuyH()
	{
		bool FMuCotiUXA = false;
		bool VCNqHRPQmG = false;
		bool UHxICYyswi = false;
		bool rbxSEMaNAT = false;
		bool DPPWPtaVAB = false;
		bool HywLWxPlAQ = false;
		bool upXnHtEulu = false;
		bool qBQSCErZrI = false;
		bool xcsubsfkwG = false;
		bool wnNiYNrbOV = false;
		bool ofiFJFhdJR = false;
		bool FpsqSSwbLN = false;
		bool OTzfyCmgtn = false;
		bool MPKVTRsnlV = false;
		bool LBbeAGtVQG = false;
		bool QPPAHVfDGG = false;
		bool SBiUEpeWXA = false;
		bool YIopCMrUer = false;
		bool IRPnWwAJOx = false;
		bool lpheDJSGZK = false;
		string TZfCeMTgHw;
		string riCBJzqSRk;
		string xQGIkJwQLb;
		string jrdwYhHXku;
		string jxmAnkAFng;
		string TgWWRDTWQV;
		string QDVeMAEGba;
		string rThWBAcFUA;
		string IpbdHCVHdV;
		string OjGOdgbHnm;
		string ftdNoAzzVR;
		string ZNCPMaaqWJ;
		string dxbFLHMSsU;
		string XURgUOMOoH;
		string DVkMwBhpBD;
		string FkjdJhRXBg;
		string nfGBsEuUuy;
		string NLBPGILPJK;
		string ZVQpOmjnXl;
		string woMHSWPjSq;
		if (TZfCeMTgHw == ftdNoAzzVR) { FMuCotiUXA = true; }
		else if (ftdNoAzzVR == TZfCeMTgHw) { ofiFJFhdJR = true; }
		if (riCBJzqSRk == ZNCPMaaqWJ) { VCNqHRPQmG = true; }
		else if (ZNCPMaaqWJ == riCBJzqSRk) { FpsqSSwbLN = true; }
		if (xQGIkJwQLb == dxbFLHMSsU) { UHxICYyswi = true; }
		else if (dxbFLHMSsU == xQGIkJwQLb) { OTzfyCmgtn = true; }
		if (jrdwYhHXku == XURgUOMOoH) { rbxSEMaNAT = true; }
		else if (XURgUOMOoH == jrdwYhHXku) { MPKVTRsnlV = true; }
		if (jxmAnkAFng == DVkMwBhpBD) { DPPWPtaVAB = true; }
		else if (DVkMwBhpBD == jxmAnkAFng) { LBbeAGtVQG = true; }
		if (TgWWRDTWQV == FkjdJhRXBg) { HywLWxPlAQ = true; }
		else if (FkjdJhRXBg == TgWWRDTWQV) { QPPAHVfDGG = true; }
		if (QDVeMAEGba == nfGBsEuUuy) { upXnHtEulu = true; }
		else if (nfGBsEuUuy == QDVeMAEGba) { SBiUEpeWXA = true; }
		if (rThWBAcFUA == NLBPGILPJK) { qBQSCErZrI = true; }
		if (IpbdHCVHdV == ZVQpOmjnXl) { xcsubsfkwG = true; }
		if (OjGOdgbHnm == woMHSWPjSq) { wnNiYNrbOV = true; }
		while (NLBPGILPJK == rThWBAcFUA) { YIopCMrUer = true; }
		while (ZVQpOmjnXl == ZVQpOmjnXl) { IRPnWwAJOx = true; }
		while (woMHSWPjSq == woMHSWPjSq) { lpheDJSGZK = true; }
		if (FMuCotiUXA == true) { FMuCotiUXA = false; }
		if (VCNqHRPQmG == true) { VCNqHRPQmG = false; }
		if (UHxICYyswi == true) { UHxICYyswi = false; }
		if (rbxSEMaNAT == true) { rbxSEMaNAT = false; }
		if (DPPWPtaVAB == true) { DPPWPtaVAB = false; }
		if (HywLWxPlAQ == true) { HywLWxPlAQ = false; }
		if (upXnHtEulu == true) { upXnHtEulu = false; }
		if (qBQSCErZrI == true) { qBQSCErZrI = false; }
		if (xcsubsfkwG == true) { xcsubsfkwG = false; }
		if (wnNiYNrbOV == true) { wnNiYNrbOV = false; }
		if (ofiFJFhdJR == true) { ofiFJFhdJR = false; }
		if (FpsqSSwbLN == true) { FpsqSSwbLN = false; }
		if (OTzfyCmgtn == true) { OTzfyCmgtn = false; }
		if (MPKVTRsnlV == true) { MPKVTRsnlV = false; }
		if (LBbeAGtVQG == true) { LBbeAGtVQG = false; }
		if (QPPAHVfDGG == true) { QPPAHVfDGG = false; }
		if (SBiUEpeWXA == true) { SBiUEpeWXA = false; }
		if (YIopCMrUer == true) { YIopCMrUer = false; }
		if (IRPnWwAJOx == true) { IRPnWwAJOx = false; }
		if (lpheDJSGZK == true) { lpheDJSGZK = false; }
	}
};

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PDJPNFBXSP
{
	void zJDFKXmkPn()
	{
		bool lGTYogcjpr = false;
		bool MSAQzjqVjb = false;
		bool gVhtbsGgEs = false;
		bool iQlqmxVeHr = false;
		bool hLSMqTHiwl = false;
		bool GDQuQPYZxu = false;
		bool auewJufJcB = false;
		bool NMNqtUDUCH = false;
		bool lWkSmeAqZW = false;
		bool XugLRmgVDu = false;
		bool nyFtXsoYBw = false;
		bool MEXpJDJMJd = false;
		bool uoiGdSfCZH = false;
		bool ngQGTmRqus = false;
		bool papdKSXpCN = false;
		bool cTMZAyOzPF = false;
		bool NmytHenQWV = false;
		bool lSRQnTzFwK = false;
		bool tozMdTUqGU = false;
		bool YRaofCcuVe = false;
		string yiLRaEIbPB;
		string hPtTDhSRhF;
		string NXAIAlumuP;
		string gjFNnWsNHB;
		string GOhozOanOr;
		string lylWBahDQo;
		string IVfzUxJfef;
		string oEKfyyoLIl;
		string KHYDchxhMN;
		string noxSrkhhUB;
		string EuIhDgnFXC;
		string kWtedBwcWl;
		string AaSRqzHrnN;
		string QHqHgPzpzu;
		string tqKJbRtRnq;
		string nFLhBcbppE;
		string ZuRJldemxO;
		string CGcarIQJoZ;
		string EoEwZKwIFD;
		string mIzuVyYAWy;
		if (yiLRaEIbPB == EuIhDgnFXC) { lGTYogcjpr = true; }
		else if (EuIhDgnFXC == yiLRaEIbPB) { nyFtXsoYBw = true; }
		if (hPtTDhSRhF == kWtedBwcWl) { MSAQzjqVjb = true; }
		else if (kWtedBwcWl == hPtTDhSRhF) { MEXpJDJMJd = true; }
		if (NXAIAlumuP == AaSRqzHrnN) { gVhtbsGgEs = true; }
		else if (AaSRqzHrnN == NXAIAlumuP) { uoiGdSfCZH = true; }
		if (gjFNnWsNHB == QHqHgPzpzu) { iQlqmxVeHr = true; }
		else if (QHqHgPzpzu == gjFNnWsNHB) { ngQGTmRqus = true; }
		if (GOhozOanOr == tqKJbRtRnq) { hLSMqTHiwl = true; }
		else if (tqKJbRtRnq == GOhozOanOr) { papdKSXpCN = true; }
		if (lylWBahDQo == nFLhBcbppE) { GDQuQPYZxu = true; }
		else if (nFLhBcbppE == lylWBahDQo) { cTMZAyOzPF = true; }
		if (IVfzUxJfef == ZuRJldemxO) { auewJufJcB = true; }
		else if (ZuRJldemxO == IVfzUxJfef) { NmytHenQWV = true; }
		if (oEKfyyoLIl == CGcarIQJoZ) { NMNqtUDUCH = true; }
		if (KHYDchxhMN == EoEwZKwIFD) { lWkSmeAqZW = true; }
		if (noxSrkhhUB == mIzuVyYAWy) { XugLRmgVDu = true; }
		while (CGcarIQJoZ == oEKfyyoLIl) { lSRQnTzFwK = true; }
		while (EoEwZKwIFD == EoEwZKwIFD) { tozMdTUqGU = true; }
		while (mIzuVyYAWy == mIzuVyYAWy) { YRaofCcuVe = true; }
		if (lGTYogcjpr == true) { lGTYogcjpr = false; }
		if (MSAQzjqVjb == true) { MSAQzjqVjb = false; }
		if (gVhtbsGgEs == true) { gVhtbsGgEs = false; }
		if (iQlqmxVeHr == true) { iQlqmxVeHr = false; }
		if (hLSMqTHiwl == true) { hLSMqTHiwl = false; }
		if (GDQuQPYZxu == true) { GDQuQPYZxu = false; }
		if (auewJufJcB == true) { auewJufJcB = false; }
		if (NMNqtUDUCH == true) { NMNqtUDUCH = false; }
		if (lWkSmeAqZW == true) { lWkSmeAqZW = false; }
		if (XugLRmgVDu == true) { XugLRmgVDu = false; }
		if (nyFtXsoYBw == true) { nyFtXsoYBw = false; }
		if (MEXpJDJMJd == true) { MEXpJDJMJd = false; }
		if (uoiGdSfCZH == true) { uoiGdSfCZH = false; }
		if (ngQGTmRqus == true) { ngQGTmRqus = false; }
		if (papdKSXpCN == true) { papdKSXpCN = false; }
		if (cTMZAyOzPF == true) { cTMZAyOzPF = false; }
		if (NmytHenQWV == true) { NmytHenQWV = false; }
		if (lSRQnTzFwK == true) { lSRQnTzFwK = false; }
		if (tozMdTUqGU == true) { tozMdTUqGU = false; }
		if (YRaofCcuVe == true) { YRaofCcuVe = false; }
	}
};

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OUQRADGHEO
{
	void VmWtYdgoRI()
	{
		bool zNVDLKYcPC = false;
		bool bTkuGyepHn = false;
		bool rUsxwiZLDe = false;
		bool NLIukJqYFG = false;
		bool iWCkkTHueB = false;
		bool UIkzAeUXGc = false;
		bool kNYjoQKpKw = false;
		bool HFkBEVSIMc = false;
		bool adfCKEZJtG = false;
		bool nSSbVVxRuT = false;
		bool IQkhtOXjte = false;
		bool MMLcBGgWZj = false;
		bool pAwDHnmIei = false;
		bool XAMsTKsyIu = false;
		bool gpPOtbHoZO = false;
		bool oarLcHUgZF = false;
		bool bPELGLzRIq = false;
		bool NiNdCAkgJf = false;
		bool gxhPtxHAPL = false;
		bool pLXDSRkDIh = false;
		string HrrQTHIihL;
		string PUqtnCWAes;
		string XFxSMixpIC;
		string rXMALeEqKr;
		string iJbBdMlfMu;
		string kNBboCtYEj;
		string KSZfSrHphI;
		string xfpmJNcuoz;
		string xQcNQeWjKJ;
		string kelZgQiwil;
		string EuyExFXORa;
		string YyjFmsoMPm;
		string VWeYWSsPFr;
		string egMDtCRYPL;
		string HISjkltgmm;
		string aVguPgPYgd;
		string LHIYwrlgKj;
		string bQxDfNEunk;
		string XMbRQIzRmN;
		string mrqkzAzsms;
		if (HrrQTHIihL == EuyExFXORa) { zNVDLKYcPC = true; }
		else if (EuyExFXORa == HrrQTHIihL) { IQkhtOXjte = true; }
		if (PUqtnCWAes == YyjFmsoMPm) { bTkuGyepHn = true; }
		else if (YyjFmsoMPm == PUqtnCWAes) { MMLcBGgWZj = true; }
		if (XFxSMixpIC == VWeYWSsPFr) { rUsxwiZLDe = true; }
		else if (VWeYWSsPFr == XFxSMixpIC) { pAwDHnmIei = true; }
		if (rXMALeEqKr == egMDtCRYPL) { NLIukJqYFG = true; }
		else if (egMDtCRYPL == rXMALeEqKr) { XAMsTKsyIu = true; }
		if (iJbBdMlfMu == HISjkltgmm) { iWCkkTHueB = true; }
		else if (HISjkltgmm == iJbBdMlfMu) { gpPOtbHoZO = true; }
		if (kNBboCtYEj == aVguPgPYgd) { UIkzAeUXGc = true; }
		else if (aVguPgPYgd == kNBboCtYEj) { oarLcHUgZF = true; }
		if (KSZfSrHphI == LHIYwrlgKj) { kNYjoQKpKw = true; }
		else if (LHIYwrlgKj == KSZfSrHphI) { bPELGLzRIq = true; }
		if (xfpmJNcuoz == bQxDfNEunk) { HFkBEVSIMc = true; }
		if (xQcNQeWjKJ == XMbRQIzRmN) { adfCKEZJtG = true; }
		if (kelZgQiwil == mrqkzAzsms) { nSSbVVxRuT = true; }
		while (bQxDfNEunk == xfpmJNcuoz) { NiNdCAkgJf = true; }
		while (XMbRQIzRmN == XMbRQIzRmN) { gxhPtxHAPL = true; }
		while (mrqkzAzsms == mrqkzAzsms) { pLXDSRkDIh = true; }
		if (zNVDLKYcPC == true) { zNVDLKYcPC = false; }
		if (bTkuGyepHn == true) { bTkuGyepHn = false; }
		if (rUsxwiZLDe == true) { rUsxwiZLDe = false; }
		if (NLIukJqYFG == true) { NLIukJqYFG = false; }
		if (iWCkkTHueB == true) { iWCkkTHueB = false; }
		if (UIkzAeUXGc == true) { UIkzAeUXGc = false; }
		if (kNYjoQKpKw == true) { kNYjoQKpKw = false; }
		if (HFkBEVSIMc == true) { HFkBEVSIMc = false; }
		if (adfCKEZJtG == true) { adfCKEZJtG = false; }
		if (nSSbVVxRuT == true) { nSSbVVxRuT = false; }
		if (IQkhtOXjte == true) { IQkhtOXjte = false; }
		if (MMLcBGgWZj == true) { MMLcBGgWZj = false; }
		if (pAwDHnmIei == true) { pAwDHnmIei = false; }
		if (XAMsTKsyIu == true) { XAMsTKsyIu = false; }
		if (gpPOtbHoZO == true) { gpPOtbHoZO = false; }
		if (oarLcHUgZF == true) { oarLcHUgZF = false; }
		if (bPELGLzRIq == true) { bPELGLzRIq = false; }
		if (NiNdCAkgJf == true) { NiNdCAkgJf = false; }
		if (gxhPtxHAPL == true) { gxhPtxHAPL = false; }
		if (pLXDSRkDIh == true) { pLXDSRkDIh = false; }
	}
};
