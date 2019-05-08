#pragma once
#include "GUI.h"
#include "Controls.h"
class CAimbotTab : public CTab
{
public:
	void Setup();
	CLabel ActiveLabel;
	CCheckBox Active;

	CComboBox psilent;
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox lag_pred;
	CCheckBox backtrack;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotSilentAim;
	CCheckBox legit_mode;
	CCheckBox legit_trigger;
	CKeyBind legit_trigger_key;
	CCheckBox apply_smooth;
	CComboBox AimbotResolver;
	CSlider AimbotFov;
	//-------------------------
	//--------------------------
	CCheckBox LowerbodyFix;
	CCheckBox PreferBodyAim;
	CCheckBox AWPAtBody;
	CSlider BaimIfUnderXHealth;
	CSlider XShots;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox pointscaleyes;
	CCheckBox Multienable;
	CSlider pointscaleval;
	CSlider Multival;
	CSlider MultiVal3;
	CCheckBox nospread;
	CSlider AimbotAimStep2;
	CSlider shotlimit;
	CCheckBox AimbotKeyPress;

	CCheckBox baim_fake;
	CCheckBox baim_inair;
	CCheckBox baim_fakewalk;
	CCheckBox baim_lethal;
	CCheckBox baim_muslim;

	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;
	CGroupBox TargetGroup;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CComboBox delay_shot;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;
	CSlider   bruteX;
	CSlider   baim;

	//--------------//
	CGroupBox multipoint;


	//--------------//
	CGroupBox weapongroup;

	CSlider hc_auto;
	CSlider hc_scout;
	CSlider hc_awp;
	CSlider hc_pistol;
	CSlider hc_smg;
	CSlider hc_otr;

	CSlider md_auto;
	CSlider md_scout;
	CSlider md_awp;
	CSlider md_pistol;
	CSlider md_smg;
	CSlider md_otr;

	// CComboBox target_auto;
	CDropBox target_auto;
	CDropBox target_scout;
	CDropBox target_awp;
	CDropBox target_pistol;
	CDropBox target_smg;
	CDropBox target_otr;

	CCheckBox enemyhp_auto;
	CCheckBox enemyhp_scout;
	CCheckBox enemyhp_awp;
	CCheckBox enemyhp_pistol;

	//--------------//

	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;


	CCheckBox AccuracyAutoWall;
	CSlider AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CSlider Multival2;

	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider   AccuracyHitchance;
	CComboBox resolver;

	CComboBox preso;

	CCheckBox custom_hitscan;

	CComboBox OverrideMode;
	CKeyBind bigbaim;
	CKeyBind flip180;
	CCheckBox extrapolation;

	CCheckBox toggledebug;
	CDropBox debug;
};
class CLegitBotTab : public CTab
{
public:
	void Setup();
	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox WeaponSnipPSilent;
	CCheckBox WeaponPistPSilent;
	CSlider2 WeaponMainRecoil;
	CSlider2 WeaponRifleRecoilX;
	CSlider2 WeaponRifleRecoilY;
	CCheckBox WeaponRifleRecoilAYY;
	
	CComboBox aimbotfiremode;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CSlider2 WeaponMainrcs;
	CCheckBox AimbotSmokeCheck;
	CCheckBox BackTrack;
	CSlider2 weaponmpinacc;
	CCheckBox legitresolver;
	CGroupBox TriggerGroup;
	CComboBox triggertype;
	CCheckBox TriggerEnable;
	CKeyBind  TriggerKeyBind;
	CSlider2   TriggerDelay;
	CSlider2   TriggerBurst;
	CSlider2   TriggerBreak;
	CSlider2 TriggerRecoil; CSlider2   TriggerHitChanceAmmount;
	CGroupBox TriggerFilterGroup;
	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;
	CCheckBox TriggerFlashCheck;
	CCheckBox TriggerSmokeCheck;
	CCheckBox aaenable;
	CComboBox aatyp;
	CComboBox aatyp2;
	CSlider2 aatyp3;
	CSlider2 aimhp;
	CSlider2 aafl;
	CSlider2 trighp;
	CGroupBox weapongroup;
	CSlider2   WeaponMainSpeed;
	CSlider2   WeaponMainFoV;
	CCheckBox legitbacktrack;
	CSlider2 legitbacktrackticks;
	CComboBox WeaponMainHitbox;
	CSlider2 WeaponMainAimtime;
	CSlider2 WeaoponMainStartAimtime;
	CGroupBox WeaponPistGroup;
	CSlider2   WeaponPistSpeed;
	CSlider2   WeaponPistFoV;
	CCheckBox legitaa;

