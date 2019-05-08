#include <array>
#include <string>
#include <deque>
#include <algorithm>
#include "Entities.h"
#include "CommonIncludes.h"
#include "Entities.h"
#include "Vector.h"
#include <map>
#include "Interfaces.h"
#include "Hacks.h"
#include "Hooks.h"

enum correction_flags
{
	DESYNC,
	NO_DESYNC,
	SLOW_WALK
};
struct extra
{
	int current_flag[64];
};
class angle_correction
{
public:
	void ac_simple(IClientEntity * entity);
	void ac_smart(IClientEntity * entity);
	void resolve_desync_smart(IClientEntity * player);
private:
	void missed_due_to_desync(IGameEvent* event);
	bool is_slow_walking(IClientEntity* entity);

};
class resolver_setup
{
public:
	bool didhit;
	
//	void resolver_main(IClientEntity * pEntity);

	float apply_freestanding_low_performance(IClientEntity * enemy);

	void FSN(IClientEntity* pEntity, ClientFrameStage_t stage); //used to get the fresh THINGS and the resolve

	void preso(IClientEntity * pEntity);
	
	bool enemy_slowwalk = false;
	bool enemy_inair;
	bool enemy_fake = false;
	bool has_desync = false;

	float resolved_pitch;
	float resolved_yaw;

	static resolver_setup GetInst()
	{
		static resolver_setup instance;
		return instance;
	}
}; extern resolver_setup * resolver;

namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern int fired;
	extern int hit;
	extern bool change;
	extern int TargetID;
	extern bool didhitHS;
	extern int missedshots;
	extern std::map<int, QAngle> storedshit;
	extern bool Up2date;
	extern Vector aim_point;
}