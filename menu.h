#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
using namespace std;

class Menu {
public:
    Menu();          
    void run();     

private:
    vector<string> items_;    
    size_t selectedIndex_;       
    string storedText_;       

    // Drawing 
    void drawMenu() const;
    void drawBorder() const;
    void drawUsageLine() const;

    // Key handling
    void handleKey(int key); 
    void onEnter();          
    void runNew();           
    void runDisplay();       

    // Input
    int getInputKey() const;

    // Console helpers
    void clearScreen() const;        
    void goToXY(int x, int y) const; 
};

#endif
