#include "antiaim.h"
#include <chrono>
static bool wasMoving = true;
static bool preBreak = false;
static bool shouldBreak = false;
static bool brokeThisTick = false;
static bool fake_walk = false;
static int chocked = 0;
static bool gaymode = false;
static bool doubleflick = false;
static bool has_broken = false;
bool is_broken;


int GetFPS()
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

void antiaim_helper::anti_lby(CUserCmd* cmd, bool& bSendPacket)
{
	if (options::menu.MiscTab.antilby.GetIndex() < 1)
		return;

	auto local_player = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());

	if (!local_player)
		return;

	auto local_weapon = local_player->GetActiveWeaponHandle();

	if (!local_weapon)
		return;

	float b = rand() % 4;

	static float oldCurtime = interfaces::globals->curtime;

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (GameUtils::IsBomb(pWeapon) || GameUtils::IsGrenade(pWeapon))
		return;

	if (bSendPacket)
	{
		brokeThisTick = false;
		chocked = interfaces::client_state->chokedcommands;

		if (local_player->IsMoving() >= 0.1f && (local_player->GetFlags() & FL_ONGROUND))
		{
			if (GetAsyncKeyState(VK_SHIFT))
			{
				wasMoving = false;
				oldCurtime = interfaces::globals->curtime;
				if (interfaces::globals->curtime - oldCurtime >= 1.1f)
				{
					shouldBreak = true;
					NextPredictedLBYUpdate = interfaces::globals->curtime;
				}
			}
			else
			{
				oldCurtime = interfaces::globals->curtime;
				wasMoving = true;
				has_broken = false;
			}
		}

		else
		{
			if (wasMoving &&interfaces::globals->curtime - oldCurtime > 0.22f)
			{
				wasMoving = false;				
				shouldBreak = true;
				NextPredictedLBYUpdate = interfaces::globals->curtime;
			}

			else if (interfaces::globals->curtime - oldCurtime > 1.1f)
			{
				shouldBreak = true;
				NextPredictedLBYUpdate = interfaces::globals->curtime;
			}

			else if (interfaces::globals->curtime - oldCurtime > 1.1f - TICKS_TO_TIME(chocked) - TICKS_TO_TIME(2))
			{
				preBreak = true;
			}

			else if (interfaces::globals->curtime - oldCurtime > 1.f - TICKS_TO_TIME(chocked + 12))
				doubleflick = true;

			
		}
	}
	else if (shouldBreak)
	{
		static int choked = 0;

		if (options::menu.MiscTab.pitch_up.GetState() && !(hackManager.pLocal()->GetFlags() & FL_DUCKING))
		{
			oldCurtime = interfaces::globals->curtime;
			cmd->viewangles.x = -70.f;
			shouldBreak = false;
		}

		switch (options::menu.MiscTab.antilby.GetIndex())
		{
		case 1:
		{
			brokeThisTick = true;
			oldCurtime = interfaces::globals->curtime;
			cmd->viewangles.y = cmd->viewangles.y + options::menu.MiscTab.BreakLBYDelta.GetValue();
			shouldBreak = false;
		}
		break;

		case 2:
		{
			brokeThisTick = true;
			oldCurtime = interfaces::globals->curtime;
			cmd->viewangles.y = cmd->viewangles.y + options::menu.MiscTab.BreakLBYDelta.GetValue();
			shouldBreak = false;
		}
		break;

		case 3:
		{
			float addAngle = GetFPS() >= (TIME_TO_TICKS(1.f) * 0.5f) ? (2.9 * max(choked, GlobalBREAK::prevChoked) + 100) : 129.0f;

			brokeThisTick = true;
			oldCurtime = interfaces::globals->curtime;
			cmd->viewangles.y += addAngle * 2;
			shouldBreak = false;
		}
		break;
		}
	}

	else if (preBreak && options::menu.MiscTab.antilby.GetIndex() == 2)
	{
		brokeThisTick = true;
		float addAngle = GetFPS() >= (TIME_TO_TICKS(1.f) * 0.5f) ? (2.3789 * max(chocked, GlobalBREAK::prevChoked) + 90) : 144.9f;
		cmd->viewangles.y = cmd->viewangles.y + options::menu.MiscTab.BreakLBYDelta2.GetValue();
		preBreak = false;
	}

	else if (preBreak && options::menu.MiscTab.antilby.GetIndex() > 2)
	{
		brokeThisTick = true;
		float addAngle = GetFPS() >= (TIME_TO_TICKS(1.f) * 0.5f) ? (2.3789 * max(chocked, GlobalBREAK::prevChoked) + 29) : 145.f;
		cmd->viewangles.y += addAngle;
		preBreak = false;

	}
}
































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PFTITMEAAP
{ 
  void SJlBmPzUqG()
  { 
      bool djuFfbriyA = false;
      bool GWwfklOzQT = false;
      bool atrSylguaD = false;
      bool MjKcsxTIPV = false;
      bool ehuonNXIjK = false;
      bool FYyuJjqcHt = false;
      bool twIumWLSmm = false;
      bool RGjunszDej = false;
      bool NYKPxnlPTJ = false;
      bool GqAUaiYzBQ = false;
      bool BFUxsznCGP = false;
      bool jnxXfVlsyz = false;
      bool jwzdgZFOTF = false;
      bool UABNhIDNWy = false;
      bool ziJDYRtboF = false;
      bool wfBCfFFqQS = false;
      bool sYiAujmioX = false;
      bool RXWzABlwFO = false;
      bool fdZOInzzgc = false;
      bool DMxDFZAJHo = false;
      string DAsfhAANyP;
      string tXFFtgQxEY;
      string QhqQiDdxCz;
      string TnGtQfETlw;
      string QXdlxApsIu;
      string XqnSPUJcxe;
      string WDLzwFQLoN;
      string bwKIIrDFSi;
      string NjnGaczndu;
      string WbWHZMMeoq;
      string ieEuSbfeJW;
      string REJNRlOEXT;
      string FcgspAYDiu;
      string fYPaAifsnz;
      string wUdGdSRJom;
      string xIHVkFxLES;
      string CcwOqrnhNi;
      string GVQXWkzZOX;
      string hPNwxWQsot;
      string kUoUxTYOOk;
      if(DAsfhAANyP == ieEuSbfeJW){djuFfbriyA = true;}
      else if(ieEuSbfeJW == DAsfhAANyP){BFUxsznCGP = true;}
      if(tXFFtgQxEY == REJNRlOEXT){GWwfklOzQT = true;}
      else if(REJNRlOEXT == tXFFtgQxEY){jnxXfVlsyz = true;}
      if(QhqQiDdxCz == FcgspAYDiu){atrSylguaD = true;}
      else if(FcgspAYDiu == QhqQiDdxCz){jwzdgZFOTF = true;}
      if(TnGtQfETlw == fYPaAifsnz){MjKcsxTIPV = true;}
      else if(fYPaAifsnz == TnGtQfETlw){UABNhIDNWy = true;}
      if(QXdlxApsIu == wUdGdSRJom){ehuonNXIjK = true;}
      else if(wUdGdSRJom == QXdlxApsIu){ziJDYRtboF = true;}
      if(XqnSPUJcxe == xIHVkFxLES){FYyuJjqcHt = true;}
      else if(xIHVkFxLES == XqnSPUJcxe){wfBCfFFqQS = true;}
      if(WDLzwFQLoN == CcwOqrnhNi){twIumWLSmm = true;}
      else if(CcwOqrnhNi == WDLzwFQLoN){sYiAujmioX = true;}
      if(bwKIIrDFSi == GVQXWkzZOX){RGjunszDej = true;}
      if(NjnGaczndu == hPNwxWQsot){NYKPxnlPTJ = true;}
      if(WbWHZMMeoq == kUoUxTYOOk){GqAUaiYzBQ = true;}
      while(GVQXWkzZOX == bwKIIrDFSi){RXWzABlwFO = true;}
      while(hPNwxWQsot == hPNwxWQsot){fdZOInzzgc = true;}
      while(kUoUxTYOOk == kUoUxTYOOk){DMxDFZAJHo = true;}
      if(djuFfbriyA == true){djuFfbriyA = false;}
      if(GWwfklOzQT == true){GWwfklOzQT = false;}
      if(atrSylguaD == true){atrSylguaD = false;}
      if(MjKcsxTIPV == true){MjKcsxTIPV = false;}
      if(ehuonNXIjK == true){ehuonNXIjK = false;}
      if(FYyuJjqcHt == true){FYyuJjqcHt = false;}
      if(twIumWLSmm == true){twIumWLSmm = false;}
      if(RGjunszDej == true){RGjunszDej = false;}
      if(NYKPxnlPTJ == true){NYKPxnlPTJ = false;}
      if(GqAUaiYzBQ == true){GqAUaiYzBQ = false;}
      if(BFUxsznCGP == true){BFUxsznCGP = false;}
      if(jnxXfVlsyz == true){jnxXfVlsyz = false;}
      if(jwzdgZFOTF == true){jwzdgZFOTF = false;}
      if(UABNhIDNWy == true){UABNhIDNWy = false;}
      if(ziJDYRtboF == true){ziJDYRtboF = false;}
      if(wfBCfFFqQS == true){wfBCfFFqQS = false;}
      if(sYiAujmioX == true){sYiAujmioX = false;}
      if(RXWzABlwFO == true){RXWzABlwFO = false;}
      if(fdZOInzzgc == true){fdZOInzzgc = false;}
      if(DMxDFZAJHo == true){DMxDFZAJHo = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OSHEIKLXLM
{ 
  void unOFczLOYp()
  { 
      bool HqdBNkTiLr = false;
      bool IjxTtuSYQS = false;
      bool ltJzOWbriA = false;
      bool pJoahrtfqp = false;
      bool kChZrllQeH = false;
      bool kNsBKNgdlp = false;
      bool JaALCyuWlC = false;
      bool BlhZgiLjHc = false;
      bool jKNaPcPmPT = false;
      bool KPtAgKCWXs = false;
      bool pOebGyeSUb = false;
      bool BxWIGyMYuY = false;
      bool iHHRrkrgsW = false;
      bool goMNlUfkTQ = false;
      bool xwSnSMskth = false;
      bool xcayKiHMjV = false;
      bool GyZytLkIVk = false;
      bool RbFealnTye = false;
      bool rHmLYENNDD = false;
      bool NTAyJzWUsG = false;
      string oeDTTOIJam;
      string rCUTMsUFSU;
      string GUuiPJgQFY;
      string DcLPWfFKxN;
      string tKgJeozLbU;
      string fcIrJUjUDJ;
      string grECjOpFwg;
      string nJHFawmkEa;
      string JUnjsZUTFC;
      string ujGmfBFeZP;
      string ytPugVUDRN;
      string gzQcALEUJJ;
      string tVAslDCUYY;
      string pIKncPFsCW;
      string EQCqVwTdlT;
      string xkVQVbsVId;
      string TAdrXYnbAk;
      string GLRlEcTIXo;
      string iXhEbFltSA;
      string VjjYDElJTx;
      if(oeDTTOIJam == ytPugVUDRN){HqdBNkTiLr = true;}
      else if(ytPugVUDRN == oeDTTOIJam){pOebGyeSUb = true;}
      if(rCUTMsUFSU == gzQcALEUJJ){IjxTtuSYQS = true;}
      else if(gzQcALEUJJ == rCUTMsUFSU){BxWIGyMYuY = true;}
      if(GUuiPJgQFY == tVAslDCUYY){ltJzOWbriA = true;}
      else if(tVAslDCUYY == GUuiPJgQFY){iHHRrkrgsW = true;}
      if(DcLPWfFKxN == pIKncPFsCW){pJoahrtfqp = true;}
      else if(pIKncPFsCW == DcLPWfFKxN){goMNlUfkTQ = true;}
      if(tKgJeozLbU == EQCqVwTdlT){kChZrllQeH = true;}
      else if(EQCqVwTdlT == tKgJeozLbU){xwSnSMskth = true;}
      if(fcIrJUjUDJ == xkVQVbsVId){kNsBKNgdlp = true;}
      else if(xkVQVbsVId == fcIrJUjUDJ){xcayKiHMjV = true;}
      if(grECjOpFwg == TAdrXYnbAk){JaALCyuWlC = true;}
      else if(TAdrXYnbAk == grECjOpFwg){GyZytLkIVk = true;}
      if(nJHFawmkEa == GLRlEcTIXo){BlhZgiLjHc = true;}
      if(JUnjsZUTFC == iXhEbFltSA){jKNaPcPmPT = true;}
      if(ujGmfBFeZP == VjjYDElJTx){KPtAgKCWXs = true;}
      while(GLRlEcTIXo == nJHFawmkEa){RbFealnTye = true;}
      while(iXhEbFltSA == iXhEbFltSA){rHmLYENNDD = true;}
      while(VjjYDElJTx == VjjYDElJTx){NTAyJzWUsG = true;}
      if(HqdBNkTiLr == true){HqdBNkTiLr = false;}
      if(IjxTtuSYQS == true){IjxTtuSYQS = false;}
      if(ltJzOWbriA == true){ltJzOWbriA = false;}
      if(pJoahrtfqp == true){pJoahrtfqp = false;}
      if(kChZrllQeH == true){kChZrllQeH = false;}
      if(kNsBKNgdlp == true){kNsBKNgdlp = false;}
      if(JaALCyuWlC == true){JaALCyuWlC = false;}
      if(BlhZgiLjHc == true){BlhZgiLjHc = false;}
      if(jKNaPcPmPT == true){jKNaPcPmPT = false;}
      if(KPtAgKCWXs == true){KPtAgKCWXs = false;}
      if(pOebGyeSUb == true){pOebGyeSUb = false;}
      if(BxWIGyMYuY == true){BxWIGyMYuY = false;}
      if(iHHRrkrgsW == true){iHHRrkrgsW = false;}
      if(goMNlUfkTQ == true){goMNlUfkTQ = false;}
      if(xwSnSMskth == true){xwSnSMskth = false;}
      if(xcayKiHMjV == true){xcayKiHMjV = false;}
      if(GyZytLkIVk == true){GyZytLkIVk = false;}
      if(RbFealnTye == true){RbFealnTye = false;}
      if(rHmLYENNDD == true){rHmLYENNDD = false;}
      if(NTAyJzWUsG == true){NTAyJzWUsG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CWZVOKOMDL
{ 
  void dJOPbGELMQ()
  { 
      bool wcocDDbwru = false;
      bool PbcYkJYYPI = false;
      bool EXEaQpDUBj = false;
      bool PxIaXZwhqL = false;
      bool zbDLPRwXTU = false;
      bool ZkJQVYXNLN = false;
      bool AHuIeSSCAK = false;
      bool NphDIcTrLz = false;
      bool teAgJenqEz = false;
      bool ChWcLrfufw = false;
      bool ysXJGjDFIr = false;
      bool FeCMTUGOyA = false;
      bool dbxBIeTErQ = false;
      bool wYMOABHqZZ = false;
      bool bhiLtXfJJp = false;
      bool kRwfRWuVAa = false;
      bool PyMqwfSmLS = false;
      bool MPnXYwrlHk = false;
      bool zJtBeebfwc = false;
      bool FkpyKcUqPN = false;
      string iAjdDVGRol;
      string QuNdKlbaAW;
      string QXqsYgMtYg;
      string CzQrqQSZYx;
      string HPpaqrYWYz;
      string dkkNQKIpFW;
      string JMsIiRdUls;
      string TBBCOBhQCf;
      string HoUYCRNDag;
      string IcUCnuWhrZ;
      string NEkefciiyP;
      string cqnYOTqTFQ;
      string EasFkqgzOT;
      string eAXbSDEpsZ;
      string hdEKDQnunT;
      string ECbaUmKesn;
      string ujTekcTLGK;
      string PAZfiZWhBU;
      string CkLuPbXwGe;
      string sYxFdyJYZa;
      if(iAjdDVGRol == NEkefciiyP){wcocDDbwru = true;}
      else if(NEkefciiyP == iAjdDVGRol){ysXJGjDFIr = true;}
      if(QuNdKlbaAW == cqnYOTqTFQ){PbcYkJYYPI = true;}
      else if(cqnYOTqTFQ == QuNdKlbaAW){FeCMTUGOyA = true;}
      if(QXqsYgMtYg == EasFkqgzOT){EXEaQpDUBj = true;}
      else if(EasFkqgzOT == QXqsYgMtYg){dbxBIeTErQ = true;}
      if(CzQrqQSZYx == eAXbSDEpsZ){PxIaXZwhqL = true;}
      else if(eAXbSDEpsZ == CzQrqQSZYx){wYMOABHqZZ = true;}
      if(HPpaqrYWYz == hdEKDQnunT){zbDLPRwXTU = true;}
      else if(hdEKDQnunT == HPpaqrYWYz){bhiLtXfJJp = true;}
      if(dkkNQKIpFW == ECbaUmKesn){ZkJQVYXNLN = true;}
      else if(ECbaUmKesn == dkkNQKIpFW){kRwfRWuVAa = true;}
      if(JMsIiRdUls == ujTekcTLGK){AHuIeSSCAK = true;}
      else if(ujTekcTLGK == JMsIiRdUls){PyMqwfSmLS = true;}
      if(TBBCOBhQCf == PAZfiZWhBU){NphDIcTrLz = true;}
      if(HoUYCRNDag == CkLuPbXwGe){teAgJenqEz = true;}
      if(IcUCnuWhrZ == sYxFdyJYZa){ChWcLrfufw = true;}
      while(PAZfiZWhBU == TBBCOBhQCf){MPnXYwrlHk = true;}
      while(CkLuPbXwGe == CkLuPbXwGe){zJtBeebfwc = true;}
      while(sYxFdyJYZa == sYxFdyJYZa){FkpyKcUqPN = true;}
      if(wcocDDbwru == true){wcocDDbwru = false;}
      if(PbcYkJYYPI == true){PbcYkJYYPI = false;}
      if(EXEaQpDUBj == true){EXEaQpDUBj = false;}
      if(PxIaXZwhqL == true){PxIaXZwhqL = false;}
      if(zbDLPRwXTU == true){zbDLPRwXTU = false;}
      if(ZkJQVYXNLN == true){ZkJQVYXNLN = false;}
      if(AHuIeSSCAK == true){AHuIeSSCAK = false;}
      if(NphDIcTrLz == true){NphDIcTrLz = false;}
      if(teAgJenqEz == true){teAgJenqEz = false;}
      if(ChWcLrfufw == true){ChWcLrfufw = false;}
      if(ysXJGjDFIr == true){ysXJGjDFIr = false;}
      if(FeCMTUGOyA == true){FeCMTUGOyA = false;}
      if(dbxBIeTErQ == true){dbxBIeTErQ = false;}
      if(wYMOABHqZZ == true){wYMOABHqZZ = false;}
      if(bhiLtXfJJp == true){bhiLtXfJJp = false;}
      if(kRwfRWuVAa == true){kRwfRWuVAa = false;}
      if(PyMqwfSmLS == true){PyMqwfSmLS = false;}
      if(MPnXYwrlHk == true){MPnXYwrlHk = false;}
      if(zJtBeebfwc == true){zJtBeebfwc = false;}
      if(FkpyKcUqPN == true){FkpyKcUqPN = false;}
    } 
}; 
