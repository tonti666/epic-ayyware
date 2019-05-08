#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include "position_adjust.h"
#include <iostream>
#include <time.h>
#include "UTIL Functions.h"
#include "xostr.h"
#include <chrono>
#include "Hooks.h"
#include "global_count.h"
#include "laggycompensation.h"
#include "MD5.cpp"
//#include "otr_awall.h"
#include "antiaim.h"
#include "fakelag.h"
#include "experimental.h"
#include "MiscHacks.h"
//float bigboi::current_yaw;

float current_desync;
Vector LastAngleAA2;
static bool dir = false;
static bool back = false;
static bool up = false;
static bool jitter = false;

static bool backup = false;
static bool default_aa = true;
static bool panic = false;
float hitchance_custom;
#define TICK_INTERVAL			(interfaces::globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
CAimbot * ragebot = new CAimbot;
void CAimbot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CAimbot::Draw()
{
}
float curtime_fixed(CUserCmd* ucmd) {
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * interfaces::globals->interval_per_tick;
	return curtime;
}

void RandomSeed(UINT seed)
{
	typedef void(*RandomSeed_t)(UINT);
	static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	m_RandomSeed(seed);
	return;
}

void CAimbot::auto_revolver(CUserCmd* m_pcmd) // credits: https://steamcommunity.com/id/x-87
{
	auto m_local = hackManager.pLocal();
	auto m_weapon = m_local->GetWeapon2();
	if (!m_weapon)
		return;

	if (!shot_this_tick && *m_weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
	{

		float flPostponeFireReady = m_weapon->GetFireReadyTime();
		if (flPostponeFireReady > 0 && flPostponeFireReady - 1 < interfaces::globals->curtime)
		{
			m_pcmd->buttons &= ~IN_ATTACK;
		}
		static int delay = 0;
		delay++;

		if (delay <= 15)
			m_pcmd->buttons |= IN_ATTACK;
		else
			delay = 0;

	}
	else
	{
		if (*m_weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
		{
			static int delay = 0;
			delay++;

			if (delay <= 15)
				m_pcmd->buttons |= IN_ATTACK;
			else
				delay = 0;
		}
	}
}


bool IsAbleToShoot(IClientEntity* pLocal)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (!pLocal)return false;
	if (!pWeapon)return false;
	float flServerTime = pLocal->GetTickBase() * interfaces::globals->interval_per_tick;
	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}
float hitchance()
{
	float hitchance = 101;
	auto m_local = hackManager.pLocal();
	auto pWeapon = m_local->GetWeapon2();
	if (pWeapon)
	{
		if (options::menu.aimbot_tab.AccuracyHitchance.GetValue() > 0)
		{
			float inaccuracy = pWeapon->GetInaccuracy();
			if (inaccuracy == 0) inaccuracy = 0.0000001;
			inaccuracy = 1 / inaccuracy;
			hitchance = inaccuracy;
		}
		return hitchance;
	}
}
bool CAimbot::CanOpenFire(IClientEntity * local)
{

	C_BaseCombatWeapon* entwep = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
	float flServerTime = (float)local->GetTickBase() * interfaces::globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();
	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;
	return !(flNextPrimaryAttack > flServerTime);
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

float GetLerpTimeX()
{
	int ud_rate = interfaces::cvar->FindVar("cl_updaterate")->GetFloat();
	ConVar *min_ud_rate = interfaces::cvar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = interfaces::cvar->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
	float ratio = interfaces::cvar->FindVar("cl_interp_ratio")->GetFloat();
	if (ratio == 0)
		ratio = 1.0f;
	float lerp = interfaces::cvar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");
	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());
	return max(lerp, (ratio / ud_rate));
}
void CAimbot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame() || !pLocalEntity->isValidPlayer())
		return;

	c_fakelag->Fakelag(pCmd, bSendPacket);

	if (options::menu.MiscTab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = 1;
	//	std::vector<dropdownboxitem> spike = options::menu.MiscTab.fl_spike.items;

		if (!pWeapon)
			return;

		if ( (ChokedPackets < 1 && pLocalEntity->GetHealth() <= 0.f && !(pWeapon->IsKnife() || pWeapon->IsC4())))
		{
			bSendPacket = false;
		}

		else
		{
			if (pLocalEntity->IsAlive() && pLocalEntity->GetMoveType() != MOVETYPE_LADDER)
			{
				c_antiaim->DoAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = 1;
		}
	}

	if (options::menu.aimbot_tab.AimbotEnable.GetState())
	{
		DoAimbot(pCmd, bSendPacket);
		DoNoRecoil(pCmd);
		auto_revolver(pCmd);
	}
	
	if (options::menu.MiscTab.OtherSafeMode.GetIndex() == 1)
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 39.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 39.f;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
	LastAngle = pCmd->viewangles;
}
Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, options::menu.aimbot_tab.TargetPointscale.GetValue() / 10), final);
	interfaces::trace->TraceRay(ray, MASK_SHOT, &filter, &tr);
	final = tr.endpos;
	return final;
}
inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
void ClampMovement(CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FastSqrt(square(pCommand->forwardmove) + square(pCommand->sidemove) + square(pCommand->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}

void CAimbot::delay_shot(IClientEntity* entity, CUserCmd* pcmd)
{
	float old_sim[65] = { 0.f };
	float current_sim[65] = { entity->GetSimulationTime() };
	
	bool lag_comp;

	int index = options::menu.aimbot_tab.delay_shot.GetIndex();

	switch (index)
	{
		case 1:
		{
			if (old_sim[entity->GetIndex()] != current_sim[entity->GetIndex()])
			{
				can_shoot = true;
				old_sim[entity->GetIndex()] = current_sim[entity->GetIndex()];
			}
			else
				can_shoot = false;
		}
		break;

		case 2:
		{
			Vector vec_position[65], origin_delta[65];

			if (entity->m_VecORIGIN() != vec_position[entity->GetIndex()])
			{
				origin_delta[entity->GetIndex()] = entity->m_VecORIGIN() - vec_position[entity->GetIndex()];
				vec_position[entity->GetIndex()] = entity->m_VecORIGIN();

				lag_comp = fabs(origin_delta[entity->GetIndex()].Length()) > 64;
			}

			if (lag_comp && entity->GetVelocity().Length2D() > 300)
			{
				can_shoot = false;
			}
			else
				can_shoot = true;
		}
		break;

		case 3:
		{
			if (backtracking->good_tick(TIME_TO_TICKS(entity->GetSimulationTime() + backtracking->GetLerpTime())))
			{
				pcmd->tick_count = TIME_TO_TICKS(entity->GetSimulationTime() + backtracking->GetLerpTime());
				can_shoot = true;
			}
			else
			{
				can_shoot = false;
			}
		}
		break;
	}

}

void ConColorMsg(Color const &color, const char* buf, ...)
{
	using ConColFn = void(__stdcall*)(Color const &, const char*, ...);
	auto ConCol = reinterpret_cast<ConColFn>((GetProcAddress(GetModuleHandle("tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ")));
	ConCol(color, buf);
}

auto hitbox_to_String = [](int hitgroup) -> std::string
{
	switch (hitgroup)
	{
	case 0:
	{
		return "HEAD";
	}
	case 1:
		return "NECK";
	case 2:
		return "PELVIS";
	case 3:
		return "STOMACH";
	case 5:
		return "LOWER CHEST";
	case 6:
		return "UPPER CHEST";
	case 7:
		return "THIGHS";
	case 8:
		return"THIGHS";
	case 11:
		return "LEGS";
	case 12:
		return "LEGS";
	case 16:
		return "ARMS";
	case 17:
		return "ARMS";
	default:
		return "BODY";
	}
};

void CAimbot::mirror_console_debug(IClientEntity * the_nignog)
{

	bool gay = backtracking->good_tick(TIME_TO_TICKS(pTarget->GetSimulationTime() + backtracking->GetLerpTime()));
	bool delayshot = options::menu.aimbot_tab.delay_shot.GetIndex() != 0;
	bool sw = the_nignog->GetVelocity().Length2D() < 50.f && the_nignog->GetVelocity().Length2D() > 25.f;

	int c = hitchance() / 1.5;
	int s = the_nignog->GetVelocity().Length2D();
	int h = options::menu.aimbot_tab.AccuracyHitchance.GetValue();
	std::string EVENT_2 = " > spuri_framework < Shot at hitbox [ ";
	std::string one = hitbox_to_String(HitBox);;
	std::string two = " ] with an actual hit chance of [ ";
	std::string three = std::to_string(c);
	std::string four = " ] and a configured value of [ ";
	std::string nn = std::to_string(h);
	std::string ffs = " ]";
	//----------------------------------------------
	std::string EVENT_1 = " > spuri_framework < ";
	std::string five = " Fake: ";
	std::string six = resolver->enemy_fake ? "TRUE." : "FALSE.";
	std::string seven = " Valid Tick: ";
	std::string eight = gay ? "TRUE." : "FALSE.";
	std::string nine = " Delay Shot: ";
	std::string ten = delayshot ? "TRUE." : "FALSE.";
	std::string eleven = " Enemy Speed: [ ";
	std::string twelve = std::to_string(s);
	std::string thirteen = " ]"; 
	std::string fourteen = " Enemy Health: [ ";
	std::string fifteen = std::to_string(the_nignog->GetHealth());
	std::string sixteen = " ]";
	std::string newline = ".     \n";
	//----------------------------------------------
	std::string EVENT_3 = " > spuri_framework < ";
	std::string desync1 = " Desync: ";
	std::string desync2 = resolver->has_desync ? "TRUE." : "FALSE.";
	std::string slow1 = " Slow Walk: ";
	std::string slow2 = sw ? "TRUE." : "FALSE.";
	std::string r1 = " Primary Resolver: ";
	std::string r2 = options::menu.aimbot_tab.resolver.GetIndex() == 2 ? "TRUE" : "FALSE";
//	std::string choke3 = " ]";

	std::string homo = "         ";

	std::string uremam = EVENT_2 + one + two + three + four + nn + ffs + newline;
	std::string ruined_is_gay = EVENT_1 + five + six + seven + eight + nine + ten + eleven + twelve + thirteen + fourteen + fifteen + sixteen + newline;
	std::string no_muslim = EVENT_3 + desync1 + desync2 + slow1 + slow2 + r1 + r2 + newline;
	std::string killme = homo + newline;

	interfaces::cvar->ConsoleColorPrintf(Color(250, 100, 250, 255), uremam.c_str());
	interfaces::cvar->ConsoleColorPrintf(Color(250, 100, 250, 255), ruined_is_gay.c_str());
	interfaces::cvar->ConsoleColorPrintf(Color(250, 100, 250, 255), no_muslim.c_str());
	interfaces::cvar->ConsoleColorPrintf(Color(100, 100, 100, 100), killme.c_str());
}

void CAimbot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	bool vac_kick = options::menu.MiscTab.OtherSafeMode.GetIndex() == 1;
	IClientEntity* pLocal = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	bool FindNewTarget = true;
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return;

	if (pWeapon)
	{
		if ((!pWeapon->isZeus() && pWeapon->GetAmmoInClip() == 0) || pWeapon->IsKnife() || pWeapon->IsC4() || pWeapon->IsGrenade())
			return;
	}
	else
		return;

	if (IsLocked && TargetID > -0 && HitBox >= 0)
	{
		pTarget = interfaces::ent_list->get_client_entity(TargetID);
		if (pTarget && TargetMeetsRequirements(pTarget, pLocal))
		{
			HitBox = HitScan(pTarget, pLocal);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; interfaces::engine->get_viewangles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < vac_kick ? 39.f : options::menu.aimbot_tab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		Globals::Shots = 0;
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;
		TargetID = get_target_hp(pLocal);

		if (TargetID >= 0)
		{
			pTarget = interfaces::ent_list->get_client_entity(TargetID);
		}

	}

	if (TargetID >= 0 && pTarget)
	{

		HitBox = HitScan(pTarget, pLocal);

		if (!CanOpenFire(pLocal))
			return;

		bool IsAtPeakOfJump = fabs(pLocal->GetVelocity().z) <= 5.0f;

		Vector AimPoint;
		AimPoint = options::menu.aimbot_tab.Multienable.GetState() ? GetHitboxPosition(pTarget, HitBox) : hitbox_location(pTarget, HitBox);
		AimPoint = BestPoint(pTarget, AimPoint);
		lagcomp->start_position_adjustment();		
		pTarget->GetPredicted(AimPoint);
		lagcomp->finish_position_adjustment();

		shot_this_tick = false;

		if (options::menu.MiscTab.QuickStop.GetState() && pLocal->GetFlags() & FL_ONGROUND)
			ClampMovement(pCmd, c_misc->get_gun(pWeapon));

		float hc = hitchance();
		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && options::menu.aimbot_tab.AccuracyAutoScope.GetState())
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if (pWeapon->isZeus27())
			{
				Vector pelvis = hitbox_location(pTarget, 2);
				if ((pLocal->GetOrigin() - pTarget->GetOrigin()).Length() <= 180)
				{
					Vector pos = CalcAngleA(pLocal->GetEyePosition(), pelvis);
					pCmd->buttons |= IN_ATTACK;
				}
			}
			else if ((hc >= options::menu.aimbot_tab.AccuracyHitchance.GetValue() * 1.5))
			{
				cbacktracking::Get().ShotBackTrackAimbotStart(pTarget);
				cbacktracking::Get().ShotBackTrackStoreFSN(pTarget);
				cbacktracking::Get().RestoreTemporaryRecord(pTarget);
				cbacktracking::Get().ShotBackTrackedTick(pTarget);

				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{	
					if (options::menu.aimbot_tab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{

						if (options::menu.aimbot_tab.delay_shot.GetIndex() != 0)
						{
							delay_shot(pTarget, pCmd);

							if (can_shoot == true)
							{
								if (options::menu.aimbot_tab.toggledebug.GetState())
								{
									mirror_console_debug(pTarget);
								}
								pCmd->buttons |= IN_ATTACK;
								shot_this_tick = true;
							}
						}
						else
						{
							if (options::menu.aimbot_tab.toggledebug.GetState())
							{
								mirror_console_debug(pTarget);
							}
							pCmd->buttons |= IN_ATTACK;
							c_fakelag->shot = true;
							shot_this_tick = true;
						}

						was_firing_test = true;

						if (!(pCmd->buttons |= IN_ATTACK))
						{
							shot_this_tick = false;
						}

					}
					else if (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2)
					{
			
						was_firing_test = false;
						c_fakelag->shot = false;
						return;
					}

					if (*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() != 64)
					{
						static bool WasFiring = false;
						if (*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 31)
						{
							if (pCmd->buttons & IN_ATTACK)
							{
								if (WasFiring)
								{
									pCmd->buttons &= ~IN_ATTACK;
								//	was_firing = true;
								}
							}
					//		else
					//			was_firing = false;

							WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
						}
					}
				}
			}
		
		}

		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK) {
			Globals::Shots += 1;
		}

	}
}

bool CAimbot::TargetMeetsRequirements(IClientEntity* pEntity, IClientEntity* local)
{
//	auto local = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (pEntity->isValidPlayer())
	{

		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && interfaces::engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (options::menu.MiscTab.OtherSafeMode.GetIndex() != 3 ? pEntity->team() != local->team() : pEntity->GetIndex() != local->GetIndex())
			{
				// Spawn Check
				if (!pEntity->has_gungame_immunity())
				{
					if (!(pEntity->GetFlags() & FL_FROZEN))
					{
						return true;
					}
				}

			}
		}
	}
	return false;
}

