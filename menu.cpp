#include "menu.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

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
    clearScreen();
    cout << "Enter text (Enter = save, Backspace = cancel):\n\n";

    string input;

    while (true) {
        int c = getInputKey();

        if (c == 127) { // cancel
            cout << "\nCancelled.\n";
            sleep(1);
            return;
        }
        else if (c == 10) { // save
            storedText_ = input;
            cout << "\nSaved.\n";
            sleep(1);
            return;
        }
        else {
            input += static_cast<char>(c);
            cout << static_cast<char>(c);
            cout.flush();
        }
    }
}


// ==================== DISPLAY ====================
void Menu::runDisplay() {
    clearScreen();

    if (storedText_.empty())
        cout << "No text stored.\n\n";
    else
        cout << "Stored text:\n" << storedText_ << "\n\n";

    cout << "Press Enter or Backspace to return...";

    while (true) {
        int c = getInputKey();
        if (c == 10 || c == 127)
            return;
    }
}
