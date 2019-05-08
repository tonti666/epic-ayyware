#include "antiaim.h"
#include "Hooks.h"
#include "MathFunctions.h"
#include "RageBot.h"
#include "fakelag.h"
anti_aim * c_antiaim = new anti_aim();
antiaim_helper * c_helper = new antiaim_helper();

static bool dir = false;
static bool back = false;
static bool up = false;
static bool jitter = false;
static bool jitter2 = false;
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

float anti_aim::get_feet_yaw()
{
	auto local_player = static_cast<IClientEntity*>(interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer()));

	if (!local_player)
		return 0.f;

	auto state = local_player->GetBasePlayerAnimState();

	float current_feet_yaw = state->m_flGoalFeetYaw;

	if (current_feet_yaw >= -360)
		current_feet_yaw = min(current_feet_yaw, 360.f);

	return current_feet_yaw;
}

#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)

//--------------------------------------------------------------------------------
void anti_aim::Base_AntiAim(CUserCmd* pCmd, IClientEntity* pLocal, bool &bSendPacket)
{

	if (GetAsyncKeyState(options::menu.MiscTab.manualleft.GetKey())) // right
	{
		dir = true;
		back = false;
		up = false;
		bigboi::indicator = 1;
	}

	if (GetAsyncKeyState(options::menu.MiscTab.manualright.GetKey())) // left
	{
		dir = false;
		back = false;
		up = false;
		bigboi::indicator = 2;
	}

	if (GetAsyncKeyState(options::menu.MiscTab.manualback.GetKey()))
	{
		dir = false;
		back = true;
		up = false;
		bigboi::indicator = 3;
	}

	if (GetAsyncKeyState(options::menu.MiscTab.manualfront.GetKey()))
	{
		dir = false;
		back = false;
		up = true;
		bigboi::indicator = 4;
	}

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal->IsMoving() && (pLocal->GetFlags() & FL_ONGROUND))
	{
		if ((options::menu.MiscTab.desync_aa_stand.GetState() && !bSendPacket) || !options::menu.MiscTab.desync_aa_stand.GetState())
		{
			switch (options::menu.MiscTab.AntiAimYaw.GetIndex())
			{

			case 1:
			{
				backwards(pCmd);
			}
			break;

			case 2:
			{
				backwards_jitter(pCmd, false);
			}
			break;

			case 3:
			{
				rand_lowerbody(pCmd);
			}
			break;

			case 4:
			{
				manual(pCmd);
			}
			break;

			case 5:
			{
				jitter_180(pCmd);
			}
			break;

			case 6:
			{
				jitter_crooked(pCmd);
			}
			break;

			case 7:
			{
				freestanding_jitter(pCmd);
			}
			break;

			case 8:
			{
				twisty(pCmd);
			}
			break;

			}
		}
		if (options::menu.MiscTab.desync_aa_stand.GetState() && bSendPacket)
		{
			do_desync(pCmd, false);
		}
	}

	if (pLocal->IsMoving())
	{

		switch ((pLocal->GetFlags() & FL_ONGROUND) ? options::menu.MiscTab.AntiAimYawrun.GetIndex() : options::menu.MiscTab.AntiAimYaw3.GetIndex())
		{

		case 1:
		{
			backwards(pCmd);
		}
		break;

		case 2:
		{
			backwards_jitter(pCmd, true);
		}
		break;

		case 3:
		{
			rand_lowerbody(pCmd);
		}
		break;

		case 4:
		{
			manual(pCmd);
		}
		break;

		case 5:
		{
			jitter_180(pCmd);
		}
		break;

		case 6:
		{
			jitter_crooked(pCmd);
		}
		break;

		case 7:
		{
			freestanding_jitter(pCmd);
		}
		break;

		}

		if (options::menu.MiscTab.desync_aa_move.GetState() && bSendPacket)
		{
			do_desync(pCmd, true);
		}
	}


}

