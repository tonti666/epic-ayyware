#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "CRC32.h"
#include <fstream>
#include "XorStr.hpp"
#define WINDOW_WIDTH 455
#define WINDOW_HEIGHT 480 // 510
mirror_window options::menu;
struct Config_t {
	int id;
	std::string name;
};
std::vector<Config_t> configs;
typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void MsgX(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); 	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}
void save_callback()
{
	int should_save = options::menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "spuri\\cfg\\";
	config_directory += configs[should_save].name; config_directory += ".xml";
	GUI.SaveWindowState(&options::menu, XorStr(config_directory.c_str()));
	interfaces::cvar->ConsoleColorPrintf(Color(140, 10, 250, 255), "spuri_framework ");
	std::string uremam = "Saved configuration.     \n";
	MsgX(uremam.c_str());
}
void load_callback()
{
	int should_load = options::menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "spuri\\cfg\\";
	config_directory += configs[should_load].name; config_directory += ".xml";
	GUI.LoadWindowState(&options::menu, XorStr(config_directory.c_str()));
	interfaces::cvar->ConsoleColorPrintf(Color(140, 10, 250, 255), "spuri_framework ");
	std::string uremam = "Loaded configuration.     \n";
	MsgX(uremam.c_str());
}

void list_configs() {
	configs.clear();
	options::menu.ColorsTab.ConfigListBox.ClearItems();
	std::ifstream file_in;
	file_in.open("spuri\\cfg\\spuri_configs.txt");
	if (file_in.fail()) {
		std::ofstream("spuri\\cfg\\spuri_configs.txt");
		file_in.open("spuri\\cfg\\spuri_configs.txt");
	}
	int line_count;
	while (!file_in.eof()) {
		Config_t config;
		file_in >> config.name;
		config.id = line_count;
		configs.push_back(config);
		line_count++;
		options::menu.ColorsTab.ConfigListBox.AddItem(config.name);
	}
	file_in.close();
	if (configs.size() > 7) options::menu.ColorsTab.ConfigListBox.AddItem(" ");
}

void add_config() {
	std::fstream file;
	file.open("spuri\\cfg\\spuri_configs.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("spuri\\cfg\\spuri_configs.txt");
		file.open("spuri\\cfg\\spuri_configs.txt", std::fstream::app);
	}
	file << std::endl << options::menu.ColorsTab.NewConfigName.getText();
	file.close();
	list_configs();
	int should_add = options::menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "spuri\\cfg\\";
	config_directory += options::menu.ColorsTab.NewConfigName.getText(); config_directory += ".xml";
	GUI.SaveWindowState(&options::menu, XorStr(config_directory.c_str()));
	options::menu.ColorsTab.NewConfigName.SetText("");
}

void remove_config() {
	int should_remove = options::menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "spuri\\cfg\\";
	config_directory += configs[should_remove].name; config_directory += ".xml";
	std::remove(config_directory.c_str());
	std::ofstream ofs("spuri\\cfg\\spuri_configs.txt", std::ios::out | std::ios::trunc);
	ofs.close();
	std::fstream file;
	file.open("spuri\\cfg\\spuri_configs.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("spuri\\cfg\\spuri_configs.txt");
		file.open("spuri\\cfg\\spuri_configs.txt", std::fstream::app);
	}
	for (int i = 0; i < configs.size(); i++) {
		if (i == should_remove) continue;
		Config_t config = configs[i];
		file << std::endl << config.name;
	}
	file.close();
	list_configs();
}

/*
void KnifeApplyCallbk()
{
	static ConVar* Meme = interfaces::cvar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	interfaces::engine->ClientCmd_Unrestricted("cl_fullupdate");
}*/

