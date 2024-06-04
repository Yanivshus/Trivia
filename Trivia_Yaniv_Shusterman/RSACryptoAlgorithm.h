#pragma once
#include "CryptoAlgorithm.h"
#include "cryptlib.h"

class RSACryptoAlgorithm : public CryptoAlgorithm {
public:
	virtual std::string ecnrypt(std::string);
	virtual std::string decrypt(std::string);
private:
	CryptoPP::PrivateKey serverKey;
};
