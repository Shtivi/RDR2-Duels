#include "Main.h"

TrainWorkerExecutor::TrainWorkerExecutor(MissionData* missionData, MissionStatus status, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	this->hardcart = NULL;
	this->area = area;
	this->wasCombatTriggered = false;
	this->groupId = 0;
}

void TrainWorkerExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (hardcart && !wasCombatTriggered && !VEHICLE::ARE_ANY_VEHICLE_SEATS_FREE(hardcart))
	{
		VEHICLE::SET_TRAIN_SPEED(hardcart, 3);
	}

	if (!wasCombatTriggered && shouldCombatStart())
	{
		for (Entity missionEntity : missionEntities)
		{
			if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), missionEntity))
			{
				beginCombat();
				wasCombatTriggered = true;
			}
		}
	}
}

void TrainWorkerExecutor::prepareSet()
{
	addWorker();

	int guardsCount = pow(2, (int)getMissionData()->difficultyLevel);
	log(string("spawning guards: ").append(to_string(guardsCount)));
	while (guardsCount > 0)
	{
		addGuard();
		guardsCount--;
	}
}

Ped TrainWorkerExecutor::spawnTarget()
{
	Vector3 hardcartLocation = area->getClosestTrainTrackTo(getMissionData()->startPosition).first;
	hardcart = addMissionEntity(createTrain(TrainConfiguration::Handcart, hardcartLocation));
	Ped targetPed = addWorker(false);
	groupId = PED::CREATE_GROUP(0);
	PED::SET_PED_AS_GROUP_LEADER(targetPed, groupId, 0);

	return targetPed;
}

const char* TrainWorkerExecutor::generateTrackDownMessage()
{
	return "Locate and kill the Train Worker";
}

bool TrainWorkerExecutor::shouldCombatStart()
{
	float distnaceTreshold = 6 * ((int)getMissionData()->difficultyLevel);
	float losTreshold = 35 + 10 * ((int)getMissionData()->difficultyLevel - 1);

	for (Entity missionEntity : missionEntities)
	{
		if (ENTITY::IS_ENTITY_A_PED(missionEntity) && PED::IS_PED_HUMAN(missionEntity))
		{
			if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), missionEntity) && (
				distance(player, missionEntity) <= distnaceTreshold || hasPedClearLosInFront(missionEntity, player, losTreshold)))
			{
				return true;
			}
		}
	}

	return false;
}

void TrainWorkerExecutor::beginCombat()
{
	for (Ped guard : guards)
	{
		pedEquipBestWeapon(guard);
		AI::TASK_COMBAT_PED(guard, player, 0, 16);
		createBlip(guard, BLIP_STYLE_ENEMY);
	}

	for (Ped worker : workers)
	{
		Object seq;
		AI::OPEN_SEQUENCE_TASK(&seq);
		AI::TASK_LEAVE_VEHICLE(0, hardcart, 4160, 0);
		AI::_0xFD45175A6DFD7CE9(0, player, 3, 0, -999.0f, -1, 0);
		AI::CLOSE_SEQUENCE_TASK(seq);
		AI::TASK_PERFORM_SEQUENCE(worker, seq);
		playAmbientSpeech(worker, "SCARED_HELP");
	}
}

Ped TrainWorkerExecutor::addWorker(bool asMember)
{
	int seat = VEHICLE::IS_VEHICLE_SEAT_FREE(hardcart, 0) ? 0 : -1;
	Ped worker = createPed("a_m_m_bivworker_01", hardcart, seat);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(worker, true);
	workers.push_back(worker);

	if (asMember)
	{
		PED::SET_PED_AS_GROUP_MEMBER(worker, groupId);
	}
	
	return worker;
}

Ped TrainWorkerExecutor::addGuard()
{
	Ped horse = addMissionEntity(createAmbientHorse(around(entityPos(hardcart), 12)));
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, true);
	
	Ped guard = addMissionEntity(createPedOnHorse("a_m_m_unicoachguards_01", horse, -1));
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(guard, true);
	giveWeaponToPed(guard, WeaponHash::RepeaterCarbine, 100, true);
	DECORATOR::DECOR_SET_INT(guard, "honor_override", 0);
	
	guards.push_back(guard);
	PED::SET_PED_AS_GROUP_MEMBER(guard, groupId);
	PED::SET_PED_AS_GROUP_MEMBER(horse, groupId);

	return guard;
}