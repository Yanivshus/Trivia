#pragma once
#include "IDatabase.h"
#include <iostream>
#include <vector>
#include <string>

class StatisticsManager {
private:
	IDatabase* m_database;
public:
	// returns a vector which has the highest scores by order of elemnts in it.
	std::vector<std::string> getHighScore();

	// returns user stats.
	std::vector<std::string> getUserStatistics(const std::string& username);

	// sets db for stat manager.
	void setDB(IDatabase* m_database);

	////singleton by reference.
	static StatisticsManager& getInstance() {
		static StatisticsManager theStatManager;
		return theStatManager;
	}
};
