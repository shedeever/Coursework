//
// Created by Олег Басс on 17.06.2022.
//

#ifndef UNTITLED_PAGER_H
#define UNTITLED_PAGER_H

#include "Commands.h"

class Pager;

using namespace std;

struct message{
    string text;
    Pager* out_pager = NULL;
    Pager* in_pager = NULL;
    int tact;
};

class Pager : public Commands
{
private:
    vector<string> cache_input;
    vector<string> cache_output;
    message input;
public:
    Pager(Commands* Obj, string name);				// Конструктор параметрами, который выхывает конструктор базового класса, передавая значение головного объекта и имя
    void Output(string& sig);	                    // Метод сигнала
    void PutMess(message mess);
    message* GetMess();
    void PutOutCache(string& command);
    void PutInCache(string& command);
    void handler(Commands* obj, string& text);		// Метод обработчика
    void GetCache(string& command);
};


#endif //UNTITLED_PAGER_H
