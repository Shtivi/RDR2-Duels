#include "Main.h"

void getGroundPos(Vector3 originalPos, Vector3* outPos)
{
	float groundZ;
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(originalPos.x, originalPos.y, originalPos.z, &groundZ, false);

	outPos->x = originalPos.x;
	outPos->y = originalPos.y;
	outPos->z = groundZ;
}

float distanceBetweenEntities(Entity entity1, Entity entity2)
{
	Vector3 pos1 = ENTITY::GET_ENTITY_COORDS(entity1, 1, 0);
	Vector3 pos2 = ENTITY::GET_ENTITY_COORDS(entity2, 1, 0);

	return GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, 1);
}

Ped createPed(const char* modelName, Vector3 pos)
{
	Hash model = GAMEPLAY::GET_HASH_KEY((char*)modelName);
	STREAMING::REQUEST_MODEL(model, false);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}

	Ped ped = PED::CREATE_PED(model, pos.x, pos.y, pos.z, 0, false, false, false, false);
	PED::SET_PED_VISIBLE(ped, true);

	return ped;
}

Ped createPed(int model, Vector3 pos)
{
	STREAMING::REQUEST_MODEL(model, false);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}

	Ped ped = PED::CREATE_PED(model, pos.x, pos.y, pos.z, 0, false, false, false, false);
	PED::SET_PED_VISIBLE(ped, true);

	return ped;
}


Ped createPed(const char* modelName, Vehicle vehicle, int seatIndex)
{
	Hash model = GAMEPLAY::GET_HASH_KEY((char*)modelName);
	STREAMING::REQUEST_MODEL(model, false);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}

	Ped ped = PED::CREATE_PED_INSIDE_VEHICLE(vehicle, model, seatIndex, 0, 0, 0);
	PED::SET_PED_VISIBLE(ped, true);
	return ped;
}

Object createProp(char* model, Vector3 position, float heading, bool isStatic, bool isVisible)
{
	Hash modelHash = GAMEPLAY::GET_HASH_KEY(model);

	if (!STREAMING::HAS_MODEL_LOADED(modelHash))
	{
		STREAMING::REQUEST_MODEL(modelHash, false);
	}

	while (!STREAMING::HAS_MODEL_LOADED(modelHash))
	{
		WAIT(0);
	}

	Object prop = OBJECT::CREATE_OBJECT(modelHash, position.x, position.y, position.z, false, false, !isStatic, 0, 0);
	ENTITY::SET_ENTITY_HEADING(prop, heading);
	ENTITY::FREEZE_ENTITY_POSITION(prop, isStatic);
	ENTITY::SET_ENTITY_VISIBLE(prop, isVisible);
	
	if (!isStatic) {
		OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(prop, 0);
	}

	return prop;
}

tm getGameTime()
{
	tm gameTime;
	gameTime.tm_year = 70; // make problems only with years aroud 1970, not 1900.
	gameTime.tm_mon = TIME::GET_CLOCK_MONTH();
	gameTime.tm_mday = TIME::GET_CLOCK_DAY_OF_MONTH();
	gameTime.tm_wday = TIME::GET_CLOCK_DAY_OF_WEEK();
	gameTime.tm_hour = TIME::GET_CLOCK_HOURS();
	gameTime.tm_min = TIME::GET_CLOCK_MINUTES();
	gameTime.tm_sec = TIME::GET_CLOCK_SECONDS();

	return gameTime;
}

RaycastResult raycast(Vector3 source, Vector3 direction, float maxDist, RaycastIntersectionOptions intersectionOptions, Entity ignore)
{
	RaycastResult result;
	Vector3 target = source + direction * maxDist;
	int rayHandle = SHAPETEST::_START_SHAPE_TEST_RAY(source.x, source.y, source.z, target.x, target.y, target.z, intersectionOptions, ignore, 7);
	SHAPETEST::GET_SHAPE_TEST_RESULT(rayHandle, (BOOL*)&result.didHit, &result.hitPos, &result.normal, &result.hitEntity);
	return result;
}