void UnLoadCallbk()
{
	DoUnload = true;
}
void mirror_window::Setup()
{
	SetPosition(350, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("");
	RegisterTab(&aimbot_tab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&visuals_tab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorsTab);
	RegisterTab(&SkinTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;
	aimbot_tab.Setup();
	LegitBotTab.Setup();
	visuals_tab.Setup();
	MiscTab.Setup();
	ColorsTab.Setup();
	SkinTab.Setup();

#pragma endregion
}
void CAimbotTab::Setup()
{
	SetTitle("rage");
#pragma region Aimbot
	AimbotGroup.SetPosition(4, -5); // 15, 25
	AimbotGroup.SetText("main");
	AimbotGroup.SetSize(215, 195);
	AimbotGroup.AddTab(CGroupTab("aimbot", 1));
	//AimbotGroup.AddTab(CGroupTab("accuracy", 2));
	AimbotGroup.AddTab(CGroupTab("resolver", 2));
	AimbotGroup.AddTab(CGroupTab("bodyaim", 3));
	//	AimbotGroup.AddTab(CGroupTab("Resolver Mods", 4));
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl(1, "enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotAutoFire.SetState(true);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotSilentAim.SetState(true);
	//AimbotGroup.PlaceLabledControl(1, "choke shot", this, &AimbotSilentAim);


	AccuracyAutoScope.SetFileId("aim_scope");
	AccuracyAutoScope.SetState(true);
	//AimbotGroup.PlaceLabledControl(1, "auto scope", this, &AccuracyAutoScope);

	AccuracyHitchance.SetFileId("aim_hc");
	AccuracyHitchance.SetBoundaries(0.f, 100.f);
	AccuracyHitchance.SetValue(20);

	AccuracyMinimumDamage.SetFileId("aim_md");
	AccuracyMinimumDamage.SetBoundaries(0.f, 100.f);
	AccuracyMinimumDamage.SetValue(20);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.extension = XorStr("�");
	AimbotFov.SetValue(180.f);

	lag_pred.SetFileId("aim_lag_pred");
	AimbotGroup.PlaceLabledControl(1, "lag prediction", this, &lag_pred);

	extrapolation.SetFileId("aim_acc_extra_P_lation");
	AimbotGroup.PlaceLabledControl(1, "extrapolation", this, &extrapolation);

	delay_shot.SetFileId("aim_delay_shot");
	delay_shot.AddItem("none");
	delay_shot.AddItem("sim-time");
	delay_shot.AddItem("lag compensation");
	delay_shot.AddItem("refine shot");
	AimbotGroup.PlaceLabledControl(1, "delay shot", this, &delay_shot);




	preso.SetFileId("aim_posadjust");
	preso.AddItem("default");
	preso.AddItem("down");
	preso.AddItem("up");
	preso.AddItem("zero");
	preso.AddItem("automatic spread");
	AimbotGroup.PlaceLabledControl(2, "pitch adjustment", this, &preso);

	resolver.SetFileId("aim_aaa");
	resolver.AddItem("default");
	resolver.AddItem("stickrpg");
	resolver.AddItem("spuri primary");
	AimbotGroup.PlaceLabledControl(2, "yaw adjustment", this, &resolver);

	flip180.SetFileId("aim_flip180");
	AimbotGroup.PlaceLabledControl(2, "override", this, &flip180);


	baim_fake.SetFileId("aim_bodyaim_fake");
	AimbotGroup.PlaceLabledControl(3, "if fake", this, &baim_fake); // if we have to resort to a brute

	baim_fakewalk.SetFileId("aim_bodyaim_fakewalk");
	AimbotGroup.PlaceLabledControl(3, "if slow walk", this, &baim_fakewalk);

	baim_inair.SetFileId("aim_bodyaim_inair");
	AimbotGroup.PlaceLabledControl(3, "if in air", this, &baim_inair); //if they be flyin like a plane

	BaimIfUnderXHealth.SetFileId("aim_BaimIfUnderXHealth");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.extension = XorStr("hp");
	BaimIfUnderXHealth.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "if hp lower than", this, &BaimIfUnderXHealth);

	bigbaim.SetFileId("aim_bigbaim");
	AimbotGroup.PlaceLabledControl(3, "on key", this, &bigbaim);

	///////////////////////////////////////////////////////////
	multipoint.SetText("multipoint");
	multipoint.SetPosition(225, -5); // 15, 230
	multipoint.SetSize(209, 195);
	multipoint.AddTab(CGroupTab("settings", 1));
	RegisterControl(&multipoint);

	TargetPointscale.SetFileId("aim_hitbox_Scale");
	TargetPointscale.SetBoundaries(0, 100);
	TargetPointscale.SetValue(50);
	TargetPointscale.extension = ("%%");
	multipoint.PlaceLabledControl(1, "hitbox scale", this, &TargetPointscale);

	Multienable.SetFileId("aim_multipoint_enable");
	multipoint.PlaceLabledControl(1, "enable multipoint", this, &Multienable);

	Multival2.SetFileId("hitbox_scale_head");
	Multival2.SetBoundaries(0.1, 100);
	Multival2.SetValue(20);
	Multival2.extension = XorStr("%%");
	multipoint.PlaceLabledControl(1, "head", this, &Multival2);

	Multival.SetFileId("hitbox_scale_body");
	Multival.SetBoundaries(0.1, 100);
	Multival.SetValue(20);
	Multival.extension = XorStr("%%");
	multipoint.PlaceLabledControl(1, "body", this, &Multival);

	MultiVal3.SetFileId("hitbox_scale_legs");
	MultiVal3.SetBoundaries(0.1, 100);
	MultiVal3.SetValue(20);
	MultiVal3.extension = XorStr("%%");
	multipoint.PlaceLabledControl(1, "leg", this, &MultiVal3);

	///////////////////////////////////////////////////


	// -<--------------------------------------------------------------->- //
	/*
	legit_mode.SetFileId("aim_legit_toggle");
	AimbotGroup.PlaceLabledControl(4, "Enable", this, &legit_mode);

	legit_trigger.SetFileId("aim_legit_trigger");
	AimbotGroup.PlaceLabledControl(4, "Trigger", this, &legit_trigger);

	legit_trigger_key.SetFileId("aim_legit_trigger_key");
	AimbotGroup.PlaceLabledControl(4, "Trigger Key", this, &legit_trigger_key);

	apply_smooth.SetFileId("aim_legit_apply_smooth");
	AimbotGroup.PlaceLabledControl(4, "Apply Smoothness", this, &apply_smooth); */
	// -<--------------------------------------------------------------->- //

	weapongroup.SetText("weapon settings");
	weapongroup.SetPosition(4, 205); // 15, 230
	weapongroup.SetSize(430, 195);
	weapongroup.AddTab(CGroupTab("auto", 1));
	weapongroup.AddTab(CGroupTab("pistol", 2));
	weapongroup.AddTab(CGroupTab("scout", 3));
	weapongroup.AddTab(CGroupTab("awp", 4));
	weapongroup.AddTab(CGroupTab("smg", 5));
	weapongroup.AddTab(CGroupTab("rifle", 6));
	RegisterControl(&weapongroup);

	target_auto.SetFileId("tgt_hitbox_auto");
	target_auto.items.push_back(dropdownboxitem(false, XorStr("head")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("upper body")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("lower body")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("arms")));
	target_auto.items.push_back(dropdownboxitem(false, XorStr("legs")));
	weapongroup.PlaceLabledControl(1, "hitscan", this, &target_auto);

	custom_hitscan.SetFileId("auto_hitscan_auto");
	weapongroup.PlaceLabledControl(1, "automatic hitscan", this, &custom_hitscan);

	hc_auto.SetFileId("auto_hitchance");
	hc_auto.SetBoundaries(0, 100);
	hc_auto.SetValue(25);
	hc_auto.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(1, "hitchance", this, &hc_auto);

	md_auto.SetFileId("auto_minimumdamage");
	md_auto.SetBoundaries(0, 100);
	md_auto.SetValue(25);
	md_auto.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(1, "minimum damage", this, &md_auto);

	//	preset_auto.SetFileId("auto_automatic_cfg");
	//	weapongroup.PlaceLabledControl(1, "Automatic Auto Sniper Configuration", this, &preset_auto);
	//----------------------------------------------------------------------

	target_pistol.SetFileId("tgt_hitbox_pistol");
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("head")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("upper body")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("lower body")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("arms")));
	target_pistol.items.push_back(dropdownboxitem(false, XorStr("legs")));
	weapongroup.PlaceLabledControl(2, "hitscan", this, &target_pistol);

	hc_pistol.SetFileId("pistol_hitchance");
	hc_pistol.SetBoundaries(0, 100);
	hc_pistol.SetValue(25);
	hc_pistol.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(2, "hitchance", this, &hc_pistol);

	md_pistol.SetFileId("pistol_minimumdamage");
	md_pistol.SetBoundaries(0, 100);
	md_pistol.SetValue(25);
	md_pistol.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(2, "minimum damage", this, &md_pistol);


	//	preset_pistol.SetFileId("pistol_automatic_cfg");
	//	weapongroup.PlaceLabledControl(2, "Automatic Pistol Configuration", this, &preset_pistol);

	//----------------------------------------------------------------------

	target_scout.SetFileId("tgt_hitbox_scout");
	target_scout.items.push_back(dropdownboxitem(false, XorStr("head")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("upper body")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("lower body")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("arms")));
	target_scout.items.push_back(dropdownboxitem(false, XorStr("legs")));
	weapongroup.PlaceLabledControl(3, "hitscan", this, &target_scout);

	hc_scout.SetFileId("scout_hitchance");
	hc_scout.SetBoundaries(0, 100);
	hc_scout.SetValue(25);
	hc_scout.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(3, "hitchance", this, &hc_scout);

	md_scout.SetFileId("scout_minimumdamage");
	md_scout.SetBoundaries(0, 100);
	md_scout.SetValue(25);
	md_scout.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(3, "minimum damage", this, &md_scout);

	//	headonly_if_vis_scout.SetFileId("headonly_if_vis_scout");
	//	weapongroup.PlaceLabledControl(3, "headshot Only If Hittable", this, &headonly_if_vis_scout);

	//	preset_scout.SetFileId("scout_automatic_cfg");
	//	weapongroup.PlaceLabledControl(3, "Automatic Scout Configuration", this, &preset_scout);
	//----------------------------------------------------------------------

	target_awp.SetFileId("tgt_hitbox_awp");
	target_awp.items.push_back(dropdownboxitem(false, XorStr("head")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("upper body")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("lower body")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("arms")));
	target_awp.items.push_back(dropdownboxitem(false, XorStr("legs")));
	weapongroup.PlaceLabledControl(4, "hitscan", this, &target_awp);

	hc_awp.SetFileId("awp_hitchance");
	hc_awp.SetBoundaries(0, 100);
	hc_awp.SetValue(25);
	hc_awp.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(4, "hitchance", this, &hc_awp);

	md_awp.SetFileId("awp_minimumdamage");
	md_awp.SetBoundaries(0, 100);
	md_awp.SetValue(25);
	md_awp.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(4, "minimum damage", this, &md_awp);

	//----------------------------------------------------------------------

	target_smg.SetFileId("tgt_hitbox_smg");
	target_smg.items.push_back(dropdownboxitem(false, XorStr("head")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("upper body")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("lower body")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("arms")));
	target_smg.items.push_back(dropdownboxitem(false, XorStr("legs")));
	weapongroup.PlaceLabledControl(5, "hitscan", this, &target_smg);

	hc_smg.SetFileId("smg_hitchance");
	hc_smg.SetBoundaries(0, 100);
	hc_smg.SetValue(25);
	hc_smg.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(5, "hitchance", this, &hc_smg);

	md_smg.SetFileId("smg_minimumdamage");
	md_smg.SetBoundaries(0, 100);
	md_smg.SetValue(25);
	md_smg.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(5, "minimum damage", this, &md_smg);

	//----------------------------------------------------------------------

	target_otr.SetFileId("tgt_hitbox_otr");
	target_otr.items.push_back(dropdownboxitem(false, XorStr("head")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("upper body")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("lower body")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("arms")));
	target_otr.items.push_back(dropdownboxitem(false, XorStr("legs")));
	weapongroup.PlaceLabledControl(6, "hitscan", this, &target_otr);

	hc_otr.SetFileId("otr_hitchance");
	hc_otr.SetBoundaries(0, 100);
	hc_otr.SetValue(25);
	hc_otr.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(6, "hitchance", this, &hc_otr);

	md_otr.SetFileId("otr_minimumdamage");
	md_otr.SetBoundaries(0, 100);
	md_otr.SetValue(25);
	md_otr.extension = XorStr("%%");
	weapongroup.PlaceLabledControl(6, "minimum damage", this, &md_otr);

	//	headonly_if_vis_otr.SetFileId("headonly_if_vis_otr");
	//	weapongroup.PlaceLabledControl(5, "headshot Only If Hittable", this, &headonly_if_vis_otr);

	//	preset_otr.SetFileId("otr_automatic_cfg");
	//	weapongroup.PlaceLabledControl(5, "Automatic  Weapon Configuration", this, &preset_otr);
#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{	


SetTitle("legit");
AimbotGroup.SetText("main");
AimbotGroup.SetPosition(4, -5);
AimbotGroup.SetSize(430, 195);
AimbotGroup.AddTab(CGroupTab("aimbot", 1));
AimbotGroup.AddTab(CGroupTab("misc", 2));

RegisterControl(&AimbotGroup);
RegisterControl(&Active);
Active.SetFileId("active");
AimbotGroup.PlaceLabledControl(1, "enable", this, &Active);

AimbotEnable.SetFileId("legitaimbotenable");
AimbotGroup.PlaceLabledControl(1, "aimbot", this, &AimbotEnable);


AimbotKeyPress.SetFileId("legitaimbotleyress");
AimbotKeyPress.SetState(true);
//AimbotGroup.PlaceLabledControl(1, "on key", this, &AimbotKeyPress);

AimbotKeyBind.SetFileId("legitaimbotaimkey");
AimbotGroup.PlaceLabledControl(1, "aimbot key", this, &AimbotKeyBind);

AimbotSmokeCheck.SetFileId("aimbotsmokecheck");
AimbotGroup.PlaceLabledControl(1, "smoke check", this, &AimbotSmokeCheck);

legitbacktrack.SetFileId("legitbacktrack");
AimbotGroup.PlaceLabledControl(2, "backtrack", this, &legitbacktrack);

legitbacktrackticks.SetFileId("legitbacktrackticks");
legitbacktrackticks.SetBoundaries(0.0f, 12.0f);
AimbotGroup.PlaceLabledControl(2, "backtrack ticks", this, &legitbacktrackticks);

StandAloneRCS.SetFileId("standalonercs");
AimbotGroup.PlaceLabledControl(2, "standalone rcs", this, &StandAloneRCS);

StandAloneRCSamount.SetFileId("standalonercsamount");
StandAloneRCSamount.SetBoundaries(0.0f, 2.0f);
AimbotGroup.PlaceLabledControl(2, "standalone rcs amount", this, &StandAloneRCSamount);

visualizefov.SetFileId("visualizefov");
AimbotGroup.PlaceLabledControl(2, "visualize fov", this, &visualizefov);


//visualizebacktrack.SetFileId("visualizebacktrack");
//AimbotGroup.PlaceLabledControl(2, "visualize backtrack", this, &visualizebacktrack);







//----------------------solid kys---------------------//

weapongroup.SetText("weapon settings");
weapongroup.SetPosition(4, 200);
weapongroup.SetSize(430, 213);
weapongroup.AddTab(CGroupTab("rifle", 1));
weapongroup.AddTab(CGroupTab("pistol", 2));
weapongroup.AddTab(CGroupTab("sniper", 3));
weapongroup.AddTab(CGroupTab("smg", 4));
RegisterControl(&weapongroup);
RegisterControl(&Active);

////rifle////

WeaponMainHitbox.SetFileId("riflehitbox");
WeaponMainHitbox.AddItem("head");
WeaponMainHitbox.AddItem("neck");
WeaponMainHitbox.AddItem("chest");
WeaponMainHitbox.AddItem("stomach");
WeaponMainHitbox.AddItem("nearest");
weapongroup.PlaceLabledControl(1, "hitbox", this, &WeaponMainHitbox);


WeaponMainSpeed.SetFileId("riflespeed");
WeaponMainSpeed.SetBoundaries(0.001f, 5.000f);
weapongroup.PlaceLabledControl(1, "speed", this, &WeaponMainSpeed);

WeaponMainFoV.SetFileId("riflefov");
WeaponMainFoV.SetBoundaries(0.00f, 5.00f);
weapongroup.PlaceLabledControl(1, "field of view", this, &WeaponMainFoV);


WeaponMainRecoil.SetFileId("riflercs");
WeaponMainRecoil.SetBoundaries(0.00f, 2.00f);
weapongroup.PlaceLabledControl(1, "rcs", this, &WeaponMainRecoil);


WeaponMainInacc.SetFileId("riflerandomize");
WeaponMainInacc.SetBoundaries(0.00f, 15.00f);
weapongroup.PlaceLabledControl(1, "randomize", this, &WeaponMainInacc);




// --- Pistols --- //
WeaponPistHitbox.SetFileId("pistolhitbox");
WeaponPistHitbox.AddItem("head");
WeaponPistHitbox.AddItem("neck");
WeaponPistHitbox.AddItem("chest");
WeaponPistHitbox.AddItem("stomach");
WeaponPistHitbox.AddItem("nearest");
weapongroup.PlaceLabledControl(2, "hitbox", this, &WeaponPistHitbox);


WeaponPistSpeed.SetFileId("pistolspeed");
WeaponPistSpeed.SetBoundaries(0.001f, 5.000f);
weapongroup.PlaceLabledControl(2, "speed", this, &WeaponPistSpeed);

WeaponPistFoV.SetFileId("pistolfov");
WeaponPistFoV.SetBoundaries(0.00f, 5.00f);
weapongroup.PlaceLabledControl(2, "field of view", this, &WeaponPistFoV);


WeaponPistPSilent.SetFileId("pistolpsilent");
weapongroup.PlaceLabledControl(2, "perfect silent", this, &WeaponPistPSilent);


// --- Sniper --- //
WeaponSnipHitbox.SetFileId("sniperhitbox");
WeaponSnipHitbox.AddItem("head");
WeaponSnipHitbox.AddItem("neck");
WeaponSnipHitbox.AddItem("chest");
WeaponSnipHitbox.AddItem("stomach");
WeaponSnipHitbox.AddItem("nearest");
weapongroup.PlaceLabledControl(3, "hitbox", this, &WeaponSnipHitbox);


WeaponSnipSpeed.SetFileId("sniperspeed");
WeaponSnipSpeed.SetBoundaries(0.001f, 5.000f);
weapongroup.PlaceLabledControl(3, "speed", this, &WeaponSnipSpeed);


WeaponSnipFoV.SetFileId("sniperfov");
WeaponSnipFoV.SetBoundaries(0.00f, 5.00f);
weapongroup.PlaceLabledControl(3, "field of view", this, &WeaponSnipFoV);

WeaponSnipPSilent.SetFileId("sniperpsilent");
weapongroup.PlaceLabledControl(3, "perfect silent", this, &WeaponSnipPSilent);
// --- SMG --- //
WeaponMpHitbox.SetFileId("mphitbox");
WeaponMpHitbox.AddItem("head");
WeaponMpHitbox.AddItem("neck");
WeaponMpHitbox.AddItem("chest");
WeaponMpHitbox.AddItem("stomach");
WeaponMpHitbox.AddItem("nearest");
weapongroup.PlaceLabledControl(4, "hitbox", this, &WeaponMpHitbox);

WeaponMpSpeed.SetFileId("mpspeed");
WeaponMpSpeed.SetBoundaries(0.001f, 5.000f);
weapongroup.PlaceLabledControl(4, "speed", this, &WeaponMpSpeed);


WeaponMpFoV.SetFileId("mpfov");
WeaponMpFoV.SetBoundaries(0.00f, 5.00f);
weapongroup.PlaceLabledControl(4, "field of view", this, &WeaponMpFoV);

WeaponMpRecoil.SetFileId("mprcs");
WeaponMpRecoil.SetBoundaries(0.00f, 2.00f);
weapongroup.PlaceLabledControl(4, "rcs", this, &WeaponMpRecoil);

weaponmpinacc.SetFileId("mprandomize");
weaponmpinacc.SetBoundaries(0.00f, 15.00f);
weapongroup.PlaceLabledControl(4, "randomize", this, &weaponmpinacc);

}