float CAimbot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{

	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleVectors(Angles, &Forward);

	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Normalize(Delta, Delta);

	FLOAT DotProduct = Forward.Dot(Delta);

	return (acos(DotProduct) * (MaxDegrees / PI));
}
int CAimbot::get_target_hp(IClientEntity* pLocal)
{
	int target = -1;
	int minHealth = 101;

	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; interfaces::engine->get_viewangles(View);

	for (int i = 0; i < 65; i++)
	{
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
		if (TargetMeetsRequirements(pEntity, pLocal))
		{
			if (pEntity->GetClientClass()->m_ClassID != 38)
				continue;

			if (pEntity->GetOrigin() == Vector(0, 0, 0))
				continue;

			int NewHitBox = HitScan(pEntity, pLocal);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov <= 180.f)
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}
float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;
	AngleVectors(viewAngle, &aim);
	AngleVectors(aimAngle, &ang);
	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
bool CAimbot::should_baim(IClientEntity* pEntity)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pEntity->GetActiveWeaponHandle());

	if (!pWeapon)
		return false;

	int health = options::menu.aimbot_tab.BaimIfUnderXHealth.GetValue();

	if ((GetAsyncKeyState(options::menu.aimbot_tab.bigbaim.GetKey()) ||
		(options::menu.aimbot_tab.baim_fakewalk.GetState() && (enemy_is_slow_walking(pEntity)))
		|| (options::menu.aimbot_tab.baim_fake.GetState() && resolver->enemy_fake)
		|| (options::menu.aimbot_tab.baim_inair.GetState() && !(pEntity->GetFlags() & FL_ONGROUND))
		|| pEntity->GetHealth() <= health))
	{
		return true;
	}

	return false;
}

