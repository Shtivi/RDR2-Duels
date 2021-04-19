#include "Main.h"

QuestOfferExecutor::QuestOfferExecutor(MapAreas area, Ped questGiver)
{
	this->area = area;
	this->questGiver = questGiver;
	this->outcome = QuestOfferOutcome::None;
	initialDistance = distanceBetweenEntities(questGiver, player);
	_isRunning = false;

	positiveReply = new Prompt("Accept", GAMEPLAY::GET_HASH_KEY("INPUT_CONTEXT_X"));
	negativeReply = new Prompt("Decline", GAMEPLAY::GET_HASH_KEY("INPUT_CONTEXT_B"));
	positiveReply->hide();
	negativeReply->hide();
}

bool QuestOfferExecutor::isRunning()
{
	return _isRunning;
}

QuestOfferOutcome QuestOfferExecutor::getOutcome()
{
	return outcome;
}

void QuestOfferExecutor::start()
{
	if (_isRunning) {
		return;
	}
	_isRunning = true;

	decorateQuestGiver(questGiver);
	Blip blip = createBlip(questGiver, BLIP_STYLE_WHITE_DOT, 0x1DCFAA8C);
	setBlipLabel(blip, "Contract Offer");
	RADAR::_0x662D364ABF16DE2F(blip, 0xD34253F0); // Pulse effect
	AI::TASK_LOOK_AT_ENTITY(questGiver, player, -1, 0, 0, 0);
	showHelpText("Someone is offerring you a contact. Interact with him to accept it.");
}

void QuestOfferExecutor::update()
{
	if (!isRunning())
	{
		return;
	}

	if (ENTITY::IS_ENTITY_DEAD(questGiver) || 
		!ENTITY::DOES_ENTITY_EXIST(questGiver) || 
		PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), questGiver) || 
		PED::IS_PED_SHOOTING(player) ||
		(int)PURSUIT::_0x259CE340A8738814(PLAYER::PLAYER_ID()) != 0 || 
		PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0 ||
		PED::IS_PED_IN_MELEE_COMBAT(player))
	{
		log("Quest giver spooked");
		setOutcome(QuestOfferOutcome::Declined);
	}

	float distance = distanceBetweenEntities(player, questGiver);

	if (distance <= 20 && !hadGreeted)
	{
		greetPlayer();
		hadGreeted = true;
	}

	if (distance <= 8)
	{
		positiveReply->show();
		negativeReply->show();
	}
	else
	{
		positiveReply->hide();
		negativeReply->hide();
	}

	if (positiveReply->isActivatedByPlayer())
	{
		positiveReply->setIsEnabled(false);
		negativeReply->setIsEnabled(false);
		AI::CLEAR_PED_TASKS(questGiver, 1, 0);
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(questGiver, true);
		Conversation* questOfferInteraction = QuestOfferInteractionBuilder().buildInteraction(questGiver);
		questOfferInteraction->play();
		WAIT(2000);
		setOutcome(QuestOfferOutcome::Accepted);
	}
	else if (negativeReply->isActivatedByPlayer())
	{
		positiveReply->setIsEnabled(false);
		negativeReply->setIsEnabled(false);
		setOutcome(QuestOfferOutcome::Declined);
	}

	if (distance >= 30 + initialDistance)
	{
		setOutcome(QuestOfferOutcome::Declined);
	}
}

void QuestOfferExecutor::stop()
{
	cleanup();
	_isRunning = false;
}

MapAreas QuestOfferExecutor::getArea()
{
	return area;
}

void QuestOfferExecutor::greetPlayer()
{
	playAmbientSpeech(questGiver, "GREET_SHOUTED");
	AI::TASK_TURN_PED_TO_FACE_ENTITY(questGiver, player, -1, 0, 0, 0);
}

void QuestOfferExecutor::decorateQuestGiver(Ped questGiver)
{
	//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(questGiver, true);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(questGiver, 1, 1);
	positiveReply->setTargetEntity(questGiver);
	negativeReply->setTargetEntity(questGiver);
}


void QuestOfferExecutor::setOutcome(QuestOfferOutcome outcome)
{
	log(string("outcome: ").append(outcome == QuestOfferOutcome::Accepted ? "ACCEPETED" : "DECLINED"));
	this->outcome = outcome;
	stop();
}

void QuestOfferExecutor::cleanup()
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(questGiver, false);
	positiveReply->remove();
	negativeReply->remove();
	Blip pedBlip = RADAR::GET_BLIP_FROM_ENTITY(questGiver);
	deleteBlipSafe(&pedBlip);
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&questGiver);
}