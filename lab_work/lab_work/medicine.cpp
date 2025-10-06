#include "medicine.h"
#include <string>
#include <iostream>
using namespace std;

const char* timeNames[] = { "����", "����� ������", "����", "����", "����" };
Medicine::Medicine(const char* n, int d) {
	size_t medicines_name = strlen(n);
	char* Name = new char[medicines_name + 1];
	strcpy(Name, n);
	day = d;
}

char* Medicine::getName() const {
	return name;
}

int Medicine::getDay() const {
	return day;
}

void Medicine::print() const{
	cout << "������������ ���������: " << name << endl;
	cout << "������ � " << day << "����." << endl;
	for (int i = 0; i < required_reciption; i++) {
		cout << timeNames[i] << ": ���� - " << (plan_reciption[i] ? "��\t" : "���\t");
		cout << "������ - " << (did_it[i] ? "��\t" : "���\t") << endl;
	}
}

bool Medicine::check_plan(TOD time) const{
	int index = static_cast<int>(time); //����������� enum � ����� �����
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("������ ������� �� ������� �����!");
	}
	if ((plan_reciption[index] == false && did_it[index] == true) || (plan_reciption[index == true && did_it[index] == false])) {
		return false;
	}
	return true;
}
void Medicine::set_plan(TOD time, bool shouldTake) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("�������� ����� ������!");
	}
	plan_reciption[index] = shouldTake;
}

void Medicine::set_did(TOD time, bool taken) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("�������� ����� ������!");
	}
	did_it[index] = taken;
}
string Medicine::need_to_take(TOD time) {
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("�������� ����� ������!");
	}
	cout << (plan_reciption[index] ? "����� �������!" : "�� ���� ���������!") << endl;
}
bool Medicine::is_correct(TOD time) const{
	int index = static_cast<int>(time);
	if (index < 0 || index >= required_reciption) {
		throw out_of_range("�������� ����� ������!");
	}
	cout << (plan_reciption[index] == did_it[index] ? "������!" : "�� ������!") << endl;
}