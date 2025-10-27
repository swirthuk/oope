#pragma once



class Medicine
{
private:
	unsigned int day;
	static const int requiredReception = 5;
	bool didIt[requiredReception] = {0};
	static const char* const timeNames[];
protected:
	char* name;
	bool planReception[requiredReception] = { 0 };
public:
	Medicine(const char* name, int day);
	Medicine(const Medicine& info, bool copy);
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
	bool needToTake();

	bool checkPlan(TOD time) const;
	bool isCorrect(TOD time) const;

	
	bool& operator[] (TOD time);
};

std::ostream& operator<< (std::ostream& out, const Medicine& ourObject);
