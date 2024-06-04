#pragma once
#include <iostream>

class CryptoAlgorithm {
public:
	virtual std::string ecnrypt(std::string) = 0;
	virtual std::string decrypt(std::string) = 0;
};
