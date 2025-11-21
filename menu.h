#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
using namespace std;

struct BirthDate {
    int day;
    int month;
    int year;
};

struct Employee {
    string firstName;
    string lastName;
    int age;
    BirthDate birth;
};

class Menu {
public:
    Menu();
    void run();

private:
    static const int MAX_EMP = 100;
    Employee employees_[MAX_EMP];
    int empCount_ = 0;

    vector<string> items_;
    int selectedIndex_;
    string storedText_;

    // Drawing
    void drawMenu() const;
    void drawBorder() const;
    void drawUsageLine() const;

    // Logic
    void handleKey(int key);
    void onEnter();

    void runNew();      // add employee
    void runDisplay();  // list employees

    // Input helpers
    int getInputKey() const;
    void clearScreen() const;
    void goToXY(int, int) const;

    // Validation helpers
    string readNonEmptyString(const string& msg);
    int readInt(const string& msg, int min, int max);
};

#endif
