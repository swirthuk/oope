#pragma once
#include "IMedicine.h"
#include <iostream>

class Medicine : public IMedicine
{
private:
	char* name;
	unsigned int day;
	static const int requiredReception = 5;
	bool didIt[requiredReception] = {0};
	static const char* const timeNames[];
	bool planReception[requiredReception] = { 0 };
public:
	Medicine(const char* name, int day);
	Medicine(const Medicine& info, bool copy);
	virtual ~Medicine();
	const char* getName() const override;
	int getDay() const override;

	void print() const;
	void setPlan(TOD time, bool shouldTake) override;
	void setDid(TOD time, bool taken) override;
	bool needToTake(TOD time) const override;
	bool needToTake() override;

	bool checkPlan(TOD time) const override;
	bool isCorrect(TOD time) const override;

	
	bool& operator[] (TOD time) override;
};

std::ostream& operator<< (std::ostream& out, const Medicine& ourObject);
