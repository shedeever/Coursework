#include "Analytic.h"

Analytic::Analytic(Commands* Obj, string name) : Commands (Obj, name) { this->type = 5; };

void Analytic::signal(string& sig) {	// Метод сигнала
    cout << "\nSignal from " << GetCoord();
    sig = sig + " (class: 5)";
}

void Analytic::handler(Commands* Obj, string text) {	// Метод обработчика
    cout << "\nSignal to " << Obj->GetCoord() << " Text: " << text;
}