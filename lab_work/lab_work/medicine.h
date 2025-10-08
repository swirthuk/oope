#pragma once



class Medicine
{
private:
	char* name;
	unsigned int day;
	static const int requiredReception = 5;
	bool planReception[requiredReception] = {0};
	bool didIt[requiredReception] = {0};

	static const char* const timeNames[]; // TODO имя в CONST_CASE
public:
	Medicine(const char* name, int day); // TODO Name
	~Medicine();
	const char* const getName() const;
	int getDay() const;

	enum TOD {
		morning,
		before_dinner,
		dinner,
		supper,
		night
	};

	void print() const;
	void setPlan(TOD time, bool shouldTake);
	void setDid(TOD time, bool taken);
	bool needToTake(TOD time); // TODO return bool
	bool checkPlan(TOD time) const;
	bool isCorrect(TOD time) const; // TODO return bool
};