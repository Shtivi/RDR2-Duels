#include "Main.h"

GenericShackExecutor::GenericShackExecutor(MissionData* missionData, MissionStatus status, Place* place, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	this->place = place;
	this->area = area;
	hasCombatTriggered = false;
}

void GenericShackExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (!hasCombatTriggered)
	{
		if (distance(playerPos(), place->getCoords()) <= place->getRadius())
		{
			for (vector<Ped>::iterator itr = enemies.begin(); itr != enemies.end(); itr++)
			{
				if (PED::IS_PED_IN_COMBAT(*itr, player))
				{
					hasCombatTriggered = true;
					showEnemyBlips();
				} 
				else if (hasPedClearLosInFront(*itr, player, 35))
				{
					AI::TASK_COMBAT_PED(*itr, player, 0, 16);
				}
			}
		}
	}
}

bool GenericShackExecutor::wasTargetJustFound()
{
	return AssassinationMissionExecutor::wasTargetJustFound();
}

void GenericShackExecutor::onTargetFound()
{
	AssassinationMissionExecutor::onTargetFound();
	showEnemyBlips();
}

void GenericShackExecutor::prepareSet()
{
	place->populate();

	vector<int> scenarioPoints = place->listScenarioPointIds();
	int targetScenario = rndInt(0, scenarioPoints.size());
	for (vector<int>::iterator itr = scenarioPoints.begin(); itr != scenarioPoints.end(); itr++)
	{
		Ped ped;
		if (itr == scenarioPoints.begin() + targetScenario)
		{
			ped = getTargetPed();
		}
		else
		{
			Vector3 spawnLocation = AI::_0xA8452DD321607029(*itr, 0);
			ped = addEnemy(spawnLocation);
		}

		AI::_0xCCDAE6324B6A821C(ped, *itr, 0, -1, 0, 1, 0, 0, -1082130432, 0);
		PED::SET_PED_KEEP_TASK(ped, true);
	}
}

Ped GenericShackExecutor::spawnTarget()
{
	return addEnemy(around(place->getCoords(), 2));
}

Ped GenericShackExecutor::addEnemy(Vector3 coords)
{
	const char* model;

	if (getMissionData()->difficultyLevel == MissionDifficulty::Normal)
	{
		log(to_string((int)GENERIC_OUTLAWS_MODELS.size()).c_str());
		model = GENERIC_OUTLAWS_MODELS.at(rndInt(0, GENERIC_OUTLAWS_MODELS.size()));
	}
	else if (place->getTypicalPedModel())
	{
		model = place->getTypicalPedModel();
	}
	else
	{
		model = area->getRandomLocalGangPedModel();
	}

	Ped enemy = addMissionEntity(createPed(model, coords));
	DECORATOR::DECOR_SET_INT(enemy, "honor_override", 0);
	enemies.push_back(enemy);
	return enemy;
}

const char* GenericShackExecutor::generateTrackDownMessage()
{
	return "Find and assassinate the target";
}

void GenericShackExecutor::showEnemyBlips()
{
	for (vector<Ped>::iterator itr = enemies.begin(); itr != enemies.end(); itr++)
	{
		Ped enemy = *itr;
		if (enemy != getTargetPed() && !RADAR::DOES_BLIP_EXIST(RADAR::GET_BLIP_FROM_ENTITY(enemy)))
		{
			createBlip(enemy, BLIP_STYLE_ENEMY);
		}
	}
}