void anti_aim::DoPitch(CUserCmd * pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (GameUtils::AutoSniper(pWeapon) && ragebot->was_firing_test == true)
		return;

	if (pCmd->buttons & IN_ATTACK && !(GameUtils::IsPistol(pWeapon) && GameUtils::AutoSniper(pWeapon)) && options::menu.MiscTab.OtherSafeMode.GetIndex() < 3)
		return;

	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (pCmd->buttons & IN_ATTACK && !GameUtils::IsPistol(pWeapon) && options::menu.MiscTab.OtherSafeMode.GetIndex() == 3)
	{
		pCmd->viewangles.x = 91.f;
		return;
	}
	switch (options::menu.MiscTab.AntiAimPitch.GetIndex())
	{
	case 0:
		break;
	case 1:
		pitchdown(pCmd);
		break;
	case 2:
		pitchup(pCmd);
		break;
	case 3:
	{
		pitchjitter(pCmd);
	}
	break;
	case 4:
	{
		pitchrandom(pCmd);
	}
	break;
	case 5:
	{
		zero(pCmd);
	}
	case 6:
	{
		fakedown(pCmd);
	}
	break;
	case 7:
	{
		fakeup(pCmd);
	}
	break;

	}
}

void anti_aim::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal)
		return;

	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (pCmd->buttons & IN_USE)
		return;

	if (GameUtils::IsGrenade(pWeapon))
		return;

	if (options::menu.MiscTab.OtherSafeMode.GetIndex() > 2 && pLocal->GetVelocity().Length2D() < 0.1)
	{
		pCmd->viewangles.z -= 90;
	}

	if (options::menu.MiscTab.OtherSafeMode.GetIndex() > 2 && pCmd->buttons & IN_ATTACK)
	{
		pCmd->viewangles.x = 92;
	}

	if (options::menu.MiscTab.disable_on_dormant.GetState())
	{
		if (c_helper->closest() == -1)
			return;
	}

	if (interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		return;

	if (options::menu.MiscTab.desync_twist_onshot.GetState())
	{
		if (c_fakelag->shot)
			*pLocal->GetBasePlayerAnimState()->feetyaw() = c_beam->real - 30.f;
	}

	DoPitch(pCmd);
	Base_AntiAim(pCmd, hackManager.pLocal(), bSendPacket);

	if (options::menu.MiscTab.antilby.GetIndex() > 0 && (pLocal->GetFlags() & FL_ONGROUND))
		c_helper->anti_lby(pCmd, bSendPacket);
}

void anti_aim::backwards(CUserCmd * pcmd)
{
	pcmd->viewangles.y -= 180;
}

void anti_aim::backwards_jitter(CUserCmd * pcmd, bool moving)
{
	jitter = !jitter;
	pcmd->viewangles.y += moving ? (jitter2 ? 130 : -130) : (jitter2 ? 145 + rand() % 15 : -145 - rand() % 15);
}

void anti_aim::lowerbody(CUserCmd * pcmd)
{
	pcmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + options::menu.MiscTab.lby1.GetValue();
}

void anti_aim::rand_lowerbody(CUserCmd * pcmd)
{
	pcmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + RandomFloat(options::menu.MiscTab.randlbyr.GetValue(), -options::menu.MiscTab.randlbyr.GetValue());
}

void anti_aim::jitter_180(CUserCmd * pcmd)
{
	jitter2 = !jitter2;
	pcmd->viewangles.y += jitter2 ? 180 : 0;
}

void anti_aim::manual(CUserCmd * pCmd)
{
	if (dir && !back && !up)
		pCmd->viewangles.y -= 90.f;
	else if (!dir && !back && !up)
		pCmd->viewangles.y += 90.f;
	else if (!dir && back && !up)
		pCmd->viewangles.y -= 180.f;
	else if (!dir && !back && up)
		pCmd->viewangles.y;
}

void anti_aim::twisty(CUserCmd * cmd)
{
	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	jitter2 = !jitter2;
	cmd->viewangles.y = 180;
	*local_player->GetBasePlayerAnimState()->feetyaw() += jitter2 ? 125.f : -125.f;
}

void anti_aim::pitchdown(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 89.f;
}

void anti_aim::pitchup(CUserCmd * pcmd)
{
	pcmd->viewangles.x = -89.f;
}

void anti_aim::zero(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 0.f;
}

