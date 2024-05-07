#pragma once
#include <iostream>
#include "Question.h"
#include <list>
#include <vector>
#include "StructHelper.h"
#include "GameData.h"

class IDatabase {
public:
	virtual bool open() = 0;
	virtual bool close() = 0;

	virtual int doesUserExist(const std::string& username) = 0;
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;
	virtual std::list<Question> getQuestions(const int amount) = 0;
	virtual double getPlayerAverageAnswerTime(const std::string& username) = 0;
	virtual int getNumOfCorrectAnswers(const std::string& username) = 0;
	virtual int getNumOfTotalAnswers(const std::string& username) = 0;
	virtual int getNumOfPlayerGames(const std::string& username) = 0;
	virtual int getPlayerScore(const std::string& username, int gameId) = 0;
	virtual std::vector<std::string> getHighScores() = 0;
	virtual int submitGameStatistics(GameData data, int gameId, const std::string& uNmae) = 0;
	virtual int addGameToGames(const unsigned int gameId) = 0;
	virtual int addQuestionToDB(const std::string& question, const std::string& w_answer1, const std::string& w_answer2, const std::string& w_answer3, const std::string& c_answer4) = 0;
};