#include "Main.h";

const int ABANDON_AREA_SECS_TRESHOLD = 5;

using namespace std;

AssassinationMissionExecutor::AssassinationMissionExecutor(MissionData* missionData, MissionStatus status)
{
	this->challenges = new Challenges();
	this->missionData = missionData;
	this->objective = MissionObjective::Initializing;

	if (distance(missionData->startPosition, playerPos()) < missionData->areaRadius * 2) 
	{
		status = MissionStatus::Blocked;
	}
	areaBlip = NULL;
	target = NULL;
	targetBlip = NULL;

	setStatus(status);
	challenges->setStatus(OptionalObjectives::Stealth, true);
}

MissionData* AssassinationMissionExecutor::getMissionData()
{
	return missionData;
}

MissionObjective AssassinationMissionExecutor::getObjective()
{
	return objective;
}

MissionStatus AssassinationMissionExecutor::getStatus()
{
	return status;
}

void AssassinationMissionExecutor::setStatus(MissionStatus status)
{
	MissionStatus previousStatus = status;
	this->status = status;

	switch (status) {
		case MissionStatus::Accepted: 
		{
			objective = MissionObjective::Accepted;
			onAccepted();
			break;
		}
		case MissionStatus::Unavailable:
		{
			cleanup();
			break;
		}
		case MissionStatus::Blocked:
		{
			if (previousStatus != MissionStatus::Blocked)
			{
				cleanup();
			}
			block();
		}
	}
}

int AssassinationMissionExecutor::getTotalReward()
{
	int baseReward = missionData->getReward();
	int reward = baseReward + calculateBonuses();

	return (int)reward;
}

void AssassinationMissionExecutor::update()
{
	if (status == MissionStatus::Blocked || status == MissionStatus::Failed)
	{
		return;
	}

	if (status == MissionStatus::InProgress)
	{
		if (PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()))
		{
			fail("You have died", true);
			return;
		}

		if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, GAMEPLAY::GET_HASH_KEY("INPUT_REVEAL_HUD")))
		{
			displayObjectives();
		}

		if (objective < MissionObjective::Escape)
		{
			updateOptionalObjectives();
		}
	}

	switch (objective) {
	case MissionObjective::Accepted:
	{
		nextObjective();
		break;
	}
	case MissionObjective::ReachArea:
	{
		if (getPlayerDistanceToArea() <= missionData->areaRadius)
		{
			nextObjective();
		}
		break;
	}
	case MissionObjective::TrackTarget:
	{
		if (willAreaBeAbandoned())
		{
			if (!stopwatch.isStarted())
			{
				log("Mission area is about to be abandoned.");
				stopwatch.start();
			} 
			else if (stopwatch.getElapsedSeconds() > ABANDON_AREA_SECS_TRESHOLD)
			{
				stopwatch.stop();
				onAreaAbandoned();
			}
		}
		else 
		{
			stopwatch.stop();
			if (wasTargetJustFound() || ENTITY::IS_ENTITY_DEAD(target))
			{
				if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), target))
				{
					playAmbientSpeech(player, "HUNTING_NEAR");
				}

				WAIT(500);
				nextObjective();
			}
		}
		break;
	}
	case MissionObjective::AssassinateTarget:
	{
		if (willAreaBeAbandoned())
		{
			fail("Mission failed: You have abandoned the area or the target has escaped.");
		}
		else if (ENTITY::IS_ENTITY_DEAD(target))
		{
			nextObjective();
		}

		break;
	}
	case MissionObjective::Escape:
	{
		if (getPlayerDistanceToArea() >= missionData->areaRadius * 1.5)
		{
			nextObjective();
		}
	}
	default:
		break;
	}
}

void AssassinationMissionExecutor::block()
{
	status = MissionStatus::Blocked;
	deleteBlipSafe(&areaBlip);
	areaBlip = createBlip(missionData->startPosition, 0, 0xB04092F8, 0x1DCFAA8C);
	RADAR::_0x662D364ABF16DE2F(areaBlip, 0x2B30E11F);
	stringstream blipLabel;
	blipLabel << "Contract Target: " << getMissionData()->getTemplate()->title << ", $" << missionData->getReward();
	setBlipLabel(areaBlip, blipLabel.str().c_str());
}

void AssassinationMissionExecutor::resume()
{
	deleteBlipSafe(&areaBlip);
	setStatus(MissionStatus::Accepted);
}