void anti_aim::pitchjitter(CUserCmd * pcmd)
{
	if (jitter)
		pcmd->viewangles.x = 89.f;
	else
		pcmd->viewangles.x = -89.f;
	jitter = !jitter;
}

void anti_aim::pitchrandom(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 0.f + RandomFloat(-89.f, 89.f);
}

void anti_aim::fakedown(CUserCmd * pcmd)
{
	pcmd->viewangles.x = 540;
}

void anti_aim::fakeup(CUserCmd * pcmd)
{
	pcmd->viewangles.x = -540;
}


#define RandomInt(min, max) (rand() % (max - min + 1) + min)
#define	MASK_ALL				(0xFFFFFFFF)
#define	MASK_SOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 			/**< everything that is normally solid */
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 	/**< everything that blocks player movement */
#define	MASK_NPCSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) /**< blocks npc movement */
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME) 							/**< water physics in these contents */
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE) 							/**< everything that blocks line of sight for AI, lighting, etc */
#define MASK_OPAQUE_AND_NPCS	(MASK_OPAQUE|CONTENTS_MONSTER)										/**< everything that blocks line of sight for AI, lighting, etc, but with monsters added. */
#define	MASK_VISIBLE			(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE) 								/**< everything that blocks line of sight for players */
#define MASK_VISIBLE_AND_NPCS	(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE) 							/**< everything that blocks line of sight for players, but with monsters added. */
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX) 	/**< bullets see these as solid */
#define MASK_SHOT_HULL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE) 	/**< non-raycasted weapons see this as solid (includes grates) */
#define MASK_SHOT_PORTAL		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW) 							/**< hits solids (not grates) and passes through everything else */
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS) // non-raycasted weapons see this as solid (includes grates)
#define MASK_SOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE) 					/**< everything normally solid, except monsters (world+brush only) */
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for player movement, except monsters (world+brush only) */
#define MASK_NPCSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for npc movement, except monsters (world+brush only) */
#define MASK_NPCWORLDSTATIC		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 					/**< just the world, used for route rebuilding */
#define MASK_SPLITAREAPORTAL	(CONTENTS_WATER|CONTENTS_SLIME) 		

void anti_aim::freestanding_jitter(CUserCmd* pCmd) 
{

	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	float range = options::menu.MiscTab.freerange.GetValue() / 2;
	static int Ticks = 0;

	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	static float hold = 0.f;
	Vector besthead;
	float opposite = 0.f;

	auto leyepos = hackManager.pLocal()->GetOrigin_likeajew() + hackManager.pLocal()->GetViewOffset();
	auto headpos = hitbox_location(local_player, 0);
	auto origin = hackManager.pLocal()->GetOrigin_likeajew();

	auto checkWallThickness = [&](IClientEntity* pPlayer, Vector newhead) -> float
	{

		Vector endpos1, endpos2;

		Vector eyepos = pPlayer->GetOrigin_likeajew() + pPlayer->GetViewOffset();
		Ray_t ray;
		ray.Init(newhead, eyepos);
		CTraceFilterSkipTwoEntities filter(pPlayer, hackManager.pLocal());

		trace_t trace1, trace2;
		interfaces::trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.endpos;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		interfaces::trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.endpos;

		float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 3.f;
		return endpos1.Dist(endpos2) + add / 3;

	};

	int index = c_helper->closest();
	static IClientEntity* entity;

	if (index != -1)
		entity = interfaces::ent_list->get_client_entity(index); // maybe?

	if (!entity->isValidPlayer() || entity == nullptr)
	{
		pCmd->viewangles.y -= 178.f;
		return;
	}

	float radius = Vector(headpos - origin).Length2D();

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float besthead = 0; besthead < 7; besthead += 0.1)
		{
			Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
			float totalthickness = 0.f;
			no_active = false;
			totalthickness += checkWallThickness(entity, newhead);
			if (totalthickness > highestthickness)
			{

				highestthickness = totalthickness;
				bestrotation = besthead;
			}
		}
	}
	if (no_active)
	{
		pCmd->viewangles.y = 178 + RandomFloat(-range, range);
		return;
	}

	else
	{
		jitter = !jitter;
		if (jitter)
			pCmd->viewangles.y = RAD2DEG(bestrotation) + range;
		else
			pCmd->viewangles.y = RAD2DEG(bestrotation) - range;
	}
}

