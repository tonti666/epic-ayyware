#include "DamageIndicator.h"
#include "RenderManager.h"

DamageIndicators damage_indicators;

void DamageIndicators::paint() {
	auto m_local = hackManager.pLocal();

	float current_time = m_local->GetTickBase() * interfaces::globals->interval_per_tick;

	for (int i = 0; i < data.size(); i++) {
		if (data[i].flEraseTime < current_time) {
			data.erase(data.begin() + i);
			continue;
		}

		if (!data[i].bInitialized) {
			data[i].Position = data[i].Player->GetHeadPos();
			data[i].bInitialized = true;
		}

		if (current_time - data[i].flLastUpdate > 0.0001f) {
			data[i].Position.z -= (0.1f * (current_time - data[i].flEraseTime));
			data[i].flLastUpdate = current_time;
		}

		Vector screen_pos;

		if (GameUtils::World2Screen(data[i].Position, screen_pos)) {
			Render::text2(screen_pos.x, screen_pos.y, std::to_string(data[i].iDamage).c_str(), Render::Fonts::Menutop, Color(26, 130, 228, 255));
		}
	}
}