	CSlider2 WeaponPistRecoil;
	CComboBox WeaponPistHitbox;
	CSlider2 WeaponPistAimtime;
	CSlider2 WeaoponPistStartAimtime;
	CCheckBox visualizebacktrack;
	CGroupBox WeaponSnipGroup;
	CSlider2   WeaponSnipSpeed;
	CCheckBox StandAloneRCS;
	CSlider2 StandAloneRCSamount;
	CCheckBox visualizefov;
	CCheckBox geci;
	CComboBox LegitChoice;
	CSlider2 WeaponMainInacc;
	CSlider2   WeaponSnipFoV;
	CSlider2 WeaponSnipRecoil;
	CComboBox WeaponSnipHitbox;
	CSlider2 WeaponSnipAimtime;
	CSlider2 WeaoponSnipStartAimtime;
	CGroupBox WeaponMpGroup;
	CSlider2   WeaponMpSpeed;
	CSlider2   WeaponMpFoV;
	CSlider2 WeaponMpRecoil;
	CComboBox WeaponMpHitbox;
	CSlider2 WeaponMpAimtime;
	CSlider2 WeaoponMpStartAimtime;
	CGroupBox WeaponShotgunGroup;
	CSlider2   WeaponShotgunSpeed;
	CSlider2   WeaponShotgunFoV;
	CSlider2 WeaponShotgunRecoil;
	CComboBox WeaponShotgunHitbox;
	CSlider2 WeaponShotgunAimtime;
	CSlider2 WeaoponShotgunStartAimtime;
	CGroupBox WeaponMGGroup;
	CSlider2   WeaponMGSpeed;
	CSlider2   WeaponMGFoV;
	CSlider2 WeaponMGRecoil;
	CComboBox WeaponMGHitbox;
	CSlider2 WeaponMGAimtime;
	CSlider2 WeaoponMGStartAimtime;




};
class CVisualTab : public CTab
{
public:
	void Setup();
	CLabel ActiveLabel;
	CCheckBox Active;
	CCheckBox logs;
	CCheckBox flashindicator;
	CCheckBox visualize_desync;
	CCheckBox AutowallCrosshair2;
	CCheckBox laser;
	CCheckBox AWHitmarker;
	CCheckBox damageindi;
	CSlider flashAlpha;
	CCheckBox footstep;
	CCheckBox arrows;
	CCheckBox deadesp;
	CGroupBox OptionsGroup;
	CCheckBox leftknife;
	CCheckBox misczoom;
	CKeyBind misczoomkey;
	CCheckBox GrenadePred;
	CCheckBox OptionsWeapone;
	CComboBox OptionsBox;
	CComboBox OptionsName;
	CCheckBox HitmarkerSound;
	CCheckBox OtherHitmarker;
	CCheckBox ChamsEnemyvisible;
	CCheckBox ChamsLocal;
	CCheckBox aa_helper;
	CCheckBox ChamsEnemyinVisible;
	CComboBox helpermat;
	CCheckBox OffscreenESP;
	CComboBox chamstype;
	CColorSelector urdadyoufaggot;

	CSlider Glowz_lcl;
	CCheckBox BlendIfScoped;
	CSlider BlendValue;

	CComboBox OptionsChams;
	CComboBox OptionsArmor;
	CComboBox OptionsHealth;
	CComboBox OptionsWeapon;
	CComboBox manualaa_type;
	CComboBox HandCHAMS;
	CComboBox GunCHAMS;
	CComboBox LBYIndicator;
	CComboBox LCIndicator;
	CComboBox FakeDuckIndicator;
	CComboBox OtherThirdpersonAngle;
	CComboBox WeaponChams;
	CComboBox fakelag_ghost;
	CComboBox visible_chams_type;
	CComboBox invisible_chams_type;

