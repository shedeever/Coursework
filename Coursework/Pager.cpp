#include "Pager.h"
#include "Status.h"

Pager::Pager(Commands* Obj, string name) : Commands (Obj, name) { this->type = 3; };

void Pager::PutOutCache(string& sig) {
    this->cache_output.push_back(sig);
}

void Pager::PutInCache(string& sig) {
    this->cache_input.push_back(sig);
}

void Pager::PutMess(message mess){
    input = mess;
}

message* Pager::GetMess(){
    return &input;
}

void Pager::GetCache(string& command){
    cout << to_string(this->cache_output.size()) << " " << to_string(this->cache_input.size());
}

void Pager::Output(string& sig){
    string buf_sig;
    if (status == 1)
        return;
    else{
        buf_sig = "The list of messages sent by the pager " + this->name;
        (this->*(SIGNALL(Status::signal)))(buf_sig);
        for(string i : this->cache_output) {
            (this->*(SIGNALL(Status::signal)))(i);
        }
        buf_sig = "List of received messages by the pager " + this->name;
        (this->*(SIGNALL(Status::signal)))(buf_sig);
        for(string i : this->cache_input) {
            (this->*(SIGNALL(Status::signal)))(i);
        }
    }
}

void Pager::handler(Commands* obj, string& text) {
    cout << "\nPager " << obj->GetName() << " ";
    (obj->*(SIGNALL(Pager::GetCache)))(text);
}