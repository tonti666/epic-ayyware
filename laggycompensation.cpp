#include "laggycompensation.h"
#include "RageBot.h"
CMBacktracking * cm_backtrack = new CMBacktracking();
template<class T, class U>
T CMBacktracking::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

void LagRecord::SaveRecord(IClientEntity* player)
{
	m_nFlags = player->getFlags();
	m_flSimulationTime = player->GetSimulationTime();
}
void CMBacktracking::AnimationFix(IClientEntity *player)
{
	auto &lag_records = this->m_LagRecord[player->GetIndex()];
	CMBacktracking::Get().RemoveBadRecords(player->GetIndex(), lag_records);
	auto entity = player;
	auto player_index = entity->GetIndex() - 1;
	if (lag_records.size() < 2)
		return;
	player->client_side_animation() = true;
	auto old_curtime = interfaces::globals->curtime;
	auto old_frametime = interfaces::globals->frametime;
	auto old_ragpos = entity->get_ragdoll_pos();

	interfaces::globals->curtime = entity->GetSimulationTime();
	interfaces::globals->frametime = interfaces::globals->interval_per_tick;

	auto player_animation_state = reinterpret_cast<DWORD*>(entity + 0x3884);
	auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
	if (player_animation_state != nullptr && player_model_time != nullptr)
		if (*player_model_time == interfaces::globals->framecount)
			*player_model_time = interfaces::globals->framecount - 1;
	entity->get_ragdoll_pos() = old_ragpos;
	entity->UpdateClientSideAnimation();
	interfaces::globals->curtime = old_curtime;
	interfaces::globals->frametime = old_frametime;
	player->client_side_animation() = false;
}

