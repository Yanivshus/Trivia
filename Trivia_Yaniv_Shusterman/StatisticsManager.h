#pragma once
#include "IDatabase.h"
#include <iostream>
#include <vector>
#include <string>

class StatisticsManager {
private:
	IDatabase* m_database;
public:
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(const std::string& username);
	void setDB(IDatabase* m_database);
};
