#ifndef APPLICATION_H
#define APPLICATION_H

#include "Commands.h"
#include "Control.h"

class System : public Commands
{
private:
    Control* center;
public:
    System(Commands* root);						//Конструктор с параметрами, который создает корневой объект, путем вызова конструктора базового класса

    void BildTree();								// Метод заполнения дерева согласно заданию
    void SetStatus(string way);						// Метод устанавливания статуса для каждого узла дерева
    int RunProg();									// Метод, вызывающий методы согласно заданию
    void Signal(string& sig);						// Метод сигнала
    void Handler(Commands* obj, string text);		// Метод обработчика
};

#endif