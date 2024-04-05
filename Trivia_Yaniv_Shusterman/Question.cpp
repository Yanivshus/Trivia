#include "Question.h"

Question::Question(std::string m_question, std::vector<std::string> m_possiableAnswers)
{
	this->m_question = m_question;
	this->m_possiableAnswers = m_possiableAnswers;
}

std::string Question::getQuestion()
{
	return this->m_question;
}

std::vector<std::string> Question::getPossiableAnswers() const
{
	return this->m_possiableAnswers;
}

std::string Question::getCorrectAnswer() const
{
	return this->m_possiableAnswers[CORRECT];
}