	CCheckBox SleeveChams;
	CCheckBox Weapons;
	CCheckBox ChamsTeamVis;
	CCheckBox ChamsTeamNoVis;
	CCheckBox OptionsGlow_lcl;
	CCheckBox NoGlowIfScoped;
	CCheckBox NoChamsIfScoped;
	CCheckBox armorbar;
	CCheckBox OptionsInfo;
	CCheckBox BulletTrace;
	CCheckBox BulletTrace_enemy;
	CCheckBox ChamsVisibleOnly;
	CCheckBox Ammo;
	CCheckBox GrenadePrediction;
	CCheckBox LocalPlayerESP;
	CCheckBox CanHit;
	CCheckBox OptionsHelmet;
	CCheckBox NightMode;
	CCheckBox OptionsKit;
	CCheckBox SpecList;
	CCheckBox IsScoped;
	CCheckBox CompRank;
	CCheckBox HasDefuser;
	CCheckBox GrenadeTrace;
	CCheckBox show_hostage;
	CCheckBox DiscoLights;
	CCheckBox Money;
	CCheckBox OptionsDefuse;
	CCheckBox OptionsDefusing;
	CCheckBox SniperCrosshair;
	CCheckBox OptionsSkeleton;
	CCheckBox NightSky;
	CCheckBox hitbone;
	CCheckBox OptionsAimSpot;
	CCheckBox OtherNoScope;
	CCheckBox OptionsCompRank;
	CCheckBox resoinfo;
	CCheckBox killfeed;
	CCheckBox override_viewmodel;
	CCheckBox selfglow;
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox show_players;
	CCheckBox show_team;
	CCheckBox show_local;
	CCheckBox FiltersWeapons;
	CKeyBind flashlight;
	CColorSelector BoxCol;
	CCheckBox FiltersChickens;
	CCheckBox FiltersNades;
	CCheckBox FiltersC4;
	CCheckBox FiltersChicken;
	CGroupBox ChamsGroup;
	CComboBox asus_type;
	CCheckBox ChamsThruWalls;
	CCheckBox ModulateSkyBox;

	CSlider sky_r;
	CSlider sky_g;
	CSlider sky_b;
	CSlider  GlowZ;
	CSlider  team_glow;

	CColorSelector NameCol;
	CColorSelector GlowEnemy;
	CColorSelector Skeleton;
	CSlider enemy_blend;
	CSlider enemy_blend_invis;
	CSlider offset_x;

	CSlider aspectratiovalue;
	CCheckBox aspectratio;

	CSlider offset_y;
	CSlider offset_z;
	CCheckBox ChamsPlayers;
	CCheckBox ChamsEnemyOnly;
	CSlider blend_local;
	CGroupBox OtherGroup;
	CComboBox OtherCrosshair;
	CCheckBox OtherRecoilCrosshair;
	CCheckBox OtherRecoilCrosshair2;
	CCheckBox AutowallCrosshair;
	CCheckBox OptionsGlow;
	CCheckBox EntityGlow;
	CCheckBox OtherEntityGlow;
	CCheckBox DisablePostProcess;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky;
	CCheckBox OtherNoFlash;
	CCheckBox nosmoke;
	CSlider nosmoke_slider;
	CCheckBox OtherAsusWalls;	
	CCheckBox LegitMode1;
	CComboBox OtherNoHands;
	CCheckBox visualize_backtrack;
	CCheckBox AAIndicators;
	CCheckBox BulletTracers;
	CComboBox localmaterial;
	CComboBox scopemat;
	CSlider transparency;
	CSlider hand_transparency;
	CSlider gun_transparency;
	CSlider sleeve_transparency;
	CSlider asusamount;
	CComboBox asustype;
	CSlider beamtime;
	CCheckBox watermark;
	CSlider beamsize;
	CCheckBox cheatinfo;
	CCheckBox RemoveZoom;
	CComboBox sound;
	CCheckBox SpreadCross;
	CSlider SpreadCrossSize;
	CCheckBox DamageIndicator;
	CSlider OtherViewmodelFOV;
	CComboBox SpreadCrosshair;
	CSlider OtherFOV;
	CGroupBox worldgroup;
	CCheckBox colmodupdate;
	CSlider colmod;
	CCheckBox customskies2;
	CComboBox customskies;
	CCheckBox optimize;
};
class CColorTab : public CTab
{
public:
	void Setup();
	//you pasted the fucking colorselector but didn't use it... man man freak no baka
	//you have no fucking idea how many errors and how much cancer this gave me
	CGroupBox ColorsGroup;
	CComboBox beam_type;
	CGroupBox ConfigGroup;
	CGroupBox OtherOptions;
	CGroupBox OtherOptions2;
	CCheckBox ClanTag;
	CSlider radar_alpha;
	CSlider owo_slider;
	CListBox ConfigListBox;
	CButton SaveConfig;
	CButton LoadConfig;
	CButton RemoveConfig;
	CTextField NewConfigName;
	CComboBox MenuBar;
	CButton AddConfig;

