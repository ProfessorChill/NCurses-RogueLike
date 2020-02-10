#include "functions.hpp"

int Function::getRandNum(int maxNum)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, maxNum);

	return dis(gen);
}

int Function::getRandNum(int minNum, int maxNum)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(minNum, maxNum);

	return dis(gen);
}
