#include "medicine.h"
#include <string>
#include <iostream>
using namespace std;

 const char* const Medicine::timeNames[] = { "утро", "до обеда", "обед", "ужин", "ночь" };

Medicine::Medicine(const char* medicalName, int startDay) { // TODO n и d

	// TODO n - nullptr, strlen(n), -> требования к имени (!)
	if (startDay < 1 || startDay > 365) {
		throw range_error("Выходит за диапазон (от 1 до 365 включительно)!");
	}
	if (medicalName == nullptr || medicalName == 0 || isdigit(medicalName[0]) || ispunct(medicalName[0])) {
		throw invalid_argument("Неверное значение названия лекарства!");
	}
	size_t medicines_name = strlen(medicalName);
	name = new char[medicines_name + 1];
	strcpy_s(name, medicines_name + 1, medicalName);
	day = startDay;
	for (int i = 0; i < requiredReception; i++) {
		didIt[i] = false;
		planReception[i] = false;
	}
}

Medicine::~Medicine() {
	delete[] name;
}
const char* const Medicine::getName() const {
	return name;
}

int Medicine::getDay() const {
	return day;
}


void Medicine::print() const{
	cout << "Наименование лекарства: " << name << endl;
	cout << "Принял в " << day << " день." << endl;
	for (int i = 0; i < requiredReception; i++) {
		cout << timeNames[i] << ": Надо - " << (planReception[i] ? "Да;\t" : "Нет;\t");
		cout << "сделал - " << (didIt[i] ? "Да;\t" : "Нет;\t") << endl;
	}
}

bool Medicine::checkPlan(TOD time) const{
	int index = static_cast<int>(time); //change enum to integer
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("Индекс выходит за границы приёма!");
	}
	if ((!planReception[index] && didIt[index] ) || (planReception[index]  && !didIt[index] )) {
		return false;
	}
	return true;
}
void Medicine::setPlan(TOD time, bool shouldTake) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("Неверное время приема!");
	}
	planReception[index] = shouldTake;
}

void Medicine::setDid(TOD time, bool taken) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("Неверное время приема!");
	}
	didIt[index] = taken;
}
bool Medicine::needToTake(TOD time) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("Неверное время приема!");
	}

	return planReception[index];
}
bool Medicine::isCorrect(TOD time) const{
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("Неверное время приема!");
	}

	return planReception[index];
}