float AssassinationMissionExecutor::getPlayerDistanceToArea()
{
	if (status == MissionStatus::InProgress)
	{
		return distance(target, player);
	}
	else 
	{
		return distance(missionData->startPosition, playerPos());
	}
}

Ped AssassinationMissionExecutor::getTargetPed()
{
	return target;
}

void AssassinationMissionExecutor::initialize()
{

}

void AssassinationMissionExecutor::nextObjective()
{
	switch (objective) {
	case MissionObjective::Accepted:
		objective = MissionObjective::ReachArea;
		break;
	case MissionObjective::ReachArea:
		objective = MissionObjective::TrackTarget;
		setStatus(MissionStatus::InProgress);
		onAreaReached();
		break;
	case MissionObjective::TrackTarget:
		objective = MissionObjective::AssassinateTarget;
		onTargetFound();
		break;
	case MissionObjective::AssassinateTarget:
		if ((int)PURSUIT::_0x259CE340A8738814(PLAYER::PLAYER_ID()) != 0 || PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0)
		{
			objective = MissionObjective::Escape;
		}
		else
		{
			objective = MissionObjective::Done;
			setStatus(MissionStatus::Completed);
			onCompleted();
		}
		onTargetKilled();
		break;
	case MissionObjective::Escape:
		objective = MissionObjective::Done;
		setStatus(MissionStatus::Completed);
		onCompleted();
	default:
		break;
	}
}

void AssassinationMissionExecutor::onAccepted()
{
	areaBlip = createBlip(missionData->startPosition, missionData->areaRadius, 0xB04092F8, 0x1DCFAA8C);
	stringstream blipLabel;
	blipLabel << "Contract Target: " << getMissionData()->getTemplate()->title << ", $" << missionData->getReward();
	setBlipLabel(areaBlip, blipLabel.str().c_str());
}

void AssassinationMissionExecutor::onAreaReached()
{
	stringstream logMsg;
	logMsg << "Reached mission area: " << getMissionData()->id;
	log(logMsg.str());

	displayObjectives();

	if (!target || !ENTITY::DOES_ENTITY_EXIST(target))
	{
		log("Spawning target and prepared mission set");
		WAIT(500);
		target = spawnTarget();
		log("Target spawned");
		decorateTarget();
		prepareSet();
		log("mission set prepared");
	}
}

void AssassinationMissionExecutor::onAreaAbandoned()
{
	log("Mission area was abandoned");
	cleanup();
	objective = MissionObjective::ReachArea;
	setStatus(MissionStatus::Accepted);
}

bool AssassinationMissionExecutor::wasTargetJustFound()
{
	if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), target))
	{
		return true;
	}


	return 
		distanceBetweenEntities(target, player) <= getMissionData()->identificationDistance || 
		ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(player, target, 50);
}

void AssassinationMissionExecutor::onTargetFound()
{
	log("Target found");
	deleteBlipSafe(&areaBlip);
	targetBlip = createBlip(target, BLIP_STYLE_ENEMY, 0x1DCFAA8C);
}

void AssassinationMissionExecutor::onTargetKilled()
{
	int bone;
	if (PED::GET_PED_LAST_DAMAGE_BONE(getTargetPed(), &bone) && bone == 21030)
	{
		challenges->complete(OptionalObjectives::Headshot);
	}

	int causeOfDeath = PED::GET_PED_CAUSE_OF_DEATH(getTargetPed());
	log(string("cause of death: " ).append(to_string(causeOfDeath)));

	// _IS_WEAPON_MELEE
	if (WEAPON::_0x959383DCD42040DA(causeOfDeath))
	{
		challenges->complete(OptionalObjectives::MeleeKill);
	}

	if (distance(player, getTargetPed()) >= 60 && WEAPON::_0x6AD66548840472E5(causeOfDeath))
	{
		challenges->complete(OptionalObjectives::LongDistanceKill);
	}

	if (causeOfDeath == WeaponHash::ThrownDynamite)
	{
		challenges->complete(OptionalObjectives::Dynamite);
	}

	if (objective == MissionObjective::Escape)
	{
		areaBlip = createBlip(entityPos(target), missionData->areaRadius * 1.5, 0x318C617C, 0x1DCFAA8C);
		showSubtitle("Escape the area");
	}
}

void AssassinationMissionExecutor::onEscapedArea()
{

}

