#pragma once
#include "Question.h"

class GameData {
public:
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	double averageAnswerTime;
	bool hasPlayersFinished = false;

	GameData(const Question currentQuestion, const unsigned int correctAnswerCount, const unsigned int wrongAnswerCount, const double averageAnswerTime);
	GameData() {};

};