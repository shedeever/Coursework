#include "Commands.h"
#include <iostream>
#include <algorithm>

using namespace std;

Commands::Commands(Commands* Obj, string name){
    this->head = Obj;						// Задаю значение поля head
    this->name = name;						// Задаю значение поля name
    this->type = 1;
    if (head)
        this->head->PutNode(this); // Добавляю созданный элемент в вектор дочерних для родителя
}

Commands::~Commands(){
    for (Commands* i : this->childs)	// Очищаю память
        delete i;
}

void Commands::PutName(string name) {
    this->name = name;
}

void Commands::PutNode(Commands* node) {
    this->childs.push_back(node);
}

void Commands::PutStat(int stat) {
    if (stat == 0)
        this->TurnOff();
    else if (!this->GetHead() || this->GetHead()->GetStat() != 0) 	// Если header указывает на корневой объект или статус
        this->status = stat;								 		// головного объекта не равен нулю, то присваиваю статусу текущего элемента n
}

string Commands::GetName() {
    return this->name;
}

Commands* Commands::GetHead() {
    return this->head;
}

int Commands::GetStat() {
    return this->status;
}

int Commands::GetType() {
    return this->type;
}

string Commands::GetCoord() {
    string coord = "";
    Commands* current = this;

    while (current->GetHead()) {
        coord = "/" + current->GetName() + coord;
        current = current->GetHead();
    }

    return coord.empty() ? "/" : coord;
}

Commands* Commands::SrcNd(Commands* header, string head){	// Метод рекурсивного поиска по дереву
    for (Commands* i : this->childs)
        header = i->SrcNd(header, head);

    if (this->GetName() == head)
        return this;
    else
        return header;
}

Commands* Commands::SrcEl(string way) {	// Метод поиска элемента по пути
    bool flag = false;
    Commands* header = this;
    string curr_el;	// Строка для записи имени элемента из пути

    if (way == ".") // Если путь равен "." вывожу текущий установленный объект, если "/" то вывожу корневой объект
        return this;

    if (way == "/"){
        while(header->GetHead())
            header = header->GetHead();

        return header;
    }

    if (way[1] == way[0] && way[1] == '/'){ //Если первые 2 знака пути: "//", то рекурсивно пройду все дерево
        while(header->GetHead())
            header = header->GetHead();

        way.erase(0, 2);
        header = header->SrcNd(header, way);
        if (header->GetName() == way)
            return header;
        else
            return nullptr;
    }
    if (way[0] == '/') {
        while(header->GetHead())
            header = header->GetHead();

        way.erase(0, 1);
    }

    while (!way.empty()) {
        flag = false;
        curr_el = way.substr(0, way.find("/")); // Иначе разделяю строку по разделителю "/" и ищу элемент имя которого равно вырезанному из пути по разделителю
        way.erase(0, way.find("/"));

        for (Commands* i : header->childs){
            if (i->GetName() == curr_el) {
                flag = true;
                if (way.empty())
                    return i;
                else {
                    header = i;
                    way.erase(0, 1);
                    break;
                }
            }
        }

        if (!flag)
            return nullptr;
    }

    return header;
}

void Commands::PrintTree(int i) {
    i++;								// Постфиксный инкремент для реализации отступа
    cout << endl;
    for (int j = 0; j < i; j++)			// Цикл, который реализовывает отступ
        cout << "    ";
    cout << this->GetName();	// Вывод названия узла
    for (Commands* k : this->childs)		// Цикл перехода на следующий элемент
        k->PrintTree(i);
}

void Commands::PrintStatus(int i) {
    cout << endl;
    i++;
    for (int j = 0; j < i; j++)
        cout << "    ";
    cout << this->GetName() << (this->GetStat() == 0 ? " is not ready" : " is ready"); // С помощью тернарного оператора, вывожу сообщение о статусе, в зависимоси от значения поля status для текущего узла
    for (Commands* k : this->childs)
        k->PrintStatus(i);
}

void Commands::ChangeHeadObject(Commands* node) {
    if (this->head){	// Если текущий элемент не корневой, то удаляю текущий из вектора дочерних объектов для текущего
        auto iter = find(this->head->childs.begin(), this->head->childs.end(), this); // Нахожу итератор на текущий элемент в векторе дочерних элементов текущего класса

        if (iter != this->head->childs.end())
            this->head->childs.erase(iter); // Удаляю указатель на элемент из вектора дочерних элементов родительского класса

        this->head = node; // Присваиваю родительскому объекту для текущего указатель на новый родительский объект
    }

    node->PutNode(this); // Добавляю указатель текущий элемент в вектор нового родительского класса
}

void Commands::TurnOff() {
    this->status = 0;				// Меняю статус элемента node или его дочернего на 0

    for (Commands* i : this->childs)	// Рекурсивно вызываю этот метод для каждого из дочерних жлементов node
        i->TurnOff();
}

void Commands::ReadyAll() {
    this->PutStat(1);
    for (Commands* i : childs)
        i->ReadyAll();
}

void Commands::Connect(signall in, handler hand, Commands* to_obj) {
    for (con i : connects) {	// Перебираю вектор соединений и если не нахожу повторений то добавляю в вектор новый элемент
        if (i.sig == in && i.hand == hand && i.obj == to_obj)
            return;
    }

    connects.push_back(con { in, hand, to_obj });
}

void Commands::DelConn(signall in, handler hand, Commands* to_obj) {
    for (auto i = connects.begin(); i != connects.end(); i++) {	// Перебираю вектор соединений и если нахожу повторение то удаляю элемент из вектора и выхожу из метода
        if (i->sig == in && i->hand == hand && i->obj == to_obj){
            connects.erase(i);
            return;
        }
    }
}

void Commands::Send(signall in, string& command){
    bool flag = false;
    string buf;
    for (int i = 0; i < connects.size(); i++) {	// Перебираю вектор соединений и если нахожу совпадающие сигналы и объект обработчика, то выполняю метод сигнала и метод обработчика
        if (connects[i].sig == in && connects[i].obj->GetStat() != 0){
            if (!flag){
                (this->*connects[i].sig)(command);
                flag = true;
            }
            buf = command;
            (this->*connects[i].hand)(connects[i].obj, buf);
        }
    }
}