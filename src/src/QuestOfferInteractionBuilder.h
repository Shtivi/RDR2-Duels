#pragma once

class QuestOfferInteractionBuilder
{
public:
	QuestOfferInteractionBuilder();
	Conversation* buildInteraction(Ped questGiver);
};