void anti_aim::jitter_crooked(CUserCmd * pcmd) //by faxzee
{
	jitter2 = !jitter2;
	float fljit = 35;
	float flCrookedoffset = -15;

	if (jitter2) {
		pcmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + flCrookedoffset;
		pcmd->viewangles.y += fljit;
	}
	else {
		pcmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + flCrookedoffset;
		pcmd->viewangles.y -= fljit;
	}
}

void anti_aim::do_desync(CUserCmd * cmd, bool moving)
{
	if (!cmd)
		return;

	if (!moving)
	{
		switch (options::menu.MiscTab.desync_type_stand.GetIndex())
		{
		case 0: desync_simple(cmd, moving);
			break;
		case 1: desync_default(cmd, moving);
			break;
		case 2: desync_stretch(cmd, moving);
			break;
		case 3: desync_crooked(cmd, moving);
			break;
		case 4: desync_auto(cmd, moving);//desync_spin(cmd, moving);
			break;
		case 5: desync_jitter(cmd, moving);
			break;
		case 6: desync_static(cmd, moving);
			break;
		case 7: desync_switch(cmd, moving);
			return;
		}
	}

	if (moving)
	{
		switch (options::menu.MiscTab.desync_type_move.GetIndex())
		{
		case 0: desync_simple(cmd, moving);
			break;
		case 1: desync_default(cmd, moving);
			break;
		case 2: desync_stretch(cmd, moving);
			break;
		case 3: desync_crooked(cmd, moving);
			break;
		case 4: desync_auto(cmd, moving);
			break;
		case 5: desync_jitter(cmd, moving);
			break;
		case 6: desync_static(cmd, moving);
			break;
		case 7: desync_switch(cmd, moving);
			return;
		}
	}
}
void anti_aim::desync_simple(CUserCmd * cmd, bool moving)
{
	jitter2 = !jitter2;
	cmd->viewangles.y = jitter2 ? c_beam->real + 58 : c_beam->real - 58;
}
void anti_aim::desync_default(CUserCmd * cmd, bool moving)
{
	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;

	float r = moving ? move_range : stand_range;

	bool done = false;

//	*local_player->GetBasePlayerAnimState()->feetyaw() += jitter2 ? -r : r;
	if (!done)
	{
		cmd->viewangles.y = c_beam->real + RandomFloat(29, -29);
		cmd->viewangles.y += r;
		done = true;
	}
	else
	{
		cmd->viewangles.y = c_beam->real + RandomFloat(29, -29);
		cmd->viewangles.y -= r;
		done = false;
	}
	jitter2 = !jitter2;
	*local_player->GetBasePlayerAnimState()->feetyaw() += (moving ? (jitter2 ? 29 : 0) : (jitter2 ? 40.f : 0));
}

void anti_aim::desync_stretch(CUserCmd * cmd, bool moving)
{
	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;

	jitter2 = !jitter2;
	int n = cmd->command_number % 3;

	if (!moving)
	{
		if (n != 3)
		{
			cmd->viewangles.y = c_beam->real + stand_range;
		}
		else
		{
			cmd->viewangles.y = c_beam->real - stand_range;
		}
	}

	if (moving)
	{
		if (n != 3)
		{
			cmd->viewangles.y = c_beam->real + move_range;
		}
		else
		{
			cmd->viewangles.y = c_beam->real - stand_range;
		}
	}

	*local_player->GetBasePlayerAnimState()->feetyaw() += jitter2 ? -58 : 58;
}

void anti_aim::desync_crooked(CUserCmd * cmd, bool moving)
{
	jitter2 = !jitter2;
	float fljit = moving ? move_range : stand_range;

	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (jitter2) {
		cmd->viewangles.y = *local_player->GetBasePlayerAnimState()->feetyaw() + fljit;	
	}
	else {
		cmd->viewangles.y = c_beam->real - RandomFloat(-fljit, fljit);
	}
	*local_player->GetBasePlayerAnimState()->feetyaw() = c_beam->real - jitter2 ? 40.f : -40.f;
}

