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
	const char* sParam0 = UI::_CREATE_VAR_STRING(2, positive ? "PLAYER_HONOR_CHANGE_POS" : "PLAYER_HONOR_CHANGE_NEG");
	const char* sParam1 = "ITEMTYPE_TEXTURES";
	int iParam2 = GAMEPLAY::GET_HASH_KEY(positive ? "TRANSACTION_HONOR_GOOD" : "TRANSACTION_HONOR_BAD");

	Var13.f_1 = sParam0;
	Var13.f_2 = sParam1;
	Var13.f_3 = iParam2;
	Var13.f_4 = 1;
	Var13.f_5 = positive ? 109029619 : 859817522;
	Var13.f_6 = 0;
	UIUNK::_0xB249EBCB30DD88E0((Any*)&Var0, (Any*)&Var13, 1);
	DUMMY2::_0x74BCCEB233AD95B2(-466562563, GAMEPLAY::GET_HASH_KEY((char*)sParam0));
	DUMMY2::_0x74BCCEB233AD95B2(1885309238, GAMEPLAY::GET_HASH_KEY((char*)sParam1));
	DUMMY2::_0x74BCCEB233AD95B2(-826961056, iParam2);
}