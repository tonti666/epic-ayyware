#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "IClientMode.h"
#include <chrono>
#include <algorithm>
#include <time.h>
#include "Hooks.h"
#include "position_adjust.h"
#include "RageBot.h"
#include "Autowall.h"
CMiscHacks* c_misc = new CMiscHacks;
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
inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}
inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}
Vector AutoStrafeView;
void CMiscHacks::Init()
{
}
void CMiscHacks::Draw()
{
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

}

void set_name(const char* name)
{
	ConVar* nameConvar = interfaces::cvar->FindVar(("name"));
	*(int*)((DWORD)&nameConvar->fnChangeCallback + 0xC) = NULL;
	nameConvar->SetValueChar(name);
}
void CMiscHacks::namespam()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < .5)
		return;
	const char* result;
	std::vector <std::string> names;
	if (interfaces::engine->IsInGame() && interfaces::engine->IsConnected()) {
		for (int i = 1; i < interfaces::globals->maxClients; i++)
		{
			IClientEntity *entity = interfaces::ent_list->get_client_entity(i);

			player_info_t pInfo;

			if (entity && hackManager.pLocal()->team() == entity->team())
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();

				if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
				{
					if (interfaces::engine->GetPlayerInfo(i, &pInfo))
					{
						if (!strstr(pInfo.name, "GOTV"))
							names.push_back(pInfo.name);
					}
				}
			}
		}
	}

	set_name("\n\xAD\xAD\xAD");
	int randomIndex = rand() % names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", names[randomIndex].c_str());
	result = buffer;



	set_name(result);
	start_t = clock();
}





void CMiscHacks::buybot_primary()
{
	bool is_ct = hackManager.pLocal()->team() == TEAM_CS_CT;
	switch (options::menu.MiscTab.buybot_primary.GetIndex())
	{
	case 1: is_ct ? (interfaces::engine->ExecuteClientCmd("buy scar20;")) : (interfaces::engine->ExecuteClientCmd("buy g3sg1;"));
		break;
	case 2: interfaces::engine->ExecuteClientCmd("buy ssg08;");
		break;
	case 3: interfaces::engine->ExecuteClientCmd("buy awp;");
		break;
	case 4: is_ct ? (interfaces::engine->ExecuteClientCmd("buy m4a1; buy m4a1_silencer")) : (interfaces::engine->ExecuteClientCmd("buy ak47;"));
		break;
	case 5: is_ct ? (interfaces::engine->ExecuteClientCmd("buy aug;")) : (interfaces::engine->ExecuteClientCmd("buy sg556"));
		break;
	case 6: is_ct ? (interfaces::engine->ExecuteClientCmd("buy mp9")) : (interfaces::engine->ExecuteClientCmd("buy mac-10;"));
		break;
	}

}

void CMiscHacks::buybot_secondary()
{
	switch (options::menu.MiscTab.buybot_secondary.GetIndex())
	{
	case 1: interfaces::engine->ExecuteClientCmd("buy elite;");
		break;
	case 2: interfaces::engine->ExecuteClientCmd("buy deagle;");
		break;
	case 3: interfaces::engine->ExecuteClientCmd("buy fn57");
		break;
	}

}

