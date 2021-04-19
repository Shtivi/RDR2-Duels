#pragma once

enum class MissionObjective : int
{
	Initializing = 0,
	Accepted,
	ReachArea,
	TrackTarget,
	AssassinateTarget,
	Escape,
	Done
};

enum class MissionStatus : int
{
	Unavailable = -1,
	Accepted,
	InProgress,
	Completed,
	Blocked,
	Failed
};

enum class MissionTemplateType : int
{
	TransportedPrisoner,
	BusinessManConvoy,
	TrainStation,
	Lawman,
	Saloon,
	JailPrisoner,
	BanditsCamp,
	Riders,
	Hunter,
	AmbientPed,
	AnnesburgMineManager,
	AnnesburgMineGuard,
	BountyHuntersRiders,
	BanditsShack,
	BanditHideout,
	AmbientRancher,
	ArmyConvoy,
	MoonshinerConvoy,
	OilTraderConvoy,
	Grey,
	Braithwaite,
	TrainTraveler,
	TrainWorker,
	HotelRoom
};

enum class MissionDifficulty : int
{
	Normal = 1, 
	Hard,
	Extreme
};

enum class OptionalObjectives : int
{
	Stealth,
	LongDistanceKill,
	Headshot,
	MeleeKill,
	Dynamite,
	TieYourLasso,
	BurnEmAll,
	TrainTracks
};

enum class HonorEffect : int
{
	None,
	Negative,
	Positive
};

#include "MissionTemplate.h"
#include "MapArea.h"
#include "MapAreasFactory.h"
#include "OptionalObjective.h"
#include "MissionData.h"
#include "MissionDataFactory.h"
#include "Challenges.h"