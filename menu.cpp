#include "menu.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <limits>

using namespace std;

Menu::Menu()
    : items_({"New", "Display", "Exit"}), selectedIndex_(0) {}


// ==================== INPUT HANDLER ====================
int Menu::getInputKey() const {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    // Handle arrow keys
    if (ch == 27) {
        getchar();
        ch = getchar();
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}


// ==================== SCREEN HELPERS ====================
void Menu::clearScreen() const {
    cout << "\033[2J\033[H";
}

void Menu::goToXY(int x, int y) const {
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}


// ==================== DRAW UI ====================
void Menu::drawBorder() const {
    cout << "\033[1;34m╔════════ MENU ═══════╗\033[0m\n";
}

void Menu::drawUsageLine() const {
    cout << "\n\033[1;34m↑↓ Move | Enter Select | Esc Exit | Backspace Main | Home New | End Exit\033[0m\n";
}

void Menu::drawMenu() const {
    clearScreen();
    drawBorder();

    for (size_t i = 0; i < items_.size(); ++i) {
        if (i == selectedIndex_) {
            cout << " \033[1;33m›\033[0m \033[1;32m" << items_[i] << "\033[0m\n";
        } else {
            cout << "   " << items_[i] << "\n";
        }
    }

    cout << "\033[1;34m╚═════════════════════╝\033[0m\n";
    drawUsageLine();
}


// ==================== MAIN LOGIC ====================
void Menu::run() {
    int key = 0;

    while (!(key == 27 || (key == 10 && items_[selectedIndex_] == "Exit"))) {
        drawMenu();
        key = getInputKey();
        handleKey(key);
    }
}


// ==================== KEY ACTIONS ====================
void Menu::handleKey(int key) {
    switch (key) {
        case 65: // up
            selectedIndex_ = (selectedIndex_ - 1 + items_.size()) % items_.size();
            break;

        case 66: // down
            selectedIndex_ = (selectedIndex_ + 1) % items_.size();
            break;

        case 72: // HOME
            selectedIndex_ = 0;
            break;

        case 70: // END
            selectedIndex_ = items_.size() - 1;
            break;

        case 10: // enter
            onEnter();
            break;

        case 127: // backspace
            break;

        case 27: // esc
            exit(0);
    }
}


// ==================== ENTER ACTION ====================
void Menu::onEnter() {
    string sel = items_[selectedIndex_];

    if (sel == "New")
        runNew();
    else if (sel == "Display")
        runDisplay();
    else if (sel == "Exit")
        exit(0);
}


// ==================== NEW ====================
void Menu::runNew() {
    while (true) {
        clearScreen();

        if (empCount_ >= MAX_EMP) {
            cout << "Employee limit reached! Cannot add more.\n";
            cout << "Press Enter to return...";
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            cin.get();
            return;
        }
        cout << "=== ADD NEW EMPLOYEE ===\n\n";
        Employee emp;

        emp.firstName = readNonEmptyString("Enter First Name: ");
        emp.lastName  = readNonEmptyString("Enter Last Name: ");
        emp.age       = readInt("Enter Age (1-120): ", 1, 120);

        emp.birth.day   = readInt("Enter Birth Day (1-31): ", 1, 31);
        emp.birth.month = readInt("Enter Birth Month (1-12): ", 1, 12);
        emp.birth.year  = readInt("Enter Birth Year (1900-2025): ", 1900, 2025);

        employees_[empCount_++] = emp;

        cout << "\nEmployee Saved Successfully!\n";

        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cout << "Add Another? (y/n): ";

        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');  // Clear the input buffer

        if (choice != 'y' && choice != 'Y') {
            return;
        }
    }
}


// ==================== DISPLAY ====================
void Menu::runDisplay() {
    clearScreen();

    if (empCount_ == 0) {
        cout << "No Employees Added.\n\n";
    } else {
        cout << "=== EMPLOYEE LIST ===\n\n";
        for (int i = 0; i < empCount_; ++i) {
            cout << i + 1 << ") "
                 << employees_[i].firstName << " "
                 << employees_[i].lastName
                 << " | Age: " << employees_[i].age
                 << " | Birth: "
                 << employees_[i].birth.day << "/"
                 << employees_[i].birth.month << "/"
                 << employees_[i].birth.year
                 << "\n";
        }
    }

    cout << "\nPress Enter or Backspace to return...";

    while (true) {
        int c = getInputKey();
        if (c == 10 || c == 127) return;
    }
}


// ==================== VALIDATION ====================
string Menu::readNonEmptyString(const string& msg) {
    string s;
    while (true) {
        cout << msg;
        cin >> s;
        if (!s.empty()) return s;
        cout << "Input cannot be empty. Try again.\n";
    }
}

int Menu::readInt(const string& msg, int min, int max) {
    int x;
    while (true) {
        cout << msg;
        if (cin >> x && x >= min && x <= max)
            return x;

        cout << "Invalid number! Enter a value between "
             << min << " and " << max << ".\n";

        cin.clear();
        cin.ignore(1000, '\n');
    }
}
