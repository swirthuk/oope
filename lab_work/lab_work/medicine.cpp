#include "medicine.h"
#include <string>
#include <iostream>
using namespace std;

const char* timeNames[] = { "утро", "до обеда", "обед", "ужин", "ночь" };
Medicine::Medicine(const char* n, int d) {
	if (d < 1 || d > 365) {
		throw range_error("Выходит за диапазон (от 1 до 365 включительно)!");
	}
	size_t medicines_name = strlen(n);
	name = new char[medicines_name + 1];
	strcpy_s(name, medicines_name+1, n);
	day = d;
	for (int i = 0; i < required_reciption; i++) {
		did_it[i] = false;
		plan_reciption[i] = false;
	}
}

Medicine::~Medicine() {
	delete[] name;
}
char* Medicine::getName() const {
	return name;
}

int Medicine::getDay() const {
	return day;
}

void Medicine::print() const{
	cout << "Наименование лекарства: " << name << endl;
	cout << "Принял в " << day << " день." << endl;
	for (int i = 0; i < required_reciption; i++) {
		cout << timeNames[i] << ": Надо - " << (plan_reciption[i] ? "Да;\t" : "Нет;\t");
		cout << "сделал - " << (did_it[i] ? "Да;\t" : "Нет;\t") << endl;
	}
}

bool Medicine::check_plan(TOD time) const{
	int index = static_cast<int>(time); //преобразует enum в целое число
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("Индекс выходит за границы приёма!");
	}
	if ((plan_reciption[index] == false && did_it[index] == true) || (plan_reciption[index] == true && did_it[index] == false)) {
		return false;
	}
	return true;
}
void Medicine::set_plan(TOD time, bool shouldTake) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("Неверное время приема!");
	}
	plan_reciption[index] = shouldTake;
}

void Medicine::set_did(TOD time, bool taken) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("Неверное время приема!");
	}
	did_it[index] = taken;
}
char* Medicine::need_to_take(TOD time) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("Неверное время приема!");
	}
	cout << (plan_reciption[index] ? "Стоит принять!" : "Не надо принимать!") << endl;
}
bool Medicine::is_correct(TOD time) const{
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("Неверное время приема!");
	}
	cout << "Принял ли " << timeNames[index] << " - " << (plan_reciption[index] == did_it[index] ? "Принял!" : "Не принял!") << endl;
}