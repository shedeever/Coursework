#ifndef OBJECT4_H
#define OBJECT4_H

#include "Commands.h"
#include "Pager.h"

using namespace std;

class Control : public Commands // Объект-наследник от базового класса, используемый для заполнения дерева
{
private:
    vector<Pager*> pagers;
    vector<message> queue;
    int success_nums = 0;

public:
    Control(Commands* Obj, string name);				// Конструктор параметрами, который выхывает конструктор базового класса, передавая значение головного объекта и имя
    void AddPager(string& pager);
    void Check(string& command);
    void Status(string& command);
    void Sort();
    void SendMessage(string& sig);						// Метод сигнала
    void Output(string& sig);		                    // Метод обработчика
};

#endif