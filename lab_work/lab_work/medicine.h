#pragma once

enum TOD {
	morning,
	before_dinner,
	dinner,
	supper,
	night
};

class Medicine
{
private:
	char* name;
	unsigned int day;
	static const int required_reciption = 5;
	bool plan_reciption[required_reciption];
	bool did_it[required_reciption];
public:
	Medicine(const char* Name, int day);
	~Medicine();
	char* getName() const;
	int getDay() const;

	void print() const;
	void set_plan(TOD time, bool shouldTake);
	void set_did(TOD time, bool taken);
	char* need_to_take(TOD time);
	bool check_plan(TOD time) const;
	bool is_correct(TOD time) const;
};