RaycastResult raycastCrosshair(float maxDist, RaycastIntersectionOptions intersectionOptions, Entity ignore)
{
	Vector3 source = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 rot = ((float)3.1452 / 180.0) * CAM::_GET_GAMEPLAY_CAM_ROT(2);
	Vector3 forward = normalOf(toVector3(
		-sin(rot.z) * abs(cos(rot.x)),
		cos(rot.z) * abs(cos(rot.x)),
		sin(rot.x)
	));
	return raycast(source, forward, maxDist, intersectionOptions, ignore);
}

RaycastResult shapeTestSphere(Vector3 source, Vector3 target, float radius, RaycastIntersectionOptions intersectionOptions, Entity ignore)
{
	RaycastResult result;
	int rayHandle = SHAPETEST::START_SHAPE_TEST_CAPSULE(source.x, source.y, source.z, target.x, target.y, target.z, radius, intersectionOptions, ignore, 7);
	SHAPETEST::GET_SHAPE_TEST_RESULT(rayHandle, (BOOL*)&result.didHit, &result.hitPos, &result.normal, &result.hitEntity);
	return result;
}

Ped findCarriedPedBy(Ped carrier)
{
	return PED::_0xD806CD2A4F2C2996(carrier);

	//int nearbyEntities[5 * 2 + 2];
	//Ped targetEntity = NULL;

	//nearbyEntities[0] = 5;
	//int n = PED::GET_PED_NEARBY_PEDS(carrier, (int*)&nearbyEntities, -1, -1);
	//for (int i = 0; i < n; i++)
	//{
	//	if (ENTITY::IS_ENTITY_A_PED(nearbyEntities[i]) &&
	//		ENTITY::DOES_ENTITY_EXIST(nearbyEntities[i]) &&
	//		ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(carrier, nearbyEntities[i]))
	//	{
	//		return nearbyEntities[i];
	//	}
	//}

	//return NULL;
}

void getGroundPos(Vector3* originalPos)
{
	getGroundPos(*originalPos, originalPos);
}

Vector3 getGroundPos(Vector3 originalPos)
{
	float groundZ;
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(originalPos.x, originalPos.y, originalPos.z, &groundZ, false);
	return toVector3(originalPos.x, originalPos.y, groundZ);
}

Vector3 getRandomPositionInRange(Vector3 center, int radius)
{
	int x = rndInt((int)center.x - radius, (int)center.x + radius + 1);
	int topOrBottom = rndInt(0, 2) == 1 ? 1 : -1;
	double y = topOrBottom * sqrt(pow(radius, 2) - pow(x - center.x, 2)) + center.y;

	Vector3 output;
	output.x = x;
	output.y = (float)y;
	return getGroundPos(output);
}

Vector3 getRandomPedPositionInRange(Vector3 source, int radius)
{
	Vector3 position = getRandomPositionInRange(source, radius);
	PATHFIND::GET_SAFE_COORD_FOR_PED(position.x, position.y, position.z, true, &position, 16);
	return position;
}

Vector3 playerPos()
{
	return ENTITY::GET_ENTITY_COORDS(player, 1, 0);
}

float getModelLength(Hash model)
{
	Vector3 front, back;
	GAMEPLAY::GET_MODEL_DIMENSIONS(model, &front, &back);
	float length = get_vector_length(front - back);
	return length;
}

Vector3 getRelativeEntityCoords(Entity entity, int offsetFromCenter)
{
	float length = getModelLength(ENTITY::GET_ENTITY_MODEL(entity));
	Vector3 entityPos = ENTITY::GET_ENTITY_COORDS(entity, 1, 0);
	Vector3 forwardVec = ENTITY::GET_ENTITY_FORWARD_VECTOR(entity);
	
	return entityPos + forwardVec * offsetFromCenter * (length / 4);
}

