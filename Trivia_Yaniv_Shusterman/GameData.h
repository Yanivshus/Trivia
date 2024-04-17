#include "Question.h"

class GameData {
public:
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;

	GameData(const Question currentQuestion, const unsigned int correctAnswerCount, const unsigned int wrongAnswerCount, const unsigned int averageAnswerTime);
	GameData() {};

};