bool CAimbot::enemy_is_slow_walking(IClientEntity * entity)
{
	C_BaseCombatWeapon* weapon = entity->GetWeapon2();
	if (!weapon)
		return false;

	float speed = entity->GetVelocity().Length2D();
	float get_gun = c_misc->get_gun(weapon);

	if (speed - get_gun < 34) // if it's more or less the same.
	{
		return true;
	}
	return false;
}


int CAimbot::automatic_hitscan(IClientEntity * entity)
{
	int hp = entity->GetHealth();
	int speed = entity->GetVelocity().Length();

	if (entity == nullptr)
		return 0;

#pragma region " 1 = head, pelvis | 2 = head, stomach, pelvis | 3 - full scan | 4- body only | 5 - head, lower body, legs "

	if (speed >= 210)
	{
		if (hp >= 70)
		{
			return 1;
		}

		if (hp < 70 && hp >= 50)
		{
			return 2;
		}

		if (hp < 50)
		{
			return 3;
		}
	}

	if (speed < 210 && speed >= 150)
	{
		if (hp >= 50)
		{
			return 2;
		}

		if (hp < 50)
		{
			return 3;
		}
	}

	if (speed < 150)
	{
		if (hp >= 70)
		{
			return 5;
		}

		if (hp < 70 && hp > 50)
		{
			return 3;
		}

		if (hp <= 50)
		{
			return 4;
		}
	}
}

