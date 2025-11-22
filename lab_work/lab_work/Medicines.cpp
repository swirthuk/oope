#include "Medicines.h"
#include <iostream>
using namespace std;

Medicines::Medicines(const char* name, int day, const char* note ) : Medicine(name, day) {
	if (note && strlen(note) > 0) {
		this->note = new char[strlen(note) + 1];
		strcpy_s(this->note, strlen(note) + 1, note);
	}
	else {
		const char* text = "До еды, глотать.";
		this->note = new char[strlen(text) + 1];
		strcpy_s(this->note, strlen(text) + 1, note);
	}
}
Medicines::~Medicines(){
	delete[] note;
}
const char* const Medicines::getNote() const {
	return note;
}
void Medicines::setNote(const char* info) {
	delete[] note;
	note = nullptr;
	if (info) {
		note = new char[strlen(info) + 1];
		strcpy_s(note, strlen(info) + 1, info);
	}
	note = nullptr;
}
void Medicines::print() const {
	Medicine::print();
	if (note) {
		cout << note << endl;
	}
}