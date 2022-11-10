#include "Tower.h"

Tower::Tower(Commands* Obj, string name) : Commands (Obj, name) { this->type = 3; };

void Tower::signal(string& sig) { // Метод сигнала
    cin.clear();
    getline(cin, sig);
}

void Tower::handler(Commands* Obj, string text) {	// Метод обработчика
    cout << "\nSignal to " << Obj->GetCoord() << " Text: " << text;
}