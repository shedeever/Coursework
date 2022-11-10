#ifndef OBJECT3_H
#define OBJECT3_H

#include "Commands.h"

using namespace std;

class Tower : public Commands // Объект-наследник от базового класса, используемый для заполнения дерева
{
public:
    Tower(Commands* Obj, string name);				// Конструктор параметрами, который выхывает конструктор базового класса, передавая значение головного объекта и имя
    void signal(string& sig);					// Метод сигнала
    void handler(Commands* obj, string text);	// Метод обработчика
};

#endif