void CVisualTab::Setup()
{

	SetTitle("visuals");
#pragma region Options
	OptionsGroup.SetText("options");
	OptionsGroup.SetPosition(4, -5);
	OptionsGroup.SetSize(220, 410);
	OptionsGroup.AddTab(CGroupTab("main", 1));
	OptionsGroup.AddTab(CGroupTab("filters", 2));
	OptionsGroup.AddTab(CGroupTab("misc", 3));
	OptionsGroup.AddTab(CGroupTab("other", 4));
	OptionsGroup.AddTab(CGroupTab("xhair", 5));
	RegisterControl(&OptionsGroup);
	RegisterControl(&Active);

	Active.SetFileId("visuals_active");
	OptionsGroup.PlaceLabledControl(1, "activate visuals", this, &Active);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("none");
	OptionsBox.AddItem("default");
	OptionsBox.AddItem("genuine");
	OptionsBox.AddItem("corners");
	OptionsBox.AddItem("vis/invis");
	OptionsBox.AddItem("visible only");
	OptionsGroup.PlaceLabledControl(1, "box", this, &OptionsBox);


	OptionsName.SetFileId("opt_name");
	OptionsName.AddItem("none");
	OptionsName.AddItem("top");
	OptionsName.AddItem("right");
	OptionsGroup.PlaceLabledControl(1, "name", this, &OptionsName);



	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("none");
	OptionsHealth.AddItem("default");
	OptionsHealth.AddItem("battery");
	OptionsHealth.AddItem("bottom");
	OptionsGroup.PlaceLabledControl(1, "health", this, &OptionsHealth);

	OptionsArmor.SetFileId("otr_armor");
	OptionsArmor.AddItem("none");
	OptionsArmor.AddItem("default");
	OptionsArmor.AddItem("battery");
	OptionsArmor.AddItem("bottom");
	OptionsGroup.PlaceLabledControl(1, "armor", this, &OptionsArmor); // here

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl(1, "info", this, &OptionsInfo);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl(1, "skeleton", this, &OptionsSkeleton);



	Weapons.SetFileId("weapons");
	OptionsGroup.PlaceLabledControl(1, "weapons", this, &Weapons);

	Ammo.SetFileId("ammo");
	OptionsGroup.PlaceLabledControl(1, "ammo", this, &Ammo);

	//GrenadePred.SetFileId("GrenadePred");
	//OptionsGroup.PlaceLabledControl(1, "grenade prediction", this, &GrenadePred);

	//	nonescreenESP.SetFileId("otr_nonescreenESP");
	//	OptionsGroup.PlaceLabledControl(1, "nonescreen ESP", this, &nonescreenESP);

	GlowZ.SetFileId("opt_glowz");
	GlowZ.SetValue(0.f);
	GlowZ.SetBoundaries(0.f, 100.f);
	GlowZ.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "enemy glow", this, &GlowZ);

	team_glow.SetFileId("opt_team_glow");
	team_glow.SetValue(0.f);
	team_glow.SetBoundaries(0.f, 100.f);
	team_glow.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "team glow", this, &team_glow);

	Glowz_lcl.SetFileId("opt_glowz_lcl");
	Glowz_lcl.SetValue(0.f);
	Glowz_lcl.SetBoundaries(0.f, 100.f);
	Glowz_lcl.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "local glow", this, &Glowz_lcl);

	FiltersAll.SetFileId("ftr_all");
	OptionsGroup.PlaceLabledControl(2, "all", this, &FiltersAll);

	show_players.SetFileId("ftr_players");
	show_players.SetState(true);
	OptionsGroup.PlaceLabledControl(2, "players", this, &show_players);

	show_team.SetFileId("ftr_enemyonly");
	OptionsGroup.PlaceLabledControl(2, "show team", this, &show_team);

	FiltersWeapons.SetFileId("ftr_weaps");
	OptionsGroup.PlaceLabledControl(2, "weapons", this, &FiltersWeapons);

	FiltersNades.SetFileId("ftr_nades");
	OptionsGroup.PlaceLabledControl(2, "grenades", this, &FiltersNades);

	FiltersC4.SetFileId("ftr_c4");
	OptionsGroup.PlaceLabledControl(2, "bomb", this, &FiltersC4);

	deadesp.SetFileId("deadesp");
	OptionsGroup.PlaceLabledControl(2, "dead esp", this, &deadesp);

	//	show_hostage.SetFileId("ftr_hostage");
	//	OptionsGroup.PlaceLabledControl(2, "Hostage", this, &show_hostage);

	optimize.SetFileId("ftr_optimize");
	OptionsGroup.PlaceLabledControl(2, "disable postprocess", this, &optimize);
	//	asus_type.SetFileId("asus_wall_type");
	//	asus_type.AddItem("Props Only");
	//	asus_type.AddItem("Walls and Props");
	//	OptionsGroup.PlaceLabledControl(2, "Asus Type", this, &asus_type);


	//----------------------------------------------//




	//	DamageIndicator.SetFileId("otr_btracers");
	//	OptionsGroup.PlaceLabledControl(3, "Damage Indicator", this, &DamageIndicator);

	OtherNoScope.SetFileId("otr_noscope");
	OptionsGroup.PlaceLabledControl(3, "remove scope", this, &OtherNoScope);

	RemoveZoom.SetFileId("otr_remv_zoom");
	OptionsGroup.PlaceLabledControl(3, "remove zoom", this, &RemoveZoom);

	OtherNoFlash.SetFileId("otr_noflash");
	OptionsGroup.PlaceLabledControl(3, "remove flash", this, &OtherNoFlash);

	flashAlpha.SetFileId("otr_flash_alpha");
	flashAlpha.SetBoundaries(0, 100);
	flashAlpha.extension = XorStr("%%");
	flashAlpha.SetValue(10);
	OptionsGroup.PlaceLabledControl(3, "flash alpha", this, &flashAlpha);

	nosmoke.SetFileId("otr_nosmoke");
	OptionsGroup.PlaceLabledControl(3, "remove smoke", this, &nosmoke);

	/*	nosmoke_slider.SetFileId("otr_nosmoke_alpha");
	nosmoke_slider.SetBoundaries(0, 100);
	nosmoke_slider.extension = ("%%");
	nosmoke_slider.SetValue(10);
	OptionsGroup.PlaceLabledControl(3, "Smoke Alpha", this, &nosmoke_slider); */


	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OptionsGroup.PlaceLabledControl(3, "no visual recoil", this, &OtherNoVisualRecoil);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(30.f, 120.f);
	OtherViewmodelFOV.SetValue(90.f);
	OptionsGroup.PlaceLabledControl(3, "viewmodel fov", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 60.f);
	OtherFOV.SetValue(0.f);
	OptionsGroup.PlaceLabledControl(3, "override fov", this, &OtherFOV);

	override_viewmodel.SetFileId("otr_override_viewmodel_noneset");
	OptionsGroup.PlaceLabledControl(3, "override viewmodel offset", this, &override_viewmodel);

	offset_x.SetFileId("otr_noneset_x");
	offset_x.SetBoundaries(-30, 30);
	offset_x.SetValue(2.5);
	OptionsGroup.PlaceLabledControl(3, "offset X", this, &offset_x);

	offset_y.SetFileId("otr_noneset_Y");
	offset_y.SetBoundaries(-30, 30);
	offset_y.SetValue(2.0);
	OptionsGroup.PlaceLabledControl(3, "offset Y", this, &offset_y);

	offset_z.SetFileId("otr_noneset_z");
	offset_z.SetBoundaries(-30, 30);
	offset_z.SetValue(-2.0);
	OptionsGroup.PlaceLabledControl(3, "offset z", this, &offset_z);

	aspectratio.SetFileId("aspectratio");
	OptionsGroup.PlaceLabledControl(3, "override aspect ratio", this, &aspectratio);

	aspectratiovalue.SetFileId("aspectratiovalue");
	aspectratiovalue.SetBoundaries(0, 200);
	aspectratiovalue.SetValue(50.0);
	OptionsGroup.PlaceLabledControl(3, "aspect ratio value", this, &aspectratiovalue);

	leftknife.SetFileId("leftknife");
	OptionsGroup.PlaceLabledControl(3, "left knife", this, &leftknife);
	

	manualaa_type.SetFileId("manualaa");
	manualaa_type.AddItem("none");
	manualaa_type.AddItem("single arrow");
	manualaa_type.AddItem("all arrows");
	OptionsGroup.PlaceLabledControl(4, "manual indicator", this, &manualaa_type); // requested by: https://steamcommunity.com/id/9a-

	logs.SetFileId("eventlog");
	OptionsGroup.PlaceLabledControl(4, "event log", this, &logs);

	OffscreenESP.SetFileId("OffscreenESP");
	OptionsGroup.PlaceLabledControl(4, "offscreen esp", this, &OffscreenESP);

	//AWHitmarker.SetFileId("AWHitmarker");
	//OptionsGroup.PlaceLabledControl(4, "AWHitmarker", this, &AWHitmarker);

	damageindi.SetFileId("DamageIndicator");
	OptionsGroup.PlaceLabledControl(4, "dmg indicator", this, &damageindi);
	
	
	SpecList.SetFileId("otr_spectator");
	OptionsGroup.PlaceLabledControl(4, "spectator list", this, &SpecList);

//	killfeed.SetFileId("otr_killfeed");
//	OptionsGroup.PlaceLabledControl(4, "Preserve Killfeed", this, &killfeed); // requested by: https://steamcommunity.com/id/B1GN1GNN

	cheatinfo.SetFileId("cheatinfox");
	OptionsGroup.PlaceLabledControl(4, "player status", this, &cheatinfo);

	CompRank.SetFileId("otr_reveal__rank");
	OptionsGroup.PlaceLabledControl(4, "rank reveal", this, &CompRank);

	//OtherEntityGlow.SetFileId("otr_world_ent_glow");
//	OptionsGroup.PlaceLabledControl(4, "World Entity Glow", this, &OtherEntityGlow);

	OtherHitmarker.SetFileId("otr_hitmarker");
	OptionsGroup.PlaceLabledControl(4, "hitmarker", this, &OtherHitmarker);

	watermark.SetFileId("otr_watermark");
	watermark.SetState(true);
	OptionsGroup.PlaceLabledControl(4, "watermark", this, &watermark);


	//laser.SetFileId("laser");
	//OptionsGroup.PlaceLabledControl(4, "lasers", this, &laser);

	misczoom.SetFileId("misczoom");
	OptionsGroup.PlaceLabledControl(4, "force scope", this, &misczoom);

	misczoomkey.SetFileId("misczoomkey");
	OptionsGroup.PlaceLabledControl(4, "force scope key", this, &misczoomkey);

	SniperCrosshair.SetFileId("SniperCrosshair");
	OptionsGroup.PlaceLabledControl(5, "sniper crosshair", this, &SniperCrosshair);

	SpreadCrosshair.SetFileId(XorStr("v_spreadcrosshair"));
	SpreadCrosshair.AddItem("none");
	SpreadCrosshair.AddItem("standard");
	SpreadCrosshair.AddItem("colour");
	SpreadCrosshair.AddItem("rainbow");
	SpreadCrosshair.AddItem("rainbow animated");
	OptionsGroup.PlaceLabledControl(5, XorStr("spread crosshair"), this, &SpreadCrosshair);

	/*	SpreadCrossSize.SetFileId("otr_spreadcross_size");
	SpreadCrossSize.SetBoundaries(1.f, 100.f); //we should take smth like 650 as max so i guess *6.5?
	SpreadCrossSize.extension = XorStr("%%");
	SpreadCrossSize.SetValue(45.f);
	OptionsGroup.PlaceLabledControl(3, "Size", this, &SpreadCrossSize); */

	AutowallCrosshair.SetFileId("otr_autowallcross");
	OptionsGroup.PlaceLabledControl(5, "crosshair", this, &AutowallCrosshair);

	OtherRecoilCrosshair.SetFileId("OtherRecoilCrosshair");
	OptionsGroup.PlaceLabledControl(5, "recoil crosshair", this, &OtherRecoilCrosshair);

	//flashindicator.SetFileId("flashindicator");
	//OptionsGroup.PlaceLabledControl(5, "flashindicator", this, &flashindicator);

	//AutowallCrosshair2.SetFileId("AutowallCrosshair2");
	//OptionsGroup.PlaceLabledControl(5, "autowall crosshair", this, &AutowallCrosshair2);


	//	BulletTrace.SetFileId("otr_bullet_tracers_local");