void AssassinationMissionExecutor::onCompleted()
{
	cleanup();
	int payment = getTotalReward();

	string completedOptionalsMsg = string("Payment: $").append(to_string(payment)).append(". Completed challenges: ");
	for (OptionalObjectives optionalObjective : *missionData->optionalObjectives)
	{
		if (challenges->getStatus(optionalObjective))
		{
			completedOptionalsMsg.append(OptionalObjective::fromID(optionalObjective)->displayName).append(", ");
		}
	}
	log(completedOptionalsMsg.c_str());

	showHelpText(string("Contract completed. Visit the post office to collect your payment. It has been sent to Tacitus Kilgore. Total payment: $").append(to_string(payment)).c_str(), 8000);
	ChallengesToast::displayMissionChallenges(missionData, challenges, 8, ChallengesToast::TOP_HELP_MESSAGE);
}

void AssassinationMissionExecutor::updateOptionalObjectives()
{
	if (FIRE::IS_ENTITY_ON_FIRE(getTargetPed()))
	{
		challenges->complete(OptionalObjectives::BurnEmAll);
	}

	if (PED::_0x3AA24CCC0D451379(getTargetPed()) && isEntityOnTrainTracks(getTargetPed()))
	{
		challenges->complete(OptionalObjectives::TrainTracks);
	}

	if (DECORATOR::DECOR_GET_INT(target, "TYL_hanged") == 1)
	{
		challenges->complete(OptionalObjectives::TieYourLasso);
	}

	if ((int)PURSUIT::_0x259CE340A8738814(PLAYER::PLAYER_ID()) != 0 || PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0)
	{
		challenges->setStatus(OptionalObjectives::Stealth, false);
	}
}

Entity AssassinationMissionExecutor::addMissionEntity(Entity entity)
{
	missionEntities.push_back(entity);
	return entity;
}

void AssassinationMissionExecutor::fail(const char* reason, bool hardDeleteCleaup)
{
	setStatus(MissionStatus::Failed);

	if (reason) {
		log(reason);
		showSubtitle(reason);
	}

	cleanup(hardDeleteCleaup);
}

bool AssassinationMissionExecutor::willAreaBeAbandoned()
{
	return getPlayerDistanceToArea() >= missionData->areaRadius * 2.5;
}

void AssassinationMissionExecutor::cleanup(bool hardDelete)
{
	deleteBlipSafe(&areaBlip);
	deleteBlipSafe(&targetBlip);
	releaseEntitySafely(&target);

	for (vector<Entity>::iterator itr = missionEntities.begin(); itr != missionEntities.end(); itr++)
	{
		Entity curr = *itr;
		/*if (!ENTITY::DOES_ENTITY_EXIST(curr) || ENTITY::IS_ENTITY_DEAD(curr) || distance(playerPos(), curr) > max(200, missionData->areaRadius * 2))
		{
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&curr);
		}*/
		if (ENTITY::DOES_ENTITY_EXIST(curr))
		{
			if (hardDelete)
			{
				ENTITY::DELETE_ENTITY(&curr);
			}
			else
			{
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&curr);
			}
		}
	}
}

void AssassinationMissionExecutor::releaseEntitySafely(Entity* entity)
{
	Vector3 playerLocation = entityPos(player);
	if (!entity || !ENTITY::DOES_ENTITY_EXIST(*entity))
	{
		return;
	}

	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(entity);
}

float AssassinationMissionExecutor::calculateBonuses()
{
	float result = 0;

	for (OptionalObjectives challenge : *getMissionData()->optionalObjectives)
	{
		if (challenges->getStatus(challenge))
		{
			result += OptionalObjective::fromID(challenge)->reward;
		}
	}

	return result;
}

void AssassinationMissionExecutor::displayObjectives()
{
	showSubtitle(generateTrackDownMessage());
	ChallengesToast::displayMissionChallenges(missionData, challenges, 10);
}

void AssassinationMissionExecutor::decorateTarget()
{
	//PED::_0x4A48B6E03BABB4AC(target, (Any*)missionData->targetName); // Set ped name

	int honorOverride = 0;
	switch (missionData->getTemplate()->honorEffect)
	{
	case HonorEffect::Negative:
		honorOverride = 10;
		break;
	case HonorEffect::Positive:
		honorOverride = -10;
		break;
	default:
		honorOverride = 0;
	}

	DECORATOR::DECOR_SET_INT(target, "honor_override", honorOverride);
}