void anti_aim::desync_switch(CUserCmd * cmd, bool moving)
{
	float range = moving ? move_range : stand_range;

	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;

	cmd->viewangles.y = c_beam->real + RandomFloat(range, -range);
	*local_player->GetBasePlayerAnimState()->feetyaw() = c_beam->real + RandomFloat(range / 2, -range / 2);;

}



void anti_aim::desync_auto(CUserCmd * cmd, bool moving)
{
	float range = moving ? move_range : stand_range;

	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;
	cmd->viewangles.y = c_beam->real + RandomFloat(range, 0);

	jitter2 = !jitter2;
	*local_player->GetBasePlayerAnimState()->feetyaw() = RandomFloat(range, 0);

}

void anti_aim::desync_jitter(CUserCmd * cmd, bool moving)
{
	float range = moving ? move_range : stand_range;

	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;
	int n = cmd->command_number % 3;

	if (n == 3)
	{
		if (*local_player->GetBasePlayerAnimState()->feetyaw() - local_player->GetEyeAnglesXY()->y > 58)
		{
			jitter2 = !jitter2;
			cmd->viewangles.y = 180 + (jitter2 ? -range : range);
		}
		else
		{
			jitter2 = !jitter2;
			cmd->viewangles.y += jitter2 ? 29 : -29;
		}
	}
	else
	{
		jitter2 = !jitter2;
		cmd->viewangles.y = c_beam->real + jitter2 ? range : -range;
		*local_player->GetBasePlayerAnimState()->feetyaw() = jitter2 ? 90 : -90;
	}
}

