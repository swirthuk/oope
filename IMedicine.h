#pragma once
class IMedicine
{
public:
    enum TOD {
        morning,
        before_dinner,
        dinner,
        supper,
        night
    };
	virtual ~IMedicine() {}; // виртуальных конструкторов, увы и ах, не существует!!!
    virtual const char* getName() const = 0; // Чисто виртуальный метод, который ну прям 100% нужно переопределить (такие классы, btw, называются абстрактными с:)
    virtual int getDay() const = 0;
    virtual void print() const = 0;
    virtual bool& operator[](TOD time) = 0;
    virtual void setPlan(TOD time, bool shouldTake) = 0;
    virtual void setDid(TOD time, bool taken) = 0;
    virtual bool needToTake(TOD time) const = 0;
    virtual bool needToTake() = 0;
    virtual bool isCorrect(TOD time) const = 0;
    virtual bool checkPlan(TOD time) const = 0;
};