int CAimbot::HitScan(IClientEntity* pEntity, IClientEntity* pLocal)
{

	float health = options::menu.aimbot_tab.BaimIfUnderXHealth.GetValue();

	std::vector<int> HitBoxesToScan;
	std::vector<dropdownboxitem> auto_list = options::menu.aimbot_tab.target_auto.items;
	std::vector<dropdownboxitem> scout_list = options::menu.aimbot_tab.target_scout.items;
	std::vector<dropdownboxitem> awp_list = options::menu.aimbot_tab.target_awp.items;
	std::vector<dropdownboxitem> pistol_list = options::menu.aimbot_tab.target_pistol.items;
	std::vector<dropdownboxitem> smg_list = options::menu.aimbot_tab.target_smg.items;
	std::vector<dropdownboxitem> otr_list = options::menu.aimbot_tab.target_otr.items;

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (pWeapon != nullptr)
	{

		if (GameUtils::AutoSniper(pWeapon))
		{
			if (options::menu.aimbot_tab.custom_hitscan.GetState() && !(GetAsyncKeyState(options::menu.aimbot_tab.bigbaim.GetKey())))
			{
				switch (automatic_hitscan(pEntity))
				{
				case 1:
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::head);
				}
				break;

				case 2:
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::head);
					HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
					HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
				}
				break;

				case 3:
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::head);
					HitBoxesToScan.push_back((int)csgo_hitboxes::neck);
					HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
					HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
					HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
					HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_upper_arm);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_upper_arm);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_lower_arm);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_lower_arm);
				}
				break;

				case 4:
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
					HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
					HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
				}
				break;

				case 5:
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::head);
					HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
				}
				break;
				}
			}
			else
			{
				if (auto_list[0].GetSelected && !(GetAsyncKeyState(options::menu.aimbot_tab.bigbaim.GetKey()))) // Head
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::head);
					HitBoxesToScan.push_back((int)csgo_hitboxes::neck);
				}

				if (auto_list[2].GetSelected) // Lower body
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
					HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
					HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
				}
				if (auto_list[1].GetSelected && !should_baim(pEntity)) // Upper Body
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
				}
				if (auto_list[4].GetSelected) // Legs
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_calf);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_calf);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
				}
				if (auto_list[3].GetSelected) // Arms
				{
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_upper_arm);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_upper_arm);
					HitBoxesToScan.push_back((int)csgo_hitboxes::left_lower_arm);
					HitBoxesToScan.push_back((int)csgo_hitboxes::right_lower_arm);
				}
			}

		}
		if (pWeapon->is_scout())
		{
			if (scout_list[0].GetSelected && !should_baim(pEntity)) // Head// Head
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::head);
			}

			if (scout_list[1].GetSelected && !should_baim(pEntity)) // Head// Upper Body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::neck);
				HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
			}

			if (scout_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
				HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
				HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
			}

			if (scout_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_lower_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_lower_arm);
			}

			if (scout_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
			}
		}

		if (pWeapon->is_awp())
		{
			if (awp_list[0].GetSelected && !should_baim(pEntity)) // Head // Head
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::head);
			}

			if (awp_list[1].GetSelected && !should_baim(pEntity)) // Head // Upper Body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::neck);
				HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
			}

			if (awp_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
				HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
				HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
			}

			if (awp_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_lower_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_lower_arm);
			}

			if (awp_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
			}
		}

		if (GameUtils::IsPistol(pWeapon) && !should_baim(pEntity)) // Head
		{

			if (pistol_list[0].GetSelected) // Head
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::head);
			}

			if (pistol_list[1].GetSelected) // Upper Body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::neck);
				HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
			}

			if (pistol_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
				HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
				HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
			}

			if (pistol_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_lower_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_lower_arm);
			}

			if (pistol_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
			}
		}

		if (GameUtils::IsMP(pWeapon) && !should_baim(pEntity)) // Head
		{
			if (smg_list[0].GetSelected) // Head
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::head);
			}

			if (smg_list[1].GetSelected) // Upper Body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::neck);
				HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
			}

			if (smg_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
				HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
				HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
			}

			if (smg_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_lower_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_lower_arm);
			}

			if (smg_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
			}
		}

		if (GameUtils::IsRifle(pWeapon) || GameUtils::IsShotgun(pWeapon) || GameUtils::IsMachinegun(pWeapon))
		{
			if (otr_list[0].GetSelected && !should_baim(pEntity)) // Head // Head
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::head);
			}

			if (otr_list[1].GetSelected) // Upper Body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::neck);
				HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
			}

			if (otr_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
				HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
				HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
			}

			if (otr_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_upper_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_lower_arm);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_lower_arm);
			}

			if (otr_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_calf);
				HitBoxesToScan.push_back((int)csgo_hitboxes::right_foot);
				HitBoxesToScan.push_back((int)csgo_hitboxes::left_foot);
			}
		}

		if (GameUtils::IsZeus(pWeapon) && !should_baim(pEntity))
		{
			HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
			HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
			HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
			HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
			HitBoxesToScan.push_back((int)csgo_hitboxes::upper_chest);
			HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
			HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
		}

		if (should_baim(pEntity))
		{
			HitBoxesToScan.push_back((int)csgo_hitboxes::lower_chest);
			HitBoxesToScan.push_back((int)csgo_hitboxes::stomach);
			HitBoxesToScan.push_back((int)csgo_hitboxes::pelvis);
			HitBoxesToScan.push_back((int)csgo_hitboxes::left_thigh);
			HitBoxesToScan.push_back((int)csgo_hitboxes::right_thigh);
		}


		for (auto HitBoxID : HitBoxesToScan)
		{
			Vector Point, lol;
		
			Point =  hitbox_location(pEntity, HitBoxID);		
			lol = hitbox_location(pEntity, 0);
			float dmg = 0.f;

			if (backup_awall->can_hit(Point, &dmg) && options::menu.MiscTab.OtherSafeMode.GetIndex() != 2)
			{
				return HitBoxID;
			}

			else if (options::menu.MiscTab.OtherSafeMode.GetIndex() == 2 && backup_awall->can_hit(lol, &dmg))
			{
				return 0;
			}
		}
		return -1;
	}
}

