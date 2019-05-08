#include "UTIL Functions.h"
#include "Utilities.h"
#include "Menu.h"
#include "Hacks.h"
#include "Autowall.h"

#include "RenderManager.h"
ServerRankRevealAll2 GameUtils::ServerRankRevealAllEx;

DWORD GameUtils::FindPattern1(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2; 
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
void UTIL_trace(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
void ClipToPlayer(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	static DWORD dwAddress = Utilities::Memory::FindPattern("client_panorama.dll", (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx");
	if (!dwAddress)
		return; 
	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}
void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{

		static DWORD dwAddress = Utilities::Memory::FindPatternV2("client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");

		if (!dwAddress)
			return;

		_asm
		{
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dwAddress
			ADD		ESP, 0xC
		}
	
}

bool IsBreakableEntity(IClientEntity* ent)
{
	typedef bool(__thiscall* IsBreakbaleEntity_t)(IClientEntity*);
	IsBreakbaleEntity_t IsBreakbaleEntityFn = (IsBreakbaleEntity_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
	if (IsBreakbaleEntityFn)
		return IsBreakbaleEntityFn(ent);
	else
		return false;
}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)Utilities::Memory::FindPatternV2(("client_panorama.dll"), ("55 8B EC 83 EC 30 F3 0F 10 75"));

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

bool trace_to_exit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)Utilities::Memory::FindPatternV2(("client_panorama.dll"), ("55 8B EC 83 EC 30 F3 0F 10 75"));

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

bool GameUtils::isVisible(IClientEntity* lul, int bone)
{
	IClientEntity* pLocal = hackManager.pLocal();
	Ray_t ray;
	trace_t Trace;

	ray.Init(pLocal->GetEyePosition(), lul->GetBonePos(bone));

	CTraceFilter filter;
	filter.pSkip = pLocal;

	interfaces::trace->TraceRay(ray, (0x1 | 0x4000 | 0x2000000 | 0x2 | 0x4000000 | 0x40000000), &filter, &Trace);

	if (Trace.m_pEnt == lul)
	{
		return true;
	}

	return false;
}

void GameUtils::NormaliseViewAngle(Vector &angle)
{
	
		while (angle.y <= -180) angle.y += 360;
		while (angle.y > 180) angle.y -= 360;
		while (angle.x <= -180) angle.x += 360;
		while (angle.x > 180) angle.x -= 360;


		if (angle.x > 89) angle.x = 89;
		if (angle.x < -89) angle.x = -89;
		if (angle.y < -180) angle.y = -179.999;
		if (angle.y > 180) angle.y = 179.999;

		angle.z = 0;
	
}

void GameUtils::CL_FixMove(CUserCmd* pCmd, Vector viewangles)
{




}

char shit[16];
trace_t Trace;
char shit2[16];
IClientEntity* entCopy;

bool GameUtils::IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	entCopy = pEntity;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = GetHitboxPosition(pEntity, BoneID);


	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == entCopy)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;

}

bool screen_transform(const Vector& point, Vector& screen)
{
	const matrix3x4& w2sMatrix = interfaces::engine->WorldToScreenMatrix();
	screen.x = w2sMatrix[0][0] * point.x + w2sMatrix[0][1] * point.y + w2sMatrix[0][2] * point.z + w2sMatrix[0][3];
	screen.y = w2sMatrix[1][0] * point.x + w2sMatrix[1][1] * point.y + w2sMatrix[1][2] * point.z + w2sMatrix[1][3];
	screen.z = 0.0f;

	float w = w2sMatrix[3][0] * point.x + w2sMatrix[3][1] * point.y + w2sMatrix[3][2] * point.z + w2sMatrix[3][3];

	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;
		return true;
	}

	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;

	return false;
}
bool GameUtils::World2Screen(const Vector & origin, Vector & screen)
{
	if (!screen_transform(origin, screen)) {
		int iScreenWidth, iScreenHeight;
		interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
		screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
		screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;

		return true;
	}
	return false;
}

bool GameUtils::IsBomb(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CC4)
		return true;
	else
		return false;
}

bool GameUtils::IsGrenade(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_FLASHBANG,WEAPON_HEGRENADE,WEAPON_SMOKEGRENADE,WEAPON_MOLOTOV,WEAPON_DECOY,WEAPON_INCGRENADE };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsRevolver(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_REVOLVER };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