//	OptionsGroup.PlaceLabledControl(4, "local bullet tracers", this, &BulletTrace);

	//BulletTrace_enemy.SetFileId("otr_bullet_tracers_enemy");
	//OptionsGroup.PlaceLabledControl(4, "Enemy Bullet Tracers", this, &BulletTrace_enemy);
	//----------------------------------------------//

	ChamsGroup.SetText("chams");
	ChamsGroup.AddTab(CGroupTab("main", 1));
	ChamsGroup.AddTab(CGroupTab("misc", 2));
	ChamsGroup.AddTab(CGroupTab("mods", 3));
	ChamsGroup.SetPosition(234, -5); // 225, 30
	ChamsGroup.SetSize(200, 235);
	RegisterControl(&ChamsGroup);

	ChamsEnemyvisible.SetFileId("chams_en_vis");
	ChamsGroup.PlaceLabledControl(1, "enemies (visible only)", this, &ChamsEnemyvisible);

	ChamsEnemyinVisible.SetFileId("chams_en_novis");
	ChamsGroup.PlaceLabledControl(1, "enemies (xqz)", this, &ChamsEnemyinVisible); // *1

	ChamsTeamVis.SetFileId("ChamsTeamVis");
	ChamsGroup.PlaceLabledControl(1, "team", this, &ChamsTeamVis);

	//ChamsLocal.SetFileId("chams_local");
	//ChamsGroup.PlaceLabledControl(1, "local", this, &ChamsLocal);

	HandCHAMS.SetFileId("chams_local_hand");
	HandCHAMS.AddItem("none");
	HandCHAMS.AddItem("simple");
	HandCHAMS.AddItem("wireframe");
	HandCHAMS.AddItem("golden");
	HandCHAMS.AddItem("glass");
	HandCHAMS.AddItem("dark");
	HandCHAMS.AddItem("ghostie");
	HandCHAMS.AddItem("bubble");
	ChamsGroup.PlaceLabledControl(1, "hand", this, &HandCHAMS);

	GunCHAMS.SetFileId("chams_local_weapon");
	GunCHAMS.AddItem("none");
	GunCHAMS.AddItem("simple");
	GunCHAMS.AddItem("gloss");
	GunCHAMS.AddItem("golden");
	GunCHAMS.AddItem("glass");
	GunCHAMS.AddItem("dark");
	ChamsGroup.PlaceLabledControl(1, "weapon", this, &GunCHAMS);

	localmaterial.SetFileId("esp_localscopedmat");
	localmaterial.AddItem("default");
	localmaterial.AddItem("clear");
	localmaterial.AddItem("cham");
	localmaterial.AddItem("wireframe");
	localmaterial.AddItem("lsd");
	localmaterial.AddItem("dark");
	ChamsGroup.PlaceLabledControl(1, "scoped materials", this, &localmaterial);

	fakelag_ghost.SetFileId("otr_fakelag_ghost");
	fakelag_ghost.AddItem("none");
	fakelag_ghost.AddItem("default");
	fakelag_ghost.AddItem("pulse");
	ChamsGroup.PlaceLabledControl(2, "fakelag ghost", this, &fakelag_ghost);

	visible_chams_type.SetFileId("otr_visiblechams_type");
	visible_chams_type.AddItem("normal");
	visible_chams_type.AddItem("flat"); // like my ex :)
	visible_chams_type.AddItem("pulse");
	visible_chams_type.AddItem("blueish");
	visible_chams_type.AddItem("bubble");
	visible_chams_type.AddItem("glass");
	ChamsGroup.PlaceLabledControl(2, "visible chams type", this, &visible_chams_type);

	invisible_chams_type.SetFileId("otr_invisiblechams_type");
	invisible_chams_type.AddItem("normal");
	invisible_chams_type.AddItem("flat"); // i'm still sad about that :(
	invisible_chams_type.AddItem("pulse");
	invisible_chams_type.AddItem("blueish");
	invisible_chams_type.AddItem("bubble");
	invisible_chams_type.AddItem("glass");
	ChamsGroup.PlaceLabledControl(2, "invisible chams type", this, &invisible_chams_type);

	blend_local.SetFileId("esp_teamblend");
	blend_local.SetBoundaries(0, 100);
	blend_local.SetValue(75);
	blend_local.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(3, "local chams blend", this, &blend_local);

	transparency.SetFileId("esp_transparency");
	transparency.SetBoundaries(0, 100);
	transparency.SetValue(20);
	transparency.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(3, "scoped transparency", this, &transparency);

	
	enemy_blend_invis.SetFileId("esp_blendinvis");
	enemy_blend_invis.SetBoundaries(0, 100);
	enemy_blend_invis.SetValue(75);
	enemy_blend_invis.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(3, "visible chams blend", this, &enemy_blend_invis); // *! -> requested an update by: https://steamcommunity.com/id/Skatergirllzz
	
	hand_transparency.SetFileId("esp_hand_transparency");
	hand_transparency.SetBoundaries(0, 100);
	hand_transparency.SetValue(20);
	hand_transparency.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(3, "arm transparency", this, &hand_transparency);

	gun_transparency.SetFileId("esp_gun_transparency");
	gun_transparency.SetBoundaries(0, 100);
	gun_transparency.SetValue(20);
	gun_transparency.extension = XorStr("%%");
	ChamsGroup.PlaceLabledControl(3, "gun transparency", this, &gun_transparency);

#pragma region world

	worldgroup.SetText("World");
	worldgroup.AddTab(CGroupTab("main", 1));
	//worldgroup.AddTab(CGroupTab("mods", 2));
	worldgroup.SetPosition(234, 240);  // 225, 285
	worldgroup.SetSize(200, 165);
	RegisterControl(&worldgroup);
	colmodupdate.SetFileId("otr_night");
	worldgroup.PlaceLabledControl(1, "force update materials", this, &colmodupdate); //you could've just made this a button lol

	customskies.SetFileId("otr_skycustom");
	customskies.AddItem("default");
	customskies.AddItem("night");
	customskies.AddItem("nosky");
	customskies.AddItem("cloudy");
	customskies.AddItem("daylight");
	//customskies.AddItem("baggage");
	worldgroup.PlaceLabledControl(1, "skybox", this, &customskies);

	colmod.SetFileId("night_amm");
	colmod.SetBoundaries(000.000f, 100.00f);
	colmod.extension = XorStr("%%");
	colmod.SetValue(020.0f);
	worldgroup.PlaceLabledControl(1, "brightness", this, &colmod);

	asusamount.SetFileId("otr_asusprops");
	asusamount.SetBoundaries(1.f, 100.f);
	asusamount.extension = XorStr("%%");
	asusamount.SetValue(95.f);
	worldgroup.PlaceLabledControl(1, "asus walls", this, &asusamount);

	/*
	ModulateSkyBox.SetFileId(XorStr("sky_box_color_enable"));
	worldgroup.PlaceLabledControl(2, XorStr("enable sky color changer"), this, &ModulateSkyBox);

	sky_r.SetFileId("sky_r");
	sky_r.SetBoundaries(0.f, 25.f);
	sky_r.SetValue(10.f);
	worldgroup.PlaceLabledControl(2, "sky: r", this, &sky_r);

	sky_g.SetFileId("sky_g");
	sky_g.SetBoundaries(0.f, 25.f);
	sky_g.SetValue(1.f);
	worldgroup.PlaceLabledControl(2, "sky: g", this, &sky_g);

	sky_b.SetFileId("sky_b");
	sky_b.SetBoundaries(0.f, 25.f);
	sky_b.SetValue(20.f);
	worldgroup.PlaceLabledControl(2, "sky: b", this, &sky_b);*/

