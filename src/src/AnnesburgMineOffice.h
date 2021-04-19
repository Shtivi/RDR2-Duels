#pragma once

class AnnesburgMineOffice : public Place 
{
private:
	const int OFFICE_CHAIR_MODAL_HASH = 70472470;

public:
	AnnesburgMineOffice(int placeId)
		: Place(placeId, MapAreas::Annesburg, toVector3(2920.47, 1379.14, 56.2244), 15, PlaceType::Mine, "Mine's office")
	{
		Vector3 location = getCoords();
		Object chair = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(location.x, location.y, location.z, 3, OFFICE_CHAIR_MODAL_HASH, 0, 0, 0);
		addScenarioPoint(chair, "PROP_HUMAN_SEAT_CHAIR_SKETCHING", toVector3(0, 0, 0.5), 180);
		setTypicalPedModel("a_m_m_gamhighsociety_01");
	}

	void populate()
	{
		Place::populate();
		Ped p = createPed(getTypicalPedModel(), getCoords());
		WAIT(500);
		AI::_0xCCDAE6324B6A821C(p, scenarioPoints[0].getId(), 0, -1, 0, 1, 0, 0, -1082130432, 0);
	}
};