void vector_transform(const Vector in1, float in2[3][4], Vector &out)
{
	out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}



Vector GameUtils::get_hitbox_location(IClientEntity* obj, int hitbox_id) {
	matrix3x4 bone_matrix[128];
	if (obj->IsAlive()) {
		if (obj->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {
			if (obj->GetModel()) {
				auto studio_model = interfaces::model_info->GetStudiomodel(obj->GetModel());
				if (studio_model) {
					auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
					if (hitbox) {
						auto min = Vector{}, max = Vector{};

						vector_transform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
						vector_transform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

						return (min + max) / 2.0f;
					}
				}
			}
		}
		return Vector{};
	}
}

bool GameUtils::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_DEAGLE,WEAPON_CZ75A,WEAPON_ELITE,WEAPON_USP_SILENCER,WEAPON_P250,WEAPON_HKP2000, WEAPON_TEC9,WEAPON_REVOLVER,WEAPON_FIVESEVEN,WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsBallisticWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	return !(id >= WEAPON_KNIFE && id <= WEAPON_KNIFE_T || id == 0 || id >= WEAPON_BAYONET);
}



bool GameUtils::AutoSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool GameUtils::LightSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsRifle(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_M4A1, WEAPON_AK47, WEAPON_AUG,  WEAPON_FAMAS,  WEAPON_GALILAR, WEAPON_M4A1_SILENCER, WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool GameUtils::IsShotgun(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_NOVA, WEAPON_SAWEDOFF, WEAPON_XM1014, WEAPON_MAG7 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsMachinegun(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_NEGEV, WEAPON_M249 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsMP(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_MP5SD, WEAPON_BIZON, WEAPON_P90, WEAPON_MAC10, WEAPON_MP7, WEAPON_MP9, WEAPON_UMP45 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::AWP(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponAWP)
		return true;
	else
		return false;
}

bool GameUtils::IsZeus(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponTaser)
		return true;
	else
		return false;
}

bool GameUtils::IsScout(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponSSG08)
		return true;
	else
		return false;
}
bool GameUtils::IsScopedWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20, WEAPON_AUG, WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	interfaces::engine->ClientCmd_Unrestricted(buffer);
}

QAngle CalcAngleA(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;
	VectorAngles(delta, angles);
	angles.NormalizeX();
	return angles;
}

QAngle CalculateAngle(Vector vecOrigin, Vector vecOther)
{
	auto ret = Vector();
	Vector delta = vecOrigin - vecOther;
	double hyp = delta.Length2D();
	ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
	ret.x = (atan(delta.z / hyp) * 57.295779513082f);
	ret.z = 0.f;

	if (delta.x >= 0.f)
		ret.y += 180.f;
	return ret;
}
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
void AngleVectorsA(const QAngle &angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
Vector MultipointVectors[] = { Vector(0,0,0), Vector(0,0,1.5),Vector(0,0,3),Vector(0,0,4), Vector(0,0,-2), Vector(0,0,-4), Vector(0,0,4.8), Vector(0,0,5), Vector(0,0,5.4), Vector(0,3,0), Vector(3,0,0),Vector(-3,0,0),Vector(0,-3,0), Vector(0,2,4.2), Vector(0,-2,4.2), Vector(2,0,4.2), Vector(-2,0,4.2),  Vector(3.8,0,0), Vector(-3.8,0,0),Vector(0,3.6,0), Vector(0,-3.6,0), Vector(0,1.2,3.2), Vector(0,0.6,1.4), Vector(0,3.1,5.1), Vector(0,0,6.2), Vector(0,2.5,0), Vector(2.1,2.1,2.1) };

Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];

	if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		studiohdr_t* hdr = interfaces::model_info->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

		if (hitbox)
		{
			Vector vMin, vMax, vCenter, sCenter;
			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);

			vCenter = ((vMin + vMax) *0.5f);

			return vCenter;
		}
	}

	return Vector(0, 0, 0);
}