	CColorSelector NameCol;
	CColorSelector HandChamsCol;
	CColorSelector GunChamsCol;
	CColorSelector BoxCol;
	CColorSelector BoxColteam;

	CColorSelector BoxColvis;
	CColorSelector BoxColinvis;



	CColorSelector Skeleton;
	CColorSelector Skeletonteam;
	CColorSelector GlowEnemy; //no one has fucking team esp on so i'm not even gonna bother making a color selector for that
	CColorSelector GlowTeam;
	CColorSelector GlowLocal;
	CColorSelector GlowOtherEnt;
	CColorSelector Skyboxcolor;
	CColorSelector spreadcrosscol;
	CColorSelector Weapons;
	CColorSelector Weaponsteam;
	CColorSelector Ammo;
	CColorSelector Ammoteam;
	CColorSelector Money; //we don't really need to be able to change the color for all the flags it's just retarded
	CColorSelector Offscreen;
	CColorSelector ChamsLocal;
	CColorSelector ChamsEnemyVis;
	CColorSelector ChamsEnemyNotVis;
	CColorSelector ChamsTeamVis;
	CColorSelector ChamsTeamNotVis;
	CColorSelector Bullettracer_local;
	CColorSelector Bullettracer_enemy;
	CColorSelector Menu;
	CColorSelector bomb_timer;
	CColorSelector SleeveChams_col;
	CColorSelector scoped_c;
	CColorSelector misc_backtrackchams;
	CColorSelector misc_lagcomp;
	CColorSelector misc_lagcompChams;
	CColorSelector misc_lagcompBones;
	CColorSelector fakelag_ghost;

	CComboBox chamcount;
	CComboBox bonecount;
	CComboBox asus_type;

	CCheckBox DebugLagComp;
	CCheckBox BackTrackBones2;
	CCheckBox BackTrackBones;
	CCheckBox ambient;

	CSlider AmbientRed;
	CSlider AmbientGreen;
	CSlider AmbientBlue;


	CGroupBox gcol;
	CSlider outl_r;
	CSlider outl_g;
	CSlider outl_b;

	CSlider inl_r;
	CSlider inl_g;
	CSlider inl_b;

	CSlider inr_r;
	CSlider inr_g;
	CSlider inr_b;


	CSlider outr_r;
	CSlider outr_g;
	CSlider outr_b;

	CSlider cr;
	CSlider cg;
	CSlider cb;

	CCheckBox experimental_backtrack;
	CSlider quickstop_speed;

};
class CMiscTab : public CTab
{
public:
	void Setup();
	CCheckBox DisablePostProcess;
	CComboBox hitmarker_sound;
	CComboBox autojump_type;
	CCheckBox ragdoll;
	CComboBox airduck_type;
	CGroupBox OtherGroup;
	CCheckBox velocity;
	CCheckBox OtherAutoJump;
	CCheckBox meme;
	CCheckBox dangerzone;
	CCheckBox OtherAutoStrafe;
	CKeyBind Zstrafe;
	CComboBox OtherSafeMode;
	CCheckBox freestandtype;
	CTextField customspam;
	CKeyBind OtherCircleButton;
	CCheckBox fakevote;
	CCheckBox fakevote2;

	CCheckBox Legitbhop;
	CSlider Legitbhophitchance;
	CSlider Legitbhoprestrictedlimit;
	CSlider Legitbhopmaxhit;
	CSlider Legitbhopminhit;


	CComboBox namechangerr;
	CCheckBox namespampromo;
	CCheckBox fakevac;
	CCheckBox AutoDefuse;
	CCheckBox OtherCircleStrafe;
	CCheckBox QuickStop;
	CCheckBox CircleStrafe;
	CKeyBind CircleStrafeKey;
	CKeyBind minimal_walk;
	CKeyBind OtherCircle;
	CCheckBox choked_shot;
	CTextField CustomClantag;
	CCheckBox CheatsByPass;
	CCheckBox DebugHitbones;
	CSlider freerange;
	CCheckBox aa_helper;
	CCheckBox infinite_duck;
	CComboBox buybot_primary;
	CComboBox buybot_secondary;
	CDropBox buybot_otr;
	CCheckBox AutoAccept;
	CSlider custom_pitch;
	CCheckBox SniperCrosshair;
	CSlider CircleAmount;
	CKeyBind OtherSlowMotion;
	CKeyBind plugwalk;
	CComboBox FakeLagTyp;
	CCheckBox autofw;
	CKeyBind fw;
	CComboBox FakeYaw;
	CComboBox FakeYaw2;
	CComboBox FakeYaw3;
	CKeyBind manualleft;
	CKeyBind manualright;
	CKeyBind manualback;
	CKeyBind manualfront;
	CCheckBox do_freestanding;
	CCheckBox disable_on_dormant;

