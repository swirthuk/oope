#pragma once
#include <string>
#include <iostream>
using namespace std;

template <int REQUIREDRECEPTION>
class MedicineTemplate
{
private:
	char* name;
	unsigned int day;
	bool didIt[REQUIREDRECEPTION] = { 0 };
	bool planReception[REQUIREDRECEPTION] = { 0 };
	enum TOD {
		morning,
		before_dinner,
		dinner,
		supper,
		night,
		tod_size
	};
public:
	MedicineTemplate(const char* medicalName, int startDay) {
		if (startDay < 1 || startDay > 365) {
			throw range_error("Выходит за диапазон (от 1 до 365 включительно)!");
		}
		if (medicalName == nullptr || medicalName == 0 || !isalpha(medicalName[0])) {
			throw invalid_argument("Неверное значение названия лекарства!");
		}
		size_t medicines_name = strlen(medicalName);
		name = new char[medicines_name + 1];
		strcpy_s(name, medicines_name + 1, medicalName);
		day = startDay;
		for (int i = 0; i < REQUIREDRECEPTION; i++) {
			didIt[i] = false;
			planReception[i] = false;
		}
	}

	// ПЕРЕГРУЖЕННЫЙ КОНСТРУКТОР

	MedicineTemplate(const MedicineTemplate& info, bool copy) {
		size_t medicines_name = strlen(info.name);
		name = new char[medicines_name + 1];
		strcpy_s(name, medicines_name + 1, info.name);

		day = info.day + 1;

		if (copy) {
			for (int i = 0; i < REQUIREDRECEPTION; i++) {
				planReception[i] = info.planReception[i];
				didIt[i] = false;
			}
		}
		else {
			for (int i = 0; i < REQUIREDRECEPTION; i++) {
				planReception[i] = false;
				didIt[i] = false;
			}
		}
	}

	~MedicineTemplate() {
		delete[] name;
	}

	// ПЕРЕГРУЖЕННЫЕ МЕТОДЫ

	friend ostream& operator << (ostream& out, const MedicineTemplate& ourObject) {
		out << "Лекарство: " << ourObject.getName()
			<< " принимать в " << ourObject.getDay() << " день.";
		return out;
	}
	bool& operator[] (TOD time) {
		int index = static_cast<int>(time);
		return planReception[index];
	}

	const char* const getName() const {
		return name;
	}

	int getDay() const {
		return day;
	}

	void print() const {
		cout << "Наименование лекарства: " << name << endl;
		cout << "Принял в " << day << " день." << endl;
		for (int i = 0; i < REQUIREDRECEPTION; i++) {
			cout << "Прием в " << i+1 <<  ": Надо - " << (planReception[i] ? "Да;\t" : "Нет;\t");
			cout << "сделал - " << (didIt[i] ? "Да;\t" : "Нет;\t") << endl;
		}
	}

	bool checkPlan(TOD time) const {
		int index = static_cast<int>(time); //change enum to integer
		if (index > 5) index %= 5;
		if (index < 0 || index >= REQUIREDRECEPTION) {
			throw out_of_range("Индекс выходит за границы приёма!");
		}
		if ((!planReception[index] && didIt[index]) || (planReception[index] && !didIt[index])) {
			return false;
		}
		return true;
	}
	void setPlan(TOD time, bool shouldTake) {
		int index = static_cast<int>(time);
		if (index > 5) index %= 5;
		if (index < 0 || index >= REQUIREDRECEPTION) {
			throw out_of_range("Неверное время приема!");
		}
		planReception[index] = shouldTake;
	}

	void setDid(TOD time, bool taken) {
		int index = static_cast<int>(time);
		if (index > 5) index %= 5;
		if (index < 0 || index >= REQUIREDRECEPTION) {
			throw out_of_range("Неверное время приема!");
		}
		didIt[index] = taken;
	}
	bool needToTake(TOD time) const {
		int index = static_cast<int>(time);
		if (index > 5) index %= 5;
		if (index < 0 || index >= REQUIREDRECEPTION) {
			throw out_of_range("Неверное время приема!");
		}

		return planReception[index];
	}

	//ПЕРЕГРУЖЕННЫЙ МЕТОД

	bool needToTake() {
		for (int i = 0; i < REQUIREDRECEPTION; i++) {
			if (planReception[i] && !didIt[i] || !planReception[i] && didIt[i]) {
				return false;
			}
		}
		return true;
	}

	bool isCorrect(TOD time) const {
		int index = static_cast<int>(time);
		if (index > 5) index %= 5;
		if (index < 0 || index >= REQUIREDRECEPTION) {
			throw out_of_range("Неверное время приема!");
		}

		return planReception[index];
	}
};