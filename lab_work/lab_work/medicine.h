#pragma once



class Medicine
{
private:
	char* name;
	unsigned int day;
	static const int requiredReception = 5;
	bool planReception[requiredReception] = {0};
	bool didIt[requiredReception] = {0};
	bool needToCopy;

	static const char* const timeNames[];
public:
	Medicine(const char* name, int day);
	Medicine(const char* name, int day, bool planReception, bool copy);
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
	bool needToTake(TOD time) const; 
	bool checkPlan(TOD time) const;
	bool isCorrect(TOD time) const; 
};