void CMiscHacks::buybot_otr()
{
	std::vector<dropdownboxitem> otr_list = options::menu.MiscTab.buybot_otr.items;

	if (otr_list[0].GetSelected)
	{
		interfaces::engine->ExecuteClientCmd("buy vest; buy vesthelm;");
	}

	if (otr_list[1].GetSelected)
	{
		interfaces::engine->ExecuteClientCmd("buy hegrenade;");
	}

	if (otr_list[2].GetSelected)
	{
		interfaces::engine->ExecuteClientCmd("buy flashbang;");
	}

	if (otr_list[3].GetSelected)
	{
		interfaces::engine->ExecuteClientCmd("buy smokegrenade;");
	}

	if (otr_list[4].GetSelected)
	{
		interfaces::engine->ExecuteClientCmd("buy molotov;");
	}

	if (otr_list[5].GetSelected)
	{
		interfaces::engine->ExecuteClientCmd("buy taser;");
	}

	if (otr_list[6].GetSelected)
	{
		interfaces::engine->ExecuteClientCmd("buy defuser;");
	}

}
inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
void angleVectors(const Vector& angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity *pLocal = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	IGameEvent* Event;
	if (!hackManager.pLocal()->IsAlive() || oof)
	{
		_done = false;
		return;
	}
	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	// ------- Oi thundercunt, this is needed for the weapon configs ------- //

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (pWeapon != nullptr)
	{
		if (GetAsyncKeyState(options::menu.MiscTab.minimal_walk.GetKey()))
		{
			MinimalWalk(pCmd, get_gun(pWeapon));
		}

		if (GameUtils::AutoSniper(pWeapon))
		{

			options::menu.aimbot_tab.AccuracyHitchance.SetValue((float)options::menu.aimbot_tab.hc_auto.GetValue());
			options::menu.aimbot_tab.AccuracyMinimumDamage.SetValue((float)options::menu.aimbot_tab.md_auto.GetValue());

		}

		if (GameUtils::IsPistol(pWeapon))
		{

			options::menu.aimbot_tab.AccuracyHitchance.SetValue((float)options::menu.aimbot_tab.hc_pistol.GetValue());
			options::menu.aimbot_tab.AccuracyMinimumDamage.SetValue((float)options::menu.aimbot_tab.md_pistol.GetValue());

		}

		if (pWeapon->is_scout())
		{

			options::menu.aimbot_tab.AccuracyHitchance.SetValue((float)options::menu.aimbot_tab.hc_scout.GetValue());
			options::menu.aimbot_tab.AccuracyMinimumDamage.SetValue((float)options::menu.aimbot_tab.md_scout.GetValue());

		}

		if (pWeapon->is_awp())
		{

			options::menu.aimbot_tab.AccuracyHitchance.SetValue((float)options::menu.aimbot_tab.hc_awp.GetValue());
			options::menu.aimbot_tab.AccuracyMinimumDamage.SetValue((float)options::menu.aimbot_tab.md_awp.GetValue());

		}

		if (GameUtils::IsRifle(pWeapon) || GameUtils::IsShotgun(pWeapon) || GameUtils::IsMachinegun(pWeapon))
		{

			options::menu.aimbot_tab.AccuracyHitchance.SetValue((float)options::menu.aimbot_tab.hc_otr.GetValue());
			options::menu.aimbot_tab.AccuracyMinimumDamage.SetValue((float)options::menu.aimbot_tab.md_otr.GetValue());

		}

		if (GameUtils::IsMP(pWeapon))
		{
			options::menu.aimbot_tab.AccuracyHitchance.SetValue((float)options::menu.aimbot_tab.hc_smg.GetValue());
			options::menu.aimbot_tab.AccuracyMinimumDamage.SetValue((float)options::menu.aimbot_tab.md_smg.GetValue());
		}

		if (GameUtils::IsZeus(pWeapon))
		{
			options::menu.aimbot_tab.AccuracyHitchance.SetValue(20);
			options::menu.aimbot_tab.AccuracyMinimumDamage.SetValue(20);
		} 
	}

	if (options::menu.MiscTab.infinite_duck.GetState())
	{
		pCmd->buttons |= IN_BULLRUSH;
	}

	if (options::menu.visuals_tab.override_viewmodel.GetState())
		viewmodel_x_y_z();

	if (options::menu.visuals_tab.leftknife.GetState())
		leftknife();

	if (options::menu.visuals_tab.aspectratio.GetState())
		aspectratio();

	//	if (Options::Menu.RageBotTab.AimbotEnable.GetState())
	//		AutoPistol(pCmd);

//	RankReveal(pCmd);

	if (options::menu.MiscTab.OtherAutoJump.GetState())
		AutoJump(pCmd);

	if (options::menu.MiscTab.Legitbhop.GetState())
		LegitBhop(pCmd);


	if (options::menu.MiscTab.airduck_type.GetIndex() != 0)
	{
		airduck(pCmd);
	}
	interfaces::engine->get_viewangles(AutoStrafeView);
	if (options::menu.MiscTab.OtherAutoStrafe.GetState() && (pLocal->GetMoveType() != MOVETYPE_LADDER))
	{
		strafer(pCmd);
	}

	if (GetAsyncKeyState(options::menu.MiscTab.fw.GetKey()) && !options::menu.m_bIsOpen)
	{
		FakeWalk0(pCmd, bSendPacket);
	}

	fake_crouch(pCmd, bSendPacket, pLocal);

	if (options::menu.visuals_tab.DisablePostProcess.GetState())
		PostProcces();

	if (!_done && hackManager.pLocal()->IsAlive())
	{
		if (options::menu.MiscTab.buybot_primary.GetIndex() != 0)
			buybot_primary();

		if (options::menu.MiscTab.buybot_secondary.GetIndex() != 0)
			buybot_secondary();

		buybot_otr();

		_done = true;
	}



	if (options::menu.MiscTab.fakeping.GetState())
	{
		fakeping();
	}

	if (options::menu.MiscTab.fakevote.GetState())
	{
		FakeTimeout();
	}

	if (options::menu.MiscTab.fakevac.GetState())
	{
		fakevac();
	}
	


	switch (options::menu.MiscTab.namechangerr.GetIndex())
	{
	case 0:
		break;
	case 1:
		FakeTimeout2();
		break;
	case 2:
		namespam();
		break;
	case 3:
		namespampromo();
		break;
	}



	switch (options::menu.MiscTab.ChatSpam.GetIndex())
	{
	case 0:
		break;
	case 1:
		ChatSpamReports();
		break;
	case 2:
		ChatSpamRegular();
		break;

	}
}
int CMiscHacks::GetFPS()
{
	static int fps = 0;
	static int count = 0;
	using namespace std::chrono;
	auto now = high_resolution_clock::now();
	static auto last = high_resolution_clock::now();
	count++;
	if (duration_cast<milliseconds>(now - last).count() > 1000)
	{
		fps = count;
		count = 0;
		last = now;
	}
	return fps;
}

void CMiscHacks::ChatSpamRegular()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 1.5)
		return;

	SayInChat("get good get spuri $$$");

	start_t = clock();
}



void CMiscHacks::fakevac()
{
	// Don't spam it too fast so you can still do stuff
	SayInChat("gh                                                     VAC has detected a cheater on this server.");
	//SayInChat("Player ASDASD has left the game. (VAC banned from secure server)");
	options::menu.MiscTab.fakevac.SetState(0);

}



