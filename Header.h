#pragma once
#include <cstdint>



#define cs_gamerules_data  0x0
#define m_ArmorValue  0xB37C
#define m_Collision  0x320
#define m_CollisionGroup  0x474
#define m_Local  0x2FBC
#define m_MoveType  0x25C
#define m_OriginalOwnerXuidHigh  0x31C4
#define m_OriginalOwnerXuidLow  0x31C0
#define m_SurvivalGameRuleDecisionTypes  0x1328
#define m_SurvivalRules  0xD00
#define m_aimPunchAngle  0x302C
#define m_aimPunchAngleVel  0x3038
#define m_angEyeAnglesX  0xB380
#define m_angEyeAnglesY  0xB384
#define m_bBombDefused  0x29B0
#define m_bBombPlanted  0x9A5
#define m_bBombTicking  0x2980
#define m_bFreezePeriod  0x20
#define m_bGunGameImmunity  0x3944
#define m_bHasDefuser  0xB38C
#define m_bHasHelmet  0xB370
#define m_bInReload  0x32A5
#define m_bIsDefusing  0x3930
#define m_bIsQueuedMatchmaking  0x74
#define m_bIsScoped  0x3928
#define m_bIsValveDS  0x7C
#define m_bSpotted  0x93D
#define m_bSpottedByMask  0x980
#define m_bStartedArming  0x33F0
#define m_bUseCustomAutoExposureMax  0x9D9
#define m_bUseCustomAutoExposureMin  0x9D8
#define m_bUseCustomBloomScale  0x9DA
#define m_clrRender  0x70
#define m_dwBoneMatrix  0x26A8
#define m_fAccuracyPenalty  0x3330
#define m_fFlags  0x104
#define m_flC4Blow  0x2990
#define m_flCustomAutoExposureMax  0x9E0
#define m_flCustomAutoExposureMin  0x9DC
#define m_flCustomBloomScale  0x9E4
#define m_flDefuseCountDown  0x29AC
#define m_flDefuseLength  0x29A8
#define m_flFallbackWear  0x31D0
#define m_flFlashDuration  0xA420
#define m_flFlashMaxAlpha  0xA41C
#define m_flLastBoneSetupTime  0x2924
#define m_flLowerBodyYawTarget  0x3A90
#define m_flNextAttack  0x2D70
#define m_flNextPrimaryAttack  0x3238
#define m_flSimulationTime  0x268
#define m_flTimerLength  0x2994
#define m_hActiveWeapon  0x2EF8
#define m_hBombDefuser  0x29B4
#define m_hMyWeapons  0x2DF8
#define m_hObserverTarget  0x338C
#define m_hOwner  0x29CC
#define m_hOwnerEntity  0x14C
#define m_hViewModel  0x32F8
#define m_iAccountID  0x2FC8
#define m_iClip1  0x3264
#define m_iCompetitiveRanking  0x1A84
#define m_iCompetitiveWins  0x1B88
#define m_iCrosshairId  0xB3E8
#define m_iDefaultFOV  0x332C
#define m_iEntityQuality  0x2FAC
#define m_iFOVStart  0x31E8
#define m_iGlowIndex  0xA438
#define m_iHealth  0x100
#define m_iItemDefinitionIndex  0x2FAA
#define m_iItemIDHigh  0x2FC0
#define m_iMostRecentModelBoneCounter  0x2690
#define m_iObserverMode  0x3378
#define m_iShotsFired  0xA390
#define m_iState  0x3258
#define m_iTeamNum  0xF4
#define m_lifeState  0x25F
#define m_nBombSite  0x2984
#define m_nFallbackPaintKit  0x31C8
#define m_nFallbackSeed  0x31CC
#define m_nFallbackStatTrak  0x31D4
#define m_nForceBone  0x268C
#define m_nTickBase  0x3430
#define m_nViewModelIndex  0x29C0
#define m_rgflCoordinateFrame  0x444
#define m_szCustomName  0x303C
#define m_szLastPlaceName  0x35B4
#define m_thirdPersonViewAngles  0x31D8
#define m_vecOrigin  0x138
#define m_vecVelocity  0x114
#define m_vecViewOffset  0x108
#define m_viewPunchAngle  0x3020
#define m_zoomLevel  0x33D0

#define anim_overlays  0x2980
#define clientstate_choked_commands  0x4D30
#define clientstate_delta_ticks  0x174
#define clientstate_last_outgoing_command  0x4D2C
#define clientstate_net_channel  0x9C
#define convar_name_hash_table  0x2F0F8
#define dwClientState  0x587FE4
#define dwClientState_GetLocalPlayer  0x180
#define dwClientState_IsHLTV  0x4D48
#define dwClientState_Map  0x28C
#define dwClientState_MapDirectory  0x188
#define dwClientState_MaxPlayer  0x388
#define dwClientState_PlayerInfo  0x52C0
#define dwClientState_State  0x108
#define dwClientState_ViewAngles  0x4D90
#define dwEntityList  0x4DA20DC
#define dwForceAttack  0x31D2628
#define dwForceAttack2  0x31D2634
#define dwForceBackward  0x31D267C
#define dwForceForward  0x31D2658
#define dwForceJump  0x524BECC
#define dwForceLeft  0x31D2670
#define dwForceRight  0x31D2694
#define dwGameDir  0x626780
#define dwGameRulesProxy  0x52BF1BC
#define dwGetAllClasses  0xDB0F6C
#define dwGlobalVars  0x587CE8
#define dwGlowObjectManager  0x52EA550
#define dwInput  0x51F36A0
#define dwInterfaceLinkList  0x944D14
#define dwLocalPlayer  0xD892CC
#define dwMouseEnable  0xD8EE18
#define dwMouseEnablePtr  0xD8EDE8
#define dwPlayerResource  0x31D0990
#define dwRadarBase  0x51D6E54
#define dwSensitivity  0xD8ECB4
#define dwSensitivityPtr  0xD8EC88
#define dwSetClanTag  0x8A1B0
#define dwViewMatrix  0x4D939F4
#define dwWeaponTable  0x51F4160
#define dwWeaponTableIndex  0x325C
#define dwYawPtr  0xD8EA78
#define dwZoomSensitivityRatioPtr  0xD93D18
#define dwbSendPackets  0xD762A
#define dwppDirect3DDevice9  0xA7050
#define find_hud_element  0x261FF980
#define force_update_spectator_glow  0x3AFD6A
#define interface_engine_cvar  0x3E9EC
#define is_c4_owner  0x3BC9C0
#define m_bDormant  0xED
#define m_flSpawnTime  0xA370
#define m_pStudioHdr  0x294C
#define m_pitchClassPtr  0x51D70F0
#define m_yawClassPtr  0xD8EA78
#define model_ambient_min  0x58B05C
#define set_abs_angles  0x1E0B80
#define set_abs_origin  0x1E09C0
