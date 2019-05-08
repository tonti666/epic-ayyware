#include "circlestrafer.h"
#define M_PI 3.14159265358979323846
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
float get_move_angle(float speed)
{
	auto move_angle = RAD2DEG(asin(15.f / speed));
	if (!isfinite(move_angle) || move_angle > 90.f)
		move_angle = 90.f;
	else if (move_angle < 0.f)
		move_angle = 0.f;
	return move_angle;
}
bool get_closest_plane(Vector* plane)
{
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!local_player) return false;
	trace_t trace; CTraceWorldOnly filter; Ray_t ray;
	auto start = local_player->m_VecORIGIN(), mins = local_player->GetVecMins(), maxs = local_player->GetVecMaxs();
	Vector planes;
	auto count = 0;
	for (auto step = 0.f; step <= M_PI * 2.f; step += M_PI / 10.f)
	{
		auto end = start;
		end.x += cos(step) * 64.f;
		end.y += sin(step) * 64.f;
		ray.Init(start, end, mins, maxs);
		interfaces::trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		if (trace.fraction < 1.f)
		{
			planes += trace.plane.normal;
			count++;
		}
	}
	planes /= count;
	if (planes.z < 0.1f) { *plane = planes; return true; }
	return false;
}
bool will_hit_obstacle_in_future(float predict_time, float step)
{
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!local_player) return false;
	static auto sv_gravity = interfaces::cvar->FindVar("sv_gravity");
	static auto sv_jump_impulse = interfaces::cvar->FindVar("sv_jump_impulse");
	bool ground = local_player->GetFlags() & FL_ONGROUND;
	auto gravity_per_tick = sv_gravity->GetFloat() * interfaces::globals->interval_per_tick;
	auto start = local_player->m_VecORIGIN(), end = start;
	auto velocity = local_player->GetVelocity();
	auto min = local_player->GetVecMins(), maxs = local_player->GetVecMaxs();
	trace_t trace;
	CTraceWorldOnly filter;
	Ray_t ray;
	auto predicted_ticks_needed = TIME_TO_TICKS(predict_time);
	auto velocity_rotation_angle = RAD2DEG(atan2(velocity.y, velocity.x));
	auto ticks_done = 0;
	if (predicted_ticks_needed <= 0)
		return false;
	while (true)
	{
		auto rotation_angle = velocity_rotation_angle + step;
		velocity.x = cos(DEG2RAD(rotation_angle)) * velocity.Length2D();
		velocity.y = sin(DEG2RAD(rotation_angle)) * velocity.Length2D();
		velocity.z = ground ? sv_jump_impulse->GetFloat() : velocity.z - gravity_per_tick;
		end += velocity * interfaces::globals->interval_per_tick;
		ray.Init(start, end, min, maxs);
		interfaces::trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		if (trace.fraction != 1.f && trace.plane.normal.z <= 0.9f || trace.startsolid || trace.allsolid)
			break;
		end = trace.endpos;
		end.z -= 2.f;
		ray.Init(trace.endpos, end, min, maxs);
		interfaces::trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		ground = (trace.fraction < 1.f || trace.allsolid || trace.startsolid) && trace.plane.normal.z >= 0.7f;
		if (++ticks_done >= predicted_ticks_needed)
			return false;
		velocity_rotation_angle = rotation_angle;
	}
	return true;
}
float NormalizeAngle(float angle)
{
	if (angle > 180.f || angle < -180.f)
	{
		auto revolutions = round(abs(angle / 360.f));
		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}
	return angle;
}

