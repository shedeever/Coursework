#include "Status.h"

Status::Status(Commands* Obj, string name) : Commands (Obj, name) { this->type = 4; };

void Status::signal(string& sig) {
    cout << endl << sig;
}

void Status::handler(Commands* Obj, string text) {
    cout << "\nSignal to " << Obj->GetCoord() << " Text: " << text;
}