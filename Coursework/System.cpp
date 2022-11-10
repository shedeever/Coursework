#include "System.h"
#include "Pager.h"
#include "Tower.h"
#include <iostream>

using namespace std;

System::System(Commands* root) : Commands(root) { this->type = 1; }

void System::BildTree(){
    Commands* header; // Буфферные переменные для запоминания указателя на текущий объект и головной для текущего
    string name, way; // Переменные для ввода данных для добавления нового объекта
    int n;

    center = new Control(this, "Control Center");

    while (true) {
        (this->*(SIGNALL(Tower::signal)))(name);
        if (name == "End of information about pagers") break;

        center->AddPager(name);
    }
}

void System::SetStatus(string way) {
    Commands* header;
    int n;

    header = NULL;

    cin >> n; // Ввожу номер статуса

    header = this->SrcEl(way); // Вызов метода поиска нужного узла

    if (!header){
        cout << "\nObject " << way << " not found";
        return;
    }
    header->PutStat(n);
}


int System::RunProg() {
    string command, f_way, s_way, buf;					// Объявляю переменные для определения типа работы с деревом и задания пути к узлу
    Commands* current = 0, *buffer, *cur_buf = this;	// Объявляю указатель для задания текущего выбранного элемента, а так же 2 буфферных указателя, один общий, а один для текущего установленного объекта
    int tact = 0;
    bool flag = true;

    center->Sort();
    cout << "Ready to work";

    do {
        (this->*(SIGNALL(Tower::signal)))(command);
        if (command != "Turn off the system"){
            if (command.substr(0, 14) == "Send a message") {
                if (flag)
                    tact++;
                flag = false;
                command.erase(0, 15);
                command = to_string(tact) + " " + command;
                (center->*(SIGNALL(Control::SendMessage)))(command);
            }
            else if (command == "Display the system status"){
                flag = true;
                tact++;
                (center->*(SIGNALL(Control::Output)))(command);
            }
            else if (command.substr(0, 24) == "Display the pager status"){
                flag = true;
                tact++;
                command.erase(0, 25);
                (center->*(SIGNALL(Control::Status)))(command);
            }
            else if (command == "SET") {			// Если тип "SET", провожу поиск по дереву, и если метод SrcTp возвращает NULL, то вывожу сообщение об ошибке,
                buffer = cur_buf->SrcEl(f_way);
                if (!buffer){
                    cout << "\nObject is not found: " << current->GetName() << " " << f_way;
                }
                else {
                    cur_buf = buffer;
                    cout << "\nObject is set: " << current->GetName();
                }
            }
            else if (command == "FIND") {					// Если тип "FIND", предварительно определяю тип пути ('/' или '//'), потом вызываю поиск узла и вывожу соответстующие сообщения
                buffer = cur_buf->SrcEl(f_way);
                if (!buffer)
                    cout << endl << f_way << "     " << "Object is not found";
                else
                    cout << endl << f_way << "     " << "Object name: " << buffer->GetName();
            }
            else if (command == "EMIT"){
                getline(cin, s_way);
                buffer = this->SrcEl(f_way);
            }
            else if (command == "SET_CONNECT"){
                cin >> s_way;
                current = this->SrcEl(f_way);
                buffer = this->SrcEl(s_way);
            }
            else if (command == "DELETE_CONNECT"){
                cin >> s_way;
                current = this->SrcEl(f_way);
                buffer = this->SrcEl(s_way);
            }
            else if (command == "SET_CONDITION"){
                this->SetStatus(f_way);
            }
            else{
                flag = true;
                tact++;
            }

            buf = to_string(tact);
            center->Check(buf);
        }
    } while (command != "Turn off the system");

    return 0;
}

void System::Signal(string& sig) {
    cout << "\nSignal from " << GetCoord();
    sig += " (class: 1)";
}

void System::Handler(Commands* obj, string text) {
    cout << "\nSignal to " << obj->GetCoord() << " Text: " << text;
}