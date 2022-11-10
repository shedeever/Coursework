#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Commands;

typedef void (Commands::*signall)(string&);
typedef void (Commands::*handler)(Commands*, string&);

#define SIGNALL(_signal) (signall) (&_signal)
#define HANDLERR(_handler) (handler) (&_handler)

class Commands // Базовый объект
{
protected:
    string name;
    Commands* head;
    vector<Commands*> childs;
    int status = 0;
    int type;

    struct con{
        signall sig;
        handler hand;
        Commands* obj;
    };
    vector<con>connects;

public:
    Commands(Commands* Obj, string name = "Base_Object");				// Конструктор с параметрами (имя - параметр по умолчанию ("Base_Object"))
    ~Commands();														// Деструктор

    void PutName(string name);  									// Метод задающий имя текущего элемента
    void PutNode(Commands* node); 									// Метод добавляющий новый дочерний элемент к текущему
    void PutStat(int stat);											// Метод задающий статус узла
    string GetName(); 												// Метод, возвращающий имя текущего объекта
    Commands* GetHead();												// Метод, возвращающий указатель на головной элемент
    int GetStat();													// Метод, возвращающий статус
    int GetType();													// Метод, возвращающий тип объекта
    string GetCoord();												// Метод, возвращающий абсолютную координату для объекта
    Commands* SrcNd(Commands* header, string head);						// Метод поиска элемента по абсолютному адресу
    Commands* SrcEl(string way);										// Метод, который рекурсивно ищет нужный узел в дереве и возвращает ссылку на него
    void PrintTree(int i = -1); 									// Метод рекурсивного вывода дерева
    void PrintStatus(int i = -1);									// Метод рекурсивного вывода дерева со статусом готовности
    void ChangeHeadObject(Commands* node);							// Метод смены головного объекта для текущего
    void TurnOff();													// Метод выключения всех дочерних элементов для текущего
    void ReadyAll();												// Метод, включающий все узлы
    void Connect(signall in, handler hand, Commands* to_obj);			// Метод соединения узлов
    void DelConn(signall in, handler hand, Commands* to_obj);			// Метод разрыва связи между узлами
    void Send(signall in, string& to_obj);							// Метод отправки сообщения
};

#endif