#pragma endregion Setting up the Other controls
}
void CMiscTab::Setup()
{
	/*
	__  __ _____  _____  _____
	|  \/  |_   _|/ ____|/ ____|
	| \  / | | | | (___ | |
	| |\/| | | |  \___ \| |
	| |  | |_| |_ ____) | |____
	|_|  |_|_____|_____/ \_____|


	*/
	SetTitle("misc");
#pragma region Other
	OtherGroup.SetText("other");
	OtherGroup.AddTab(CGroupTab("main", 1));
	OtherGroup.AddTab(CGroupTab("buybot", 2));
	OtherGroup.AddTab(CGroupTab("spam", 3));
	OtherGroup.AddTab(CGroupTab("move", 4));
	OtherGroup.SetPosition(4, -5);
	OtherGroup.SetSize(210, 400);
	RegisterControl(&OtherGroup);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.AddItem("anti untrusted");
	OtherSafeMode.AddItem("anti vac kick");
	OtherSafeMode.AddItem("danger zone");
	//OtherSafeMode.AddItem("meme");
	//OtherSafeMode.AddItem("unrestricted (!)");
	OtherGroup.PlaceLabledControl(1, "safety mode", this, &OtherSafeMode);

	dangerzone.SetFileId("misc_dangerzone");
	OtherGroup.PlaceLabledControl(1, "dangerzone support", this, &dangerzone);
	//


	hitmarker_sound.SetFileId("hitmarker_sound");
	hitmarker_sound.AddItem("none");
	hitmarker_sound.AddItem("cod");
	hitmarker_sound.AddItem("arenaswitch");
	hitmarker_sound.AddItem("bubble");
	hitmarker_sound.AddItem("bameware");
	hitmarker_sound.AddItem("anime");
	hitmarker_sound.AddItem("hitler"); // this is for CruZZ and Crytec. Heil hitler
	hitmarker_sound.AddItem("bell"); // diiiing	
	hitmarker_sound.AddItem("skeet"); // diiiing	
	OtherGroup.PlaceLabledControl(1, "hitmarker sound", this, &hitmarker_sound);

	OtherThirdperson.SetFileId("aa_1thirpbind");
	OtherGroup.PlaceLabledControl(1, "thirdperson", this, &OtherThirdperson);
	ThirdPersonKeyBind.SetFileId("aa_thirpbind");
	OtherGroup.PlaceLabledControl(1, "", this, &ThirdPersonKeyBind);


	unload.SetText("unload cheat");
	unload.SetCallback(UnLoadCallbk);
	OtherGroup.PlaceLabledControl(1, "", this, &unload);

	//Radar.SetFileId("Radar");
	//OtherGroup.PlaceLabledControl(1, "Draw Radar", this, &Radar);

	//RadarX.SetFileId("misc_radar_xax1");
	//RadarX.SetBoundaries(0, 1920);
	//RadarX.SetValue(0);
	//OtherGroup.PlaceLabledControl(1, "X-Axis", this, &RadarX);

	//RadarY.SetFileId("misc_radar_yax2");
	//RadarY.SetBoundaries(0, 1080);
	//RadarY.SetValue(0);
	//OtherGroup.PlaceLabledControl(1, "Y-Axis", this, &RadarY);



	//ragdoll.SetFileId("ragdoll");
	//OtherGroup.PlaceLabledControl(1, "ragdoll", this, &ragdoll);

	buybot_primary.SetFileId("buybot_primary");
	buybot_primary.AddItem("none");
	buybot_primary.AddItem("auto");
	buybot_primary.AddItem("scout");
	buybot_primary.AddItem("awp");
	buybot_primary.AddItem("ak-47 / m4");
	buybot_primary.AddItem("aug / sg553");
	buybot_primary.AddItem("mp9 / mac10");
	OtherGroup.PlaceLabledControl(2, "primary weapon", this, &buybot_primary);

	buybot_secondary.SetFileId("buybot_secondary");
	buybot_secondary.AddItem("none");
	buybot_secondary.AddItem("duals");
	buybot_secondary.AddItem("revolver / deagle");
	buybot_secondary.AddItem("five-feven / cz75");
	OtherGroup.PlaceLabledControl(2, "secondary weapon", this, &buybot_secondary);

	buybot_otr.SetFileId("buybot_other");
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("kevlar")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("he-grenade")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("flashbang")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("smoke grenade")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("molotov")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("zeus")));
	buybot_otr.items.push_back(dropdownboxitem(false, XorStr("defuse-kit")));
	OtherGroup.PlaceLabledControl(2, "others", this, &buybot_otr);

	killsay.SetFileId("misc_killsay");
	killsay.items.push_back(dropdownboxitem(false, XorStr("on kill")));
	killsay.items.push_back(dropdownboxitem(false, XorStr("on death")));
	OtherGroup.PlaceLabledControl(3, "trash talk", this, &killsay);

	ChatSpam.SetFileId("misc_chatspam");
	ChatSpam.AddItem("none");
	ChatSpam.AddItem("report");
	ChatSpam.AddItem("promo");
	OtherGroup.PlaceLabledControl(3, "chat spam", this, &ChatSpam);

	//AutoDefuse.SetFileId("misc_fakevac");
	//OtherGroup.PlaceLabledControl(3, "auto defuse", this, &AutoDefuse);

	fakeping.SetFileId("misc_fakeping");
	OtherGroup.PlaceLabledControl(3, "fake ping", this, &fakeping);

	fakepingvalue.SetFileId("misc_fakepingvalue");
	fakepingvalue.SetBoundaries(0.00f, 800.00f);
	OtherGroup.PlaceLabledControl(3, "fake ping value", this, &fakepingvalue);

	fakevote.SetFileId("misc_fakevote");
	OtherGroup.PlaceLabledControl(3, "silent name changer", this, &fakevote);


	namechangerr.SetFileId("misc_namechangerrrr");
	namechangerr.AddItem("none");
	namechangerr.AddItem("fake kick");
	namechangerr.AddItem("name stealer");
	namechangerr.AddItem("promo");
	OtherGroup.PlaceLabledControl(3, "namechanger", this, &namechangerr);

	ClanTag.SetFileId("misc_clantg");
	OtherGroup.PlaceLabledControl(3, XorStr("clan tag"), this, &ClanTag); // requested by: https://steamcommunity.com/id/hitoridekun and https://steamcommunity.com/id/123x456x789



	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl(4, "normal bhop", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherGroup.PlaceLabledControl(4, "air strafe", this, &OtherAutoStrafe);

	autojump_type.SetFileId("misc_autojump_type");
	autojump_type.AddItem("normal");
	autojump_type.AddItem("always on");
	OtherGroup.PlaceLabledControl(4, "bhop type", this, &autojump_type);

	Legitbhop.SetFileId("misc_legitbhop");
	OtherGroup.PlaceLabledControl(4, "legit bhop", this, &Legitbhop);

	
	Legitbhophitchance.SetFileId("misc_lbhophitchance");
	Legitbhophitchance.SetBoundaries(0.00f, 100.00f);
	Legitbhophitchance.extension = XorStr("%%");
	OtherGroup.PlaceLabledControl(4, "legit bhop hitchance", this, &Legitbhophitchance);

	Legitbhopminhit.SetFileId("misc_legitbhopminhit");
	Legitbhopminhit.SetBoundaries(0.00f, 11.00f);
	OtherGroup.PlaceLabledControl(4, "legit bhop min hops", this, &Legitbhopminhit);

	Legitbhopmaxhit.SetFileId("misc_legitbhopmaxhit");
	Legitbhopmaxhit.SetBoundaries(0.00f, 12.00f);
	OtherGroup.PlaceLabledControl(4, "legit bhop max hops", this, &Legitbhopmaxhit);


		
	velocity.SetFileId("misc_velocity");
	OtherGroup.PlaceLabledControl(4, "draw velocity", this, &velocity);


	//fakevote2.SetFileId("misc_fakevote");
	//OtherGroup.PlaceLabledControl(3, "fake kick", this, &fakevote2);

	//NameChanger.SetFileId("misc_NameChanger");
	//OtherGroup.PlaceLabledControl(3, "name stealer", this, &NameChanger);

	//namespampromo.SetFileId("misc_NameChangerpromo");
	//OtherGroup.PlaceLabledControl(3, "name spam promo", this, &namespampromo);


	AntiAimGroup.SetPosition(226, -5);
	AntiAimGroup.SetText("anti-aim");
	AntiAimGroup.SetSize(210, 400);
	AntiAimGroup.AddTab(CGroupTab("main", 1));
	AntiAimGroup.AddTab(CGroupTab("mods", 2));
	AntiAimGroup.AddTab(CGroupTab("misc", 3));
	AntiAimGroup.AddTab(CGroupTab("desync", 4));
	RegisterControl(&AntiAimGroup);
	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl(1, "enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("none");
	AntiAimPitch.AddItem("down");
	AntiAimPitch.AddItem("up");
	AntiAimPitch.AddItem("jitter");
	AntiAimPitch.AddItem("random");
	AntiAimPitch.AddItem("zero");
	AntiAimPitch.AddItem("fake down (!)");
	AntiAimPitch.AddItem("fake up (!)");
	AntiAimGroup.PlaceLabledControl(1, "pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("none");
	AntiAimYaw.AddItem("backward");
	AntiAimYaw.AddItem("backward jitter");
	AntiAimYaw.AddItem("random lowerbody");
	AntiAimYaw.AddItem("manual");
	AntiAimYaw.AddItem("jitter 180");
	AntiAimYaw.AddItem("jitter crooked");
	AntiAimYaw.AddItem("freestanding");
//	AntiAimYaw.AddItem("Twist");
	AntiAimGroup.PlaceLabledControl(1, "standing yaw", this, &AntiAimYaw);

	desync_aa_stand.SetFileId("v_desync_aa_stand");
	AntiAimGroup.PlaceLabledControl(1, "desync", this, &desync_aa_stand);

	AntiAimYawrun.SetFileId("aa_y2");
	AntiAimYawrun.AddItem("none");
	AntiAimYawrun.AddItem("backward");
	AntiAimYawrun.AddItem("backward jitter");
	AntiAimYawrun.AddItem("random lowerbody");
	AntiAimYawrun.AddItem("manual");
	AntiAimYawrun.AddItem("jitter 180");
	AntiAimYawrun.AddItem("jitter crooked");
	AntiAimYawrun.AddItem("freestanding");
	AntiAimGroup.PlaceLabledControl(1, "moving yaw", this, &AntiAimYawrun);

	desync_aa_move.SetFileId("v_desync_aa_move");
	AntiAimGroup.PlaceLabledControl(1, "desync", this, &desync_aa_move);

	AntiAimYaw3.SetFileId("aa_y3");
	AntiAimYaw3.AddItem("none");
	AntiAimYaw3.AddItem("backward");
	AntiAimYaw3.AddItem("backward jitter");
	AntiAimYaw3.AddItem("random lowerbody");
	AntiAimYaw3.AddItem("manual");
	AntiAimYaw3.AddItem("jitter crooked");
	//	AntiAimYaw3.AddItem("freestanding");
	AntiAimGroup.PlaceLabledControl(1, "in air yaw", this, &AntiAimYaw3);

	//	air_desync.SetFileId("v_air_desync");
	//	AntiAimGroup.PlaceLabledControl(1, "Air Desync", this, &air_desync);

	pitch_up.SetFileId("pitch_up");
	AntiAimGroup.PlaceLabledControl(1, "pitch flick", this, &pitch_up);

	disable_on_dormant.SetFileId("disable_on_dormant");
	AntiAimGroup.PlaceLabledControl(1, "disable on dormant", this, &disable_on_dormant);

	minimal_walk.SetFileId("minimal_walk");
	AntiAimGroup.PlaceLabledControl(1, "minimal walk key", this, &minimal_walk);

	freerange.SetFileId("freestanding_range");
	freerange.SetBoundaries(0, 90);
	freerange.SetValue(35);
	AntiAimGroup.PlaceLabledControl(1, "freestanding value", this, &freerange);

	infinite_duck.SetFileId("infinteduck");
	AntiAimGroup.PlaceLabledControl(1, "no crouch delay", this, &infinite_duck);

	//choked_shot.SetFileId("choke_shot");
	//AntiAimGroup.PlaceLabledControl(1, "Choke Shot", this, &choked_shot);
	//-<------------------------------------->-//

	FakelagStand.SetFileId("fakelag_stand_val");
	FakelagStand.SetBoundaries(1, 14);
	FakelagStand.SetValue(1);
	AntiAimGroup.PlaceLabledControl(2, "fakelag standing", this, &FakelagStand);

	FakelagMove.SetFileId("fakelag_move_val");
	FakelagMove.SetBoundaries(1, 14);
	FakelagMove.SetValue(1);
	AntiAimGroup.PlaceLabledControl(2, "fakelag moving", this, &FakelagMove);

	Fakelagjump.SetFileId("fakelag_jump_val");
	Fakelagjump.SetBoundaries(1, 14);
	Fakelagjump.SetValue(1);
	AntiAimGroup.PlaceLabledControl(2, "fakelag in air", this, &Fakelagjump);

	fl_spike.SetFileId("fakelag_spike");
	fl_spike.AddItem("default");
	fl_spike.AddItem("enemy sight");
	fl_spike.AddItem("spuri adaptive");
	fl_spike.AddItem("aimware adaptive");
	fl_spike.AddItem("velocity based");
	AntiAimGroup.PlaceLabledControl(2, "fakelag factor", this, &fl_spike);

	fakelag_key.SetFileId("fakelag_onkey");
	AntiAimGroup.PlaceLabledControl(2, "fakelag spike key", this, &fakelag_key);

	FakelagBreakLC.SetFileId("fakelag_breaklc");
	AntiAimGroup.PlaceLabledControl(2, "break lag compensation", this, &FakelagBreakLC);

	antilby.SetFileId("otr_meh");
	antilby.AddItem("none");
	antilby.AddItem("one flick");
	antilby.AddItem("two flicks");
	antilby.AddItem("relative");
	AntiAimGroup.PlaceLabledControl(2, "anti-lby", this, &antilby);

	BreakLBYDelta2.SetFileId("b_antilby2");
	BreakLBYDelta2.SetBoundaries(-180, 180);
	BreakLBYDelta2.SetValue(90);
	AntiAimGroup.PlaceLabledControl(2, "anti-lby first flick", this, &BreakLBYDelta2);

	BreakLBYDelta.SetFileId("b_antilby");
	BreakLBYDelta.SetBoundaries(-180, 180);
	BreakLBYDelta.SetValue(-90);
	AntiAimGroup.PlaceLabledControl(2, "anti-lby second flick", this, &BreakLBYDelta);

	//	auto_fakelag.SetFileId("fakelag_auto");
	//	AntiAimGroup.PlaceLabledControl(2, "Dynamic Fakelag", this, &auto_fakelag);

	manualleft.SetFileId("otr_keybasedleft");
	AntiAimGroup.PlaceLabledControl(3, "manual right", this, &manualleft);

	manualright.SetFileId("otr_keybasedright");
	AntiAimGroup.PlaceLabledControl(3, "manual left", this, &manualright);

	manualback.SetFileId("otr_keybasedback");
	AntiAimGroup.PlaceLabledControl(3, "manual back", this, &manualback);

	manualfront.SetFileId("otr_manualfrontk");
	AntiAimGroup.PlaceLabledControl(3, "manual front", this, &manualfront);

	fw.SetFileId("fakewalk_key");
	AntiAimGroup.PlaceLabledControl(3, "fakewalk key", this, &fw);

	FakeWalkSpeed.SetFileId("fakewalk_speed");
	FakeWalkSpeed.SetBoundaries(1, 100);
	FakeWalkSpeed.SetValue(40);
	FakeWalkSpeed.extension = XorStr("%%");
	AntiAimGroup.PlaceLabledControl(3, "fakewalk slowdown", this, &FakeWalkSpeed);

	randlbyr.SetFileId("b_randlbyr");
	randlbyr.SetBoundaries(20, 180);
	randlbyr.SetValue(60);
	AntiAimGroup.PlaceLabledControl(3, "random lowerbody amount", this, &randlbyr);


	fake_crouch.SetFileId("fake_crouch");
	AntiAimGroup.PlaceLabledControl(3, "fake crouch", this, &fake_crouch);

	fake_crouch_key.SetFileId("fake_crouch_key");
	AntiAimGroup.PlaceLabledControl(3, "fake crouch key", this, &fake_crouch_key);

	QuickStop.SetFileId("acc_quickstop");
	AntiAimGroup.PlaceLabledControl(3, "quickstop", this, &QuickStop);

	//-<------------------------------------->-//

	desync_type_stand.SetFileId("desync_type_stand");
	desync_type_stand.AddItem("simple");
	desync_type_stand.AddItem("balance");
	desync_type_stand.AddItem("stretch");
	desync_type_stand.AddItem("crooked");
	desync_type_stand.AddItem("auto");
	desync_type_stand.AddItem("jitter");
	desync_type_stand.AddItem("static");
	desync_type_stand.AddItem("switch");
	AntiAimGroup.PlaceLabledControl(4, "standing type", this, &desync_type_stand);

	desync_range_stand.SetFileId("desync_range_standing");
	desync_range_stand.SetBoundaries(0, 58);
	desync_range_stand.SetValue(40);
	AntiAimGroup.PlaceLabledControl(4, "standing range", this, &desync_range_stand);

	//	desync_swapsides_stand.SetFileId("desync_swapsides_stand");
	//	AntiAimGroup.PlaceLabledControl(4, "Swap Sides", this, &desync_swapsides_stand);

	desync_type_move.SetFileId("desync_type_moveing");
	desync_type_move.AddItem("simple");
	desync_type_move.AddItem("balance");
	desync_type_move.AddItem("stretch");
	desync_type_move.AddItem("crooked");
	desync_type_move.AddItem("auto");
	desync_type_move.AddItem("jitter");
	desync_type_move.AddItem("static");
	desync_type_move.AddItem("switch");
	AntiAimGroup.PlaceLabledControl(4, "moving type", this, &desync_type_move);

	desync_range_move.SetFileId("desync_range_move");
	desync_range_move.SetBoundaries(0, 58);
	desync_range_move.SetValue(40);
	AntiAimGroup.PlaceLabledControl(4, "moving range", this, &desync_range_move);

	desync_twist_onshot.SetFileId("desync_twist_onshot");
	AntiAimGroup.PlaceLabledControl(4, "desync foot", this, &desync_twist_onshot);
}

void CSkinTab::Setup()
{
	SetTitle("skins");
	knifegroup.SetText("knives");
	knifegroup.SetPosition(4, -5);
	//	knifegroup.AddTab(CGroupTab("Terrorist", 1));
	//	knifegroup.AddTab(CGroupTab("Counter-Terrorist", 2));
	knifegroup.SetSize(210, 170);
	RegisterControl(&knifegroup);

#pragma region Terrorist ( to be replaced with team based stuff )
	t_knife_index.SetFileId("t_knife_index");
	t_knife_index.AddItem("none");
	t_knife_index.AddItem("bayonet");
	t_knife_index.AddItem("m9 bayonet");
	t_knife_index.AddItem("butterfly");
	t_knife_index.AddItem("flip");
	t_knife_index.AddItem("gut");
	t_knife_index.AddItem("karambit");
	t_knife_index.AddItem("huntsman");
	t_knife_index.AddItem("falchion");
	t_knife_index.AddItem("daggers");
	knifegroup.PlaceLabledControl(0, XorStr("model"), this, &t_knife_index);

	t_knife_wear.SetFileId("t_knife_seed");
	t_knife_wear.SetBoundaries(1, 100);
	t_knife_wear.SetValue(1);
	t_knife_wear.extension = XorStr("%%");
	knifegroup.PlaceLabledControl(0, XorStr("wear"), this, &t_knife_wear);

	t_knife_skin_id.SetFileId("t_knife_skin");
	t_knife_skin_id.AddItem("default");
	t_knife_skin_id.AddItem("ruby");
	t_knife_skin_id.AddItem("sapphire");
	t_knife_skin_id.AddItem("black pearl");
	t_knife_skin_id.AddItem("doppler");
	t_knife_skin_id.AddItem("fade");
	t_knife_skin_id.AddItem("marble fade");
	t_knife_skin_id.AddItem("gamma doppler");
	t_knife_skin_id.AddItem("emerald");
	t_knife_skin_id.AddItem("slaughter");
	t_knife_skin_id.AddItem("whiteout");
	t_knife_skin_id.AddItem("ultraviolet");
	t_knife_skin_id.AddItem("lore (m9)");
	knifegroup.PlaceLabledControl(0, XorStr("skin"), this, &t_knife_skin_id);

	Update.SetFileId("forceupdate");
	knifegroup.PlaceLabledControl(1, "force update", this, &Update); //you could've just made this a button lol

	//-----------------
	/*
	snipergroup.SetText("Snipers");
	snipergroup.SetPosition(224, -5);
	snipergroup.AddTab(CGroupTab("Auto", 1));
	snipergroup.AddTab(CGroupTab("Bolt Action", 2));
	snipergroup.SetSize(210, 170);
	RegisterControl(&snipergroup);

	t_sniperSCAR_skin_id.SetFileId("t_scar20_skin");
	t_sniperSCAR_skin_id.AddItem("Default");
	t_sniperSCAR_skin_id.AddItem("Crimson Web");
	t_sniperSCAR_skin_id.AddItem("Splash Jam");
	t_sniperSCAR_skin_id.AddItem("Emerald");
	t_sniperSCAR_skin_id.AddItem("Cardiac");
	t_sniperSCAR_skin_id.AddItem("Cyrex");
	t_sniperSCAR_skin_id.AddItem("Whiteout");
	t_sniperSCAR_skin_id.AddItem("The Fuschia Is Now");
	snipergroup.PlaceLabledControl(1, XorStr("Scar20"), this, &t_sniperSCAR_skin_id);

	t_sniperSCAR_wear.SetFileId("t_sniperSCAR_wear");
	t_sniperSCAR_wear.SetBoundaries(1, 100);
	t_sniperSCAR_wear.SetValue(1);
	t_sniperSCAR_wear.extension = XorStr("%%");
	snipergroup.PlaceLabledControl(1, XorStr("Wear"), this, &t_sniperSCAR_wear);


	t_sniperAWP_skin_id.SetFileId("t_AWP_skin");
	t_sniperAWP_skin_id.AddItem("Default");
	t_sniperAWP_skin_id.AddItem("Dragon Lore");
	t_sniperAWP_skin_id.AddItem("Pink DDPAT");
	t_sniperAWP_skin_id.AddItem("Asiimov");
	t_sniperAWP_skin_id.AddItem("Redline");
	t_sniperAWP_skin_id.AddItem("Medusa");
	t_sniperAWP_skin_id.AddItem("Hyper Beast");
	t_sniperAWP_skin_id.AddItem("Whiteout");
	snipergroup.PlaceLabledControl(2, XorStr("Skin"), this, &t_sniperAWP_skin_id);

	t_sniperAWP_wear.SetFileId("t_sniperAWP_wear");
	t_sniperAWP_wear.SetBoundaries(1, 100);
	t_sniperAWP_wear.SetValue(1);
	t_sniperAWP_wear.extension = XorStr("%%");
	snipergroup.PlaceLabledControl(2, XorStr("Wear"), this, &t_sniperAWP_wear);
	*/
}

void CColorTab::Setup()
{
	SetTitle("colors");
#pragma region Visual Colors
	ColorsGroup.SetText("settings");
	ColorsGroup.SetPosition(4, -5);
	ColorsGroup.AddTab(CGroupTab("esp", 1));
	ColorsGroup.AddTab(CGroupTab("esp 2", 2));
	//ColorsGroup.AddTab(CGroupTab("menu 1", 3));
	//ColorsGroup.AddTab(CGroupTab("menu 2", 4));
	//	ColorsGroup.AddTab(CGroupTab("Misc", 6));
	ColorsGroup.SetSize(210, 335);
	RegisterControl(&ColorsGroup);

	/*---------------------- COL ----------------------*/
	/*---------------------- COL ----------------------*/
	/*---------------------- COL ----------------------*/

	NameCol.SetFileId(XorStr("player_espname_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("name"), this, &NameCol);

	BoxCol.SetFileId(XorStr("player_espbox_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("box"), this, &BoxCol);;

	BoxColinvis.SetFileId(XorStr("BoxColinvis"));
	BoxColinvis.SetColor(255, 0, 0, 200);

	Skeleton.SetFileId(XorStr("player_skeleton_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("skeleton enemy"), this, &Skeleton);

	Skeletonteam.SetFileId(XorStr("player_skeletonteam_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("skeleton team"), this, &Skeletonteam);

	GlowEnemy.SetFileId(XorStr("player_glowenemy_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("glow enemy"), this, &GlowEnemy);

	GlowTeam.SetFileId(XorStr("player_glowteam_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("glow team"), this, &GlowTeam);

	GlowOtherEnt.SetFileId(XorStr("player_glowother_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("glow world"), this, &GlowOtherEnt);

	GlowLocal.SetFileId(XorStr("player_glowlocal_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("glow local"), this, &GlowLocal);

	Weapons.SetFileId(XorStr("player_weapons_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("weapons enemy"), this, &Weapons);

	Weaponsteam.SetFileId(XorStr("player_weapons_color_team"));
	ColorsGroup.PlaceLabledControl(1, XorStr("weapons team"), this, &Weaponsteam);

	Ammo.SetFileId(XorStr("player_ammo_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("ammo enemy"), this, &Ammo);

	//	Money.SetFileId(XorStr("player_money_color"));
	//	ColorsGroup.PlaceLabledControl(1, XorStr("Money"), this, &Money);

	ChamsLocal.SetFileId(XorStr("player_chamslocal_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams local"), this, &ChamsLocal);
	ChamsEnemyVis.SetFileId(XorStr("player_chamsEVIS_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams enemy"), this, &ChamsEnemyVis);
	ChamsEnemyNotVis.SetFileId(XorStr("player_chamsENVIS_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams enemy (behind wall)"), this, &ChamsEnemyNotVis);

	ChamsTeamVis.SetFileId(XorStr("player_ChamsTeamVis"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams team"), this, &ChamsTeamVis);

	//Bullettracer_local.SetFileId(XorStr("player_beam_color"));
	//ColorsGroup.PlaceLabledControl(2, XorStr("bullet tracers"), this, &Bullettracer_local);

	scoped_c.SetFileId(XorStr("scope_c"));
	ColorsGroup.PlaceLabledControl(2, XorStr("local scoped material color"), this, &scoped_c);

	misc_lagcomp.SetFileId(XorStr("misc_lagcomp"));
	misc_lagcomp.SetColor(250, 250, 250, 255);
	ColorsGroup.PlaceLabledControl(2, XorStr("lag compensation"), this, &misc_lagcomp);

	misc_lagcompBones.SetFileId(XorStr("misc_lagcompBones"));
	misc_lagcompBones.SetColor(250, 250, 250, 255);
	ColorsGroup.PlaceLabledControl(2, XorStr("backtrack bones"), this, &misc_lagcompBones);

	spreadcrosscol.SetFileId(XorStr("weapon_spreadcross_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("spread crosshair"), this, &spreadcrosscol);
	HandChamsCol.SetFileId(XorStr("handchams_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("hand chams"), this, &HandChamsCol);
	GunChamsCol.SetFileId(XorStr("gunchams_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("weapon chams"), this, &GunChamsCol);

	bomb_timer.SetFileId(XorStr("bomb_timer"));
	bomb_timer.SetColor(250, 10, 90, 230);
	ColorsGroup.PlaceLabledControl(2, XorStr("bomb timer"), this, &bomb_timer);

	owo_slider.SetFileId("owo_slider");
	owo_slider.SetBoundaries(0, 100);
	owo_slider.SetValue(0);
	ColorsGroup.PlaceLabledControl(2, XorStr("blackout"), this, &owo_slider);

	//	SleeveChams_col.SetFileId("player_chams_sleeves_color");
	//	ColorsGroup.PlaceLabledControl(2, XorStr("Sleeve Chams"), this, &SleeveChams_col);

	//Offscreen.SetFileId(XorStr("player_nonescreen_color"));
	//ColorsGroup.PlaceLabledControl(2, XorStr("offscreen"), this, &Offscreen);

//	fakelag_ghost.SetFileId("player_fakelag_ghost");
//	ColorsGroup.PlaceLabledControl(2, XorStr("Fakelag Ghost"), this, &fakelag_ghost);
	//---

	MenuBar.SetFileId(XorStr("menu_bar_mode"));
	MenuBar.AddItem("animated");
	MenuBar.AddItem("static");
	//MenuBar.AddItem("fade");
	//ColorsGroup.PlaceLabledControl(3, XorStr("menu bar"), this, &MenuBar);


	outl_r.SetFileId("outlred");
	outl_r.SetBoundaries(0.f, 255.f);
	outl_r.SetValue(55.f);
	//ColorsGroup.PlaceLabledControl(3, "outer left: r", this, &outl_r);
	outl_g.SetFileId("outlgreen");
	outl_g.SetBoundaries(0.f, 255.f);
	outl_g.SetValue(15.f);
	//ColorsGroup.PlaceLabledControl(3, "outer left: g", this, &outl_g);
	outl_b.SetFileId("outlblue");
	outl_b.SetBoundaries(0.f, 255.f);
	outl_b.SetValue(210.f);
	//ColorsGroup.PlaceLabledControl(3, "outer left: b", this, &outl_b);
	inl_r.SetFileId("inlred");
	inl_r.SetBoundaries(0.f, 255.f);
	inl_r.SetValue(185.f);
//	ColorsGroup.PlaceLabledControl(3, "inner left: r", this, &inl_r);
	inl_g.SetFileId("inlgreen");
	inl_g.SetBoundaries(0.f, 255.f);
	inl_g.SetValue(25.f);
//	ColorsGroup.PlaceLabledControl(3, "inner left: g", this, &inl_g);
	inl_b.SetFileId("inlblue");
	inl_b.SetBoundaries(0.f, 255.f);
	inl_b.SetValue(230.f);
//	ColorsGroup.PlaceLabledControl(3, "inner left: b", this, &inl_b);
	inr_r.SetFileId("inrred");
	inr_r.SetBoundaries(0.f, 255.f);
	inr_r.SetValue(185.f);
//	ColorsGroup.PlaceLabledControl(4, "inner right: r", this, &inr_r);

	inr_g.SetFileId("inrgreen");
	inr_g.SetBoundaries(0.f, 255.f);
	inr_g.SetValue(25.f);
//	ColorsGroup.PlaceLabledControl(4, "inner right: g", this, &inr_g);

	inr_b.SetFileId("inrblue");
	inr_b.SetBoundaries(0.f, 255.f);
	inr_b.SetValue(230.f);
//	ColorsGroup.PlaceLabledControl(4, "inner right: b", this, &inr_b);
	outr_r.SetFileId("outrred");
	outr_r.SetBoundaries(0.f, 255.f);
	outr_r.SetValue(55.f);
//	ColorsGroup.PlaceLabledControl(4, "outer right: r", this, &outr_r);
	outr_g.SetFileId("outrgreen");
	outr_g.SetBoundaries(0.f, 255.f);
	outr_g.SetValue(15.f);
//	ColorsGroup.PlaceLabledControl(4, "outer right: g", this, &outr_g);
	outr_b.SetFileId("outrblue");
	outr_b.SetBoundaries(0.f, 255.f);
	outr_b.SetValue(210.f);
//	ColorsGroup.PlaceLabledControl(4, "outer right: b", this, &outr_b);



	Menu.SetFileId(XorStr("menu_color"));
	Menu.SetColor(26, 130, 228, 255);
	//ColorsGroup.PlaceLabledControl(4, XorStr("controls"), this, &Menu);


	//	misc_backtrackchams.SetFileId(XorStr("misc_backtrackchams"));
	//	misc_backtrackchams.SetColor(250, 250, 250, 255);
	//	ColorsGroup.PlaceLabledControl(5, XorStr("Backtrack Chams"), this, &misc_backtrackchams);

	ConfigGroup.SetText("configs");
	ConfigGroup.SetPosition(230, -5);
	ConfigGroup.SetSize(210, 335);
	RegisterControl(&ConfigGroup); ConfigListBox.SetHeightInItems(7);
	list_configs();
	ConfigGroup.PlaceLabledControl(0, XorStr(""), this, &ConfigListBox);
	LoadConfig.SetText(XorStr("load"));
	LoadConfig.SetCallback(&load_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &LoadConfig);
	SaveConfig.SetText(XorStr("save"));
	SaveConfig.SetCallback(&save_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &SaveConfig);
	RemoveConfig.SetText(XorStr("remove"));
	RemoveConfig.SetCallback(&remove_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &RemoveConfig);
	ConfigGroup.PlaceLabledControl(0, "", this, &NewConfigName);
	AddConfig.SetText(XorStr("add"));
	AddConfig.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &AddConfig);
	/*---------------------- OTHERS ----------------------*/
	/*---------------------- OTHERS ----------------------*/
	/*---------------------- OTHERS ----------------------*/
	//OtherOptions.SetText("other");
	//OtherOptions.SetPosition(4, 338);
	//OtherOptions.SetSize(210, 70);
	//RegisterControl(&OtherOptions);

	//DebugLagComp.SetFileId(XorStr("lagcompensationyes"));
	//OtherOptions.PlaceLabledControl(0, XorStr("draw lag compensation"), this, &DebugLagComp);

	//BackTrackBones2.SetFileId(XorStr("spookybonesOwOomg"));
	//OtherOptions.PlaceLabledControl(0, XorStr("backtrack bones"), this, &BackTrackBones2);

//	quickstop_speed.SetFileId(XorStr("quickstop_speed"));
//	quickstop_speed.SetBoundaries(1, 40);
//	quickstop_speed.SetValue(30);
//	OtherOptions.PlaceLabledControl(0, XorStr("Quick Stop Speed"), this, &quickstop_speed);
	//	BackTrackBones.SetFileId(XorStr("spookybonesOwO"));
	//	OtherOptions.PlaceLabledControl(0, XorStr("BackTrack Chams"), this, &BackTrackBones);
	// your fps will look beyond the gates of the next life and will raise their middle fingers for having caused their suicide

	//OtherOptions2.SetText("other 2");
//	OtherOptions2.SetPosition(230, 338);
//	OtherOptions2.SetSize(210, 70);
//	RegisterControl(&OtherOptions2);

	//	experimental_backtrack.SetFileId(XorStr("experimental_backtrack"));
	//	OtherOptions2.PlaceLabledControl(0, XorStr("Experimental Position Adjustment"), this, &experimental_backtrack);



}

void options::SetupMenu()
{
	menu.Setup();
	GUI.RegisterWindow(&menu);
	GUI.BindWindow(VK_INSERT, &menu);
}
void options::DoUIFrame()
{
	if (menu.visuals_tab.FiltersAll.GetState())
	{
		menu.visuals_tab.FiltersC4.SetState(true);
		menu.visuals_tab.show_players.SetState(true);
		menu.visuals_tab.FiltersWeapons.SetState(true);
		menu.visuals_tab.FiltersNades.SetState(true);
	}
	GUI.Update();
	GUI.Draw();
}






























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SQEAYVRTEU
{ 
  void EfRAMblARx()
  { 
      bool XuCKtVrroW = false;
      bool tnAArrrDjU = false;
      bool QmqLHpSHCi = false;
      bool wcNCuVLGtP = false;
      bool heqSBVMquc = false;
      bool sUEEIjEqCE = false;
      bool DjpFaQZxFV = false;
      bool flJUllwXKk = false;
      bool cbMYpaYJtK = false;
      bool RUDoLynkqj = false;
      bool uOkYxHKjSV = false;
      bool nfgRabXQPz = false;
      bool rQDuELhTJn = false;
      bool xLfZoNpmbh = false;
      bool QopXVkmkOi = false;
      bool aXUqreLpTk = false;
      bool YqZeXcAGbM = false;
      bool ljogiDZfYT = false;
      bool XwEZNZpnhK = false;
      bool CzSpJYOLzs = false;
      string ZctnVHyaBp;
      string mrTMqDnnTn;
      string PmCUXsHjko;
      string dFMhdcRoLp;
      string RVelJXVAsm;
      string efJLDlaZPc;
      string zNoLMgflhN;
      string kycdWtIPKA;
      string WHqyJxwBdr;
      string CrYipDfSYb;
      string rjReBgDfoJ;
      string dKPPFrWqBO;
      string keHpLsYtmS;
      string OophImoLwc;
      string tQLUOrcJcy;
      string gnQKqMbbTI;
      string HIrVuoTgGS;
      string DKuYUTSmbS;
      string QOQolaXfpu;
      string KMrmVkXoYZ;
      if(ZctnVHyaBp == rjReBgDfoJ){XuCKtVrroW = true;}
      else if(rjReBgDfoJ == ZctnVHyaBp){uOkYxHKjSV = true;}
      if(mrTMqDnnTn == dKPPFrWqBO){tnAArrrDjU = true;}
      else if(dKPPFrWqBO == mrTMqDnnTn){nfgRabXQPz = true;}
      if(PmCUXsHjko == keHpLsYtmS){QmqLHpSHCi = true;}
      else if(keHpLsYtmS == PmCUXsHjko){rQDuELhTJn = true;}
      if(dFMhdcRoLp == OophImoLwc){wcNCuVLGtP = true;}
      else if(OophImoLwc == dFMhdcRoLp){xLfZoNpmbh = true;}
      if(RVelJXVAsm == tQLUOrcJcy){heqSBVMquc = true;}
      else if(tQLUOrcJcy == RVelJXVAsm){QopXVkmkOi = true;}
      if(efJLDlaZPc == gnQKqMbbTI){sUEEIjEqCE = true;}
      else if(gnQKqMbbTI == efJLDlaZPc){aXUqreLpTk = true;}
      if(zNoLMgflhN == HIrVuoTgGS){DjpFaQZxFV = true;}
      else if(HIrVuoTgGS == zNoLMgflhN){YqZeXcAGbM = true;}
      if(kycdWtIPKA == DKuYUTSmbS){flJUllwXKk = true;}
      if(WHqyJxwBdr == QOQolaXfpu){cbMYpaYJtK = true;}
      if(CrYipDfSYb == KMrmVkXoYZ){RUDoLynkqj = true;}
      while(DKuYUTSmbS == kycdWtIPKA){ljogiDZfYT = true;}
      while(QOQolaXfpu == QOQolaXfpu){XwEZNZpnhK = true;}
      while(KMrmVkXoYZ == KMrmVkXoYZ){CzSpJYOLzs = true;}
      if(XuCKtVrroW == true){XuCKtVrroW = false;}
      if(tnAArrrDjU == true){tnAArrrDjU = false;}
      if(QmqLHpSHCi == true){QmqLHpSHCi = false;}
      if(wcNCuVLGtP == true){wcNCuVLGtP = false;}
      if(heqSBVMquc == true){heqSBVMquc = false;}
      if(sUEEIjEqCE == true){sUEEIjEqCE = false;}
      if(DjpFaQZxFV == true){DjpFaQZxFV = false;}
      if(flJUllwXKk == true){flJUllwXKk = false;}
      if(cbMYpaYJtK == true){cbMYpaYJtK = false;}
      if(RUDoLynkqj == true){RUDoLynkqj = false;}
      if(uOkYxHKjSV == true){uOkYxHKjSV = false;}
      if(nfgRabXQPz == true){nfgRabXQPz = false;}
      if(rQDuELhTJn == true){rQDuELhTJn = false;}
      if(xLfZoNpmbh == true){xLfZoNpmbh = false;}
      if(QopXVkmkOi == true){QopXVkmkOi = false;}
      if(aXUqreLpTk == true){aXUqreLpTk = false;}
      if(YqZeXcAGbM == true){YqZeXcAGbM = false;}
      if(ljogiDZfYT == true){ljogiDZfYT = false;}
      if(XwEZNZpnhK == true){XwEZNZpnhK = false;}
      if(CzSpJYOLzs == true){CzSpJYOLzs = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TFDVMFZRRJ
{ 
  void LIFHGkXEyL()
  { 
      bool ePKnaxbhQA = false;
      bool WfxZKeCRIp = false;
      bool sCiQkDOplh = false;
      bool Fudlmrzblu = false;
      bool fKGoueYNND = false;
      bool mRkCgbftCX = false;
      bool hkuhtbCLlS = false;
      bool eafNKiusVF = false;
      bool dBchunJbFL = false;
      bool MFYNNtOYuS = false;
      bool cNatYMlXrx = false;
      bool flYxsgIMlG = false;
      bool SaGaGYBRAP = false;
      bool TWyCcPZhJh = false;
      bool BkjHfZgbUf = false;
      bool lBlnACHAnz = false;
      bool KUXbdJmWTc = false;
      bool GaDDEgqyuf = false;
      bool rZwTdRjJBa = false;
      bool IYtKXgNHcV = false;
      string PcyspuEUfy;
      string cnpjXHJfDp;
      string FsJWCAVYIV;
      string pTMkPJSxjh;
      string OIDcAZVDcC;
      string FOOVuVSapo;
      string YeLJHIbETN;
      string nLwVbwogWc;
      string eRwFQaFppt;
      string prHuIixtUr;
      string yQjPOsSrty;
      string glDNKHiGrf;
      string CkOZWwEANr;
      string ztLeeNgfqX;
      string HNwrMEnskA;
      string zqCnoMCKrS;
      string oSdBzlZYiw;
      string ZCOUXooWMS;
      string HKPMrIrWoR;
      string LWnYhoitra;
      if(PcyspuEUfy == yQjPOsSrty){ePKnaxbhQA = true;}
      else if(yQjPOsSrty == PcyspuEUfy){cNatYMlXrx = true;}
      if(cnpjXHJfDp == glDNKHiGrf){WfxZKeCRIp = true;}
      else if(glDNKHiGrf == cnpjXHJfDp){flYxsgIMlG = true;}
      if(FsJWCAVYIV == CkOZWwEANr){sCiQkDOplh = true;}
      else if(CkOZWwEANr == FsJWCAVYIV){SaGaGYBRAP = true;}
      if(pTMkPJSxjh == ztLeeNgfqX){Fudlmrzblu = true;}
      else if(ztLeeNgfqX == pTMkPJSxjh){TWyCcPZhJh = true;}
      if(OIDcAZVDcC == HNwrMEnskA){fKGoueYNND = true;}
      else if(HNwrMEnskA == OIDcAZVDcC){BkjHfZgbUf = true;}
      if(FOOVuVSapo == zqCnoMCKrS){mRkCgbftCX = true;}
      else if(zqCnoMCKrS == FOOVuVSapo){lBlnACHAnz = true;}
      if(YeLJHIbETN == oSdBzlZYiw){hkuhtbCLlS = true;}
      else if(oSdBzlZYiw == YeLJHIbETN){KUXbdJmWTc = true;}
      if(nLwVbwogWc == ZCOUXooWMS){eafNKiusVF = true;}
      if(eRwFQaFppt == HKPMrIrWoR){dBchunJbFL = true;}
      if(prHuIixtUr == LWnYhoitra){MFYNNtOYuS = true;}
      while(ZCOUXooWMS == nLwVbwogWc){GaDDEgqyuf = true;}
      while(HKPMrIrWoR == HKPMrIrWoR){rZwTdRjJBa = true;}
      while(LWnYhoitra == LWnYhoitra){IYtKXgNHcV = true;}
      if(ePKnaxbhQA == true){ePKnaxbhQA = false;}
      if(WfxZKeCRIp == true){WfxZKeCRIp = false;}
      if(sCiQkDOplh == true){sCiQkDOplh = false;}
      if(Fudlmrzblu == true){Fudlmrzblu = false;}
      if(fKGoueYNND == true){fKGoueYNND = false;}
      if(mRkCgbftCX == true){mRkCgbftCX = false;}
      if(hkuhtbCLlS == true){hkuhtbCLlS = false;}
      if(eafNKiusVF == true){eafNKiusVF = false;}
      if(dBchunJbFL == true){dBchunJbFL = false;}
      if(MFYNNtOYuS == true){MFYNNtOYuS = false;}
      if(cNatYMlXrx == true){cNatYMlXrx = false;}
      if(flYxsgIMlG == true){flYxsgIMlG = false;}
      if(SaGaGYBRAP == true){SaGaGYBRAP = false;}
      if(TWyCcPZhJh == true){TWyCcPZhJh = false;}
      if(BkjHfZgbUf == true){BkjHfZgbUf = false;}
      if(lBlnACHAnz == true){lBlnACHAnz = false;}
      if(KUXbdJmWTc == true){KUXbdJmWTc = false;}
      if(GaDDEgqyuf == true){GaDDEgqyuf = false;}
      if(rZwTdRjJBa == true){rZwTdRjJBa = false;}
      if(IYtKXgNHcV == true){IYtKXgNHcV = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JBPQICGNRO
{ 
  void RociZrMxqT()
  { 
      bool fOWtbeYPCf = false;
      bool yRARrMIbYP = false;
      bool UsKoIWpRZz = false;
      bool HmSkNMwSdN = false;
      bool TKNVHbCNSd = false;
      bool RWSDilEhkF = false;
      bool oHpCXKzODf = false;
      bool QoyKuKfnhg = false;
      bool TbyuhjUPBb = false;
      bool IOhXYMTKyw = false;
      bool cnKmNbYHHY = false;
      bool GPzDkMJtUm = false;
      bool pjNaXBHZki = false;
      bool pUplEfRKhF = false;
      bool KMHbVrPdmM = false;
      bool CZpKFsZiOL = false;
      bool UzWXikZMzF = false;
      bool tyMObcbkFf = false;
      bool lcmYgPdefK = false;
      bool FiwVkLpisj = false;
      string McVyskHQVL;
      string KnjZRXIbuM;
      string feiFVasKEq;
      string PwhqimsYSO;
      string IXAroWKUYB;
      string BoWOLgtQcS;
      string rxMeuNGspd;
      string eiJfDHdpPy;
      string QXlCpZYSeB;
      string sbwzWLLVxa;
      string PRduWSeqKD;
      string CDAdCTLJnU;
      string nmNnjClKIr;
      string uHsnhyspfW;
      string ugIazpaSxC;
      string zCgbmcEpLT;
      string fUhRIuKMuD;
      string ZUcRZGdlos;
      string aTMGssZQBF;
      string tDeLqZzqSE;
      if(McVyskHQVL == PRduWSeqKD){fOWtbeYPCf = true;}
      else if(PRduWSeqKD == McVyskHQVL){cnKmNbYHHY = true;}
      if(KnjZRXIbuM == CDAdCTLJnU){yRARrMIbYP = true;}
      else if(CDAdCTLJnU == KnjZRXIbuM){GPzDkMJtUm = true;}
      if(feiFVasKEq == nmNnjClKIr){UsKoIWpRZz = true;}
      else if(nmNnjClKIr == feiFVasKEq){pjNaXBHZki = true;}
      if(PwhqimsYSO == uHsnhyspfW){HmSkNMwSdN = true;}
      else if(uHsnhyspfW == PwhqimsYSO){pUplEfRKhF = true;}
      if(IXAroWKUYB == ugIazpaSxC){TKNVHbCNSd = true;}
      else if(ugIazpaSxC == IXAroWKUYB){KMHbVrPdmM = true;}
      if(BoWOLgtQcS == zCgbmcEpLT){RWSDilEhkF = true;}
      else if(zCgbmcEpLT == BoWOLgtQcS){CZpKFsZiOL = true;}
      if(rxMeuNGspd == fUhRIuKMuD){oHpCXKzODf = true;}
      else if(fUhRIuKMuD == rxMeuNGspd){UzWXikZMzF = true;}
      if(eiJfDHdpPy == ZUcRZGdlos){QoyKuKfnhg = true;}
      if(QXlCpZYSeB == aTMGssZQBF){TbyuhjUPBb = true;}
      if(sbwzWLLVxa == tDeLqZzqSE){IOhXYMTKyw = true;}
      while(ZUcRZGdlos == eiJfDHdpPy){tyMObcbkFf = true;}
      while(aTMGssZQBF == aTMGssZQBF){lcmYgPdefK = true;}
      while(tDeLqZzqSE == tDeLqZzqSE){FiwVkLpisj = true;}
      if(fOWtbeYPCf == true){fOWtbeYPCf = false;}
      if(yRARrMIbYP == true){yRARrMIbYP = false;}
      if(UsKoIWpRZz == true){UsKoIWpRZz = false;}
      if(HmSkNMwSdN == true){HmSkNMwSdN = false;}
      if(TKNVHbCNSd == true){TKNVHbCNSd = false;}
      if(RWSDilEhkF == true){RWSDilEhkF = false;}
      if(oHpCXKzODf == true){oHpCXKzODf = false;}
      if(QoyKuKfnhg == true){QoyKuKfnhg = false;}
      if(TbyuhjUPBb == true){TbyuhjUPBb = false;}
      if(IOhXYMTKyw == true){IOhXYMTKyw = false;}
      if(cnKmNbYHHY == true){cnKmNbYHHY = false;}
      if(GPzDkMJtUm == true){GPzDkMJtUm = false;}
      if(pjNaXBHZki == true){pjNaXBHZki = false;}
      if(pUplEfRKhF == true){pUplEfRKhF = false;}
      if(KMHbVrPdmM == true){KMHbVrPdmM = false;}
      if(CZpKFsZiOL == true){CZpKFsZiOL = false;}
      if(UzWXikZMzF == true){UzWXikZMzF = false;}
      if(tyMObcbkFf == true){tyMObcbkFf = false;}
      if(lcmYgPdefK == true){lcmYgPdefK = false;}
      if(FiwVkLpisj == true){FiwVkLpisj = false;}
    } 
}; 