void anti_aim::desync_static(CUserCmd * cmd, bool moving)
{
	IClientEntity* local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;

	float range = moving ? 29.f: stand_range;

	cmd->viewangles.y = c_beam->real - range;
// 	*local_player->GetBasePlayerAnimState()->feetyaw() -= range;
}

































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SIRASZMRGA
{ 
  void QZsMBdEuPr()
  { 
      bool PBJbhCJiDw = false;
      bool dAlcGTrfHq = false;
      bool eNkmzGflMJ = false;
      bool XgYZwJnCHH = false;
      bool SidPEDBTPd = false;
      bool IGBAcCPVbb = false;
      bool WWVsXxbMfx = false;
      bool epYzYujVhw = false;
      bool ELWiBQeXxt = false;
      bool sRZfjuNjqH = false;
      bool bonSyRhBah = false;
      bool RsnfWLAmrG = false;
      bool RDPkDNQsPc = false;
      bool hzeaUOXDki = false;
      bool icwObAdsIY = false;
      bool uZqjoXZCPj = false;
      bool AolyzSOMLH = false;
      bool mwBJXnGAID = false;
      bool iWjphHItkw = false;
      bool oVnAhtqyPJ = false;
      string PPBdrBKgPu;
      string HZyIPusLBy;
      string rbeqTFfrQi;
      string FmayDmXMtC;
      string MgylwhrNqz;
      string ZznsZnjJJm;
      string VRPgoTyzEb;
      string XfRzkLdstl;
      string NRBQIGyhYP;
      string DNDGfuQRxb;
      string jrJObthoHN;
      string JqPXZDPyXi;
      string UPanYJcKSp;
      string tLuSCVImoO;
      string XJaKFLyegc;
      string fSsNHTQUpR;
      string QWVRSxuwVE;
      string tcJJKLoPhM;
      string LxdpKbSgLh;
      string EnxWxBmIqI;
      if(PPBdrBKgPu == jrJObthoHN){PBJbhCJiDw = true;}
      else if(jrJObthoHN == PPBdrBKgPu){bonSyRhBah = true;}
      if(HZyIPusLBy == JqPXZDPyXi){dAlcGTrfHq = true;}
      else if(JqPXZDPyXi == HZyIPusLBy){RsnfWLAmrG = true;}
      if(rbeqTFfrQi == UPanYJcKSp){eNkmzGflMJ = true;}
      else if(UPanYJcKSp == rbeqTFfrQi){RDPkDNQsPc = true;}
      if(FmayDmXMtC == tLuSCVImoO){XgYZwJnCHH = true;}
      else if(tLuSCVImoO == FmayDmXMtC){hzeaUOXDki = true;}
      if(MgylwhrNqz == XJaKFLyegc){SidPEDBTPd = true;}
      else if(XJaKFLyegc == MgylwhrNqz){icwObAdsIY = true;}
      if(ZznsZnjJJm == fSsNHTQUpR){IGBAcCPVbb = true;}
      else if(fSsNHTQUpR == ZznsZnjJJm){uZqjoXZCPj = true;}
      if(VRPgoTyzEb == QWVRSxuwVE){WWVsXxbMfx = true;}
      else if(QWVRSxuwVE == VRPgoTyzEb){AolyzSOMLH = true;}
      if(XfRzkLdstl == tcJJKLoPhM){epYzYujVhw = true;}
      if(NRBQIGyhYP == LxdpKbSgLh){ELWiBQeXxt = true;}
      if(DNDGfuQRxb == EnxWxBmIqI){sRZfjuNjqH = true;}
      while(tcJJKLoPhM == XfRzkLdstl){mwBJXnGAID = true;}
      while(LxdpKbSgLh == LxdpKbSgLh){iWjphHItkw = true;}
      while(EnxWxBmIqI == EnxWxBmIqI){oVnAhtqyPJ = true;}
      if(PBJbhCJiDw == true){PBJbhCJiDw = false;}
      if(dAlcGTrfHq == true){dAlcGTrfHq = false;}
      if(eNkmzGflMJ == true){eNkmzGflMJ = false;}
      if(XgYZwJnCHH == true){XgYZwJnCHH = false;}
      if(SidPEDBTPd == true){SidPEDBTPd = false;}
      if(IGBAcCPVbb == true){IGBAcCPVbb = false;}
      if(WWVsXxbMfx == true){WWVsXxbMfx = false;}
      if(epYzYujVhw == true){epYzYujVhw = false;}
      if(ELWiBQeXxt == true){ELWiBQeXxt = false;}
      if(sRZfjuNjqH == true){sRZfjuNjqH = false;}
      if(bonSyRhBah == true){bonSyRhBah = false;}
      if(RsnfWLAmrG == true){RsnfWLAmrG = false;}
      if(RDPkDNQsPc == true){RDPkDNQsPc = false;}
      if(hzeaUOXDki == true){hzeaUOXDki = false;}
      if(icwObAdsIY == true){icwObAdsIY = false;}
      if(uZqjoXZCPj == true){uZqjoXZCPj = false;}
      if(AolyzSOMLH == true){AolyzSOMLH = false;}
      if(mwBJXnGAID == true){mwBJXnGAID = false;}
      if(iWjphHItkw == true){iWjphHItkw = false;}
      if(oVnAhtqyPJ == true){oVnAhtqyPJ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MLAKDTHNLO
{ 
  void AyxsjWMNmD()
  { 
      bool zOBFMzMrDF = false;
      bool KQTeQMWJbJ = false;
      bool KNpawPACIq = false;
      bool lQXJYBJENx = false;
      bool yycytPoBmo = false;
      bool PUDEMkYaIw = false;
      bool yKiLStsKFP = false;
      bool DSueLMkqfg = false;
      bool azOSaRNTwm = false;
      bool bhuRzmORfA = false;
      bool WeuXlxYYrA = false;
      bool AgubVywQjA = false;
      bool fYNRmUWdfX = false;
      bool xTKeJuEpfd = false;
      bool robwmumSuA = false;
      bool qygeTxQckR = false;
      bool aurTCgWQRZ = false;
      bool sFjGHqxRer = false;
      bool gDhegzmfKM = false;
      bool ebFODxwHsF = false;
      string lOCbMugULt;
      string bfaYcSyHCC;
      string ZGMKUowEqd;
      string EneVzFSfTU;
      string amWUlFefuD;
      string jMdbmeBqGX;
      string zbdgomBFZX;
      string zSnhbmjrXP;
      string KPJntrJCiT;
      string DbYuVzIpez;
      string EZGXDRYwAz;
      string uISqNiSpmx;
      string dyVFTXsrMc;
      string dsNYLjJkjb;
      string TNCiYtIhhl;
      string fSDgUFDAEf;
      string YgjXUBusfc;
      string YgFLdPulgY;
      string PLJawKpgDF;
      string gJfDCmdeOX;
      if(lOCbMugULt == EZGXDRYwAz){zOBFMzMrDF = true;}
      else if(EZGXDRYwAz == lOCbMugULt){WeuXlxYYrA = true;}
      if(bfaYcSyHCC == uISqNiSpmx){KQTeQMWJbJ = true;}
      else if(uISqNiSpmx == bfaYcSyHCC){AgubVywQjA = true;}
      if(ZGMKUowEqd == dyVFTXsrMc){KNpawPACIq = true;}
      else if(dyVFTXsrMc == ZGMKUowEqd){fYNRmUWdfX = true;}
      if(EneVzFSfTU == dsNYLjJkjb){lQXJYBJENx = true;}
      else if(dsNYLjJkjb == EneVzFSfTU){xTKeJuEpfd = true;}
      if(amWUlFefuD == TNCiYtIhhl){yycytPoBmo = true;}
      else if(TNCiYtIhhl == amWUlFefuD){robwmumSuA = true;}
      if(jMdbmeBqGX == fSDgUFDAEf){PUDEMkYaIw = true;}
      else if(fSDgUFDAEf == jMdbmeBqGX){qygeTxQckR = true;}
      if(zbdgomBFZX == YgjXUBusfc){yKiLStsKFP = true;}
      else if(YgjXUBusfc == zbdgomBFZX){aurTCgWQRZ = true;}
      if(zSnhbmjrXP == YgFLdPulgY){DSueLMkqfg = true;}
      if(KPJntrJCiT == PLJawKpgDF){azOSaRNTwm = true;}
      if(DbYuVzIpez == gJfDCmdeOX){bhuRzmORfA = true;}
      while(YgFLdPulgY == zSnhbmjrXP){sFjGHqxRer = true;}
      while(PLJawKpgDF == PLJawKpgDF){gDhegzmfKM = true;}
      while(gJfDCmdeOX == gJfDCmdeOX){ebFODxwHsF = true;}
      if(zOBFMzMrDF == true){zOBFMzMrDF = false;}
      if(KQTeQMWJbJ == true){KQTeQMWJbJ = false;}
      if(KNpawPACIq == true){KNpawPACIq = false;}
      if(lQXJYBJENx == true){lQXJYBJENx = false;}
      if(yycytPoBmo == true){yycytPoBmo = false;}
      if(PUDEMkYaIw == true){PUDEMkYaIw = false;}
      if(yKiLStsKFP == true){yKiLStsKFP = false;}
      if(DSueLMkqfg == true){DSueLMkqfg = false;}
      if(azOSaRNTwm == true){azOSaRNTwm = false;}
      if(bhuRzmORfA == true){bhuRzmORfA = false;}
      if(WeuXlxYYrA == true){WeuXlxYYrA = false;}
      if(AgubVywQjA == true){AgubVywQjA = false;}
      if(fYNRmUWdfX == true){fYNRmUWdfX = false;}
      if(xTKeJuEpfd == true){xTKeJuEpfd = false;}
      if(robwmumSuA == true){robwmumSuA = false;}
      if(qygeTxQckR == true){qygeTxQckR = false;}
      if(aurTCgWQRZ == true){aurTCgWQRZ = false;}
      if(sFjGHqxRer == true){sFjGHqxRer = false;}
      if(gDhegzmfKM == true){gDhegzmfKM = false;}
      if(ebFODxwHsF == true){ebFODxwHsF = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HUEZNUWHHP
{ 
  void fTroIFkkdE()
  { 
      bool kUAILayUwT = false;
      bool xNTMwxtyGb = false;
      bool kwuuKERJyS = false;
      bool UlOobJQECP = false;
      bool wsObohTlAP = false;
      bool bGThGnYExG = false;
      bool JoWcyKydjl = false;
      bool fpupfrhTFb = false;
      bool keIhpORgin = false;
      bool kMGuGGWkdd = false;
      bool joBNilKdCO = false;
      bool FFzUVefnmi = false;
      bool sQuwBTizjY = false;
      bool VwparucuzE = false;
      bool EndTheMlZb = false;
      bool ZhytkStFLP = false;
      bool CKscrJPXca = false;
      bool BkoieFmaNW = false;
      bool GxZIzmywpY = false;
      bool AWiiBstAbY = false;
      string OfMwosgVwA;
      string YnOuAquzlr;
      string pKdDZZwlyR;
      string XizHmyEhTh;
      string bGxebizhgG;
      string aNkwFECJVP;
      string wtWdcpnHHB;
      string mMeEPiMybq;
      string ZOErqKyDCi;
      string wtMEkgiSCk;
      string trZeTOhffV;
      string rDhdogpemR;
      string qjFYFrzaSi;
      string zsupwhoYYb;
      string cRXfqrMhOE;
      string qEfksJlBZb;
      string OGMxVyHrhG;
      string rQFLoZMYhM;
      string eRMOqtNKwl;
      string umzwMVEaJr;
      if(OfMwosgVwA == trZeTOhffV){kUAILayUwT = true;}
      else if(trZeTOhffV == OfMwosgVwA){joBNilKdCO = true;}
      if(YnOuAquzlr == rDhdogpemR){xNTMwxtyGb = true;}
      else if(rDhdogpemR == YnOuAquzlr){FFzUVefnmi = true;}
      if(pKdDZZwlyR == qjFYFrzaSi){kwuuKERJyS = true;}
      else if(qjFYFrzaSi == pKdDZZwlyR){sQuwBTizjY = true;}
      if(XizHmyEhTh == zsupwhoYYb){UlOobJQECP = true;}
      else if(zsupwhoYYb == XizHmyEhTh){VwparucuzE = true;}
      if(bGxebizhgG == cRXfqrMhOE){wsObohTlAP = true;}
      else if(cRXfqrMhOE == bGxebizhgG){EndTheMlZb = true;}
      if(aNkwFECJVP == qEfksJlBZb){bGThGnYExG = true;}
      else if(qEfksJlBZb == aNkwFECJVP){ZhytkStFLP = true;}
      if(wtWdcpnHHB == OGMxVyHrhG){JoWcyKydjl = true;}
      else if(OGMxVyHrhG == wtWdcpnHHB){CKscrJPXca = true;}
      if(mMeEPiMybq == rQFLoZMYhM){fpupfrhTFb = true;}
      if(ZOErqKyDCi == eRMOqtNKwl){keIhpORgin = true;}
      if(wtMEkgiSCk == umzwMVEaJr){kMGuGGWkdd = true;}
      while(rQFLoZMYhM == mMeEPiMybq){BkoieFmaNW = true;}
      while(eRMOqtNKwl == eRMOqtNKwl){GxZIzmywpY = true;}
      while(umzwMVEaJr == umzwMVEaJr){AWiiBstAbY = true;}
      if(kUAILayUwT == true){kUAILayUwT = false;}
      if(xNTMwxtyGb == true){xNTMwxtyGb = false;}
      if(kwuuKERJyS == true){kwuuKERJyS = false;}
      if(UlOobJQECP == true){UlOobJQECP = false;}
      if(wsObohTlAP == true){wsObohTlAP = false;}
      if(bGThGnYExG == true){bGThGnYExG = false;}
      if(JoWcyKydjl == true){JoWcyKydjl = false;}
      if(fpupfrhTFb == true){fpupfrhTFb = false;}
      if(keIhpORgin == true){keIhpORgin = false;}
      if(kMGuGGWkdd == true){kMGuGGWkdd = false;}
      if(joBNilKdCO == true){joBNilKdCO = false;}
      if(FFzUVefnmi == true){FFzUVefnmi = false;}
      if(sQuwBTizjY == true){sQuwBTizjY = false;}
      if(VwparucuzE == true){VwparucuzE = false;}
      if(EndTheMlZb == true){EndTheMlZb = false;}
      if(ZhytkStFLP == true){ZhytkStFLP = false;}
      if(CKscrJPXca == true){CKscrJPXca = false;}
      if(BkoieFmaNW == true){BkoieFmaNW = false;}
      if(GxZIzmywpY == true){GxZIzmywpY = false;}
      if(AWiiBstAbY == true){AWiiBstAbY = false;}
    } 
}; 