	CCheckBox fake_crouch;
	CKeyBind fake_crouch_key;
	CKeyBind fake_stand_key;
	CGroupBox AntiAimGroup;

	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CComboBox antiaimtype_stand;
	CComboBox antiaimtype_move;
	CCheckBox desync_aa_stand;
	CCheckBox desync_aa_move;

	CSlider staticr;
	CSlider staticf;
	CSlider ThirdPersonVal;
	CComboBox AntiAimYaw3;
	CComboBox AntiAimYawrun;
	CCheckBox Radar;
	CCheckBox OtherTeamChat;
	CSlider ClanTagSpeed;
	CSlider	  OtherChatDelay;
	CCheckBox NameChanger;
	CCheckBox AutoPistol;
	CCheckBox ClanTag;
	CDropBox killsay;
	CComboBox ChatSpam;
	CCheckBox fakeping;
	CSlider fakepingvalue;
	CKeyBind  OtherAirStuck;
	CCheckBox OtherSpectators;
	CCheckBox OtherThirdperson;

	CComboBox desync_type_stand;
	CSlider desync_range_stand;
	CCheckBox desync_swapsides_stand;

	CComboBox desync_type_move;
	CSlider desync_range_move;
	CCheckBox desync_twist_onshot;

	CKeyBind fakelag_key;
	CKeyBind ThirdPersonKeyBind;
	CSlider FakeWalkSpeed;
	CSlider ThirdPersonValue;
	CCheckBox standing_desync;
	CCheckBox moving_desync;
	CCheckBox air_desync;
	CSlider twitchf;
	CSlider spinf;
	CSlider randlbyr;
	CSlider randlbyf;
	CSlider spinspeed;
	CSlider lby1;
	CCheckBox squaredance;
	CComboBox antilby;
	CCheckBox pitch_up;
	CCheckBox experimental;
	CSlider BreakLBYDelta;
	CSlider BreakLBYDelta2;
	CGroupBox FakeLagGroup;
	CSlider RadarX;
	CSlider RadarY;
	CCheckBox FakeLagEnable;
	CButton unload;
	CSlider Fakelagjump;
	CCheckBox PingSpike;
	CKeyBind LagSpikeKey;
	CComboBox KnifeModel;
	CButton   SkinApply;
	CCheckBox SkinEnable;
	CSlider FakelagMove;
	CSlider FakelagStand;
	CCheckBox fl_onshot;
	CCheckBox FakelagBreakLC;
	CSlider   FakeLagChoke;
	CCheckBox FakelagOnground;
	CCheckBox antilby2;
	CSlider FakeLagChoke2;
	CKeyBind lagkey;
	CComboBox ConfigBox;
	CGroupBox ConfigGroup;

	CSlider pingspike_val;
	CKeyBind pingspike_key;

	CComboBox fl_spike;
};

class CSkinTab : public CTab
{
public:
	void Setup();
	//--------------------------//
	CGroupBox knifegroup;
	CGroupBox snipergroup;

	//--------------------------//
	CComboBox t_knife_index;
	CComboBox ct_knife_index;

	//--------------------------//
	CSlider t_knife_wear;
	CSlider t_sniperSCAR_wear;
	CSlider t_sniperAWP_wear;

	//--------------------------//
	CComboBox t_knife_skin_id;
	CComboBox t_sniperAWP_skin_id;
	CComboBox t_sniperSCAR_skin_id;

	CCheckBox Update;

	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;
	CComboBox CZSkin;

	// MPs


	// Rifles
	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;
	CComboBox M249Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;

};

class mirror_window : public CWindow
{
public:
	void Setup();
	CAimbotTab aimbot_tab;
	CLegitBotTab LegitBotTab;
	CVisualTab visuals_tab;
	CMiscTab MiscTab;
	CSkinTab SkinTab;

	CColorTab ColorsTab;
	CButton   SkinApply;
	CButton SaveButton1;
	CButton LoadButton1;
	CButton SaveButton;
	CButton LoadButton;
	CButton UnloadButton;
	CButton PanicButton;
	CButton ThirdButton;
	CButton ByPass;
	CButton FirstButton;
	CComboBox ConfigBox;
};
namespace options
{
	void SetupMenu();
	void DoUIFrame();
	extern mirror_window menu;
};