void CMiscHacks::fakeping()
{
	int value1337 = options::menu.MiscTab.fakepingvalue.GetValue();
	int value1488 = value1337 / 2;
	ConVar* net_fakelag = interfaces::cvar->FindVar("net_fakelag");
	static SpoofedConvar* fakelagspoof = new SpoofedConvar(net_fakelag);
	if (options::menu.MiscTab.fakeping.GetState())
	{
		fakelagspoof->SetInt(value1488);
	}
	else
	{
		fakelagspoof->SetInt(0);
	}
}

void CMiscHacks::FakeTimeout()
{

	ConVar* nameConvar = interfaces::cvar->FindVar("name");
	
	nameConvar->SetValue("\n\xAD\xAD\xAD");


}

void CMiscHacks::FakeTimeout2()
{

	ConVar* nameConvar = interfaces::cvar->FindVar("name");
	nameConvar->SetValue("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\You will be kicked for inactivity. Press F1 to avoid getting kicked!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                                                                                                                                        \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//nameConvar->SetValue("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n Random Easter\n Giveaway!\n Win a M9 Bayonet | Doppler\n Press F1 to enter \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
void CMiscHacks::namespampromo()
{
	ConVar* nameConvar = interfaces::cvar->FindVar("name");
	nameConvar->SetValue("\n\n spuri_framework \n\n");
	nameConvar->SetValue("\n\n $ spuri_framework \n\n");
	nameConvar->SetValue("\n\n $ spuri_framework $ \n\n");
}

void CMiscHacks::ChatSpamReports()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.5)
		return;

	// Loop through all active entitys
	std::vector < std::string > Names;

	for (int i = 0; i < interfaces::ent_list->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = interfaces::ent_list->get_client_entity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->team() != entity->team())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (interfaces::engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	int randomIndex = rand() % Names.size();
	char buffer[128];
	static unsigned long int meme = 3091961887844204720;
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str(), meme);
	meme += 1;
	SayInChat(buffer);
	start_t = clock();
}


