/*
Syn's AyyWare Framework 2015
*/


#pragma once

#include "Hacks.h"

class CLegitBot : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:
	// Targetting
	int GetTargetCrosshair();
	bool TargetMeetsRequirements(IClientEntity* pEntity);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity* pEntity);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket);

	void SyncWeaponSettings();

	// Functionality
	void DoAimbot(CUserCmd *pCmd, bool &bSendpacket);
	void DoTrigger(CUserCmd *pCmd);
	void DoRecoilControl(CUserCmd *pCmd);
	void StandAloneRCS(IClientEntity* pLocal, CUserCmd* pCmd, C_BaseCombatWeapon* pWeapon);
	void LegitAA(CUserCmd *pCmd, bool& bSendPacket);

	// Aimbot
	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;

	float Speed;
	bool Multihitbox;
	int besthitbox;
	float FoV;
	bool RecoilControl1;
	float RecoilControl;
	bool PSilent;
	float Inacc;
	float RecoilAmountX;
	float RecoilAmountY;
};
