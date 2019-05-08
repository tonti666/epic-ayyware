#pragma once

#include "Hacks.h"
#include "Autowall.h"


class CVisuals : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:
	void DrawCrosshair();
	void NoScopeCrosshair();
	void DrawRecoilCrosshair();
	void AutowallCrosshairDMG();
	void DrawFOVCrosshair();
	void DrawDamageWall();
	void DrawRecoilCrosshair2();
	void SpreadCrosshair();
	void AutowallCrosshair();
	void NoScopeCrosshair2();

};

struct impact_info {
	float x, y, z;
	long long time;
};

struct hitmarker_info {
	impact_info impact;
	int alpha;
};




extern std::vector<impact_info> impacts;
extern std::vector<hitmarker_info> Xhitmarkers;
bool CanWallbang(float &dmg);
bool HandleBulletPenetration420(CSWeaponInfo *wpn_data, FireBulletData &data);