void CAimbot::DoNoRecoil(CUserCmd *pCmd)
{

	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2.00;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}
void CAimbot::aimAtPlayer(CUserCmd *pCmd, IClientEntity* pLocal)
{
//	IClientEntity* pLocal = hackManager.pLocal();

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->range;

	IClientEntity* target = nullptr;

	for (int i = 0; i <= 65; i++)
	{
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
		if (TargetMeetsRequirements(pEntity, pLocal))
		{
			if (TargetID != -1)
				target = interfaces::ent_list->get_client_entity(TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();
			Vector CurPos = target->GetEyePosition() + target->GetAbsOrigin();

			float temp_dist = eye_position.DistTo(target_position);
			QAngle angle = QAngle(0, 0, 0);
			float lowest = 99999999.f;
			if (CurPos.DistToSqr(eye_position) < lowest)
			{
				lowest = CurPos.DistTo(eye_position);
				CalcAngle(eye_position, target_position, angle);
			}
		}
	}
}

bool CAimbot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;
	if (point.Length() == 0) return ReturnValue;
	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();
	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);
	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return ReturnValue;
	}
	IsLocked = true;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle;
	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, interfaces::ent_list->get_client_entity(TargetID), 0);
	Vector AddAngs = angles - LastAimstepAngle;
	if (fovLeft > 29.0f && options::menu.MiscTab.OtherSafeMode.GetIndex() == 1)
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 29.0f;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else if (pCmd->buttons & IN_ATTACK && fovLeft > 2 && options::menu.MiscTab.OtherSafeMode.GetIndex() == 2)
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 2.f;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}
//	if (Options::Menu.aimbot_tab.AimbotSilentAim.GetState())
//	{
		pCmd->viewangles = angles;
