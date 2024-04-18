#include "GameData.h"

GameData::GameData(const Question currentQuestion, const unsigned int correctAnswerCount, const unsigned int wrongAnswerCount, const double averageAnswerTime)
{
	this->averageAnswerTime = averageAnswerTime;
	this->correctAnswerCount = correctAnswerCount;
	this->wrongAnswerCount = wrongAnswerCount;
	this->currentQuestion = currentQuestion;
}