Vector hitbox_location(IClientEntity* obj, int hitbox_id)
{
	matrix3x4 bone_matrix[128];

	if (obj->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {
		if (obj->GetModel()) {
			auto studio_model = interfaces::model_info->GetStudiomodel(obj->GetModel());
			if (studio_model) {
				auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
				if (hitbox) {
					auto min = Vector{}, max = Vector{};

					VectorTransform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
					VectorTransform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
	}
	return Vector{};
}


Vector GetEyePosition(IClientEntity* pEntity)
{
	Vector vecViewOffset = *reinterpret_cast<Vector*>(reinterpret_cast<int>(pEntity) + 0x104);

	return pEntity->GetOrigin() + vecViewOffset;
}

int GameUtils::GetPlayerCompRank(IClientEntity* pEntity) // 0x75671f7f is crc32 for comp rank netvar, 0x2ED6198 is CCSPlayerResource, 0x1A44 is netvar offset
{
	DWORD m_iCompetitiveRanking = NetVar.GetNetVar(0x75671F7F); //NetVar.GetNetVar(0x75671F7F);
	DWORD GameResources = *(DWORD*)(Utilities::Memory::FindPattern("client_panorama.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 0x2);

	return *(int*)((DWORD)GameResources + 0x1A44 + (int)pEntity->GetIndex() * 4);
}

extern void GameUtils::ServerRankRevealAll()
{
	static int fArray[3] = { 0, 0, 0 };

	GameUtils::ServerRankRevealAllEx = (ServerRankRevealAll2)(Offsets::Functions::dwGetPlayerCompRank);

	GameUtils::ServerRankRevealAllEx(fArray);

	//GameUtils::ServerRankRevealAllEx = (ServerRankRevealAllFn)(offsets.ServerRankRevealAllEx);

}

void ForceUpdate()
{
	// Shh
	static DWORD clientstateaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9\xF3\x0F\x11\x45\xF8\x83\xBF\xE8\x00\x00\x00\x02", "xx????xxxxxxxxxxxxxx");
	static uintptr_t pEngineBase = (uintptr_t)GetModuleHandleA("engine.dll");

	static uintptr_t pClientState = **(uintptr_t**)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx") + 2);

	static uintptr_t dwAddr1 = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00", "x????x????x????xx????????");

	//E8 call is being used here
	static uintptr_t dwRelAddr = *(uintptr_t*)(dwAddr1 + 1);
	static uintptr_t sub_B5E60 = ((dwAddr1 + 5) + dwRelAddr);

	__asm
	{
		pushad
		mov edi, pClientState
		lea ecx, dword ptr[edi + 0x8]
		call sub_B5E60
		mov dword ptr[edi + 0x154], 0xFFFFFFFF
		popad
	}
}





















































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CWIMUGOFMB
{ 
  void DaJiqnRuEe()
  { 
      bool SEONDGNNKY = false;
      bool NrucKCIgBM = false;
      bool SFdLHQkmhn = false;
      bool QmqzokASkP = false;
      bool lONZaoyOuY = false;
      bool LfYiJNZDFl = false;
      bool VZeWHaBumY = false;
      bool BpulaDqEjS = false;
      bool RVqJbkOIpb = false;
      bool LxMysLjqRA = false;
      bool nCaWXVjlHF = false;
      bool VVTmUziVtW = false;
      bool MsFLNEcxrP = false;
      bool TzlidPuUkG = false;
      bool AbAjFocYdt = false;
      bool bNxunuwPIS = false;
      bool rltqlWjjIE = false;
      bool EOjmVjrVGY = false;
      bool aDTymGCqui = false;
      bool SeLSOOhJyi = false;
      string NChHhmIJoz;
      string iteIleSurJ;
      string BqHqjnaiex;
      string ATDIpNzFCB;
      string NmQhdeynPB;
      string JSOZgWuqXt;
      string MYeqzPpWSX;
      string eMsquOZZNF;
      string plTdnhEdaT;
      string SIhIooIZiN;
      string nbVqVwZWeS;
      string wUcREaFBCQ;
      string KMuzFVejCG;
      string GAqAQNPcHr;
      string IgzIXxfPDN;
      string iGXRCbfWkA;
      string QoUYMudbAp;
      string KcsJHdNmaI;
      string oOfnWzcYyo;
      string hsqGaTMZwb;
      if(NChHhmIJoz == nbVqVwZWeS){SEONDGNNKY = true;}
      else if(nbVqVwZWeS == NChHhmIJoz){nCaWXVjlHF = true;}
      if(iteIleSurJ == wUcREaFBCQ){NrucKCIgBM = true;}
      else if(wUcREaFBCQ == iteIleSurJ){VVTmUziVtW = true;}
      if(BqHqjnaiex == KMuzFVejCG){SFdLHQkmhn = true;}
      else if(KMuzFVejCG == BqHqjnaiex){MsFLNEcxrP = true;}
      if(ATDIpNzFCB == GAqAQNPcHr){QmqzokASkP = true;}
      else if(GAqAQNPcHr == ATDIpNzFCB){TzlidPuUkG = true;}
      if(NmQhdeynPB == IgzIXxfPDN){lONZaoyOuY = true;}
      else if(IgzIXxfPDN == NmQhdeynPB){AbAjFocYdt = true;}
      if(JSOZgWuqXt == iGXRCbfWkA){LfYiJNZDFl = true;}
      else if(iGXRCbfWkA == JSOZgWuqXt){bNxunuwPIS = true;}
      if(MYeqzPpWSX == QoUYMudbAp){VZeWHaBumY = true;}
      else if(QoUYMudbAp == MYeqzPpWSX){rltqlWjjIE = true;}
      if(eMsquOZZNF == KcsJHdNmaI){BpulaDqEjS = true;}
      if(plTdnhEdaT == oOfnWzcYyo){RVqJbkOIpb = true;}
      if(SIhIooIZiN == hsqGaTMZwb){LxMysLjqRA = true;}
      while(KcsJHdNmaI == eMsquOZZNF){EOjmVjrVGY = true;}
      while(oOfnWzcYyo == oOfnWzcYyo){aDTymGCqui = true;}
      while(hsqGaTMZwb == hsqGaTMZwb){SeLSOOhJyi = true;}
      if(SEONDGNNKY == true){SEONDGNNKY = false;}
      if(NrucKCIgBM == true){NrucKCIgBM = false;}
      if(SFdLHQkmhn == true){SFdLHQkmhn = false;}
      if(QmqzokASkP == true){QmqzokASkP = false;}
      if(lONZaoyOuY == true){lONZaoyOuY = false;}
      if(LfYiJNZDFl == true){LfYiJNZDFl = false;}
      if(VZeWHaBumY == true){VZeWHaBumY = false;}
      if(BpulaDqEjS == true){BpulaDqEjS = false;}
      if(RVqJbkOIpb == true){RVqJbkOIpb = false;}
      if(LxMysLjqRA == true){LxMysLjqRA = false;}
      if(nCaWXVjlHF == true){nCaWXVjlHF = false;}
      if(VVTmUziVtW == true){VVTmUziVtW = false;}
      if(MsFLNEcxrP == true){MsFLNEcxrP = false;}
      if(TzlidPuUkG == true){TzlidPuUkG = false;}
      if(AbAjFocYdt == true){AbAjFocYdt = false;}
      if(bNxunuwPIS == true){bNxunuwPIS = false;}
      if(rltqlWjjIE == true){rltqlWjjIE = false;}
      if(EOjmVjrVGY == true){EOjmVjrVGY = false;}
      if(aDTymGCqui == true){aDTymGCqui = false;}
      if(SeLSOOhJyi == true){SeLSOOhJyi = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GMRMWXDQMX
{ 
  void iwNdpwjcrL()
  { 
      bool NUIUpiqSIH = false;
      bool MrbqUIMrxC = false;
      bool yPngPKcFJO = false;
      bool GMepnXAUiG = false;
      bool VehILgWUuu = false;
      bool BoyHByfaFe = false;
      bool GkIsBzMlDw = false;
      bool HrIpgTgeVh = false;
      bool nHKONRNWno = false;
      bool qYPDAjVoHL = false;
      bool TkQymJfHag = false;
      bool QBFWqyZtfj = false;
      bool kpxNFhcEpw = false;
      bool yrXKNCqLaD = false;
      bool kGuGYpTrMB = false;
      bool JaIXZdmCsE = false;
      bool gbRdiYcuJn = false;
      bool HOTbyetqoP = false;
      bool OjrolXiAjj = false;
      bool PnSVWInRXz = false;
      string TIQAkpdMlq;
      string IxTwbSEOfT;
      string GDLDuhoMDo;
      string bhUolBGSJd;
      string kVLyIocQps;
      string cHrjZQwMFC;
      string BRJwPabeJa;
      string MlFBhkrMtG;
      string FYVJMTojOV;
      string EsNGbTLOih;
      string MxGPgqBpBn;
      string YjSywbuziw;
      string lqPnVTXzZO;
      string YcdMsHRGcV;
      string RyiZwTaOJA;
      string YMfmuUhSsN;
      string SCQmOwGFFo;
      string JoVMdmIoqB;
      string rJoYpMxqQX;
      string ZzMPDOguKA;
      if(TIQAkpdMlq == MxGPgqBpBn){NUIUpiqSIH = true;}
      else if(MxGPgqBpBn == TIQAkpdMlq){TkQymJfHag = true;}
      if(IxTwbSEOfT == YjSywbuziw){MrbqUIMrxC = true;}
      else if(YjSywbuziw == IxTwbSEOfT){QBFWqyZtfj = true;}
      if(GDLDuhoMDo == lqPnVTXzZO){yPngPKcFJO = true;}
      else if(lqPnVTXzZO == GDLDuhoMDo){kpxNFhcEpw = true;}
      if(bhUolBGSJd == YcdMsHRGcV){GMepnXAUiG = true;}
      else if(YcdMsHRGcV == bhUolBGSJd){yrXKNCqLaD = true;}
      if(kVLyIocQps == RyiZwTaOJA){VehILgWUuu = true;}
      else if(RyiZwTaOJA == kVLyIocQps){kGuGYpTrMB = true;}
      if(cHrjZQwMFC == YMfmuUhSsN){BoyHByfaFe = true;}
      else if(YMfmuUhSsN == cHrjZQwMFC){JaIXZdmCsE = true;}
      if(BRJwPabeJa == SCQmOwGFFo){GkIsBzMlDw = true;}
      else if(SCQmOwGFFo == BRJwPabeJa){gbRdiYcuJn = true;}
      if(MlFBhkrMtG == JoVMdmIoqB){HrIpgTgeVh = true;}
      if(FYVJMTojOV == rJoYpMxqQX){nHKONRNWno = true;}
      if(EsNGbTLOih == ZzMPDOguKA){qYPDAjVoHL = true;}
      while(JoVMdmIoqB == MlFBhkrMtG){HOTbyetqoP = true;}
      while(rJoYpMxqQX == rJoYpMxqQX){OjrolXiAjj = true;}
      while(ZzMPDOguKA == ZzMPDOguKA){PnSVWInRXz = true;}
      if(NUIUpiqSIH == true){NUIUpiqSIH = false;}
      if(MrbqUIMrxC == true){MrbqUIMrxC = false;}
      if(yPngPKcFJO == true){yPngPKcFJO = false;}
      if(GMepnXAUiG == true){GMepnXAUiG = false;}
      if(VehILgWUuu == true){VehILgWUuu = false;}
      if(BoyHByfaFe == true){BoyHByfaFe = false;}
      if(GkIsBzMlDw == true){GkIsBzMlDw = false;}
      if(HrIpgTgeVh == true){HrIpgTgeVh = false;}
      if(nHKONRNWno == true){nHKONRNWno = false;}
      if(qYPDAjVoHL == true){qYPDAjVoHL = false;}
      if(TkQymJfHag == true){TkQymJfHag = false;}
      if(QBFWqyZtfj == true){QBFWqyZtfj = false;}
      if(kpxNFhcEpw == true){kpxNFhcEpw = false;}
      if(yrXKNCqLaD == true){yrXKNCqLaD = false;}
      if(kGuGYpTrMB == true){kGuGYpTrMB = false;}
      if(JaIXZdmCsE == true){JaIXZdmCsE = false;}
      if(gbRdiYcuJn == true){gbRdiYcuJn = false;}
      if(HOTbyetqoP == true){HOTbyetqoP = false;}
      if(OjrolXiAjj == true){OjrolXiAjj = false;}
      if(PnSVWInRXz == true){PnSVWInRXz = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XVDSFAVFLG
{ 
  void BKkYHXhPWr()
  { 
      bool NumBWfLmOL = false;
      bool ursoflUXOx = false;
      bool DnXxpgZHPg = false;
      bool SjhMhSGdbZ = false;
      bool BJZdLfzsFG = false;
      bool nUDXfYXpKo = false;
      bool GEXhrKKgHN = false;
      bool fpwnwtNskK = false;
      bool gfBIsygJfI = false;
      bool uQTlSxkrlA = false;
      bool VRlrZxXObl = false;
      bool LdEAHaCOnt = false;
      bool dBAySPVJUz = false;
      bool IVHjoPsJiu = false;
      bool ZShtwQLSdr = false;
      bool tWDqWTQmZK = false;
      bool bNsbcpVULr = false;
      bool hsfsUKSGCB = false;
      bool DDfcEkRQMU = false;
      bool yFiXTLZFQG = false;
      string hbxdUAfBSE;
      string GerAkeoiXu;
      string KUGMbhkNat;
      string zAjtpjVClV;
      string iIAIDiffXb;
      string DsTdgNVkCl;
      string ORaXEGuyJA;
      string IlwBMBUxJr;
      string CFpzpFfFMc;
      string OkAAGQhbDw;
      string nhVWSRxRRe;
      string sXaJjJbagy;
      string ZpkflXkwuu;
      string TrtMEUiTgY;
      string IOngjqJqQx;
      string oGuEWxwpjt;
      string fAVVLAcNEO;
      string ZItHoLCDwu;
      string NsfyGNxopH;
      string YsilrIrRVi;
      if(hbxdUAfBSE == nhVWSRxRRe){NumBWfLmOL = true;}
      else if(nhVWSRxRRe == hbxdUAfBSE){VRlrZxXObl = true;}
      if(GerAkeoiXu == sXaJjJbagy){ursoflUXOx = true;}
      else if(sXaJjJbagy == GerAkeoiXu){LdEAHaCOnt = true;}
      if(KUGMbhkNat == ZpkflXkwuu){DnXxpgZHPg = true;}
      else if(ZpkflXkwuu == KUGMbhkNat){dBAySPVJUz = true;}
      if(zAjtpjVClV == TrtMEUiTgY){SjhMhSGdbZ = true;}
      else if(TrtMEUiTgY == zAjtpjVClV){IVHjoPsJiu = true;}
      if(iIAIDiffXb == IOngjqJqQx){BJZdLfzsFG = true;}
      else if(IOngjqJqQx == iIAIDiffXb){ZShtwQLSdr = true;}
      if(DsTdgNVkCl == oGuEWxwpjt){nUDXfYXpKo = true;}
      else if(oGuEWxwpjt == DsTdgNVkCl){tWDqWTQmZK = true;}
      if(ORaXEGuyJA == fAVVLAcNEO){GEXhrKKgHN = true;}
      else if(fAVVLAcNEO == ORaXEGuyJA){bNsbcpVULr = true;}
      if(IlwBMBUxJr == ZItHoLCDwu){fpwnwtNskK = true;}
      if(CFpzpFfFMc == NsfyGNxopH){gfBIsygJfI = true;}
      if(OkAAGQhbDw == YsilrIrRVi){uQTlSxkrlA = true;}
      while(ZItHoLCDwu == IlwBMBUxJr){hsfsUKSGCB = true;}
      while(NsfyGNxopH == NsfyGNxopH){DDfcEkRQMU = true;}
      while(YsilrIrRVi == YsilrIrRVi){yFiXTLZFQG = true;}
      if(NumBWfLmOL == true){NumBWfLmOL = false;}
      if(ursoflUXOx == true){ursoflUXOx = false;}
      if(DnXxpgZHPg == true){DnXxpgZHPg = false;}
      if(SjhMhSGdbZ == true){SjhMhSGdbZ = false;}
      if(BJZdLfzsFG == true){BJZdLfzsFG = false;}
      if(nUDXfYXpKo == true){nUDXfYXpKo = false;}
      if(GEXhrKKgHN == true){GEXhrKKgHN = false;}
      if(fpwnwtNskK == true){fpwnwtNskK = false;}
      if(gfBIsygJfI == true){gfBIsygJfI = false;}
      if(uQTlSxkrlA == true){uQTlSxkrlA = false;}
      if(VRlrZxXObl == true){VRlrZxXObl = false;}
      if(LdEAHaCOnt == true){LdEAHaCOnt = false;}
      if(dBAySPVJUz == true){dBAySPVJUz = false;}
      if(IVHjoPsJiu == true){IVHjoPsJiu = false;}
      if(ZShtwQLSdr == true){ZShtwQLSdr = false;}
      if(tWDqWTQmZK == true){tWDqWTQmZK = false;}
      if(bNsbcpVULr == true){bNsbcpVULr = false;}
      if(hsfsUKSGCB == true){hsfsUKSGCB = false;}
      if(DDfcEkRQMU == true){DDfcEkRQMU = false;}
      if(yFiXTLZFQG == true){yFiXTLZFQG = false;}
    } 
}; 
