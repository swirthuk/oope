#include <iostream>
#include <vector>
#include "medicine.h"
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int day;
	char name[50];
	cout << "Введите число, не превышающее 365: ";
	cin >> day;
	cin.getline(name, 50);
	cout << "\nВведите наименование лекарства: ";
	cin >> name;
	Medicine med(name, day);
	med.set_plan(morning, true);
	med.set_plan(before_dinner, true);
	med.set_plan(dinner, false);
	med.set_plan(supper, true);
	med.set_plan(night, false);
	//char* checkName = med.getName(); 
	//cout << "Наименование -> " << checkName << endl;
	//int checkDay = med.getDay();
	//cout << "День -> " << checkDay << endl;
	med.set_did(morning, false);
	med.set_did(before_dinner, true);
	med.set_did(dinner, false);
	med.set_did(supper, true);
	med.set_did(night, false);
	med.check_plan(morning);
	med.print();
	med.is_correct(dinner);



	return 0;
}