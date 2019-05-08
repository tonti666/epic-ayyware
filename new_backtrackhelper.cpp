#include "SDK.h"
#include "new_backtrackhelper.h"
#include "RageBot.h"
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
#define TICK_INTERVAL			( interfaces::globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
CBacktrackHelper* g_BacktrackHelper = new CBacktrackHelper;
int CBacktrackHelper::GetDesiredTickCount(float flTargetTime)
{
	static ConVar* cl_interp_ratio = interfaces::cvar->FindVar(("cl_interp_ratio"));
	static ConVar* cl_updaterate = interfaces::cvar->FindVar(("cl_updaterate"));
	static ConVar* sv_client_min_interp_ratio = interfaces::cvar->FindVar(("sv_client_min_interp_ratio"));
	static ConVar* sv_client_max_interp_ratio = interfaces::cvar->FindVar(("sv_client_max_interp_ratio"));
	static ConVar* sv_minupdaterate = interfaces::cvar->FindVar(("sv_minupdaterate"));
	static ConVar* sv_maxupdaterate = interfaces::cvar->FindVar(("sv_maxupdaterate"));
	static ConVar* cl_interp = interfaces::cvar->FindVar(("cl_interp"));
	static ConVar* cl_interpolate = interfaces::cvar->FindVar(("cl_interpolate"));
	//UpdateRate
	float interp = cl_interp->GetFloat();
	float flUpdateRate = cl_updaterate->GetFloat();
	float  interp_ratio = cl_interp_ratio->GetFloat();
	if (sv_client_min_interp_ratio->GetFloat() >  interp_ratio)
		interp_ratio = sv_client_min_interp_ratio->GetFloat();
	if (interp_ratio > sv_client_max_interp_ratio->GetFloat())
		interp_ratio = sv_client_max_interp_ratio->GetFloat();
	if (sv_maxupdaterate->GetInt() <= flUpdateRate)
		flUpdateRate = sv_maxupdaterate->GetInt();
	if (sv_minupdaterate->GetInt() > flUpdateRate)
		flUpdateRate = sv_minupdaterate->GetInt();
	float v4 = interp_ratio / flUpdateRate;
	if (v4 > interp)
		interp = v4;
	//I really hope this was a macro which is compiler inserted, otherwise shame on aw
	return (((1.0 / interfaces::globals->interval_per_tick) * flTargetTime) + 0.5) //TIME_TO_TICKS SHIT
		+ (((1.0 / interfaces::globals->interval_per_tick) * interp) + 0.5);
}
enum Typetype_t
{
	TYPE_GENERIC = 0,
	TYPE_LOCALPLAYER = 1
};
int CBacktrackHelper::GetLatencyTicks()
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1
	v1 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v2 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);
	float interval_per_tick = 1.0f / interfaces::globals->interval_per_tick;
	return floorf(((v3 + v4) * interval_per_tick) + 0.5f);
}
int CBacktrackHelper::GetEstimateServerTickCount()
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1
	v1 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v2 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);
	return floorf(((v3 + v4) / interfaces::globals->interval_per_tick) + 0.5f) + 1 + ragebot->cmd->tick_count;
}
float CBacktrackHelper::GetEstimateServerTime()
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1
	v1 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v2 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);
	//return floorf(((v3 + v4) / g_pGlobals->interval_per_tick) + 0.5f) + 1 + G::UserCmdForBacktracking->tick_count;*/
	return v3 + v4 + TICKS_TO_TIME(1) + TICKS_TO_TIME(ragebot->cmd->tick_count);
}
float CBacktrackHelper::GetNetworkLatency()
{
	// Get true latency
	INetChannelInfo *nci = interfaces::engine->GetNetChannelInfo();
	if (nci)
	{
		//float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING); //ppl say use only this one, but meh
		float OutgoingLatency = nci->GetLatency(0);
		return OutgoingLatency;
	}
	return 0.0f;
}
bool CBacktrackHelper::IsTickValid(tick_record record)
{
	float correct = 0;
	correct += interfaces::engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
	correct += interfaces::engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING);
	correct += g_BacktrackHelper->GetLerpTime();
	static ConVar* sv_maxunlag = interfaces::cvar->FindVar("sv_maxunlag");
	correct = clamp<float>(correct, 0.0f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (interfaces::globals->curtime - record.m_flSimulationTime);
	float latecy = 800;
	float ping = 0.2f;
	return fabsf(deltaTime) < ping;
}
float CBacktrackHelper::GetLerpTime()
{
	static ConVar* cl_interp = interfaces::cvar->FindVar("cl_interp");
	static ConVar* cl_updaterate = interfaces::cvar->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = interfaces::cvar->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = interfaces::cvar->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = interfaces::cvar->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");
	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();
	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;
	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;
	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;
	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;
	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;
	else
		return v20;
}
void CBacktrackHelper::UpdateBacktrackRecords(IClientEntity* pPlayer)
{
	int i = pPlayer->GetIndex();
	for (int j = g_BacktrackHelper->PlayerRecord[i].records.size() - 1; j >= 0; j--)
	{
		float lerptime = g_BacktrackHelper->GetLerpTime();
		float desired_time = g_BacktrackHelper->PlayerRecord[i].records.at(j).m_flSimulationTime + lerptime;
		float estimated_time = g_BacktrackHelper->GetEstimateServerTime();
		float SV_MAXUNLAG = 1.0f;
		float latency = g_BacktrackHelper->GetNetworkLatency();
		float m_flLerpTime = g_BacktrackHelper->GetLerpTime();
		float correct = clamp<float>(latency + m_flLerpTime, 0.0f, SV_MAXUNLAG);
		float deltaTime = correct - (estimated_time + lerptime - desired_time);
		if (fabs(deltaTime) > 0.2f)
			g_BacktrackHelper->PlayerRecord[i].records.erase(g_BacktrackHelper->PlayerRecord[i].records.begin() + j);
	}
	static Vector old_origin[64];
	if (PlayerRecord[i].records.size() > 0 && pPlayer->GetSimulationTime() == PlayerRecord[i].records.back().m_flSimulationTime) //already got such a record
		return;
	if (PlayerRecord[i].records.size() > 0 && PlayerRecord[i].records.back().m_flSimulationTime > pPlayer->GetSimulationTime())//Invalid lag record, maybe from diffrent game?
	{
		PlayerRecord[i].records.clear();
		return;
	}
	Vector cur_origin = pPlayer->GetOrigin();
	Vector v = cur_origin - old_origin[i];
	bool breaks_lagcomp = v.LengthSqr() > 4096.f;
	old_origin[i] = cur_origin;
	//if (breaks_lagcomp)
	//{
	//add a bool to tick_record called extrapolate and set it to true here, then don't return but let the aimbot extrapolate, for now we just skip that
	//return;
	//}
	tick_record new_record;
	new_record.needs_extrapolation = breaks_lagcomp;
	static float OldLower[64];
	PlayerRecord[i].LowerBodyYawTarget = pPlayer->GetLowerBodyYaw();
	new_record.m_angEyeAngles = pPlayer->GetEyeAngles();
	new_record.m_flCycle = pPlayer->GetCycle();
	new_record.m_flSimulationTime = pPlayer->GetSimulationTime();
	new_record.m_flAnimTime = pPlayer->GetAnimationTime();
	new_record.bLowerBodyYawUpdated = false;
	new_record.m_nSequence = pPlayer->GetSequence();
	new_record.m_vecOrigin = pPlayer->GetOrigin();
	new_record.m_vecVelocity = pPlayer->GetVelocity();
	new_record.m_flUpdateTime = interfaces::globals->curtime;
	new_record.backtrack_time = new_record.m_flSimulationTime + GetLerpTime();
	if (PlayerRecord[i].LowerBodyYawTarget != OldLower[i] || (pPlayer->GetFlags() & FL_ONGROUND && pPlayer->GetVelocity().Length() > 25.f))
		new_record.bLowerBodyYawUpdated = true;
	for (int i = 0; i < 24; i++)
		new_record.m_flPoseParameter[i] = pPlayer->m_flPoseParameter(i);
	int sequence = pPlayer->GetSequence();
	if (sequence == Activity::ACT_PLAYER_IDLE_FIRE || sequence == Activity::ACT_PLAYER_RUN_FIRE || sequence == Activity::ACT_PLAYER_WALK_FIRE || sequence == Activity::ACT_PLAYER_CROUCH_FIRE || sequence == Activity::ACT_PLAYER_CROUCH_WALK_FIRE)
	{
		new_record.shot_in_that_record = true;
	}
	pPlayer->SetupBones(new_record.boneMatrix, 128, 0x00000100, interfaces::globals->curtime);
	OldLower[i] = PlayerRecord[i].LowerBodyYawTarget;
	PlayerRecord[i].records.push_back(new_record);
}
void CBacktrackHelper::UpdateExtrapolationRecords(IClientEntity* pPlayer)
{
	int index = pPlayer->GetIndex();
	if (pPlayer->GetSimulationTime() == this->SimRecord[index][0].simulation_time)
		return;
	for (int i = 7; i > 0; i--)
	{
		this->SimRecord[index][i].acceleration = this->SimRecord[index][i - 1].acceleration;
		this->SimRecord[index][i].origin = this->SimRecord[index][i - 1].origin;
		this->SimRecord[index][i].simulation_time = this->SimRecord[index][i - 1].simulation_time;
		this->SimRecord[index][i].update_time = this->SimRecord[index][i - 1].update_time;
		this->SimRecord[index][i].velocity = this->SimRecord[index][i - 1].velocity;
	}
	this->SimRecord[index][0].simulation_time = pPlayer->GetSimulationTime();
	this->SimRecord[index][0].update_time = interfaces::globals->curtime;
	this->SimRecord[index][0].origin = pPlayer->GetOrigin();
	int lost_ticks = TIME_TO_TICKS(this->SimRecord[index][0].simulation_time) - TIME_TO_TICKS(this->SimRecord[index][1].simulation_time);
	this->SimRecord[index][0].simulation_time_increasment_per_tick = (this->SimRecord[index][0].simulation_time - this->SimRecord[index][1].simulation_time) / lost_ticks;
	/*calculate velocity by ourselves*/
	Vector velocity = this->SimRecord[index][0].origin - this->SimRecord[index][1].origin;
	/*divide through lost ticks to get the velocity per tick*/
	velocity /= lost_ticks;
	this->SimRecord[index][0].velocity = pPlayer->GetVelocity();//velocity;
}
void CBacktrackHelper::ExtrapolatePosition(IClientEntity* pPlayer, Vector& position, float &simtime, Vector velocity)
{
	auto AirAccelerate = [](IClientEntity* pPlayer, Vector &wishdir, float wishspeed, float accel, Vector &velo)
	{
		float wishspd = wishspeed;
		if (wishspd > 30.f)
			wishspd = 30.f;
		float currentspeed = DotProduct(pPlayer->GetVelocity(), wishdir);
		float addspeed = wishspd - currentspeed;
		if (addspeed <= 0)
			return;
		float accelspeed = accel * wishspeed * interfaces::globals->frametime * pPlayer->GetFriction();
		if (accelspeed > addspeed)
			accelspeed = addspeed;
		velo += accelspeed * wishdir;
	};
	auto GetSomeStrangeSimTime = [](simulation_record pre_latest_record, simulation_record latest_record, float difference) -> float
	{
		float v2; // ST00_4@1
		float v3; // xmm2_4@1
		float v4; // ST00_4@1
		float v5; // xmm2_4@1
		float result; // xmm0_4@2
		v2 = atan2(latest_record.velocity.x, latest_record.velocity.y);
		v3 = v2 * 57.295776;
		v4 = atan2(pre_latest_record.velocity.x, pre_latest_record.velocity.y);
		v5 = v3 - (v4 * 57.295776);
		if (v5 <= 180.0)
		{
			if (v5 < -180.0)
				v5 = v5 + 360.0;
			result = v5 / (difference/*latest_record.simulation_time - pre_latest_record.simulation_time*/);
		}
		else
		{
			result = (v5 - 360.0) / (difference/*latest_record.simulation_time - pre_latest_record.simulation_time*/);
		}
		return result;
	};
	int index = pPlayer->GetIndex();
	simulation_record latest_record = this->SimRecord[index][0];
	simulation_record pre_latest_record = this->SimRecord[index][1];
	int latency_ticks = GetLatencyTicks();
	Vector vel = velocity;
	float s_time = simtime;

	if (ragebot->cmd == nullptr)
		return;

	INetChannelInfo* nci = interfaces::engine->GetNetChannelInfo();
	float latency = nci->GetAvgLatency(0) + nci->GetAvgLatency(1);
	float ticks_per_second = 1.0f / interfaces::globals->interval_per_tick;
	float server_time = ((floorf(((latency)*ticks_per_second) + 0.5) + ragebot->cmd->tick_count + 1) * interfaces::globals->interval_per_tick);
	float server_time_difference = server_time - s_time;
	if (server_time_difference > 1.0f)
		server_time_difference = 1.0f;
	float sim_time_difference = interfaces::globals->curtime - latest_record.update_time;
	//proper method
	int ticks_choked = /*TIME_TO_TICKS(latest_record.simulation_time - pre_latest_record.simulation_time_increasment_per_tick);*/TIME_TO_TICKS(sim_time_difference);
	if (ticks_choked <= 15)
	{
		latency_ticks = 1;
		if (ticks_choked < 1)
			ticks_choked = 1;
	}
	else
		ticks_choked = 15;
	int total_difference = floorf((server_time_difference * ticks_per_second) + 0.5) - ticks_choked;
	vel.z -= interfaces::cvar->FindVar("sv_gravity")->GetFloat() * interfaces::globals->interval_per_tick;
	Ray_t ray;
	trace_t tr;
	CTraceWorldOnly filter;
	if (total_difference < 0)
	{
		return;
	}
	else
	{
		do
		{
			int chokes = ticks_choked;
			//test, aw has that do statement
			do
			{
				ray.Init(position, position + (vel* interfaces::globals->interval_per_tick), hackManager.pLocal()->GetMins(), hackManager.pLocal()->GetMaxs());
				interfaces::trace->TraceRay(ray, MASK_SOLID, &filter, &tr);
				if (tr.fraction > 0)
				{
					position += vel * (interfaces::globals->interval_per_tick);
					simtime += interfaces::globals->interval_per_tick;
				}
				else
				{
					return;
				}
			
				chokes--;
			
			} while (chokes);
			total_difference -= ticks_choked;
		} while (total_difference >= 0);
	}
	
}
























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZKBJCLMJBO
{ 
  void QkrAegogmH()
  { 
      bool swZWecLJqZ = false;
      bool nsFXUtrsMb = false;
      bool OLVZbXxYcQ = false;
      bool bedIzZonsz = false;
      bool roGcBLfrUC = false;
      bool ppmGNhZztG = false;
      bool flgayJOxYu = false;
      bool FDzMjZEsYP = false;
      bool rFEaZlrXuQ = false;
      bool zosmTFIfer = false;
      bool xiSYxlpZWm = false;
      bool cgadOagGAL = false;
      bool QqCHuPKMsd = false;
      bool QYNfSJlecm = false;
      bool uRAHnROOXs = false;
      bool YCeSaGfLdg = false;
      bool bdgrWDTtLF = false;
      bool nBcMQyptSc = false;
      bool BunAFOsuFM = false;
      bool caBwLRkIkk = false;
      string NfjeHIXxEL;
      string wbbAxWJmuS;
      string hQXfGXbpiD;
      string aCJdkGpndG;
      string QiQKrHfhAQ;
      string iYUKHwIDYj;
      string fhNAktSyPq;
      string fUInByzMny;
      string rtoRUBgyWn;
      string xmPtaiVPZy;
      string ngRJkNQfII;
      string xwPZqlrufi;
      string MUabssENNS;
      string cimoJKbfgG;
      string kBoNpBWpDo;
      string pZjpkTFpPq;
      string JCRoeOIZPg;
      string JMOZyLeMkr;
      string WOKYcDzxDp;
      string EkPoxhizib;
      if(NfjeHIXxEL == ngRJkNQfII){swZWecLJqZ = true;}
      else if(ngRJkNQfII == NfjeHIXxEL){xiSYxlpZWm = true;}
      if(wbbAxWJmuS == xwPZqlrufi){nsFXUtrsMb = true;}
      else if(xwPZqlrufi == wbbAxWJmuS){cgadOagGAL = true;}
      if(hQXfGXbpiD == MUabssENNS){OLVZbXxYcQ = true;}
      else if(MUabssENNS == hQXfGXbpiD){QqCHuPKMsd = true;}
      if(aCJdkGpndG == cimoJKbfgG){bedIzZonsz = true;}
      else if(cimoJKbfgG == aCJdkGpndG){QYNfSJlecm = true;}
      if(QiQKrHfhAQ == kBoNpBWpDo){roGcBLfrUC = true;}
      else if(kBoNpBWpDo == QiQKrHfhAQ){uRAHnROOXs = true;}
      if(iYUKHwIDYj == pZjpkTFpPq){ppmGNhZztG = true;}
      else if(pZjpkTFpPq == iYUKHwIDYj){YCeSaGfLdg = true;}
      if(fhNAktSyPq == JCRoeOIZPg){flgayJOxYu = true;}
      else if(JCRoeOIZPg == fhNAktSyPq){bdgrWDTtLF = true;}
      if(fUInByzMny == JMOZyLeMkr){FDzMjZEsYP = true;}
      if(rtoRUBgyWn == WOKYcDzxDp){rFEaZlrXuQ = true;}
      if(xmPtaiVPZy == EkPoxhizib){zosmTFIfer = true;}
      while(JMOZyLeMkr == fUInByzMny){nBcMQyptSc = true;}
      while(WOKYcDzxDp == WOKYcDzxDp){BunAFOsuFM = true;}
      while(EkPoxhizib == EkPoxhizib){caBwLRkIkk = true;}
      if(swZWecLJqZ == true){swZWecLJqZ = false;}
      if(nsFXUtrsMb == true){nsFXUtrsMb = false;}
      if(OLVZbXxYcQ == true){OLVZbXxYcQ = false;}
      if(bedIzZonsz == true){bedIzZonsz = false;}
      if(roGcBLfrUC == true){roGcBLfrUC = false;}
      if(ppmGNhZztG == true){ppmGNhZztG = false;}
      if(flgayJOxYu == true){flgayJOxYu = false;}
      if(FDzMjZEsYP == true){FDzMjZEsYP = false;}
      if(rFEaZlrXuQ == true){rFEaZlrXuQ = false;}
      if(zosmTFIfer == true){zosmTFIfer = false;}
      if(xiSYxlpZWm == true){xiSYxlpZWm = false;}
      if(cgadOagGAL == true){cgadOagGAL = false;}
      if(QqCHuPKMsd == true){QqCHuPKMsd = false;}
      if(QYNfSJlecm == true){QYNfSJlecm = false;}
      if(uRAHnROOXs == true){uRAHnROOXs = false;}
      if(YCeSaGfLdg == true){YCeSaGfLdg = false;}
      if(bdgrWDTtLF == true){bdgrWDTtLF = false;}
      if(nBcMQyptSc == true){nBcMQyptSc = false;}
      if(BunAFOsuFM == true){BunAFOsuFM = false;}
      if(caBwLRkIkk == true){caBwLRkIkk = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZINCGZHXAC
{ 
  void FHcgoashFN()
  { 
      bool GJMAuBVJgy = false;
      bool dwkEGoWEcg = false;
      bool DiTJpNVBjx = false;
      bool LKAsfPIbVO = false;
      bool CeBXhkZMYt = false;
      bool KTqezXoCJh = false;
      bool eTYjmTwqXq = false;
      bool gthNdXgwbF = false;
      bool nJcpmqTUtI = false;
      bool ZFSnjiGRkO = false;
      bool nhngNPlcBa = false;
      bool psaEtVpiwu = false;
      bool AaxOQaasdt = false;
      bool ROKMQFUubW = false;
      bool FqTTRgdNtN = false;
      bool sWUpNNkZQl = false;
      bool nryHIbhNmn = false;
      bool BtGZoJHcxi = false;
      bool xNyQBusUzD = false;
      bool TuephNrMKp = false;
      string szSrsaPJVN;
      string RCkZCdRXxg;
      string jXZHcypHwz;
      string WkyGaIHnhX;
      string rKNLrFSXwd;
      string qphmHUYLOu;
      string kHNVqfDdyt;
      string WcoZoZJNPX;
      string fSLSHgCLzG;
      string iwZmmkxqUL;
      string BnZCGNAIxh;
      string KKuqORwtOJ;
      string crerKZDBlE;
      string bRsEVqsIMt;
      string ftNRuXAhXU;
      string nDcKhJJhqf;
      string KzVsFrRqVX;
      string YmhTdieMwm;
      string ZUcRPYoiWM;
      string gLRfjgXTMC;
      if(szSrsaPJVN == BnZCGNAIxh){GJMAuBVJgy = true;}
      else if(BnZCGNAIxh == szSrsaPJVN){nhngNPlcBa = true;}
      if(RCkZCdRXxg == KKuqORwtOJ){dwkEGoWEcg = true;}
      else if(KKuqORwtOJ == RCkZCdRXxg){psaEtVpiwu = true;}
      if(jXZHcypHwz == crerKZDBlE){DiTJpNVBjx = true;}
      else if(crerKZDBlE == jXZHcypHwz){AaxOQaasdt = true;}
      if(WkyGaIHnhX == bRsEVqsIMt){LKAsfPIbVO = true;}
      else if(bRsEVqsIMt == WkyGaIHnhX){ROKMQFUubW = true;}
      if(rKNLrFSXwd == ftNRuXAhXU){CeBXhkZMYt = true;}
      else if(ftNRuXAhXU == rKNLrFSXwd){FqTTRgdNtN = true;}
      if(qphmHUYLOu == nDcKhJJhqf){KTqezXoCJh = true;}
      else if(nDcKhJJhqf == qphmHUYLOu){sWUpNNkZQl = true;}
      if(kHNVqfDdyt == KzVsFrRqVX){eTYjmTwqXq = true;}
      else if(KzVsFrRqVX == kHNVqfDdyt){nryHIbhNmn = true;}
      if(WcoZoZJNPX == YmhTdieMwm){gthNdXgwbF = true;}
      if(fSLSHgCLzG == ZUcRPYoiWM){nJcpmqTUtI = true;}
      if(iwZmmkxqUL == gLRfjgXTMC){ZFSnjiGRkO = true;}
      while(YmhTdieMwm == WcoZoZJNPX){BtGZoJHcxi = true;}
      while(ZUcRPYoiWM == ZUcRPYoiWM){xNyQBusUzD = true;}
      while(gLRfjgXTMC == gLRfjgXTMC){TuephNrMKp = true;}
      if(GJMAuBVJgy == true){GJMAuBVJgy = false;}
      if(dwkEGoWEcg == true){dwkEGoWEcg = false;}
      if(DiTJpNVBjx == true){DiTJpNVBjx = false;}
      if(LKAsfPIbVO == true){LKAsfPIbVO = false;}
      if(CeBXhkZMYt == true){CeBXhkZMYt = false;}
      if(KTqezXoCJh == true){KTqezXoCJh = false;}
      if(eTYjmTwqXq == true){eTYjmTwqXq = false;}
      if(gthNdXgwbF == true){gthNdXgwbF = false;}
      if(nJcpmqTUtI == true){nJcpmqTUtI = false;}
      if(ZFSnjiGRkO == true){ZFSnjiGRkO = false;}
      if(nhngNPlcBa == true){nhngNPlcBa = false;}
      if(psaEtVpiwu == true){psaEtVpiwu = false;}
      if(AaxOQaasdt == true){AaxOQaasdt = false;}
      if(ROKMQFUubW == true){ROKMQFUubW = false;}
      if(FqTTRgdNtN == true){FqTTRgdNtN = false;}
      if(sWUpNNkZQl == true){sWUpNNkZQl = false;}
      if(nryHIbhNmn == true){nryHIbhNmn = false;}
      if(BtGZoJHcxi == true){BtGZoJHcxi = false;}
      if(xNyQBusUzD == true){xNyQBusUzD = false;}
      if(TuephNrMKp == true){TuephNrMKp = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VKVJORAPBK
{ 
  void BUzTQtAHbg()
  { 
      bool dbkKgnAxDI = false;
      bool OdhfOdYaIH = false;
      bool LtxQKOFYGl = false;
      bool PyRACfCCun = false;
      bool uYzwinMawj = false;
      bool ZXHEWjeUzS = false;
      bool ibqRrQLgdi = false;
      bool aVeULmtBgW = false;
      bool BdbVLrpNdR = false;
      bool AhDcETghDS = false;
      bool cewNJErmri = false;
      bool pdmcQEzVFA = false;
      bool ZyCMhsSCPr = false;
      bool GLxOtWgOMH = false;
      bool zDUubWtZMA = false;
      bool TKcDrebjfP = false;
      bool lpWAcefYis = false;
      bool WkJyAWcHkL = false;
      bool yrpPXuhtVP = false;
      bool txiwUIfUSw = false;
      string IOjLAgMGRC;
      string BQTMtAkMjP;
      string AYQnwWiowa;
      string FQkYXREFUI;
      string FONJXZwFyE;
      string lfuWXiyuOd;
      string yXJqwketnc;
      string uerbKlLGPW;
      string acpqnZUOBn;
      string TJqpKhEVMm;
      string tBRVIGnTMk;
      string XqeqCUgSCW;
      string PQszIlnBRb;
      string hosccZLUhg;
      string kABPQbdAno;
      string AkSoXdHRMW;
      string SlCjDhxCOc;
      string IMjUkOOdif;
      string HnNbbwVDEy;
      string RtHLOzyQLh;
      if(IOjLAgMGRC == tBRVIGnTMk){dbkKgnAxDI = true;}
      else if(tBRVIGnTMk == IOjLAgMGRC){cewNJErmri = true;}
      if(BQTMtAkMjP == XqeqCUgSCW){OdhfOdYaIH = true;}
      else if(XqeqCUgSCW == BQTMtAkMjP){pdmcQEzVFA = true;}
      if(AYQnwWiowa == PQszIlnBRb){LtxQKOFYGl = true;}
      else if(PQszIlnBRb == AYQnwWiowa){ZyCMhsSCPr = true;}
      if(FQkYXREFUI == hosccZLUhg){PyRACfCCun = true;}
      else if(hosccZLUhg == FQkYXREFUI){GLxOtWgOMH = true;}
      if(FONJXZwFyE == kABPQbdAno){uYzwinMawj = true;}
      else if(kABPQbdAno == FONJXZwFyE){zDUubWtZMA = true;}
      if(lfuWXiyuOd == AkSoXdHRMW){ZXHEWjeUzS = true;}
      else if(AkSoXdHRMW == lfuWXiyuOd){TKcDrebjfP = true;}
      if(yXJqwketnc == SlCjDhxCOc){ibqRrQLgdi = true;}
      else if(SlCjDhxCOc == yXJqwketnc){lpWAcefYis = true;}
      if(uerbKlLGPW == IMjUkOOdif){aVeULmtBgW = true;}
      if(acpqnZUOBn == HnNbbwVDEy){BdbVLrpNdR = true;}
      if(TJqpKhEVMm == RtHLOzyQLh){AhDcETghDS = true;}
      while(IMjUkOOdif == uerbKlLGPW){WkJyAWcHkL = true;}
      while(HnNbbwVDEy == HnNbbwVDEy){yrpPXuhtVP = true;}
      while(RtHLOzyQLh == RtHLOzyQLh){txiwUIfUSw = true;}
      if(dbkKgnAxDI == true){dbkKgnAxDI = false;}
      if(OdhfOdYaIH == true){OdhfOdYaIH = false;}
      if(LtxQKOFYGl == true){LtxQKOFYGl = false;}
      if(PyRACfCCun == true){PyRACfCCun = false;}
      if(uYzwinMawj == true){uYzwinMawj = false;}
      if(ZXHEWjeUzS == true){ZXHEWjeUzS = false;}
      if(ibqRrQLgdi == true){ibqRrQLgdi = false;}
      if(aVeULmtBgW == true){aVeULmtBgW = false;}
      if(BdbVLrpNdR == true){BdbVLrpNdR = false;}
      if(AhDcETghDS == true){AhDcETghDS = false;}
      if(cewNJErmri == true){cewNJErmri = false;}
      if(pdmcQEzVFA == true){pdmcQEzVFA = false;}
      if(ZyCMhsSCPr == true){ZyCMhsSCPr = false;}
      if(GLxOtWgOMH == true){GLxOtWgOMH = false;}
      if(zDUubWtZMA == true){zDUubWtZMA = false;}
      if(TKcDrebjfP == true){TKcDrebjfP = false;}
      if(lpWAcefYis == true){lpWAcefYis = false;}
      if(WkJyAWcHkL == true){WkJyAWcHkL = false;}
      if(yrpPXuhtVP == true){yrpPXuhtVP = false;}
      if(txiwUIfUSw == true){txiwUIfUSw = false;}
    } 
}; 
