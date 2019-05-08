#include "fakelag.h"
#include "Autowall.h"
#include "RageBot.h"
#include <array>
#include "MiscHacks.h"
fakelag * c_fakelag = new fakelag();

int random_int(int min, int max)
{
	return rand() % max + min;
}
void fakelag::break_lagcomp(bool & send_packet, int ammount)
{
	static int factor = random_int(1, ammount);
	static int tick; tick++;
	bool m_switch = false;
	int m_stepdown;
	send_packet = !(tick % factor);

	if (send_packet)
	{
		int m_choke = tick % ammount + 1;

		m_stepdown = 15 - m_choke;
		if (m_choke >= ammount)
			m_switch = !m_switch;

		factor = m_switch ? m_stepdown : m_choke;

		if (factor >= 15)
			factor = 14;
	}

}

int fakelag::break_lagcomp_mm(float ammount)
{
	float speed = hackManager.pLocal()->GetVelocity().Length2D();
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);

	if (speed > 0.f)
	{
		auto distance_per_tick = speed * interfaces::globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, ammount);
	}
}
int fakelag::break_lagcomp_mm_2()
{
	float speed = hackManager.pLocal()->GetVelocity().Length2D();
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);

	if (speed > 0.f)
	{
		auto distance_per_tick = speed * interfaces::globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
}
int random_integer(int min, int max)
{
	return rand() % max + min;
}
namespace i_hate_visual_studio
{
	template<class T, class U>
	T clamp(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
	
}
static bool whatever = false;
void fakelag::when_enemy_sight(bool &bSendPacket, IClientEntity * local, float ammount, bool &break_lc)
{
	int s = break_lc ? break_lagcomp_mm(ammount) : ammount;

	if (ragebot->can_autowall)
	{
		bSendPacket = ( s <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else
	{
		whatever = !whatever;
		if (whatever)
			bSendPacket = true;
		else
			bSendPacket = false;
	}
}

void fakelag::mirror_adaptive(bool &packet, IClientEntity * local, int ammount, bool break_lc, CUserCmd* cmd)
{
	bool done = false;
	int choked;
	auto anim_state = local->GetBasePlayerAnimState();
	if (cmd->buttons & IN_ATTACK)
	{
		packet = (2 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else if (local->GetVelocity().Length2D() >= 120.f)
	{
		if (ragebot->can_autowall)
		{
			break_lagcomp(packet, 14);
		}
		else
		{
			break_lagcomp_mm(6);
		}
	}
	else if (anim_state &&  anim_state->m_bInHitGroundAnimation && local->GetVelocity().Length2D() < 20)
	{
		packet = (14 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else if (local->GetVelocity().Length2D() < 120 && local->GetVelocity().Length2D() > 40.f)
	{
		if (break_lc)
			break_lagcomp(packet, ammount);
		else
			packet = (ammount <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else
	{
		whatever = !whatever;
		if (whatever)
			packet = true;
		else
			packet = false;
	}
}


void fakelag::aimware_adaptive(bool &send_packet, IClientEntity * local, int ammount)
{
	if (!local)
		return;

	float ExtrapolatedSpeed;
	int WishTicks;
	float VelocityY;
	float VelocityX;
	int WishTicks_1;
	signed int AdaptTicks;
	WishTicks = (ammount + 0.5) - 1;

	VelocityY = local->GetVelocity().y;
	VelocityX = local->GetVelocity().x;
	WishTicks_1 = 0;
	AdaptTicks = 2;
	ExtrapolatedSpeed = sqrt((VelocityX * VelocityX) + (VelocityY * VelocityY))
		* interfaces::globals->interval_per_tick;

	while ((WishTicks_1 * ExtrapolatedSpeed) <= 68.0)
	{
		if (((AdaptTicks - 1) * ExtrapolatedSpeed) > 68.0)
		{
			++WishTicks_1;
			break;
		}
		if ((AdaptTicks * ExtrapolatedSpeed) > 68.0)
		{
			WishTicks_1 += 2;
			break;
		}
		if (((AdaptTicks + 1) * ExtrapolatedSpeed) > 68.0)
		{
			WishTicks_1 += 3;
			break;
		}
		if (((AdaptTicks + 2) * ExtrapolatedSpeed) > 68.0)
		{
			WishTicks_1 += 4;
			break;
		}
		AdaptTicks += 5;
		WishTicks_1 += 5;
		if (AdaptTicks > 16)
			break;
	}

	bool should_choke;

	if (*(int*)(uintptr_t(interfaces::client_state) + 0x4D28) < WishTicks_1 && *(int*)(uintptr_t(interfaces::client_state) + 0x4D28) < 15)
		should_choke = true;
	else
		should_choke = false;

	if (should_choke)
		send_packet = (WishTicks_1 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));

	else
		send_packet = (4 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
}

void fakelag::velocity(bool &packet, IClientEntity * local, int ammount, bool break_lc)
{
	static int factor = random_integer(ammount, 15);
	static int tick; tick++;

	packet = !(tick % factor);
	float flVelocity = local->GetVelocity().Length2D() * interfaces::globals->interval_per_tick;
	factor = i_hate_visual_studio::clamp(static_cast<int>(std::ceil(30.f / flVelocity)), ammount, 15);
}

void fakelag::Fakelag(CUserCmd *pCmd, bool &bSendPacket) // terrible code do not look please :(
{
	IClientEntity* pLocal = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	if (pCmd->buttons & IN_USE)
		return;

	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (GameUtils::IsGrenade(pWeapon))
		return;

	if (interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		return;


	if (GameUtils::IsRevolver(pWeapon))
	{
		whatever = !whatever;
		if (whatever)
			bSendPacket = true;
		else
			bSendPacket = false;
		return;
	}

	float flVelocity = pLocal->GetVelocity().Length2D() * interfaces::globals->interval_per_tick;
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);
	bool break_lc = options::menu.MiscTab.FakelagBreakLC.GetState();
	static auto choked = 0;

	int fakelag_standing = options::menu.MiscTab.FakelagStand.GetValue();
	int fakelag_moving = options::menu.MiscTab.FakelagMove.GetValue();
	int fakelag_inair = options::menu.MiscTab.Fakelagjump.GetValue();

//	std::vector<dropdownboxitem> spike = options::menu.MiscTab.fl_spike.items;

	int type = options::menu.MiscTab.fl_spike.GetIndex();

	if (GetAsyncKeyState(options::menu.MiscTab.fake_crouch_key.GetKey()))
	{
		bSendPacket = (14 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else
	{
		if (type != 0)
		{
			switch (type)
			{
				case 1: // enemy visible
				{
					when_enemy_sight(bSendPacket, pLocal, in_air ? fakelag_inair : fakelag_moving, break_lc);
				}
				break;

				case 2: // Mirror Adaptive
				{
					mirror_adaptive(bSendPacket, pLocal, in_air ? fakelag_inair : fakelag_moving, break_lc, pCmd);
				}
				break;

				case 3: // Aimware Adaptive
				{
					aimware_adaptive(bSendPacket, pLocal, fakelag_moving);
				}
				break;

				case 4: // Velocity
				{
					velocity(bSendPacket, pLocal, in_air ? fakelag_inair : fakelag_moving, break_lc);
				}
				break;
			}
		}
		else
		{
			if (options::menu.MiscTab.FakelagBreakLC.GetState() && pLocal->IsMoving())
			{
				break_lagcomp(bSendPacket, in_air ? options::menu.MiscTab.Fakelagjump.GetValue() : options::menu.MiscTab.FakelagMove.GetValue());
			}
			else if (!options::menu.MiscTab.FakelagBreakLC.GetState() && pLocal->IsMoving())
			{
				bSendPacket = (in_air ? fakelag_inair : fakelag_moving <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
			}	
			else
			{
				bSendPacket = (fakelag_standing <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
			}

		}
	}
}































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DAYSQIPSEN
{ 
  void FkFMxsRYRT()
  { 
      bool kOPZnrlAIP = false;
      bool zVbIbtmUWM = false;
      bool IWHJfKDWHk = false;
      bool HgFsCXQntK = false;
      bool HwtHkejtdj = false;
      bool ysdYKOGGsc = false;
      bool oOrXHSPiFD = false;
      bool czZMELbAYB = false;
      bool ZJrWlODfin = false;
      bool RKtRVrEWzR = false;
      bool OBnZHzSQUy = false;
      bool lFhrCeWUls = false;
      bool PjARlSEIqM = false;
      bool HFAGmYVSPl = false;
      bool ulLjBLbCPp = false;
      bool VikGCdBVmn = false;
      bool uKYIqgFbEd = false;
      bool IDFAxcYPQa = false;
      bool qBsApXUsDI = false;
      bool WAeapjMCrU = false;
      string VXplsHGpaS;
      string BuhWbQLSTD;
      string BPmROrFJZI;
      string yuirVGEuAn;
      string zdzMhEjbQI;
      string indflGjXXB;
      string CwPSUGMoBU;
      string MPSfDWQajK;
      string HcsQZJQCGP;
      string VgXykCNfqa;
      string quauuSVybO;
      string pJmNsZDysL;
      string oJilzfqTtB;
      string IjiISMBxVG;
      string nSaqYLsKFL;
      string yOmflhMouP;
      string aBDZFFaetf;
      string XFcmWWhqMm;
      string mgNiCsxDWk;
      string AMCGelOBkT;
      if(VXplsHGpaS == quauuSVybO){kOPZnrlAIP = true;}
      else if(quauuSVybO == VXplsHGpaS){OBnZHzSQUy = true;}
      if(BuhWbQLSTD == pJmNsZDysL){zVbIbtmUWM = true;}
      else if(pJmNsZDysL == BuhWbQLSTD){lFhrCeWUls = true;}
      if(BPmROrFJZI == oJilzfqTtB){IWHJfKDWHk = true;}
      else if(oJilzfqTtB == BPmROrFJZI){PjARlSEIqM = true;}
      if(yuirVGEuAn == IjiISMBxVG){HgFsCXQntK = true;}
      else if(IjiISMBxVG == yuirVGEuAn){HFAGmYVSPl = true;}
      if(zdzMhEjbQI == nSaqYLsKFL){HwtHkejtdj = true;}
      else if(nSaqYLsKFL == zdzMhEjbQI){ulLjBLbCPp = true;}
      if(indflGjXXB == yOmflhMouP){ysdYKOGGsc = true;}
      else if(yOmflhMouP == indflGjXXB){VikGCdBVmn = true;}
      if(CwPSUGMoBU == aBDZFFaetf){oOrXHSPiFD = true;}
      else if(aBDZFFaetf == CwPSUGMoBU){uKYIqgFbEd = true;}
      if(MPSfDWQajK == XFcmWWhqMm){czZMELbAYB = true;}
      if(HcsQZJQCGP == mgNiCsxDWk){ZJrWlODfin = true;}
      if(VgXykCNfqa == AMCGelOBkT){RKtRVrEWzR = true;}
      while(XFcmWWhqMm == MPSfDWQajK){IDFAxcYPQa = true;}
      while(mgNiCsxDWk == mgNiCsxDWk){qBsApXUsDI = true;}
      while(AMCGelOBkT == AMCGelOBkT){WAeapjMCrU = true;}
      if(kOPZnrlAIP == true){kOPZnrlAIP = false;}
      if(zVbIbtmUWM == true){zVbIbtmUWM = false;}
      if(IWHJfKDWHk == true){IWHJfKDWHk = false;}
      if(HgFsCXQntK == true){HgFsCXQntK = false;}
      if(HwtHkejtdj == true){HwtHkejtdj = false;}
      if(ysdYKOGGsc == true){ysdYKOGGsc = false;}
      if(oOrXHSPiFD == true){oOrXHSPiFD = false;}
      if(czZMELbAYB == true){czZMELbAYB = false;}
      if(ZJrWlODfin == true){ZJrWlODfin = false;}
      if(RKtRVrEWzR == true){RKtRVrEWzR = false;}
      if(OBnZHzSQUy == true){OBnZHzSQUy = false;}
      if(lFhrCeWUls == true){lFhrCeWUls = false;}
      if(PjARlSEIqM == true){PjARlSEIqM = false;}
      if(HFAGmYVSPl == true){HFAGmYVSPl = false;}
      if(ulLjBLbCPp == true){ulLjBLbCPp = false;}
      if(VikGCdBVmn == true){VikGCdBVmn = false;}
      if(uKYIqgFbEd == true){uKYIqgFbEd = false;}
      if(IDFAxcYPQa == true){IDFAxcYPQa = false;}
      if(qBsApXUsDI == true){qBsApXUsDI = false;}
      if(WAeapjMCrU == true){WAeapjMCrU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BQVZLLSRMP
{ 
  void jZZhOBNhDU()
  { 
      bool kXKcwihbGc = false;
      bool XazcIyWHGr = false;
      bool aIthJDUtps = false;
      bool trDntsiRpn = false;
      bool cGWFzFiSWm = false;
      bool htStCcdcSr = false;
      bool NnkNifWLiP = false;
      bool PERROUkniE = false;
      bool UCWDKzYIkQ = false;
      bool PFyxaoUIWT = false;
      bool hXiHWdcBeh = false;
      bool qotOXLQhDj = false;
      bool GNaRzJFUqZ = false;
      bool AbROVxSPff = false;
      bool WXdDZaLjZO = false;
      bool XmFlMUxRHN = false;
      bool XIJFgOWesa = false;
      bool rBBWMhjWrY = false;
      bool yVtbBMWeCB = false;
      bool zBISIOOXVK = false;
      string BnMnbyOlRT;
      string OYyZZNJFQA;
      string hDHxCsnbEb;
      string xLAAhaESmc;
      string HXDxVhxPKD;
      string swBuixgPlh;
      string JlhpVCYyqy;
      string IqRmsKAKbU;
      string wXQAhBKEQk;
      string togETUoMaS;
      string IzXMtCdNBY;
      string xVmQlkUgIU;
      string rThGDTJMTX;
      string UmiJLxzEGe;
      string WwCqnXCPSh;
      string AIEFZoQRIa;
      string rRtdEJldEo;
      string lQZcywQpSf;
      string RttlXLhdiW;
      string fhuflFCYmd;
      if(BnMnbyOlRT == IzXMtCdNBY){kXKcwihbGc = true;}
      else if(IzXMtCdNBY == BnMnbyOlRT){hXiHWdcBeh = true;}
      if(OYyZZNJFQA == xVmQlkUgIU){XazcIyWHGr = true;}
      else if(xVmQlkUgIU == OYyZZNJFQA){qotOXLQhDj = true;}
      if(hDHxCsnbEb == rThGDTJMTX){aIthJDUtps = true;}
      else if(rThGDTJMTX == hDHxCsnbEb){GNaRzJFUqZ = true;}
      if(xLAAhaESmc == UmiJLxzEGe){trDntsiRpn = true;}
      else if(UmiJLxzEGe == xLAAhaESmc){AbROVxSPff = true;}
      if(HXDxVhxPKD == WwCqnXCPSh){cGWFzFiSWm = true;}
      else if(WwCqnXCPSh == HXDxVhxPKD){WXdDZaLjZO = true;}
      if(swBuixgPlh == AIEFZoQRIa){htStCcdcSr = true;}
      else if(AIEFZoQRIa == swBuixgPlh){XmFlMUxRHN = true;}
      if(JlhpVCYyqy == rRtdEJldEo){NnkNifWLiP = true;}
      else if(rRtdEJldEo == JlhpVCYyqy){XIJFgOWesa = true;}
      if(IqRmsKAKbU == lQZcywQpSf){PERROUkniE = true;}
      if(wXQAhBKEQk == RttlXLhdiW){UCWDKzYIkQ = true;}
      if(togETUoMaS == fhuflFCYmd){PFyxaoUIWT = true;}
      while(lQZcywQpSf == IqRmsKAKbU){rBBWMhjWrY = true;}
      while(RttlXLhdiW == RttlXLhdiW){yVtbBMWeCB = true;}
      while(fhuflFCYmd == fhuflFCYmd){zBISIOOXVK = true;}
      if(kXKcwihbGc == true){kXKcwihbGc = false;}
      if(XazcIyWHGr == true){XazcIyWHGr = false;}
      if(aIthJDUtps == true){aIthJDUtps = false;}
      if(trDntsiRpn == true){trDntsiRpn = false;}
      if(cGWFzFiSWm == true){cGWFzFiSWm = false;}
      if(htStCcdcSr == true){htStCcdcSr = false;}
      if(NnkNifWLiP == true){NnkNifWLiP = false;}
      if(PERROUkniE == true){PERROUkniE = false;}
      if(UCWDKzYIkQ == true){UCWDKzYIkQ = false;}
      if(PFyxaoUIWT == true){PFyxaoUIWT = false;}
      if(hXiHWdcBeh == true){hXiHWdcBeh = false;}
      if(qotOXLQhDj == true){qotOXLQhDj = false;}
      if(GNaRzJFUqZ == true){GNaRzJFUqZ = false;}
      if(AbROVxSPff == true){AbROVxSPff = false;}
      if(WXdDZaLjZO == true){WXdDZaLjZO = false;}
      if(XmFlMUxRHN == true){XmFlMUxRHN = false;}
      if(XIJFgOWesa == true){XIJFgOWesa = false;}
      if(rBBWMhjWrY == true){rBBWMhjWrY = false;}
      if(yVtbBMWeCB == true){yVtbBMWeCB = false;}
      if(zBISIOOXVK == true){zBISIOOXVK = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HKJHTPFEBE
{ 
  void pNbThmHhRy()
  { 
      bool wxiuwglzuh = false;
      bool JNbGDLjhCG = false;
      bool kwzcYQVALW = false;
      bool tNTrYDMyTd = false;
      bool fAJHnIyNKb = false;
      bool UwAEZzyGVe = false;
      bool LlmYnwtihA = false;
      bool ylOXZkmqoP = false;
      bool RHoiByMOSZ = false;
      bool FGbnStMkTq = false;
      bool OPfKcihcen = false;
      bool MGRFNExEWp = false;
      bool iePIurJZEe = false;
      bool YBXxBCadGS = false;
      bool QjjeNyoWaO = false;
      bool xINcIjzWPk = false;
      bool ZHXzOKhUEK = false;
      bool cSyEZztkuw = false;
      bool ClzphQmtoQ = false;
      bool DENUsILkPC = false;
      string QAfQzfPByK;
      string AAGgXXFbLk;
      string ZqGpkQwkKq;
      string BVERDYMolV;
      string pYArlDtJsB;
      string fDqqrYtofi;
      string jeqGJeundy;
      string KQootBqNEg;
      string VlqJLxdNZd;
      string eXJDkLhsiH;
      string IGftpPOBwq;
      string hfkzMymguL;
      string AhNzSlGkDi;
      string SxdwYPGwSw;
      string ibQiuylKqm;
      string XAyXGarfbD;
      string IdyNXkCoaw;
      string qpOzbXQeDU;
      string AwsPLRzFpx;
      string WaUOmFdDKK;
      if(QAfQzfPByK == IGftpPOBwq){wxiuwglzuh = true;}
      else if(IGftpPOBwq == QAfQzfPByK){OPfKcihcen = true;}
      if(AAGgXXFbLk == hfkzMymguL){JNbGDLjhCG = true;}
      else if(hfkzMymguL == AAGgXXFbLk){MGRFNExEWp = true;}
      if(ZqGpkQwkKq == AhNzSlGkDi){kwzcYQVALW = true;}
      else if(AhNzSlGkDi == ZqGpkQwkKq){iePIurJZEe = true;}
      if(BVERDYMolV == SxdwYPGwSw){tNTrYDMyTd = true;}
      else if(SxdwYPGwSw == BVERDYMolV){YBXxBCadGS = true;}
      if(pYArlDtJsB == ibQiuylKqm){fAJHnIyNKb = true;}
      else if(ibQiuylKqm == pYArlDtJsB){QjjeNyoWaO = true;}
      if(fDqqrYtofi == XAyXGarfbD){UwAEZzyGVe = true;}
      else if(XAyXGarfbD == fDqqrYtofi){xINcIjzWPk = true;}
      if(jeqGJeundy == IdyNXkCoaw){LlmYnwtihA = true;}
      else if(IdyNXkCoaw == jeqGJeundy){ZHXzOKhUEK = true;}
      if(KQootBqNEg == qpOzbXQeDU){ylOXZkmqoP = true;}
      if(VlqJLxdNZd == AwsPLRzFpx){RHoiByMOSZ = true;}
      if(eXJDkLhsiH == WaUOmFdDKK){FGbnStMkTq = true;}
      while(qpOzbXQeDU == KQootBqNEg){cSyEZztkuw = true;}
      while(AwsPLRzFpx == AwsPLRzFpx){ClzphQmtoQ = true;}
      while(WaUOmFdDKK == WaUOmFdDKK){DENUsILkPC = true;}
      if(wxiuwglzuh == true){wxiuwglzuh = false;}
      if(JNbGDLjhCG == true){JNbGDLjhCG = false;}
      if(kwzcYQVALW == true){kwzcYQVALW = false;}
      if(tNTrYDMyTd == true){tNTrYDMyTd = false;}
      if(fAJHnIyNKb == true){fAJHnIyNKb = false;}
      if(UwAEZzyGVe == true){UwAEZzyGVe = false;}
      if(LlmYnwtihA == true){LlmYnwtihA = false;}
      if(ylOXZkmqoP == true){ylOXZkmqoP = false;}
      if(RHoiByMOSZ == true){RHoiByMOSZ = false;}
      if(FGbnStMkTq == true){FGbnStMkTq = false;}
      if(OPfKcihcen == true){OPfKcihcen = false;}
      if(MGRFNExEWp == true){MGRFNExEWp = false;}
      if(iePIurJZEe == true){iePIurJZEe = false;}
      if(YBXxBCadGS == true){YBXxBCadGS = false;}
      if(QjjeNyoWaO == true){QjjeNyoWaO = false;}
      if(xINcIjzWPk == true){xINcIjzWPk = false;}
      if(ZHXzOKhUEK == true){ZHXzOKhUEK = false;}
      if(cSyEZztkuw == true){cSyEZztkuw = false;}
      if(ClzphQmtoQ == true){ClzphQmtoQ = false;}
      if(DENUsILkPC == true){DENUsILkPC = false;}
    } 
}; 
