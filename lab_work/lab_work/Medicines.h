#pragma once
class Medicines : public Medicine {
private:
	char* note;
public:
	Medicines(const char* name, int day, const char* note = nullptr);
	~Medicines();
	void setNote(const char* info);
	void printNote() const;
};