Vehicle getClosestVehicle(Ped around)
{
	int nearbyEntities[5 * 2 + 2];
	nearbyEntities[0] = 5;
	int n = PED::GET_PED_NEARBY_VEHICLES(around, (int*)&nearbyEntities);

	if (n == 0)
	{
		return NULL;
	}

	Vehicle best = nearbyEntities[0];
	float bestDistance = distanceBetweenEntities(around, best);

	for (int i = 0; i < n; i++)
	{
		float dist = distanceBetweenEntities(around, nearbyEntities[i]);
		if (ENTITY::DOES_ENTITY_EXIST(nearbyEntities[i]) && dist < bestDistance)
		{
			best = nearbyEntities[i];
			bestDistance = dist;
		}
	}

	return best;
}

Vector3 entityPos(Entity entity)
{
	return ENTITY::GET_ENTITY_COORDS(entity, 1, 0);
}

float calculateHeadingToPosition(Entity entity, Vector3 dest)
{
	float entityHeading = ENTITY::GET_ENTITY_HEADING(entity);
	float angle = angleBetweenVectors(entityPos(entity), dest);
	return entityHeading - angle;
}

Ped findHogtiedTargetEntity()
{
	Entity targetEntity = NULL;
	Ped asPed;
	
	if (!PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
	{
		return false;
	}

	if (ENTITY::IS_ENTITY_A_PED(targetEntity) &&
		PED::IS_PED_HUMAN(targetEntity) &&
		AI::GET_IS_TASK_ACTIVE(targetEntity, 400))
	{
		return (Ped)targetEntity;
	}

	return false;
}

Ped getClosestPed(Ped around)
{
	int nearbyEntities[5 * 2 + 2];
	Ped targetEntity = NULL;

	nearbyEntities[0] = 5;
	int n = PED::GET_PED_NEARBY_PEDS(around, (int*)&nearbyEntities, -1, -1);

	if (n == 0)
	{
		return NULL;
	}

	Ped best = nearbyEntities[0];
	float bestDistance = distanceBetweenEntities(around, best);

	for (int i = 0; i < n; i++)
	{
		float dist = distanceBetweenEntities(around, nearbyEntities[i]);
		if (ENTITY::DOES_ENTITY_EXIST(nearbyEntities[i]) && dist < bestDistance)
		{
			best = nearbyEntities[i];
			bestDistance = dist;
		}
	}

	return best;
}

Ped getClosestPed(Vector3 around, float radius)
{
	Ped ped;
	PED::GET_CLOSEST_PED(around.x, around.y, around.z, radius, 1, 0, &ped, 0, 0, 26, 26);
	return ped;
}

Vector3* getSafeCoordForPed(Vector3 destination)
{
	Vector3* out = new Vector3();
	if (!PATHFIND::GET_SAFE_COORD_FOR_PED(destination.x, destination.y, destination.z, false, out, 16))
	{
		*out = toVector3(0, 0, 0);
	}

	return out;
}

//void playAnimation(Entity entity, const char* animName, const char* animDict, bool loop, bool stayInAnim)
//{
//	if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)animDict))
//	{
//		STREAMING::REQUEST_ANIM_DICT((char*)animDict);
//	}
//	
//	Stopwatch stopwatch;
//	stopwatch.start();
//	while (!STREAMING::HAS_ANIM_DICT_LOADED((char*)animDict) && stopwatch.getElapsedSeconds() <= 1)
//	{
//		WAIT(25);
//	}
//
//	ENTITY::PLAY_ENTITY_ANIM(entity, (char*)animName, (char*)animDict, 1.f, loop, stayInAnim, 0, 0, 0);
//}

void playAnimation(Ped ped, const char* animName, const char* animDict, int duration, float blendInSpeed, float blendOutSpeed, int flags)
{
	if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)animDict))
	{
		STREAMING::REQUEST_ANIM_DICT((char*)animDict);
	}

	Stopwatch stopwatch;
	stopwatch.start();
	while (!STREAMING::HAS_ANIM_DICT_LOADED((char*)animDict) && stopwatch.getElapsedSeconds() <= 1)
	{
		STREAMING::REQUEST_ANIM_DICT((char*)animDict);
		WAIT(25);
	}

	AI::TASK_PLAY_ANIM(ped, (char*)animDict, (char*)animName, blendInSpeed, blendOutSpeed, duration, flags, 0, 0, 0, 0, 0, 0);
}

