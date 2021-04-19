#include "Main.h"

TrainTravelerExecutor::TrainTravelerExecutor(MissionData* missionData, MissionStatus status, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	train = NULL;
	wasTargetSpooked = false;
	initialDistanceToTrain = 0;

	this->area = area;
}

void TrainTravelerExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (!wasTargetSpooked && (
		distance(player, getTargetPed()) < 5 || hasPedClearLosInFront(getTargetPed(), player, 12)))
	{
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(getTargetPed(), 0, 0);
		AI::CLEAR_PED_TASKS(getTargetPed(), 0, 0);
		pedEquipBestWeapon(getTargetPed());
		AI::TASK_COMBAT_PED(getTargetPed(), player, 0, 16);
		wasTargetSpooked = true;
	}
}

bool TrainTravelerExecutor::wasTargetJustFound()
{
	return PED::IS_PED_IN_ANY_TRAIN(player) && 
		getTargetPed() && 
		hasPedClearLosInFront(player, getTargetPed(), 12) && AssassinationMissionExecutor::wasTargetJustFound();
}

Vehicle TrainTravelerExecutor::spawnTrain()
{
	train = NULL;
	for (int attempts = 10; !train && attempts > 0; attempts--)
	{
		TrainConfiguration configuration = 
			getMissionData()->difficultyLevel == MissionDifficulty::Normal ? TrainConfiguration::PassengersPacificUnion : TrainConfiguration::Passangers;

		train = area->createTrainHeadingTowardsLocation(configuration, getMissionData()->startPosition);
		attempts--;
	}

	if (!train)
	{
		showSubtitle("Contracts: train could not be created, please try later.");
		log("Train could not be spawned");
		setStatus(MissionStatus::Blocked);
		return NULL;
	}

	WAIT(500);
}

void TrainTravelerExecutor::prepareSet()
{

}

bool TrainTravelerExecutor::waitForTrainBoarding()
{
	while (!PED::IS_PED_IN_ANY_TRAIN(player))
	{
		float distToTrain = distance(playerPos(), entityPos(train));
		if (willAreaBeAbandoned())
		{
			return false;
		}

		if (distToTrain <= 100)
		{
			deleteBlipSafe(&areaBlip);
		}

		WAIT(500);
	}

	return true;
}

Ped TrainTravelerExecutor::findTargetPed(int carriageBoarded)
{
	Ped targetPed = NULL;
	while (!targetPed)
	{
		if (willAreaBeAbandoned())
		{
			onAreaAbandoned();
			return NULL;
		}

		targetPed = getRandomPedInTrain(train, carriageBoarded);
		if (!PED::IS_PED_MALE(targetPed))
		{
			log("skipping found target, looking for another");
			targetPed = NULL;
		}

		WAIT(500);
	}

	return targetPed;
}

Ped TrainTravelerExecutor::spawnTarget()
{
	// s_m_m_unitrainguards_01

	spawnTrain();
	if (!train)
	{
		return NULL;
	}

	MissionData* missionData = getMissionData();
	initialDistanceToTrain = distance(missionData->startPosition, entityPos(train));

	log("waiting for player to board the train to continue...");
	if (!waitForTrainBoarding())
	{
		fail("You've lost the target");
		return NULL;
	}

	int boaradedCarriage = getClosestTrainCarriage(train, player);
	log(string("train boarded on carriage: ").append(to_string(boaradedCarriage)).c_str());
	deleteBlipSafe(&areaBlip);
	showSubtitle("Identify the target on the train.");

	Ped targetPed = findTargetPed(boaradedCarriage);
	if (!targetPed)
	{
		showSubtitle("Contracts: target could not be found, please try later.");
		log("Target could not be found on train");
		setStatus(MissionStatus::Blocked);
		return NULL;
	}

	addMissionEntity(targetPed);
	giveWeaponToPed(targetPed, WeaponHash::RevolverCattleman, 100, false);

	return targetPed;
}

const char* TrainTravelerExecutor::generateTrackDownMessage()
{
	return "Wait for the train and board it. Your target is inside.";
}

void TrainTravelerExecutor::cleanup()
{
	AssassinationMissionExecutor::cleanup();
	VEHICLE::SET_MISSION_TRAIN_AS_NO_LONGER_NEEDED(&train, 0);
}

void TrainTravelerExecutor::onAreaAbandoned()
{
	fail("You've lost the target");
}

bool TrainTravelerExecutor::willAreaBeAbandoned()
{
	return distance(player, train) > initialDistanceToTrain * 1.5;
}