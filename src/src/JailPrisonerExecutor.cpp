#include "Main.h"

vector<const char*> JAIL_PRISONER_TARGET_MODELS = {
	"u_m_m_asbprisoner_01",
	"u_m_m_asbprisoner_02",
	"a_m_m_skpprisoner_01",
	"a_m_m_valcriminals_01",
	"g_m_m_unicriminals_01",
	"g_m_m_unicriminals_02"
};

JailPrisonerExecutor::JailPrisonerExecutor(MissionData* missionData, MissionStatus status)
	: AssassinationMissionExecutor(missionData, status)
{
}

void JailPrisonerExecutor::update()
{
	AssassinationMissionExecutor::update();
}

bool JailPrisonerExecutor::wasTargetJustFound()
{
	return 
		distanceBetweenEntities(player, getTargetPed()) <= getMissionData()->identificationDistance && 
		AssassinationMissionExecutor::wasTargetJustFound();
}

void JailPrisonerExecutor::prepareSet()
{
	Ped targetPed = getTargetPed();
	AI::_0x524B54361229154F(targetPed, GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND"), -1, true, true, 0, true);
	WEAPON::REMOVE_ALL_PED_WEAPONS(targetPed, 1, 1);
	//AI::_0x524B54361229154F(targetPed, GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND"), -1, true, true, 0, true);
}

Ped JailPrisonerExecutor::spawnTarget()
{
	const char* model = JAIL_PRISONER_TARGET_MODELS.at(rndInt(0, JAIL_PRISONER_TARGET_MODELS.size()));
	Ped targetPed = createPed(model, getMissionData()->startPosition);
	return targetPed;
}
	
const char* JailPrisonerExecutor::generateTrackDownMessage()
{
	return "Find and eliminate the target";
}