#include "Main.h";

bool weapon;

void giveWeaponToPed(Ped ped, WeaponHash hash, int ammo, bool equipNow)
{
	if (!WEAPON::HAS_PED_GOT_WEAPON(ped, (Hash)hash, 0, 0))
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, (Hash)hash, ammo, false, false);
	}
	
	if (equipNow)
	{
		WEAPON::SET_CURRENT_PED_WEAPON(ped, (Hash)hash, true, 0, false, false);
	}
}

void pedEquipBestWeapon(Ped ped)
{
	WEAPON::SET_CURRENT_PED_WEAPON(ped, WEAPON::GET_BEST_PED_WEAPON(ped, 0, 0), true, 0, false, false);
}

Hash getPedEquipedWeapon(Ped ped, WeaponAttachPoint attachPoint)
{
	Hash weaponHash;
	WEAPON::GET_CURRENT_PED_WEAPON(ped, &weaponHash, 0, (int)attachPoint, 0);
	return weaponHash;
}

Vector3 getPedLastWeaponImpactCoord(Ped ped)
{
	Vector3 output;
	if (!WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(player, &output)) {
		return toVector3(0, 0, 0);
	}
	return output;
}

bool doesPedHaveSidearm(Ped ped)
{
	//return WEAPON::GET_PED_AMMO_BY_TYPE(ped, WEAPON::_0x5C2EA6C44F515F34(WeaponHash::RevolverCattleman))
	return WEAPON::GET_PED_AMMO_BY_TYPE(ped, AmmoType::AmmoRevolver) || WEAPON::GET_PED_AMMO_BY_TYPE(ped, AmmoType::AmmoPistol);
	//Hash opponentSidearmWeapon;
	//WEAPON::GET_CURRENT_PED_WEAPON(ped, &opponentSidearmWeapon, true, 3, false);
	//if (opponentSidearmWeapon == WeaponHash::Unarmed)
	//{
	//	return false;
	//}

	//return true;
}