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
	cout << "\nВведите наименование лекарства (in English): ";
	cin >> name;
	Medicine med(name, day);
	med.setPlan(Medicine::morning, true);
	med.setPlan(Medicine::before_dinner, true);
	med.setPlan(Medicine::dinner, false);
	med.setPlan(Medicine::supper, true);
	med.setPlan(Medicine::night, false);
	//char* checkName = med.getName(); 
	//cout << "Наименование -> " << checkName << endl;
	//int checkDay = med.getDay();
	//cout << "День -> " << checkDay << endl;
	med.setDid(Medicine::morning, false);
	med.setDid(Medicine::before_dinner, true);
	med.setDid(Medicine::dinner, false);
	med.setDid(Medicine::supper, true);
	med.setDid(Medicine::night, false);
	med.checkPlan(Medicine::morning);
	med.print();
	med.isCorrect(Medicine::morning);


	system("pause");
	return 0;
}