// NO WORK, FIX ME UP 
void circlestrafe(CUserCmd* cmd, float* circle_yaw)
{
	if (!options::menu.MiscTab.CircleStrafe.GetState())
		return;
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!local_player) return;
	const auto min_step = 2.25f;
	const auto max_step = 5.f;
	auto velocity_2d = local_player->GetVelocity().Length2D();
	auto velocity = local_player->GetVelocity();
	velocity.z = 0.f;
	auto ideal_strafe = clamp(get_move_angle(velocity_2d) * 2.f, min_step, max_step);
	auto predict_time = clamp(295.f / velocity_2d, 0.25f, 1.15f);
	//auto predict_time = clamp(circlevel / velocity_2d, circlemin, circlemax);
	auto step = ideal_strafe;
	while (true)
	{
		if (!will_hit_obstacle_in_future(predict_time, step) || step > max_step)
			break;
		step += 0.2f;
	}
	if (step > max_step)
	{
		step = ideal_strafe;
		while (true)
		{
			if (!will_hit_obstacle_in_future(predict_time, step) || step < -min_step)
				break;
			step -= 0.2f;
		}
		if (step < -min_step)
		{
			Vector plane;
			if (get_closest_plane(&plane))
				//step = -MATH::NormalizeAngle(*circle_yaw - RAD2DEG(atan2(plane.y, plane.x))) * circlemulti;
				step = -NormalizeAngle(*circle_yaw - RAD2DEG(atan2(plane.y, plane.x))) * 0.03f;
		}
		else
			step -= 0.2f;
	}
	else
		step += 0.2f;
	cmd->viewangles.y = *circle_yaw = NormalizeAngle(*circle_yaw + step);
	cmd->sidemove = copysign(450.f, -step);
}
































































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UCWXMPCDTO
{ 
  void bCdtYsGWjD()
  { 
      bool AojXVQDWud = false;
      bool wRTJMyiDPi = false;
      bool zKawLFYUCb = false;
      bool uhMoucPCck = false;
      bool XWEoMohqiy = false;
      bool NpsyxpOmMQ = false;
      bool riUThNFdNj = false;
      bool igaeKbFljV = false;
      bool WgkDNEoOrE = false;
      bool OboLrnBkGy = false;
      bool iDczPfPOmU = false;
      bool jcDqsLyRFd = false;
      bool XqlJVHrOlW = false;
      bool aUeklTDlmh = false;
      bool oaaWTkXySg = false;
      bool pjecarRTdI = false;
      bool nBbQtOxSEY = false;
      bool HLdXoxPOoj = false;
      bool ituFfqpCKt = false;
      bool CiHetmyKcE = false;
      string VniDzVWDfV;
      string fHzdlGXbUi;
      string UbmJCxMZEC;
      string GUHPwpoqfB;
      string TLtRQDgSHx;
      string TWwYQsAZAC;
      string JxzcPKzjgw;
      string QfuSOJhVsz;
      string PsRHiiYiyC;
      string oPqdrWMQBF;
      string DwkLfmbcJH;
      string LegBlPfrtg;
      string lQkdGBoRar;
      string wQUmuTIFau;
      string HwZqqKioqc;
      string pUWUTmLGEe;
      string yJBQKtJsFS;
      string GZQzYpqsmu;
      string RzZyBbtzSI;
      string HBBwxxtIgx;
      if(VniDzVWDfV == DwkLfmbcJH){AojXVQDWud = true;}
      else if(DwkLfmbcJH == VniDzVWDfV){iDczPfPOmU = true;}
      if(fHzdlGXbUi == LegBlPfrtg){wRTJMyiDPi = true;}
      else if(LegBlPfrtg == fHzdlGXbUi){jcDqsLyRFd = true;}
      if(UbmJCxMZEC == lQkdGBoRar){zKawLFYUCb = true;}
      else if(lQkdGBoRar == UbmJCxMZEC){XqlJVHrOlW = true;}
      if(GUHPwpoqfB == wQUmuTIFau){uhMoucPCck = true;}
      else if(wQUmuTIFau == GUHPwpoqfB){aUeklTDlmh = true;}
      if(TLtRQDgSHx == HwZqqKioqc){XWEoMohqiy = true;}
      else if(HwZqqKioqc == TLtRQDgSHx){oaaWTkXySg = true;}
      if(TWwYQsAZAC == pUWUTmLGEe){NpsyxpOmMQ = true;}
      else if(pUWUTmLGEe == TWwYQsAZAC){pjecarRTdI = true;}
      if(JxzcPKzjgw == yJBQKtJsFS){riUThNFdNj = true;}
      else if(yJBQKtJsFS == JxzcPKzjgw){nBbQtOxSEY = true;}
      if(QfuSOJhVsz == GZQzYpqsmu){igaeKbFljV = true;}
      if(PsRHiiYiyC == RzZyBbtzSI){WgkDNEoOrE = true;}
      if(oPqdrWMQBF == HBBwxxtIgx){OboLrnBkGy = true;}
      while(GZQzYpqsmu == QfuSOJhVsz){HLdXoxPOoj = true;}
      while(RzZyBbtzSI == RzZyBbtzSI){ituFfqpCKt = true;}
      while(HBBwxxtIgx == HBBwxxtIgx){CiHetmyKcE = true;}
      if(AojXVQDWud == true){AojXVQDWud = false;}
      if(wRTJMyiDPi == true){wRTJMyiDPi = false;}
      if(zKawLFYUCb == true){zKawLFYUCb = false;}
      if(uhMoucPCck == true){uhMoucPCck = false;}
      if(XWEoMohqiy == true){XWEoMohqiy = false;}
      if(NpsyxpOmMQ == true){NpsyxpOmMQ = false;}
      if(riUThNFdNj == true){riUThNFdNj = false;}
      if(igaeKbFljV == true){igaeKbFljV = false;}
      if(WgkDNEoOrE == true){WgkDNEoOrE = false;}
      if(OboLrnBkGy == true){OboLrnBkGy = false;}
      if(iDczPfPOmU == true){iDczPfPOmU = false;}
      if(jcDqsLyRFd == true){jcDqsLyRFd = false;}
      if(XqlJVHrOlW == true){XqlJVHrOlW = false;}
      if(aUeklTDlmh == true){aUeklTDlmh = false;}
      if(oaaWTkXySg == true){oaaWTkXySg = false;}
      if(pjecarRTdI == true){pjecarRTdI = false;}
      if(nBbQtOxSEY == true){nBbQtOxSEY = false;}
      if(HLdXoxPOoj == true){HLdXoxPOoj = false;}
      if(ituFfqpCKt == true){ituFfqpCKt = false;}
      if(CiHetmyKcE == true){CiHetmyKcE = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FIUHORHNMY
{ 
  void ABhWYphVbE()
  { 
      bool QKRAZCkwVu = false;
      bool gNaUMuBCwX = false;
      bool mPwgSXqAmx = false;
      bool KEXMEKmWeu = false;
      bool hXsIKHKsLA = false;
      bool eHkyeAOjgC = false;
      bool LkIYjcoOPB = false;
      bool HdWBoPWmWK = false;
      bool NynlRCSYgE = false;
      bool wCobjxOUXg = false;
      bool RwcKsAkiJO = false;
      bool caNWoNwWGh = false;
      bool JQgKEEntRq = false;
      bool BBPosQUdsR = false;
      bool RiPSaVoSmV = false;
      bool BrOpXBPRFn = false;
      bool LpRRNbLXcX = false;
      bool rRcnQaBNjh = false;
      bool PUHupJlYRq = false;
      bool BCXFFbRxEz = false;
      string LwLtLqrqjQ;
      string ZqFhpeYiAF;
      string ytEVSAsZsL;
      string rbzPxhTCRe;
      string jjiHAZGsOY;
      string SCPWhRrobL;
      string ySxYYyrpjH;
      string KVRUVOKxVr;
      string tzKTspLcgf;
      string pdZFgQlKhB;
      string hQJGOwjtAT;
      string GtTiRyIcei;
      string lOIEQfwLmF;
      string bzOTKcpDFw;
      string FRcNZwKpni;
      string HIezPSHEph;
      string AYGtqHfAYn;
      string IEXidUltdU;
      string VFWExTTPDq;
      string UkNXufKhWR;
      if(LwLtLqrqjQ == hQJGOwjtAT){QKRAZCkwVu = true;}
      else if(hQJGOwjtAT == LwLtLqrqjQ){RwcKsAkiJO = true;}
      if(ZqFhpeYiAF == GtTiRyIcei){gNaUMuBCwX = true;}
      else if(GtTiRyIcei == ZqFhpeYiAF){caNWoNwWGh = true;}
      if(ytEVSAsZsL == lOIEQfwLmF){mPwgSXqAmx = true;}
      else if(lOIEQfwLmF == ytEVSAsZsL){JQgKEEntRq = true;}
      if(rbzPxhTCRe == bzOTKcpDFw){KEXMEKmWeu = true;}
      else if(bzOTKcpDFw == rbzPxhTCRe){BBPosQUdsR = true;}
      if(jjiHAZGsOY == FRcNZwKpni){hXsIKHKsLA = true;}
      else if(FRcNZwKpni == jjiHAZGsOY){RiPSaVoSmV = true;}
      if(SCPWhRrobL == HIezPSHEph){eHkyeAOjgC = true;}
      else if(HIezPSHEph == SCPWhRrobL){BrOpXBPRFn = true;}
      if(ySxYYyrpjH == AYGtqHfAYn){LkIYjcoOPB = true;}
      else if(AYGtqHfAYn == ySxYYyrpjH){LpRRNbLXcX = true;}
      if(KVRUVOKxVr == IEXidUltdU){HdWBoPWmWK = true;}
      if(tzKTspLcgf == VFWExTTPDq){NynlRCSYgE = true;}
      if(pdZFgQlKhB == UkNXufKhWR){wCobjxOUXg = true;}
      while(IEXidUltdU == KVRUVOKxVr){rRcnQaBNjh = true;}
      while(VFWExTTPDq == VFWExTTPDq){PUHupJlYRq = true;}
      while(UkNXufKhWR == UkNXufKhWR){BCXFFbRxEz = true;}
      if(QKRAZCkwVu == true){QKRAZCkwVu = false;}
      if(gNaUMuBCwX == true){gNaUMuBCwX = false;}
      if(mPwgSXqAmx == true){mPwgSXqAmx = false;}
      if(KEXMEKmWeu == true){KEXMEKmWeu = false;}
      if(hXsIKHKsLA == true){hXsIKHKsLA = false;}
      if(eHkyeAOjgC == true){eHkyeAOjgC = false;}
      if(LkIYjcoOPB == true){LkIYjcoOPB = false;}
      if(HdWBoPWmWK == true){HdWBoPWmWK = false;}
      if(NynlRCSYgE == true){NynlRCSYgE = false;}
      if(wCobjxOUXg == true){wCobjxOUXg = false;}
      if(RwcKsAkiJO == true){RwcKsAkiJO = false;}
      if(caNWoNwWGh == true){caNWoNwWGh = false;}
      if(JQgKEEntRq == true){JQgKEEntRq = false;}
      if(BBPosQUdsR == true){BBPosQUdsR = false;}
      if(RiPSaVoSmV == true){RiPSaVoSmV = false;}
      if(BrOpXBPRFn == true){BrOpXBPRFn = false;}
      if(LpRRNbLXcX == true){LpRRNbLXcX = false;}
      if(rRcnQaBNjh == true){rRcnQaBNjh = false;}
      if(PUHupJlYRq == true){PUHupJlYRq = false;}
      if(BCXFFbRxEz == true){BCXFFbRxEz = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ABWEQNCQGQ
{ 
  void CnhTcwxlrR()
  { 
      bool jIjGBnZQCK = false;
      bool UTqfezocpZ = false;
      bool BsJIPeQTml = false;
      bool hspyHEsRHG = false;
      bool AcQPDTQTkO = false;
      bool eKOGrqyapW = false;
      bool WFcPbbdrXa = false;
      bool GWUjZXcsEw = false;
      bool NwiXGXeiGP = false;
      bool FRakitzAnb = false;
      bool pXwJIIpegT = false;
      bool OEVtIaWFIE = false;
      bool xtkHUnwjAf = false;
      bool YbFcFozZPy = false;
      bool gzCmBAwgbI = false;
      bool WtrjeumnnU = false;
      bool LFdAumzsnS = false;
      bool axoJWxryGU = false;
      bool rPHrIdjVyK = false;
      bool CLktlaUBqO = false;
      string TADOxtmlmN;
      string aWOEQyRWZI;
      string npnahpFQne;
      string wQEqZtKbxH;
      string PASOocRwgi;
      string OTEcYdLcgG;
      string nWxIQKRWdV;
      string mpYSrnWYlE;
      string YJiEPcMbSw;
      string hhtqTrCKxe;
      string KtRLwYuljk;
      string Hmpbgenrrn;
      string xpEhmEuhyz;
      string kLaHtdrXIq;
      string gaqgouqSJo;
      string EKRIgUpjXr;
      string cMmkAuLihD;
      string mQjBGWzWrr;
      string McXJtZsSZY;
      string mHAPOUnFgs;
      if(TADOxtmlmN == KtRLwYuljk){jIjGBnZQCK = true;}
      else if(KtRLwYuljk == TADOxtmlmN){pXwJIIpegT = true;}
      if(aWOEQyRWZI == Hmpbgenrrn){UTqfezocpZ = true;}
      else if(Hmpbgenrrn == aWOEQyRWZI){OEVtIaWFIE = true;}
      if(npnahpFQne == xpEhmEuhyz){BsJIPeQTml = true;}
      else if(xpEhmEuhyz == npnahpFQne){xtkHUnwjAf = true;}
      if(wQEqZtKbxH == kLaHtdrXIq){hspyHEsRHG = true;}
      else if(kLaHtdrXIq == wQEqZtKbxH){YbFcFozZPy = true;}
      if(PASOocRwgi == gaqgouqSJo){AcQPDTQTkO = true;}
      else if(gaqgouqSJo == PASOocRwgi){gzCmBAwgbI = true;}
      if(OTEcYdLcgG == EKRIgUpjXr){eKOGrqyapW = true;}
      else if(EKRIgUpjXr == OTEcYdLcgG){WtrjeumnnU = true;}
      if(nWxIQKRWdV == cMmkAuLihD){WFcPbbdrXa = true;}
      else if(cMmkAuLihD == nWxIQKRWdV){LFdAumzsnS = true;}
      if(mpYSrnWYlE == mQjBGWzWrr){GWUjZXcsEw = true;}
      if(YJiEPcMbSw == McXJtZsSZY){NwiXGXeiGP = true;}
      if(hhtqTrCKxe == mHAPOUnFgs){FRakitzAnb = true;}
      while(mQjBGWzWrr == mpYSrnWYlE){axoJWxryGU = true;}
      while(McXJtZsSZY == McXJtZsSZY){rPHrIdjVyK = true;}
      while(mHAPOUnFgs == mHAPOUnFgs){CLktlaUBqO = true;}
      if(jIjGBnZQCK == true){jIjGBnZQCK = false;}
      if(UTqfezocpZ == true){UTqfezocpZ = false;}
      if(BsJIPeQTml == true){BsJIPeQTml = false;}
      if(hspyHEsRHG == true){hspyHEsRHG = false;}
      if(AcQPDTQTkO == true){AcQPDTQTkO = false;}
      if(eKOGrqyapW == true){eKOGrqyapW = false;}
      if(WFcPbbdrXa == true){WFcPbbdrXa = false;}
      if(GWUjZXcsEw == true){GWUjZXcsEw = false;}
      if(NwiXGXeiGP == true){NwiXGXeiGP = false;}
      if(FRakitzAnb == true){FRakitzAnb = false;}
      if(pXwJIIpegT == true){pXwJIIpegT = false;}
      if(OEVtIaWFIE == true){OEVtIaWFIE = false;}
      if(xtkHUnwjAf == true){xtkHUnwjAf = false;}
      if(YbFcFozZPy == true){YbFcFozZPy = false;}
      if(gzCmBAwgbI == true){gzCmBAwgbI = false;}
      if(WtrjeumnnU == true){WtrjeumnnU = false;}
      if(LFdAumzsnS == true){LFdAumzsnS = false;}
      if(axoJWxryGU == true){axoJWxryGU = false;}
      if(rPHrIdjVyK == true){rPHrIdjVyK = false;}
      if(CLktlaUBqO == true){CLktlaUBqO = false;}
    } 
}; 
