#pragma once
#include "UTIL Functions.h"
#include "AW_Hitmarker.h"
/*#include <chrono>
#include "Rendermanager.h"
#include "Hacks.h"

CAW_hitmaker* hitmarker_2 = new CAW_hitmaker();

void CAW_hitmaker::paint()
{

	float time = interfaces::globals->curtime;

	for (int i = 0; i < hitmarkers.size(); i++)
	{
		bool expired = time >= hitmarkers.at(i).impact.time + 2.f;
		if (expired)
			hitmarkers.at(i).alpha -= 1;
		if (expired && hitmarkers.at(i).alpha <= 0)
		{
			hitmarkers.erase(hitmarkers.begin() + i);
			continue;
		}

		Vector pos3D = Vector(hitmarkers.at(i).impact.x, hitmarkers.at(i).impact.y, hitmarkers.at(i).impact.z), pos2D;
		if (!GameUtils::World2Screen(pos3D, pos2D))
			continue;

		int lineSize = 5;

		interfaces::surface->DrawSetColor(240, 240, 240, hitmarkers.at(i).alpha);
		interfaces::surface->DrawLine(pos2D.x - lineSize * 2, pos2D.y - lineSize * 2, pos2D.x - (1), pos2D.y - (1));
		interfaces::surface->DrawLine(pos2D.x - lineSize * 2, pos2D.y + lineSize * 2, pos2D.x - (1), pos2D.y + (1));
		interfaces::surface->DrawLine(pos2D.x + lineSize * 2, pos2D.y + lineSize * 2, pos2D.x + (2), pos2D.y + (2));
		interfaces::surface->DrawLine(pos2D.x + lineSize * 2, pos2D.y - lineSize * 2, pos2D.x + (2), pos2D.y - (2));
	}
}*/