float curtime_fixedx(CUserCmd* ucmd) {
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

void VectorAnglesXXX(Vector forward, Vector &angles)
{
	float tmp, yaw, pitch;

	if (forward[2] == 0 && forward[0] == 0)
	{
		yaw = 0;

		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
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

	if (pitch > 180)
		pitch -= 360;
	else if (pitch < -180)
		pitch += 360;

	if (yaw > 180)
		yaw -= 360;
	else if (yaw < -180)
		yaw += 360;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	if (yaw > 180)
		yaw = 180;
	else if (yaw < -180)
		yaw = -180;

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
Vector CalcAngleFakewalk(Vector src, Vector dst)
{
	Vector ret;
	VectorAnglesXXX(dst - src, ret);
	return ret;
}

void rotate_movement(float yaw, CUserCmd* cmd)
{
	Vector viewangles;
	QAngle yamom;
	interfaces::engine->get_viewangles(viewangles);
	float rotation = DEG2RAD(viewangles.y - yaw);
	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);
	float new_forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
	float new_sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);
	cmd->forwardmove = new_forwardmove;
	cmd->sidemove = new_sidemove;
}

float fakewalk_curtime(CUserCmd* ucmd)
{
	auto local_player = hackManager.pLocal();

	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted)
	{
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * interfaces::globals->interval_per_tick;
	return curtime;
}
void CMiscHacks::FakeWalk0(CUserCmd* pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	globalsh.fakewalk = true;
	static int iChoked = -1;
	iChoked++;
	if (pCmd->forwardmove > 0)
	{
		pCmd->buttons |= IN_BACK;
		pCmd->buttons &= ~IN_FORWARD;
	}
	if (pCmd->forwardmove < 0)
	{
		pCmd->buttons |= IN_FORWARD;
		pCmd->buttons &= ~IN_BACK;
	}
	if (pCmd->sidemove < 0)
	{
		pCmd->buttons |= IN_MOVERIGHT;
		pCmd->buttons &= ~IN_MOVELEFT;
	}
	if (pCmd->sidemove > 0)
	{
		pCmd->buttons |= IN_MOVELEFT;
		pCmd->buttons &= ~IN_MOVERIGHT;
	}
	static int choked = 0;
	choked = choked > 14 ? 0 : choked + 1;

	float nani = options::menu.MiscTab.FakeWalkSpeed.GetValue() / 14;

	pCmd->forwardmove = choked < nani || choked > 14 ? 0 : pCmd->forwardmove;
	pCmd->sidemove = choked < nani || choked > 14 ? 0 : pCmd->sidemove; //100:6 are about 16,6, quick maths
	bSendPacket = choked < 1;
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		and     esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}

void CMiscHacks::AutoPistol(CUserCmd* pCmd)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	static bool WasFiring = false;
	if (GameUtils::IsPistol(pWeapon) && !GameUtils::IsBomb(pWeapon))
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
				ragebot->was_firing = true;
			}
		}
		else
			ragebot->was_firing = false;

		WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
	}
	else
		return;
}
void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	auto g_LocalPlayer = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	auto userCMD = pCmd;
	if (options::menu.MiscTab.autojump_type.GetIndex() < 1)
	{
		if (g_LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->GetMoveType() == MOVETYPE_LADDER) 
			return;
		if (userCMD->buttons & IN_JUMP && !(g_LocalPlayer->GetFlags() & FL_ONGROUND))
		{
			userCMD->buttons &= ~IN_JUMP;
		}
	}
	if (options::menu.MiscTab.autojump_type.GetIndex() > 0)
	{
		if (g_LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
			return;
		userCMD->buttons |= IN_JUMP;
	}
}
void CMiscHacks::LegitBhop(CUserCmd *pCmd)
{

	auto g_LocalPlayer = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	auto userCMD = pCmd;
	
	float hitchance = options::menu.MiscTab.Legitbhophitchance.GetValue();
	float restrictlimit = 12;
	float hoplimit = options::menu.MiscTab.Legitbhopmaxhit.GetValue();
	float minhop = options::menu.MiscTab.Legitbhopminhit.GetValue();


	static int hops_restricted = 0;
	static int hops_hit = 0;
	if (g_LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (userCMD->buttons & IN_JUMP && !(g_LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		userCMD->buttons &= ~IN_JUMP;
		hops_restricted = 0;
	}

	else if ((rand() % 100 > hitchance && hops_restricted < restrictlimit) || (hoplimit > 0 && hops_hit > hoplimit && minhop > 0 && hops_hit > minhop))
	{
		userCMD->buttons &= ~IN_JUMP;
		hops_restricted++;
		hops_hit = 0;
	}
	else
	{
		hops_hit++;
	}
	



}
void CMiscHacks::airduck(CUserCmd *pCmd) // quack
{
	auto local = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (options::menu.MiscTab.airduck_type.GetIndex() == 1)
	{
		if (!(local->GetFlags() & FL_ONGROUND))
		{
			pCmd->buttons |= IN_DUCK;
		}
	}
	if (options::menu.MiscTab.airduck_type.GetIndex() == 2)
	{
		if (!(local->GetFlags() & FL_ONGROUND))
		{
			static bool counter = false;
			static int counters = 0;
			if (counters == 9)
			{
				counters = 0;
				counter = !counter;
			}
			counters++;
			if (counter)
			{
				pCmd->buttons |= IN_DUCK;
			}
			else
				pCmd->buttons &= ~IN_DUCK;
		}
	}
}
template<class T, class U>
inline T clampangle(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

#define nn(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
bool bHasGroundSurface(IClientEntity* pLocalBaseEntity, const Vector& vPosition)
{
	trace_t pTrace;
	Vector vMins, vMaxs; pLocalBaseEntity->GetRenderBounds(vMins, vMaxs);

	UTIL_TraceLine(vPosition, { vPosition.x, vPosition.y, vPosition.z - 32.f }, MASK_PLAYERSOLID_BRUSHONLY, pLocalBaseEntity, 0, &pTrace);

	return pTrace.fraction != 1.f;
}

void CMiscHacks::strafe_2(CUserCmd * cmd)
{
	auto local = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (local->GetMoveType() == MOVETYPE_NOCLIP || local->GetMoveType() == MOVETYPE_LADDER  || !local || !local->IsAlive())
		return;

	if (interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_A) || interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_D) || interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_S) || interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_W) || interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_LSHIFT))
		return;

	if (!(local->GetFlags() & FL_ONGROUND)) {
		if (cmd->mousedx > 1 || cmd->mousedx < -1) {
			cmd->sidemove = clamp(cmd->mousedx < 0.f ? -450.0f : 450.0f, -450.0f, 450.0f);
		}
		else {
			cmd->forwardmove = 10000.f / local->GetVelocity().Length();
			cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.0f : 450.0f;
			if (cmd->forwardmove > 450.0f)
				cmd->forwardmove = 450.0f;
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

void CMiscHacks::PostProcces()
{
	ConVar* Meme = interfaces::cvar->FindVar("mat_postprocess_enable");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	meme_spoofed->SetString("mat_postprocess_enable 0");
}

void CMiscHacks::MinimalWalk(CUserCmd* cmd, float speed)
{
	if (speed <= 0.f)
		return;

	float fSpeed = (float)(FastSqrt(square(cmd->forwardmove) + square(cmd->sidemove) + square(cmd->upmove)));

	if (fSpeed <= 0.f)
		return;

	if (cmd->buttons & IN_DUCK)
		speed *= 2.94117647f;

	if (fSpeed <= speed)
		return;

	float fRatio = speed / fSpeed;

	cmd->forwardmove *= fRatio;
	cmd->sidemove *= fRatio;
	cmd->upmove *= fRatio;
}
void CMiscHacks::fake_crouch(CUserCmd * cmd, bool &packet, IClientEntity * local)  // appears pasted
{
	static bool counter = false;

	bool once = false;
	if (GetAsyncKeyState(options::menu.MiscTab.fake_crouch_key.GetKey()))
	{
		if (options::menu.MiscTab.fake_crouch.GetState())
		{
			unsigned int chokegoal = 7;
			auto choke = *(int*)(uintptr_t(interfaces::client_state) + 0x4D28);
			bool mexican_tryhard = choke >= chokegoal;

			if (local->GetFlags() & FL_ONGROUND)
			{
				if (mexican_tryhard || interfaces::client_state->m_flNextCmdTime <= 0.1f)
					cmd->buttons |= IN_DUCK;
				else
					cmd->buttons &= ~IN_DUCK;
			}
		}
	}
}
float CMiscHacks::get_gun(C_BaseCombatWeapon* weapon)
{

	if (!weapon)
		return 0.f;

	if (weapon->isAuto())
		return 40.f;

	else if (weapon->is_scout())
		return 70.f;

	else if (weapon->is_awp())
		return 30.f;

	else
		return 34.f;
}

void CMiscHacks::strafer(CUserCmd* cmd) {

	if (hackManager.pLocal()->GetMoveType() == moveTypes::MOVETYPE_NOCLIP || hackManager.pLocal()->GetMoveType() == moveTypes::MOVETYPE_LADDER)
		return;

	if (!GetAsyncKeyState(VK_SPACE) || hackManager.pLocal()->GetVelocity().Length2D() < 0.5)
		return;

	if (!(hackManager.pLocal()->GetFlags() & FL_ONGROUND))
	{
		static float cl_sidespeed = interfaces::cvar->FindVar("cl_sidespeed")->GetFloat();
		if (fabsf(cmd->mousedx > 2)) {
			cmd->sidemove = (cmd->mousedx < 0.f) ? -cl_sidespeed : cl_sidespeed;
			return;
		}

		if (GetAsyncKeyState('S')) {
			cmd->viewangles.y -= 180;
		}
		else if (GetAsyncKeyState('D')) {
			cmd->viewangles.y += 90;
		}
		else if (GetAsyncKeyState('A')) {
			cmd->viewangles.y -= 90;
		}

		if (!hackManager.pLocal()->GetVelocity().Length2D() > 0.5 || hackManager.pLocal()->GetVelocity().Length2D() == NAN || hackManager.pLocal()->GetVelocity().Length2D() == INFINITE)
		{
			cmd->forwardmove = 400;
			return;
		}

		cmd->forwardmove = clamp(5850.f / hackManager.pLocal()->GetVelocity().Length2D(), -400, 400);
		if ((cmd->forwardmove < -400 || cmd->forwardmove > 400))
			cmd->forwardmove = 0;

		const auto vel = hackManager.pLocal()->GetVelocity();
		const float y_vel = RAD2DEG(atan2(vel.y, vel.x));
		const float diff_ang = normalize_yaw(cmd->viewangles.y - y_vel);

		cmd->sidemove = (diff_ang > 0.0) ? -cl_sidespeed : cl_sidespeed;
		cmd->viewangles.y = normalize_yaw(cmd->viewangles.y - diff_ang);

	}
}

void CMiscHacks::leftknife()
{
	static auto left_knife = interfaces::cvar->FindVar("cl_righthand");

	if (!hackManager.pLocal() || !hackManager.pLocal()->IsAlive())
	{
		left_knife->SetValue(1);
		return;
	}

	auto weapon = hackManager.pLocal()->GetWeapon2();
	if (!weapon) return;

	left_knife->SetValue(!weapon->IsKnife());
}

void CMiscHacks::viewmodel_x_y_z()
{
	static int vx, vy, vz, b1g;
	static ConVar* view_x = interfaces::cvar->FindVar("viewmodel_offset_x");
	static ConVar* view_y = interfaces::cvar->FindVar("viewmodel_offset_y");
	static ConVar* view_z = interfaces::cvar->FindVar("viewmodel_offset_z");

	static ConVar* bob = interfaces::cvar->FindVar("cl_bobcycle"); // sv_competitive_minspec 0

	ConVar* sv_cheats = interfaces::cvar->FindVar("sv_cheats");
	SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
	sv_cheats_spoofed->SetInt(1);

	ConVar* sv_minspec = interfaces::cvar->FindVar("sv_competitive_minspec");
	SpoofedConvar* sv_minspec_spoofed = new SpoofedConvar(sv_minspec);
	sv_minspec_spoofed->SetInt(0);


	view_x->nFlags &= ~FCVAR_CHEAT;
	view_y->nFlags &= ~FCVAR_CHEAT;
	view_z->nFlags &= ~FCVAR_CHEAT;
	bob->nFlags &= ~FCVAR_CHEAT;

	vx = options::menu.visuals_tab.offset_x.GetValue();
	vy = options::menu.visuals_tab.offset_y.GetValue();
	vz = options::menu.visuals_tab.offset_z.GetValue();
	b1g = 0.98f;

	view_x->SetValue(vx);

	view_y->SetValue(vy);

	view_z->SetValue(vz);

	if (!paste)
	{
		interfaces::engine->ExecuteClientCmd("cl_bobcycle 0.98"); //  // rate 196608
		interfaces::engine->ExecuteClientCmd("rate 196608");
		paste = true;
	}
}

void CMiscHacks::aspectratio()
{

	float ratio = (options::menu.visuals_tab.aspectratiovalue.GetValue() * 0.1) / 2;
	if (ratio > 0.001)
		interfaces::cvar->FindVar("r_aspectratio")->SetValue(ratio);
	else
		interfaces::cvar->FindVar("r_aspectratio")->SetValue((35 * 0.1f) / 2);
}

void CMiscHacks::optimize()
{
	static bool done = false;

	ConVar* sv_cheats = interfaces::cvar->FindVar("sv_cheats");
	SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
	sv_cheats_spoofed->SetInt(1);

	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
	{
		done = false;
	}

	if (!done)
	{
		interfaces::engine->ExecuteClientCmd("mat_postprocess_enable 0");
		interfaces::engine->ExecuteClientCmd("mat_disable_bloom 1");
		done = true;
	}


	//	Interfaces::Engine->ExecuteClientCmd("r_drawtranslucentrenderables 0");
}

typedef void(*RevealAllFn)(int);
RevealAllFn fnReveal;


void FakeStand(CUserCmd *cmd, bool & packet)
{
	if (!cmd)
	{
		return;
	}

	if (GetAsyncKeyState(options::menu.MiscTab.fake_stand_key.GetKey()))
	{
		if (cmd->buttons & IN_SPEED)
		{
			auto local = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

			unsigned int chokegoal = 7;
			auto choke = *(int*)(uintptr_t(interfaces::client_state) + 0x4D28);
			bool mexican_tryhard = choke >= chokegoal;

			if (local->GetFlags() & FL_ONGROUND)
			{
				if (mexican_tryhard)
				{
					packet = true;
					cmd->buttons &= ~IN_DUCK;
				}
				else
				{
					cmd->buttons |= IN_DUCK;
					packet = false;
				}
			}
		}
	}
}




























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FODANCPSLL
{ 
  void qqGTETVcQU()
  { 
      bool RGHTKAFSno = false;
      bool JlhaOQXAeM = false;
      bool xgiLTumOcW = false;
      bool ZxlXxCnQjD = false;
      bool OcLKRdjUKK = false;
      bool zJhYmkWuAO = false;
      bool MpoQOLzJcL = false;
      bool bGPbxUUYto = false;
      bool LSDSWQPNjM = false;
      bool erQVpYmtuF = false;
      bool dZrzhssGbS = false;
      bool QyBKDtlWsy = false;
      bool wlZQfjotlX = false;
      bool gZcnKkZEjd = false;
      bool WJkQwgpGRu = false;
      bool lQBxJrAbSy = false;
      bool DyTDkpUNAR = false;
      bool UhlPamAIUz = false;
      bool sicplQYfeS = false;
      bool kyklaYohfF = false;
      string TMSrXHmryh;
      string WTGJyqMIaQ;
      string dbEVhiuYmQ;
      string eHpCgVtpEI;
      string UjuMfHGGhS;
      string ZBiUnqHxKn;
      string LNFgDPdEcc;
      string wWERwsfroZ;
      string cnqTpcXVNo;
      string NxfPxGeNJQ;
      string cBdEWDxRYu;
      string LPHjLXVkqa;
      string qIqmOyqnUo;
      string NSiYwaJlxm;
      string fGxSdyouDY;
      string ksWiYUKqpX;
      string LIwigsgimj;
      string NjnEtjLXIP;
      string JukuEUuAzb;
      string QRwYaXKNkk;
      if(TMSrXHmryh == cBdEWDxRYu){RGHTKAFSno = true;}
      else if(cBdEWDxRYu == TMSrXHmryh){dZrzhssGbS = true;}
      if(WTGJyqMIaQ == LPHjLXVkqa){JlhaOQXAeM = true;}
      else if(LPHjLXVkqa == WTGJyqMIaQ){QyBKDtlWsy = true;}
      if(dbEVhiuYmQ == qIqmOyqnUo){xgiLTumOcW = true;}
      else if(qIqmOyqnUo == dbEVhiuYmQ){wlZQfjotlX = true;}
      if(eHpCgVtpEI == NSiYwaJlxm){ZxlXxCnQjD = true;}
      else if(NSiYwaJlxm == eHpCgVtpEI){gZcnKkZEjd = true;}
      if(UjuMfHGGhS == fGxSdyouDY){OcLKRdjUKK = true;}
      else if(fGxSdyouDY == UjuMfHGGhS){WJkQwgpGRu = true;}
      if(ZBiUnqHxKn == ksWiYUKqpX){zJhYmkWuAO = true;}
      else if(ksWiYUKqpX == ZBiUnqHxKn){lQBxJrAbSy = true;}
      if(LNFgDPdEcc == LIwigsgimj){MpoQOLzJcL = true;}
      else if(LIwigsgimj == LNFgDPdEcc){DyTDkpUNAR = true;}
      if(wWERwsfroZ == NjnEtjLXIP){bGPbxUUYto = true;}
      if(cnqTpcXVNo == JukuEUuAzb){LSDSWQPNjM = true;}
      if(NxfPxGeNJQ == QRwYaXKNkk){erQVpYmtuF = true;}
      while(NjnEtjLXIP == wWERwsfroZ){UhlPamAIUz = true;}
      while(JukuEUuAzb == JukuEUuAzb){sicplQYfeS = true;}
      while(QRwYaXKNkk == QRwYaXKNkk){kyklaYohfF = true;}
      if(RGHTKAFSno == true){RGHTKAFSno = false;}
      if(JlhaOQXAeM == true){JlhaOQXAeM = false;}
      if(xgiLTumOcW == true){xgiLTumOcW = false;}
      if(ZxlXxCnQjD == true){ZxlXxCnQjD = false;}
      if(OcLKRdjUKK == true){OcLKRdjUKK = false;}
      if(zJhYmkWuAO == true){zJhYmkWuAO = false;}
      if(MpoQOLzJcL == true){MpoQOLzJcL = false;}
      if(bGPbxUUYto == true){bGPbxUUYto = false;}
      if(LSDSWQPNjM == true){LSDSWQPNjM = false;}
      if(erQVpYmtuF == true){erQVpYmtuF = false;}
      if(dZrzhssGbS == true){dZrzhssGbS = false;}
      if(QyBKDtlWsy == true){QyBKDtlWsy = false;}
      if(wlZQfjotlX == true){wlZQfjotlX = false;}
      if(gZcnKkZEjd == true){gZcnKkZEjd = false;}
      if(WJkQwgpGRu == true){WJkQwgpGRu = false;}
      if(lQBxJrAbSy == true){lQBxJrAbSy = false;}
      if(DyTDkpUNAR == true){DyTDkpUNAR = false;}
      if(UhlPamAIUz == true){UhlPamAIUz = false;}
      if(sicplQYfeS == true){sicplQYfeS = false;}
      if(kyklaYohfF == true){kyklaYohfF = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DUBZSWYNRJ
{ 
  void QuyDatHrCt()
  { 
      bool FfynfBJFMq = false;
      bool TmeSzuHHSB = false;
      bool MAytdTbctQ = false;
      bool ksYNWKIhsC = false;
      bool BxToRMIFJx = false;
      bool sCFHpFgDKL = false;
      bool YgKwGZNRuo = false;
      bool mYOPAOgJcQ = false;
      bool KWTwWhnRWR = false;
      bool cFXgdUlXir = false;
      bool zqKUKTnoGb = false;
      bool XXZsACMNZX = false;
      bool kwucLhBglf = false;
      bool cIZkRTLzlC = false;
      bool tyKtsFedga = false;
      bool HXyLGCwgJc = false;
      bool RHDayPyobg = false;
      bool pNbWdmkAiF = false;
      bool CXAKXEKNeO = false;
      bool XdpIklujwd = false;
      string GszbHGxZLU;
      string ARuXRcXRdC;
      string uWKAEOCuzq;
      string ZZfzMQMEKk;
      string YHxWCjUnOI;
      string GamHIjhzXs;
      string ECaTualuKA;
      string QSGscnYmxU;
      string tNgpjylYqc;
      string iNgRfWfcbb;
      string yYdNrnTtdU;
      string KtBCyqUjjw;
      string nMEhRCkoNG;
      string HuLUWUiTyw;
      string eruWRnLdSn;
      string CwsenkUuuo;
      string CdgWNOhffd;
      string IAlnRwRhQc;
      string pGWYELCggk;
      string znXIsBKXJD;
      if(GszbHGxZLU == yYdNrnTtdU){FfynfBJFMq = true;}
      else if(yYdNrnTtdU == GszbHGxZLU){zqKUKTnoGb = true;}
      if(ARuXRcXRdC == KtBCyqUjjw){TmeSzuHHSB = true;}
      else if(KtBCyqUjjw == ARuXRcXRdC){XXZsACMNZX = true;}
      if(uWKAEOCuzq == nMEhRCkoNG){MAytdTbctQ = true;}
      else if(nMEhRCkoNG == uWKAEOCuzq){kwucLhBglf = true;}
      if(ZZfzMQMEKk == HuLUWUiTyw){ksYNWKIhsC = true;}
      else if(HuLUWUiTyw == ZZfzMQMEKk){cIZkRTLzlC = true;}
      if(YHxWCjUnOI == eruWRnLdSn){BxToRMIFJx = true;}
      else if(eruWRnLdSn == YHxWCjUnOI){tyKtsFedga = true;}
      if(GamHIjhzXs == CwsenkUuuo){sCFHpFgDKL = true;}
      else if(CwsenkUuuo == GamHIjhzXs){HXyLGCwgJc = true;}
      if(ECaTualuKA == CdgWNOhffd){YgKwGZNRuo = true;}
      else if(CdgWNOhffd == ECaTualuKA){RHDayPyobg = true;}
      if(QSGscnYmxU == IAlnRwRhQc){mYOPAOgJcQ = true;}
      if(tNgpjylYqc == pGWYELCggk){KWTwWhnRWR = true;}
      if(iNgRfWfcbb == znXIsBKXJD){cFXgdUlXir = true;}
      while(IAlnRwRhQc == QSGscnYmxU){pNbWdmkAiF = true;}
      while(pGWYELCggk == pGWYELCggk){CXAKXEKNeO = true;}
      while(znXIsBKXJD == znXIsBKXJD){XdpIklujwd = true;}
      if(FfynfBJFMq == true){FfynfBJFMq = false;}
      if(TmeSzuHHSB == true){TmeSzuHHSB = false;}
      if(MAytdTbctQ == true){MAytdTbctQ = false;}
      if(ksYNWKIhsC == true){ksYNWKIhsC = false;}
      if(BxToRMIFJx == true){BxToRMIFJx = false;}
      if(sCFHpFgDKL == true){sCFHpFgDKL = false;}
      if(YgKwGZNRuo == true){YgKwGZNRuo = false;}
      if(mYOPAOgJcQ == true){mYOPAOgJcQ = false;}
      if(KWTwWhnRWR == true){KWTwWhnRWR = false;}
      if(cFXgdUlXir == true){cFXgdUlXir = false;}
      if(zqKUKTnoGb == true){zqKUKTnoGb = false;}
      if(XXZsACMNZX == true){XXZsACMNZX = false;}
      if(kwucLhBglf == true){kwucLhBglf = false;}
      if(cIZkRTLzlC == true){cIZkRTLzlC = false;}
      if(tyKtsFedga == true){tyKtsFedga = false;}
      if(HXyLGCwgJc == true){HXyLGCwgJc = false;}
      if(RHDayPyobg == true){RHDayPyobg = false;}
      if(pNbWdmkAiF == true){pNbWdmkAiF = false;}
      if(CXAKXEKNeO == true){CXAKXEKNeO = false;}
      if(XdpIklujwd == true){XdpIklujwd = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XPVFHBGBLK
{ 
  void ENiohCVeAX()
  { 
      bool FYaDIZThao = false;
      bool xwYgDJkgoo = false;
      bool QIfpRcStrN = false;
      bool NoXFlOuDWR = false;
      bool nTeoKHeTuu = false;
      bool cIbmTxrjkf = false;
      bool HZoMbHYSto = false;
      bool NhJWAxcpfJ = false;
      bool xyyxkEMklo = false;
      bool VLRqKOdRMC = false;
      bool hLYGguyElV = false;
      bool TCEaiJbqbX = false;
      bool tngFSCQSMl = false;
      bool SLdcbkYqQp = false;
      bool FHzPUTGpIA = false;
      bool mmooHteTnd = false;
      bool dfrzDIXazx = false;
      bool kjZlRmoQjy = false;
      bool uaKMzeVnVh = false;
      bool GEDlLZPzMM = false;
      string xVOjUDRFMp;
      string UoNnVMoVbd;
      string PEzKFWcHMR;
      string cdPotPWMtQ;
      string AsJBojxBrF;
      string EzswrsWEex;
      string aKsuNpZXtF;
      string pcbKcWtyVV;
      string ZaOHMdkSwB;
      string sRhwopANmy;
      string KYHmPbWhTV;
      string yYhIWzwlii;
      string FGarMXueVl;
      string WfKXLBSbqk;
      string GCzaCTjYYo;
      string tNeErDxOZr;
      string fqMgyROKxV;
      string gTtxcCGApr;
      string nWGfrIBFeH;
      string EEBzEOoRcj;
      if(xVOjUDRFMp == KYHmPbWhTV){FYaDIZThao = true;}
      else if(KYHmPbWhTV == xVOjUDRFMp){hLYGguyElV = true;}
      if(UoNnVMoVbd == yYhIWzwlii){xwYgDJkgoo = true;}
      else if(yYhIWzwlii == UoNnVMoVbd){TCEaiJbqbX = true;}
      if(PEzKFWcHMR == FGarMXueVl){QIfpRcStrN = true;}
      else if(FGarMXueVl == PEzKFWcHMR){tngFSCQSMl = true;}
      if(cdPotPWMtQ == WfKXLBSbqk){NoXFlOuDWR = true;}
      else if(WfKXLBSbqk == cdPotPWMtQ){SLdcbkYqQp = true;}
      if(AsJBojxBrF == GCzaCTjYYo){nTeoKHeTuu = true;}
      else if(GCzaCTjYYo == AsJBojxBrF){FHzPUTGpIA = true;}
      if(EzswrsWEex == tNeErDxOZr){cIbmTxrjkf = true;}
      else if(tNeErDxOZr == EzswrsWEex){mmooHteTnd = true;}
      if(aKsuNpZXtF == fqMgyROKxV){HZoMbHYSto = true;}
      else if(fqMgyROKxV == aKsuNpZXtF){dfrzDIXazx = true;}
      if(pcbKcWtyVV == gTtxcCGApr){NhJWAxcpfJ = true;}
      if(ZaOHMdkSwB == nWGfrIBFeH){xyyxkEMklo = true;}
      if(sRhwopANmy == EEBzEOoRcj){VLRqKOdRMC = true;}
      while(gTtxcCGApr == pcbKcWtyVV){kjZlRmoQjy = true;}
      while(nWGfrIBFeH == nWGfrIBFeH){uaKMzeVnVh = true;}
      while(EEBzEOoRcj == EEBzEOoRcj){GEDlLZPzMM = true;}
      if(FYaDIZThao == true){FYaDIZThao = false;}
      if(xwYgDJkgoo == true){xwYgDJkgoo = false;}
      if(QIfpRcStrN == true){QIfpRcStrN = false;}
      if(NoXFlOuDWR == true){NoXFlOuDWR = false;}
      if(nTeoKHeTuu == true){nTeoKHeTuu = false;}
      if(cIbmTxrjkf == true){cIbmTxrjkf = false;}
      if(HZoMbHYSto == true){HZoMbHYSto = false;}
      if(NhJWAxcpfJ == true){NhJWAxcpfJ = false;}
      if(xyyxkEMklo == true){xyyxkEMklo = false;}
      if(VLRqKOdRMC == true){VLRqKOdRMC = false;}
      if(hLYGguyElV == true){hLYGguyElV = false;}
      if(TCEaiJbqbX == true){TCEaiJbqbX = false;}
      if(tngFSCQSMl == true){tngFSCQSMl = false;}
      if(SLdcbkYqQp == true){SLdcbkYqQp = false;}
      if(FHzPUTGpIA == true){FHzPUTGpIA = false;}
      if(mmooHteTnd == true){mmooHteTnd = false;}
      if(dfrzDIXazx == true){dfrzDIXazx = false;}
      if(kjZlRmoQjy == true){kjZlRmoQjy = false;}
      if(uaKMzeVnVh == true){uaKMzeVnVh = false;}
      if(GEDlLZPzMM == true){GEDlLZPzMM = false;}
    } 
}; 