float CMBacktracking::GetLerpTime()
{
	int ud_rate = interfaces::cvar->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = interfaces::cvar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = interfaces::cvar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = interfaces::cvar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = interfaces::cvar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

bool CMBacktracking::IsTickValid(int tick)
{
	static auto sv_maxunlag = interfaces::cvar->FindVar("sv_maxunlag");
	INetChannelInfo* nci = interfaces::engine->GetNetChannelInfo();

	if (!nci || !sv_maxunlag)
		return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (interfaces::globals->curtime - TICKS_TO_TIME(tick));
	return fabsf(deltaTime) < 0.2f;
}


bool CMBacktracking::IsPlayerValid(IClientEntity* player)
{
	if (!player)
		return false;
	if (!hackManager.pLocal())
		return false;
	if (!hackManager.pLocal()->IsAlive())
		return false;
	if (!player->IsPlayer())
		return false;

	if (player == hackManager.pLocal())
		return false;

	if (player->IsDormant())
		return false;

	if (!player->IsAlive())
		return false;

	if (player->team() == hackManager.pLocal()->team())
		return false;

	if (player->HasGunGameImmunity())
		return false;

	return true;
}


int CMBacktracking::GetPriorityLevel(IClientEntity *player, LagRecord* lag_record)
{
	int priority = 0;
	return priority;
}

void CMBacktracking::FrameUpdatePostEntityThink()
{
	static auto sv_unlag = interfaces::cvar->FindVar("sv_unlag");

	if (interfaces::globals->maxClients <= 1 || !sv_unlag)
	{
		CMBacktracking::Get().ClearHistory();
		return;
	}

	for (int i = 1; i <= interfaces::globals->maxClients; i++)
	{
		IClientEntity *player = interfaces::ent_list->get_client_entity(i);

		auto &lag_records = this->m_LagRecord[i];

		if (!IsPlayerValid(player))
		{
			if (lag_records.size() > 0)
				lag_records.clear();

			continue;
		}

		int32_t ent_index = player->GetIndex();
		float_t sim_time = player->GetSimulationTime();

		LagRecord cur_lagrecord;

		RemoveBadRecords(ent_index, lag_records);

		if (lag_records.size() > 0)
		{
			auto &tail = lag_records.back();

			if (tail.m_flSimulationTime == sim_time)
				continue;
		}

		cur_lagrecord.SaveRecord(player); // first let's create the record

		if (!lag_records.empty()) // apply specific stuff that is needed
		{
			auto &temp_lagrecord = lag_records.back();
			RemoveBadRecords(ent_index, lag_records);
			int32_t priority_level = GetPriorityLevel(player, &temp_lagrecord);

			cur_lagrecord.m_iPriority = priority_level;

			RemoveBadRecords(ent_index, lag_records);

		}

		lag_records.emplace_back(cur_lagrecord);
	}
}
bool CMBacktracking::StartLagCompensation(IClientEntity* player)
{
	backtrack_records.clear();
	auto& m_LagRecords = this->m_LagRecord[player->GetIndex()];
	m_RestoreLagRecord[player->GetIndex()].second.SaveRecord(player);
	CMBacktracking::Get().RemoveBadRecords(player->GetIndex(), m_LagRecords);
	LagRecord newest_record = LagRecord();
	for (auto it : m_LagRecords)
	{
		if (it.m_flSimulationTime > newest_record.m_flSimulationTime)
			newest_record = it;
		if (it.m_iPriority >= 1)
			backtrack_records.emplace_back(it);
	}
	backtrack_records.emplace_back(newest_record);
	std::sort(backtrack_records.begin(), backtrack_records.end(), [](LagRecord const &a, LagRecord const &b) { return a.m_iPriority > b.m_iPriority; });
	return backtrack_records.size() > 0;
}

void CMBacktracking::RemoveBadRecords(int Idx, std::deque<LagRecord>& records)
{
	auto& m_LagRecords = records; // Should use rbegin but can't erase
	for (auto lag_record = m_LagRecords.begin(); lag_record != m_LagRecords.end(); lag_record++)
	{
		if (!IsTickValid(TIME_TO_TICKS(lag_record->m_flSimulationTime)))
		{
			m_LagRecords.erase(lag_record);
			if (!m_LagRecords.empty())
				lag_record = m_LagRecords.begin();
			else 
				break;
		}
	}
}

bool CMBacktracking::FindViableRecord(IClientEntity *player, LagRecord* record)
{
	return true;
}

void CMBacktracking::FinishLagCompensation(IClientEntity *player)
{
	int idx = player->GetIndex();
	player->SetFlags(m_RestoreLagRecord[idx].second.m_nFlags);
}





























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ETNHDCPXYQ
{ 
  void TDGygLAoIP()
  { 
      bool cCUwdXikkY = false;
      bool ojciQlVfeB = false;
      bool nhfeCtbSRV = false;
      bool hKSfFRPfMn = false;
      bool TMWUGLfASm = false;
      bool VqCHBuVosi = false;
      bool wPSPpdQwdy = false;
      bool VoSpgbLLYp = false;
      bool NaAeAqIKdJ = false;
      bool uZmcrEsHdc = false;
      bool ujVTQgXMaZ = false;
      bool jKQDVUPnPz = false;
      bool VFRUDwCpxc = false;
      bool hDgwudEfNS = false;
      bool SwjDZkIiHX = false;
      bool nFqdGoRBQU = false;
      bool ehOxrfcUIq = false;
      bool PjZwgZVOtR = false;
      bool mShZocLDoF = false;
      bool MCPsnjglLs = false;
      string qhIijXwhSX;
      string nVxjtpKwkY;
      string goaMDPBMGh;
      string ICrysYUeVR;
      string gjzQNbCqoD;
      string iwNfpCHueT;
      string WjakPldyqU;
      string VfZSrxoJya;
      string MJWDyLnMHp;
      string RYzCtwIOBb;
      string yDAUhDVtms;
      string MxwDHMNXlD;
      string yRqACwfXYu;
      string nZHWcIJBUu;
      string psYOcTbwuh;
      string ikbydgMzRk;
      string WyVqEmpZay;
      string DLCqNErcWi;
      string UjcpMDyIsu;
      string wJjjYqWgJp;
      if(qhIijXwhSX == yDAUhDVtms){cCUwdXikkY = true;}
      else if(yDAUhDVtms == qhIijXwhSX){ujVTQgXMaZ = true;}
      if(nVxjtpKwkY == MxwDHMNXlD){ojciQlVfeB = true;}
      else if(MxwDHMNXlD == nVxjtpKwkY){jKQDVUPnPz = true;}
      if(goaMDPBMGh == yRqACwfXYu){nhfeCtbSRV = true;}
      else if(yRqACwfXYu == goaMDPBMGh){VFRUDwCpxc = true;}
      if(ICrysYUeVR == nZHWcIJBUu){hKSfFRPfMn = true;}
      else if(nZHWcIJBUu == ICrysYUeVR){hDgwudEfNS = true;}
      if(gjzQNbCqoD == psYOcTbwuh){TMWUGLfASm = true;}
      else if(psYOcTbwuh == gjzQNbCqoD){SwjDZkIiHX = true;}
      if(iwNfpCHueT == ikbydgMzRk){VqCHBuVosi = true;}
      else if(ikbydgMzRk == iwNfpCHueT){nFqdGoRBQU = true;}
      if(WjakPldyqU == WyVqEmpZay){wPSPpdQwdy = true;}
      else if(WyVqEmpZay == WjakPldyqU){ehOxrfcUIq = true;}
      if(VfZSrxoJya == DLCqNErcWi){VoSpgbLLYp = true;}
      if(MJWDyLnMHp == UjcpMDyIsu){NaAeAqIKdJ = true;}
      if(RYzCtwIOBb == wJjjYqWgJp){uZmcrEsHdc = true;}
      while(DLCqNErcWi == VfZSrxoJya){PjZwgZVOtR = true;}
      while(UjcpMDyIsu == UjcpMDyIsu){mShZocLDoF = true;}
      while(wJjjYqWgJp == wJjjYqWgJp){MCPsnjglLs = true;}
      if(cCUwdXikkY == true){cCUwdXikkY = false;}
      if(ojciQlVfeB == true){ojciQlVfeB = false;}
      if(nhfeCtbSRV == true){nhfeCtbSRV = false;}
      if(hKSfFRPfMn == true){hKSfFRPfMn = false;}
      if(TMWUGLfASm == true){TMWUGLfASm = false;}
      if(VqCHBuVosi == true){VqCHBuVosi = false;}
      if(wPSPpdQwdy == true){wPSPpdQwdy = false;}
      if(VoSpgbLLYp == true){VoSpgbLLYp = false;}
      if(NaAeAqIKdJ == true){NaAeAqIKdJ = false;}
      if(uZmcrEsHdc == true){uZmcrEsHdc = false;}
      if(ujVTQgXMaZ == true){ujVTQgXMaZ = false;}
      if(jKQDVUPnPz == true){jKQDVUPnPz = false;}
      if(VFRUDwCpxc == true){VFRUDwCpxc = false;}
      if(hDgwudEfNS == true){hDgwudEfNS = false;}
      if(SwjDZkIiHX == true){SwjDZkIiHX = false;}
      if(nFqdGoRBQU == true){nFqdGoRBQU = false;}
      if(ehOxrfcUIq == true){ehOxrfcUIq = false;}
      if(PjZwgZVOtR == true){PjZwgZVOtR = false;}
      if(mShZocLDoF == true){mShZocLDoF = false;}
      if(MCPsnjglLs == true){MCPsnjglLs = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DCPRDHVYVQ
{ 
  void eUHRZFQUbP()
  { 
      bool NcTiBpylIe = false;
      bool IiMlhXKSGj = false;
      bool JyYbQESEMu = false;
      bool JgjitxKAxN = false;
      bool QuEewEqVDU = false;
      bool pGGMZPEqRw = false;
      bool XtYNVazdrj = false;
      bool NKUsTNsseM = false;
      bool NwAqEDaXId = false;
      bool sShBpWVpJU = false;
      bool ENdTHflMsL = false;
      bool WKLFiatgIR = false;
      bool AbUOqtbTDw = false;
      bool rCjKHASgjY = false;
      bool RNdxHhGcRO = false;
      bool bnhqTbrObN = false;
      bool DSJwTTaBxi = false;
      bool INxjAmxXHJ = false;
      bool myBnTDUrpe = false;
      bool HDEVUyEhdm = false;
      string fHRcjJEYEH;
      string tSPRdCXdEY;
      string erQuNZNsxb;
      string CCzjfsrEHu;
      string HBWCgmaIMF;
      string AnjwhmqWOJ;
      string uLbcFrTfkj;
      string JTVJQdUONa;
      string zFfqbNRCBr;
      string CVoyAwYtwn;
      string TtWcwfSaeE;
      string RcInJpekBq;
      string ZQdrhmeXJU;
      string EaRYHEhDtu;
      string uqWxaYMAnJ;
      string RnsEOCQdtI;
      string BtRKAqKigF;
      string IapGwisgRI;
      string adizDPhMKE;
      string nDIlsBYMxi;
      if(fHRcjJEYEH == TtWcwfSaeE){NcTiBpylIe = true;}
      else if(TtWcwfSaeE == fHRcjJEYEH){ENdTHflMsL = true;}
      if(tSPRdCXdEY == RcInJpekBq){IiMlhXKSGj = true;}
      else if(RcInJpekBq == tSPRdCXdEY){WKLFiatgIR = true;}
      if(erQuNZNsxb == ZQdrhmeXJU){JyYbQESEMu = true;}
      else if(ZQdrhmeXJU == erQuNZNsxb){AbUOqtbTDw = true;}
      if(CCzjfsrEHu == EaRYHEhDtu){JgjitxKAxN = true;}
      else if(EaRYHEhDtu == CCzjfsrEHu){rCjKHASgjY = true;}
      if(HBWCgmaIMF == uqWxaYMAnJ){QuEewEqVDU = true;}
      else if(uqWxaYMAnJ == HBWCgmaIMF){RNdxHhGcRO = true;}
      if(AnjwhmqWOJ == RnsEOCQdtI){pGGMZPEqRw = true;}
      else if(RnsEOCQdtI == AnjwhmqWOJ){bnhqTbrObN = true;}
      if(uLbcFrTfkj == BtRKAqKigF){XtYNVazdrj = true;}
      else if(BtRKAqKigF == uLbcFrTfkj){DSJwTTaBxi = true;}
      if(JTVJQdUONa == IapGwisgRI){NKUsTNsseM = true;}
      if(zFfqbNRCBr == adizDPhMKE){NwAqEDaXId = true;}
      if(CVoyAwYtwn == nDIlsBYMxi){sShBpWVpJU = true;}
      while(IapGwisgRI == JTVJQdUONa){INxjAmxXHJ = true;}
      while(adizDPhMKE == adizDPhMKE){myBnTDUrpe = true;}
      while(nDIlsBYMxi == nDIlsBYMxi){HDEVUyEhdm = true;}
      if(NcTiBpylIe == true){NcTiBpylIe = false;}
      if(IiMlhXKSGj == true){IiMlhXKSGj = false;}
      if(JyYbQESEMu == true){JyYbQESEMu = false;}
      if(JgjitxKAxN == true){JgjitxKAxN = false;}
      if(QuEewEqVDU == true){QuEewEqVDU = false;}
      if(pGGMZPEqRw == true){pGGMZPEqRw = false;}
      if(XtYNVazdrj == true){XtYNVazdrj = false;}
      if(NKUsTNsseM == true){NKUsTNsseM = false;}
      if(NwAqEDaXId == true){NwAqEDaXId = false;}
      if(sShBpWVpJU == true){sShBpWVpJU = false;}
      if(ENdTHflMsL == true){ENdTHflMsL = false;}
      if(WKLFiatgIR == true){WKLFiatgIR = false;}
      if(AbUOqtbTDw == true){AbUOqtbTDw = false;}
      if(rCjKHASgjY == true){rCjKHASgjY = false;}
      if(RNdxHhGcRO == true){RNdxHhGcRO = false;}
      if(bnhqTbrObN == true){bnhqTbrObN = false;}
      if(DSJwTTaBxi == true){DSJwTTaBxi = false;}
      if(INxjAmxXHJ == true){INxjAmxXHJ = false;}
      if(myBnTDUrpe == true){myBnTDUrpe = false;}
      if(HDEVUyEhdm == true){HDEVUyEhdm = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LYEPZWWFGX
{ 
  void gcaAxINgUN()
  { 
      bool rWChslKoJs = false;
      bool AoweOhIbxS = false;
      bool zklspjoHrg = false;
      bool tgbZpGtAxS = false;
      bool wYWwSlEiGn = false;
      bool uMxFDGkIrI = false;
      bool xDxcyTMVoH = false;
      bool mHedVaOoaD = false;
      bool fFBEkgiWWj = false;
      bool VkInVsbtxz = false;
      bool ZisyAmhTic = false;
      bool ruhocqCsqR = false;
      bool jpzFlCzpIc = false;
      bool zTrbdwLCDy = false;
      bool yPnsqaDTKd = false;
      bool qhDfOjjnLx = false;
      bool nrKGdXDFLg = false;
      bool ezorSUCGNL = false;
      bool zeedkQQLMt = false;
      bool MXpGKGEgOT = false;
      string SdjRRWuyYb;
      string sTjFutNCMs;
      string bEhBNWMiYT;
      string WBCOLzZbgA;
      string fDVfNRMPBo;
      string byCJtNuhEu;
      string JLZFFrQUsw;
      string UkVMNYhCnw;
      string JxgiqXyrUx;
      string DrXlpTdQaV;
      string AqVFUqbouV;
      string fssIkLepNg;
      string sYAwrbMjVt;
      string tRaRuSabNA;
      string wJwzRKyPAe;
      string omDtcbpZNr;
      string drTBaNSaMR;
      string kDcNkjbBdh;
      string dwiqEHOgoS;
      string bAOUVDotYD;
      if(SdjRRWuyYb == AqVFUqbouV){rWChslKoJs = true;}
      else if(AqVFUqbouV == SdjRRWuyYb){ZisyAmhTic = true;}
      if(sTjFutNCMs == fssIkLepNg){AoweOhIbxS = true;}
      else if(fssIkLepNg == sTjFutNCMs){ruhocqCsqR = true;}
      if(bEhBNWMiYT == sYAwrbMjVt){zklspjoHrg = true;}
      else if(sYAwrbMjVt == bEhBNWMiYT){jpzFlCzpIc = true;}
      if(WBCOLzZbgA == tRaRuSabNA){tgbZpGtAxS = true;}
      else if(tRaRuSabNA == WBCOLzZbgA){zTrbdwLCDy = true;}
      if(fDVfNRMPBo == wJwzRKyPAe){wYWwSlEiGn = true;}
      else if(wJwzRKyPAe == fDVfNRMPBo){yPnsqaDTKd = true;}
      if(byCJtNuhEu == omDtcbpZNr){uMxFDGkIrI = true;}
      else if(omDtcbpZNr == byCJtNuhEu){qhDfOjjnLx = true;}
      if(JLZFFrQUsw == drTBaNSaMR){xDxcyTMVoH = true;}
      else if(drTBaNSaMR == JLZFFrQUsw){nrKGdXDFLg = true;}
      if(UkVMNYhCnw == kDcNkjbBdh){mHedVaOoaD = true;}
      if(JxgiqXyrUx == dwiqEHOgoS){fFBEkgiWWj = true;}
      if(DrXlpTdQaV == bAOUVDotYD){VkInVsbtxz = true;}
      while(kDcNkjbBdh == UkVMNYhCnw){ezorSUCGNL = true;}
      while(dwiqEHOgoS == dwiqEHOgoS){zeedkQQLMt = true;}
      while(bAOUVDotYD == bAOUVDotYD){MXpGKGEgOT = true;}
      if(rWChslKoJs == true){rWChslKoJs = false;}
      if(AoweOhIbxS == true){AoweOhIbxS = false;}
      if(zklspjoHrg == true){zklspjoHrg = false;}
      if(tgbZpGtAxS == true){tgbZpGtAxS = false;}
      if(wYWwSlEiGn == true){wYWwSlEiGn = false;}
      if(uMxFDGkIrI == true){uMxFDGkIrI = false;}
      if(xDxcyTMVoH == true){xDxcyTMVoH = false;}
      if(mHedVaOoaD == true){mHedVaOoaD = false;}
      if(fFBEkgiWWj == true){fFBEkgiWWj = false;}
      if(VkInVsbtxz == true){VkInVsbtxz = false;}
      if(ZisyAmhTic == true){ZisyAmhTic = false;}
      if(ruhocqCsqR == true){ruhocqCsqR = false;}
      if(jpzFlCzpIc == true){jpzFlCzpIc = false;}
      if(zTrbdwLCDy == true){zTrbdwLCDy = false;}
      if(yPnsqaDTKd == true){yPnsqaDTKd = false;}
      if(qhDfOjjnLx == true){qhDfOjjnLx = false;}
      if(nrKGdXDFLg == true){nrKGdXDFLg = false;}
      if(ezorSUCGNL == true){ezorSUCGNL = false;}
      if(zeedkQQLMt == true){zeedkQQLMt = false;}
      if(MXpGKGEgOT == true){MXpGKGEgOT = false;}
    } 
}; 
