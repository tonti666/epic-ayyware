#include "kurvaback.h"
#include "RageBot.h"

#define TIME_TO_TICKS( dt )    ( ( int )( 0.5f + ( float )( dt ) / interfaces::globals->interval_per_tick ) )
/*
void becktrekk::frissit(int tick_count)
{
	if (options::menu.LegitBotTab.geci.GetState())
		return;

	utso_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{
		felvetelfrissites(i);
	}
}

bool becktrekk::rendestick(int tick)
{
	int delta = utso_tick - tick;
	float deltaTime = delta * interfaces::globals->interval_per_tick;
	return (fabs(deltaTime) <= 0.2f);
}

void becktrekk::felvetelfrissites(int i)
{
	IClientEntity* pEntity = interfaces::ent_list->get_client_entity(i);
	

		if (pEntity->isValidPlayer())
		{
			float lby = pEntity->GetLowerBodyYaw();
			felvetelek[i].zaszlok = pEntity->GetFlags();
			if (lby != felvetelek[i].alsotest)
			{
				felvetelek[i].tick_szamm = utso_tick;
				felvetelek[i].alsotest = lby;
				felvetelek[i].headPositionssss = pEntity->GetBonePos(0);

			}
		}
		else
		{
			felvetelek[i].tick_szamm = 0;
		}

}

bool becktrekk::lbybecktrekk(CUserCmd* cmd, Vector& aimPoint, IClientEntity * ent)
{

	for (int i = 0; i < interfaces::engine->GetMaxClients(); i++)
		{
			if (ent->GetVelocity().Length2D() > 1.f)
				continue;

			if (rendestick(felvetelek[i].tick_szamm))
			{
				aimPoint = felvetelek[i].headPositionssss;
				cmd->tick_count = felvetelek[i].tick_szamm;
				return true;
			}
		}
		return false;
}
void becktrekk::legitbecktrekk(CUserCmd* pCmd, IClientEntity* pLocal)
{
	if (options::menu.LegitBotTab.legitbacktrack.GetState())
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		player_info_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 0; i < interfaces::engine->GetMaxClients(); i++)
		{
			auto pEntity = (IClientEntity*)interfaces::ent_list->get_client_entity(i);

			if (!pEntity || !pLocal)
				continue;

			if (pEntity == pLocal)
				continue;

			if (!interfaces::engine->GetPlayerInfo(i, &info))
				continue;

			if (pEntity->IsDormant())
				continue;

			if (pEntity->team() == pLocal->team())
				continue;

			if (pEntity->IsAlive())
			{

				float simtime = pEntity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPosition(pEntity, 0);

				headPositionssss[i][pCmd->command_number % 13] = becktrekdata{ simtime, hitboxPos };
				Vector ViewDir = angle_vector(pCmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
				float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(pCmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < options::menu.LegitBotTab.legitbacktrackticks.GetValue(); ++t)
			{
				float tempFOVDistance = distance_point_to_line(headPositionssss[bestTargetIndex][t].hitboxpozicio, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositionssss[bestTargetIndex][t].simido > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositionssss[bestTargetIndex][t].simido;
				}
			}
			if (pCmd->buttons & IN_ATTACK)
			{
				pCmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}
	}
}

becktrekk* bekktrekkeles = new becktrekk();
becktrekdata headPositionssss[64][12];*/