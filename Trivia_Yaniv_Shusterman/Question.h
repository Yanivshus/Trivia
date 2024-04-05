#pragma once
#include <iostream>
#include <vector>

#define CORRECT 3

class Question {
private:
	std::string m_question;
	std::vector<std::string> m_possiableAnswers; 
public:
	Question(std::string m_question, std::vector<std::string> m_possiableAnswers);
	std::string getQuestion();
	std::vector<std::string> getPossiableAnswers() const;
	std::string getCorrectAnswer() const;
};