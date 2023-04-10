#include "Main.h"

Entity getPlayerTargetEntity()
{
	Entity targetEntity;
	if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
	{
		return targetEntity;
	}

	return NULL;
}

void makeHonorEffect(bool positive, bool big)
{
	struct {
		alignas(8) int f_0;
		alignas(8) const char* f_1;
		alignas(8) const char* f_2;
		alignas(8) int f_3;
		alignas(8) int f_4;
		alignas(8) int f_5;
		alignas(8) int f_6;
	} Var13;

	struct {
		alignas(8) int f_0;
		alignas(8) const char* f_1;
		alignas(8) const char* f_2;
		alignas(8) int f_3;
	} Var0;

	Var0.f_0 = -2;
	Var0.f_1 = "Honor_Display_Sounds";

	if (positive) 
	{
		Var0.f_2 = big ? "Honor_Increase_Big" : "Honor_Increase_Small";
	}
	else
	{
		Var0.f_2 = big ? "Honor_Decrease_Big" : "Honor_Decrease_Small";
	}
			
	Var0.f_3 = 0;
	Var0.f_0 = 450;
	const char* sParam0 = MISC::VAR_STRING(2, positive ? "PLAYER_HONOR_CHANGE_POS" : "PLAYER_HONOR_CHANGE_NEG");
	const char* sParam1 = "ITEMTYPE_TEXTURES";
	int iParam2 = MISC::GET_HASH_KEY(positive ? "TRANSACTION_HONOR_GOOD" : "TRANSACTION_HONOR_BAD");

	Var13.f_1 = sParam0;
	Var13.f_2 = sParam1;
	Var13.f_3 = iParam2;
	Var13.f_4 = 1;
	Var13.f_5 = positive ? 109029619 : 859817522;
	Var13.f_6 = 0;
	UIFEED::_UI_FEED_POST_SAMPLE_TOAST_RIGHT((Any*)&Var0, (Any*)&Var13, 1);
	COMPAPP::_0x74BCCEB233AD95B2(-466562563, MISC::GET_HASH_KEY((char*)sParam0));
	COMPAPP::_0x74BCCEB233AD95B2(1885309238, MISC::GET_HASH_KEY((char*)sParam1));
	COMPAPP::_0x74BCCEB233AD95B2(-826961056, iParam2);
}

Ped getPlayerPrimaryHorse()
{
	return PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
}

void playerFleeHorse(Ped horse, bool quiet)
{
	if (!ENTITY::DOES_ENTITY_EXIST(horse) || 
		!PED::_IS_THIS_MODEL_A_HORSE(ENTITY::GET_ENTITY_MODEL(horse)))
	{
		return;
	}

	const char* speech = NULL;
	if (quiet)
	{
		speech = "HORSE_RUN_AWAY_QUIET";
	}
	else if (PED::IS_PED_MALE(horse))
	{
		speech = "HORSE_RUN_AWAY_MALE";
	} 
	else
	{
		speech = "HORSE_RUN_AWAY_FEMALE";
	}

	playAmbientSpeech(player, (char*)speech);
	TASK::TASK_FLEE_PED(horse, player, 3, 0, 25, 10000, 0);
}