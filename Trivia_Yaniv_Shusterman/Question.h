#pragma once
#include <iostream>
#include <vector>

#define CORRECT 3

class Question {
private:
	std::string m_question;
	std::vector<std::string> m_possiableAnswers; 
public:
	Question() {};
	Question(std::string m_question, std::vector<std::string> m_possiableAnswers);
	//getters.
	std::string getQuestion() const;
	std::vector<std::string> getPossiableAnswers() const;
	std::string getCorrectAnswer() const;
};