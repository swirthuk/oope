#include "medicine.h"
#include <string>
#include <iostream>
using namespace std;

 const char* const Medicine::timeNames[] = { "����", "�� �����", "����", "����", "����" };

Medicine::Medicine(const char* medicalName, int startDay) { // TODO n � d

	// TODO n - nullptr, strlen(n), -> ���������� � ����� (!)
	if (startDay < 1 || startDay > 365) {
		throw range_error("������� �� �������� (�� 1 �� 365 ������������)!");
	}
	if (medicalName == nullptr || medicalName == 0 || isdigit(medicalName[0]) || ispunct(medicalName[0])) {
		throw invalid_argument("�������� �������� �������� ���������!");
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
	cout << "������������ ���������: " << name << endl;
	cout << "������ � " << day << " ����." << endl;
	for (int i = 0; i < requiredReception; i++) {
		cout << timeNames[i] << ": ���� - " << (planReception[i] ? "��;\t" : "���;\t");
		cout << "������ - " << (didIt[i] ? "��;\t" : "���;\t") << endl;
	}
}

bool Medicine::checkPlan(TOD time) const{
	int index = static_cast<int>(time); //change enum to integer
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("������ ������� �� ������� �����!");
	}
	if ((!planReception[index] && didIt[index] ) || (planReception[index]  && !didIt[index] )) {
		return false;
	}
	return true;
}
void Medicine::setPlan(TOD time, bool shouldTake) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("�������� ����� ������!");
	}
	planReception[index] = shouldTake;
}

void Medicine::setDid(TOD time, bool taken) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("�������� ����� ������!");
	}
	didIt[index] = taken;
}
bool Medicine::needToTake(TOD time) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("�������� ����� ������!");
	}

	return planReception[index];
}
bool Medicine::isCorrect(TOD time) const{
	int index = static_cast<int>(time);
	if (index < 0 || index >= requiredReception) {
		throw out_of_range("�������� ����� ������!");
	}

	return planReception[index];
}