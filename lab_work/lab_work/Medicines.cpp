#include "Medicines.h"
#include "medicine.h"
#include <iostream>
using namespace std;

Medicines::Medicines(const char* name, int day, const char* note = nullptr) : Medicine(name, day) {
	if (note) {
		this->note = new char[strlen(note) + 1];
		strcpy(this->note, note);
	}
	else {
		this->note = nullptr;
	}
}
Medicines::~Medicines(){
	delete[] note;
}
void Medicines::setNote(const char* info) {
	delete[] note;
	if (!info) {
		note = nullptr;
	}
	note = new char[strlen(info) + 1];
	strcpy(note, info);
}
void Medicines::printNote() const {
	Medicine::print();
	if (note) {
		cout << note << endl;
	}
}