#include "StatisticsManager.h"

std::vector<std::string> StatisticsManager::getHighScore()
{
	return this->m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username)
{
	std::vector<std::string> stats;
	//get all user stats.
	stats.push_back(std::to_string(this->m_database->getPlayerAverageAnswerTime(username)));
	stats.push_back(std::to_string(this->m_database->getNumOfCorrectAnswers(username)));
	stats.push_back(std::to_string(this->m_database->getNumOfTotalAnswers(username)));
	stats.push_back(std::to_string(this->m_database->getNumOfPlayerGames(username)));
	return stats;
}

void StatisticsManager::setDB(IDatabase* m_database)
{
	this->m_database = m_database;
}