void loadImap(Hash imapHash)
{
	if (!STREAMING::_0xD779B9B910BD3B7C(imapHash))
	{
		STREAMING::_0x59767C5A7A9AE6DA(imapHash);
	}
}

void loadInteriorSet(Interior interior, const char* setName)
{
	if (!INTERIOR::_IS_INTERIOR_PROP_ENABLED(interior, (char*) setName)) {
		INTERIOR::_ENABLE_INTERIOR_PROP(interior, (char*) setName, 1);
	}
}

MapAreas getMapArea(Vector3 pos)
{
	int town = ZONE::_0x43AD8FC02B429D33(pos.x, pos.y, pos.z, 1);
	if (town) 
	{
		return (MapAreas)town;
	}
	else 
	{
		int district = ZONE::_0x43AD8FC02B429D33(pos.x, pos.y, pos.z, 10);
		if (district) 
		{
			return (MapAreas)district;
		}
		return (MapAreas)ZONE::_0x43AD8FC02B429D33(pos.x, pos.y, pos.z, 11);
	}
}

MapAreas getMapArea(Entity entity)
{
	return getMapArea(entityPos(entity));
}

MapAreas getPlayerMapArea()
{
	return getMapArea(player);
}

bool isPlayerUsingCampfire()
{
	// fire camp: 1020517461
	// fire camp: -1767895052
	// fire camp: 1984305068

	int playerScenarioPointType = (int)AI::_0x2D0571BB55879DA2(player); //  _GET_SCENARIO_POINT_TYPE_PED_IS_USING
	return playerScenarioPointType == -1767895052 || playerScenarioPointType == 1020517461 || playerScenarioPointType == 1984305068;
}

bool isPlayerResting()
{
	// fire camp: 1020517461
	// fire camp: -1767895052
	// fire camp: 1984305068
	// resting: -1241981548

	int playerScenarioPointType = (int)AI::_0x2D0571BB55879DA2(player); //  _GET_SCENARIO_POINT_TYPE_PED_IS_USING
	return playerScenarioPointType == -1241981548;
}

void setPlayerCoreValue(AttributeCores core, int value)
{
	ATTRIBUTE::_0xC6258F41D86676E0(player, (int)core, value);
}

int getPlayerCoreValue(AttributeCores core)
{
	return ATTRIBUTE::_0x36731AC041289BB1(player, (int)core);
}

void addToPlayerCore(AttributeCores core, int amount)
{
	setPlayerCoreValue(core, getPlayerCoreValue(core) + amount);
}

Position getClosestVehicleNode(Vector3 around, bool flipDirection)
{
	Vector3 out;
	float heading;
	if (PATHFIND::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(around.x, around.y, around.z, &out, &heading, 1, 3, 0))
	{
		return make_pair(out, heading);
	}

	return make_pair(toVector3(0, 0, 0), 0);
}

void playEntityAnimation(Entity entity, const char* animName, const char* animDict, float duration, bool loop, bool stayInAnim, float delta, int bitset) {
	int i = 0;
	while (!STREAMING::HAS_ANIM_DICT_LOADED((char*)animDict) && i <= 500)
	{
		STREAMING::REQUEST_ANIM_DICT((char*)animDict);
		i++;
		WAIT(10);
	}

	ENTITY::PLAY_ENTITY_ANIM(entity, (char*)animName, (char*)animDict, duration, loop, stayInAnim, 1, delta, bitset);
}