//	}
//	if (!Options::Menu.aimbot_tab.AimbotSilentAim.GetState())
//	{
//		Interfaces::Engine->SetViewAngles(angles);
//	}

	if (options::menu.aimbot_tab.AimbotSilentAim.GetState())
	{
		Vector oViewangles = pCmd->viewangles;
		float oForwardmove = pCmd->forwardmove;
		float oSidemove = pCmd->sidemove;

		static auto choked = 0.f;

		//So we dont kill ourselfs
		if (choked < 2)
		{
			bSendPacket = false;//this will overwrite fakelag values when psilent is on
			choked++;
		}
		else
			bSendPacket = true;

		if (bSendPacket)
		{
			//		bSendPacket = true;
			choked++;
			pCmd->viewangles = oViewangles;
			pCmd->forwardmove = oForwardmove;
			pCmd->sidemove = oSidemove;
		}
	}

	return ReturnValue;
}





#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MAITIWATUE
{ 
  void YHrnphDXQW()
  { 
      bool zJIewVUopU = false;
      bool ojTRPaebHw = false;
      bool pymdkGpxzP = false;
      bool GHCLlVpJHs = false;
      bool QyVCFZgxKO = false;
      bool fcJKfBWdsp = false;
      bool eWpZOggIet = false;
      bool jVUOerbkXK = false;
      bool Zsljjdyqek = false;
      bool zMQROsHumX = false;
      bool rYSQeKHten = false;
      bool YlFhPlkGpj = false;
      bool ipcstXECzk = false;
      bool jgxzRBZVFY = false;
      bool VDMpMNGhBc = false;
      bool IBECoCuYVH = false;
      bool FFusErnFQO = false;
      bool LPAROfdmgg = false;
      bool QWBWchbtKO = false;
      bool GBzySQHSkU = false;
      string RwbxkMKMEM;
      string CdiopOfMeo;
      string WPBfdoOQxb;
      string ogMhDtfiWL;
      string wDMxysrYwe;
      string EZHIfBtwSw;
      string ytOcWURAlw;
      string iwpTsTDEgk;
      string woTSXPYDBB;
      string ziPaMCnjZc;
      string brmAuHinOM;
      string yoOhzJZuFH;
      string wNykwzgPHq;
      string eTeEIECTWP;
      string fkooCKThaD;
      string bseRgNsCDu;
      string ZTfXheKKYu;
      string GGYobLYJhl;
      string fznEPlnMXt;
      string KAPapLERGW;
      if(RwbxkMKMEM == brmAuHinOM){zJIewVUopU = true;}
      else if(brmAuHinOM == RwbxkMKMEM){rYSQeKHten = true;}
      if(CdiopOfMeo == yoOhzJZuFH){ojTRPaebHw = true;}
      else if(yoOhzJZuFH == CdiopOfMeo){YlFhPlkGpj = true;}
      if(WPBfdoOQxb == wNykwzgPHq){pymdkGpxzP = true;}
      else if(wNykwzgPHq == WPBfdoOQxb){ipcstXECzk = true;}
      if(ogMhDtfiWL == eTeEIECTWP){GHCLlVpJHs = true;}
      else if(eTeEIECTWP == ogMhDtfiWL){jgxzRBZVFY = true;}
      if(wDMxysrYwe == fkooCKThaD){QyVCFZgxKO = true;}
      else if(fkooCKThaD == wDMxysrYwe){VDMpMNGhBc = true;}
      if(EZHIfBtwSw == bseRgNsCDu){fcJKfBWdsp = true;}
      else if(bseRgNsCDu == EZHIfBtwSw){IBECoCuYVH = true;}
      if(ytOcWURAlw == ZTfXheKKYu){eWpZOggIet = true;}
      else if(ZTfXheKKYu == ytOcWURAlw){FFusErnFQO = true;}
      if(iwpTsTDEgk == GGYobLYJhl){jVUOerbkXK = true;}
      if(woTSXPYDBB == fznEPlnMXt){Zsljjdyqek = true;}
      if(ziPaMCnjZc == KAPapLERGW){zMQROsHumX = true;}
      while(GGYobLYJhl == iwpTsTDEgk){LPAROfdmgg = true;}
      while(fznEPlnMXt == fznEPlnMXt){QWBWchbtKO = true;}
      while(KAPapLERGW == KAPapLERGW){GBzySQHSkU = true;}
      if(zJIewVUopU == true){zJIewVUopU = false;}
      if(ojTRPaebHw == true){ojTRPaebHw = false;}
      if(pymdkGpxzP == true){pymdkGpxzP = false;}
      if(GHCLlVpJHs == true){GHCLlVpJHs = false;}
      if(QyVCFZgxKO == true){QyVCFZgxKO = false;}
      if(fcJKfBWdsp == true){fcJKfBWdsp = false;}
      if(eWpZOggIet == true){eWpZOggIet = false;}
      if(jVUOerbkXK == true){jVUOerbkXK = false;}
      if(Zsljjdyqek == true){Zsljjdyqek = false;}
      if(zMQROsHumX == true){zMQROsHumX = false;}
      if(rYSQeKHten == true){rYSQeKHten = false;}
      if(YlFhPlkGpj == true){YlFhPlkGpj = false;}
      if(ipcstXECzk == true){ipcstXECzk = false;}
      if(jgxzRBZVFY == true){jgxzRBZVFY = false;}
      if(VDMpMNGhBc == true){VDMpMNGhBc = false;}
      if(IBECoCuYVH == true){IBECoCuYVH = false;}
      if(FFusErnFQO == true){FFusErnFQO = false;}
      if(LPAROfdmgg == true){LPAROfdmgg = false;}
      if(QWBWchbtKO == true){QWBWchbtKO = false;}
      if(GBzySQHSkU == true){GBzySQHSkU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TLUXQYFXOS
{ 
  void hddMUnGfbw()
  { 
      bool oyMFnbxThZ = false;
      bool gayEtzrcNJ = false;
      bool qFYUFuZPeB = false;
      bool XXTqiWfWpZ = false;
      bool DtmWAAbIHg = false;
      bool fyLCNiYqAr = false;
      bool CgngCJGGVW = false;
      bool TUlmFxSCTE = false;
      bool SRIyUYLQdz = false;
      bool xnWxXVFGUq = false;
      bool tVljNKLXxP = false;
      bool QWWwZkYpLc = false;
      bool xocVYeyoPi = false;
      bool rMhcKxeXkK = false;
      bool NnTNiaQaXm = false;
      bool xPuqtEsQKh = false;
      bool LWINXEntlb = false;
      bool FAFAPJVOmy = false;
      bool NXQclbQpiF = false;
      bool DOMCypeFoe = false;
      string smDfQLYdWi;
      string TQukjbFdGr;
      string RjemrHtkZM;
      string iLLrSaaSLR;
      string gCZaMSNVaE;
      string mDjqrIHQnJ;
      string QnDJzIFCSI;
      string AmRoKPJEZY;
      string sYJUYfbUzJ;
      string TKeCBsyQbE;
      string NKDSEAktgb;
      string zDqOPqdijT;
      string fjDIooIPnQ;
      string RscULiVqQY;
      string tgnWKPTPlJ;
      string OPfHYcHudO;
      string rVGPKFaVDg;
      string VmRLZywfxY;
      string UrcdCBNUwh;
      string bZNBfYaYLF;
      if(smDfQLYdWi == NKDSEAktgb){oyMFnbxThZ = true;}
      else if(NKDSEAktgb == smDfQLYdWi){tVljNKLXxP = true;}
      if(TQukjbFdGr == zDqOPqdijT){gayEtzrcNJ = true;}
      else if(zDqOPqdijT == TQukjbFdGr){QWWwZkYpLc = true;}
      if(RjemrHtkZM == fjDIooIPnQ){qFYUFuZPeB = true;}
      else if(fjDIooIPnQ == RjemrHtkZM){xocVYeyoPi = true;}
      if(iLLrSaaSLR == RscULiVqQY){XXTqiWfWpZ = true;}
      else if(RscULiVqQY == iLLrSaaSLR){rMhcKxeXkK = true;}
      if(gCZaMSNVaE == tgnWKPTPlJ){DtmWAAbIHg = true;}
      else if(tgnWKPTPlJ == gCZaMSNVaE){NnTNiaQaXm = true;}
      if(mDjqrIHQnJ == OPfHYcHudO){fyLCNiYqAr = true;}
      else if(OPfHYcHudO == mDjqrIHQnJ){xPuqtEsQKh = true;}
      if(QnDJzIFCSI == rVGPKFaVDg){CgngCJGGVW = true;}
      else if(rVGPKFaVDg == QnDJzIFCSI){LWINXEntlb = true;}
      if(AmRoKPJEZY == VmRLZywfxY){TUlmFxSCTE = true;}
      if(sYJUYfbUzJ == UrcdCBNUwh){SRIyUYLQdz = true;}
      if(TKeCBsyQbE == bZNBfYaYLF){xnWxXVFGUq = true;}
      while(VmRLZywfxY == AmRoKPJEZY){FAFAPJVOmy = true;}
      while(UrcdCBNUwh == UrcdCBNUwh){NXQclbQpiF = true;}
      while(bZNBfYaYLF == bZNBfYaYLF){DOMCypeFoe = true;}
      if(oyMFnbxThZ == true){oyMFnbxThZ = false;}
      if(gayEtzrcNJ == true){gayEtzrcNJ = false;}
      if(qFYUFuZPeB == true){qFYUFuZPeB = false;}
      if(XXTqiWfWpZ == true){XXTqiWfWpZ = false;}
      if(DtmWAAbIHg == true){DtmWAAbIHg = false;}
      if(fyLCNiYqAr == true){fyLCNiYqAr = false;}
      if(CgngCJGGVW == true){CgngCJGGVW = false;}
      if(TUlmFxSCTE == true){TUlmFxSCTE = false;}
      if(SRIyUYLQdz == true){SRIyUYLQdz = false;}
      if(xnWxXVFGUq == true){xnWxXVFGUq = false;}
      if(tVljNKLXxP == true){tVljNKLXxP = false;}
      if(QWWwZkYpLc == true){QWWwZkYpLc = false;}
      if(xocVYeyoPi == true){xocVYeyoPi = false;}
      if(rMhcKxeXkK == true){rMhcKxeXkK = false;}
      if(NnTNiaQaXm == true){NnTNiaQaXm = false;}
      if(xPuqtEsQKh == true){xPuqtEsQKh = false;}
      if(LWINXEntlb == true){LWINXEntlb = false;}
      if(FAFAPJVOmy == true){FAFAPJVOmy = false;}
      if(NXQclbQpiF == true){NXQclbQpiF = false;}
      if(DOMCypeFoe == true){DOMCypeFoe = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JTTPWGGLGA
{ 
  void okXdnQpYlr()
  { 
      bool TcPxExgOKB = false;
      bool ucRoeSdChJ = false;
      bool HJoGQphZEB = false;
      bool NmgiWuemVm = false;
      bool oBFFYMOqQP = false;
      bool AzPzIJmIgN = false;
      bool OHKJnHwiSj = false;
      bool nqguUGEjhD = false;
      bool bpIoAOXgZN = false;
      bool HVFHXgpwQQ = false;
      bool QfYfaJSTeC = false;
      bool FDZclXyUIZ = false;
      bool jOIEcRkYXm = false;
      bool PHSQmwRbNq = false;
      bool uDCXIDwcVz = false;
      bool hVnqgBWTDJ = false;
      bool YswqlxjJop = false;
      bool GVHQdfMkwY = false;
      bool VHDXEtTail = false;
      bool YxYDSCGKkL = false;
      string JKAXAIQTaY;
      string jWZqrrqwXk;
      string xQGuHdfJuy;
      string EikAXfzkoZ;
      string xxNjLVWZdt;
      string EJbDFsDEPV;
      string bHXcUAkYTM;
      string rGGymADWil;
      string GFeBtuGhte;
      string btUSxpZkPK;
      string jxGRByCcQF;
      string pgUcPVcVhY;
      string sDenkxknBx;
      string ZRGYBIyNzu;
      string sdQnntZqDa;
      string MDCpRgSRfD;
      string MObSfXawbG;
      string WPVmfPlpAq;
      string XtjZjVPQGU;
      string pggwhdByIQ;
      if(JKAXAIQTaY == jxGRByCcQF){TcPxExgOKB = true;}
      else if(jxGRByCcQF == JKAXAIQTaY){QfYfaJSTeC = true;}
      if(jWZqrrqwXk == pgUcPVcVhY){ucRoeSdChJ = true;}
      else if(pgUcPVcVhY == jWZqrrqwXk){FDZclXyUIZ = true;}
      if(xQGuHdfJuy == sDenkxknBx){HJoGQphZEB = true;}
      else if(sDenkxknBx == xQGuHdfJuy){jOIEcRkYXm = true;}
      if(EikAXfzkoZ == ZRGYBIyNzu){NmgiWuemVm = true;}
      else if(ZRGYBIyNzu == EikAXfzkoZ){PHSQmwRbNq = true;}
      if(xxNjLVWZdt == sdQnntZqDa){oBFFYMOqQP = true;}
      else if(sdQnntZqDa == xxNjLVWZdt){uDCXIDwcVz = true;}
      if(EJbDFsDEPV == MDCpRgSRfD){AzPzIJmIgN = true;}
      else if(MDCpRgSRfD == EJbDFsDEPV){hVnqgBWTDJ = true;}
      if(bHXcUAkYTM == MObSfXawbG){OHKJnHwiSj = true;}
      else if(MObSfXawbG == bHXcUAkYTM){YswqlxjJop = true;}
      if(rGGymADWil == WPVmfPlpAq){nqguUGEjhD = true;}
      if(GFeBtuGhte == XtjZjVPQGU){bpIoAOXgZN = true;}
      if(btUSxpZkPK == pggwhdByIQ){HVFHXgpwQQ = true;}
      while(WPVmfPlpAq == rGGymADWil){GVHQdfMkwY = true;}
      while(XtjZjVPQGU == XtjZjVPQGU){VHDXEtTail = true;}
      while(pggwhdByIQ == pggwhdByIQ){YxYDSCGKkL = true;}
      if(TcPxExgOKB == true){TcPxExgOKB = false;}
      if(ucRoeSdChJ == true){ucRoeSdChJ = false;}
      if(HJoGQphZEB == true){HJoGQphZEB = false;}
      if(NmgiWuemVm == true){NmgiWuemVm = false;}
      if(oBFFYMOqQP == true){oBFFYMOqQP = false;}
      if(AzPzIJmIgN == true){AzPzIJmIgN = false;}
      if(OHKJnHwiSj == true){OHKJnHwiSj = false;}
      if(nqguUGEjhD == true){nqguUGEjhD = false;}
      if(bpIoAOXgZN == true){bpIoAOXgZN = false;}
      if(HVFHXgpwQQ == true){HVFHXgpwQQ = false;}
      if(QfYfaJSTeC == true){QfYfaJSTeC = false;}
      if(FDZclXyUIZ == true){FDZclXyUIZ = false;}
      if(jOIEcRkYXm == true){jOIEcRkYXm = false;}
      if(PHSQmwRbNq == true){PHSQmwRbNq = false;}
      if(uDCXIDwcVz == true){uDCXIDwcVz = false;}
      if(hVnqgBWTDJ == true){hVnqgBWTDJ = false;}
      if(YswqlxjJop == true){YswqlxjJop = false;}
      if(GVHQdfMkwY == true){GVHQdfMkwY = false;}
      if(VHDXEtTail == true){VHDXEtTail = false;}
      if(YxYDSCGKkL == true){YxYDSCGKkL = false;}
    } 
}; 
