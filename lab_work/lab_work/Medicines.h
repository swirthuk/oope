#pragma once
#include "medicine.h"

class Medicines : public Medicine {
private:
	char* note;
public:
	Medicines(const char* name, int day, const char* note = nullptr);
	~Medicines();
	void setNote(const char* info);
	void print() const;
	const char* const getNote() const;
};

