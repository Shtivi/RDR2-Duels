#include "Main.h"

QuestOfferInteractionBuilder::QuestOfferInteractionBuilder()
{

}

Conversation* QuestOfferInteractionBuilder::buildInteraction(Ped questGiver)
{
	Object* pedGetToPlayer = new Object();
	Vector3 goTo = getForwardVector(player) * 1.15;
	AI::OPEN_SEQUENCE_TASK(pedGetToPlayer);
	AI::TASK_GO_TO_ENTITY(0, player, 20000, 1.15, 1, 0, 0);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, player, 1000, 0, 0, 0);
	AI::CLOSE_SEQUENCE_TASK(*pedGetToPlayer);

	Object* pedHandNote = new Object();
	AI::OPEN_SEQUENCE_TASK(pedHandNote);
	playAnimation(0, "handover_money_plyr", "script_common@handover@generic@player_handover@money");
	AI::CLOSE_SEQUENCE_TASK(*pedHandNote);

	Object* playerFaceQuestGiver = new Object();
	AI::OPEN_SEQUENCE_TASK(playerFaceQuestGiver);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, questGiver, 1000, 0, 0, 0);
	AI::CLOSE_SEQUENCE_TASK(*playerFaceQuestGiver);

	Object* playerTakeNote = new Object();
	AI::OPEN_SEQUENCE_TASK(playerTakeNote);
	playAnimation(0, "handover_money_male", "script_common@handover@generic@player_handover@money");
	AI::CLOSE_SEQUENCE_TASK(*playerTakeNote);

	Conversation* conv = new Conversation();
	conv->addLine(player, "GREET_GENERAL");
	conv->addAction(questGiver, pedGetToPlayer, 2);
	conv->addAction(player, playerFaceQuestGiver, 1);
	conv->addAction(questGiver, pedHandNote, 0);
	conv->addAction(player, playerTakeNote, 0);
	conv->addDelay(1000);
	conv->addLine(questGiver, "GENERIC_THANKS_SHOUTED");

	return conv;
}