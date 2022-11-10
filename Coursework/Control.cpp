#include "Control.h"
#include "Status.h"
#include "Pager.h"
#include <iterator>

Control::Control(Commands* Obj, string name) : Commands (Obj, name) { this->type = 2; };

void Control::AddPager(string& pager) {
    Pager* new_el = new Pager(nullptr, pager);
    this->pagers.push_back(new_el);
}

void Control::Sort(){
    Pager* temp;

    for (int i = 0; i < this->pagers.size(); i++){
        for (int j = this->pagers.size() - 1; j > 0; j--){
            if (this->pagers[j]->GetName() < this->pagers[j - 1]->GetName()) {
                temp = this->pagers[j];
                this->pagers[j] = this->pagers[j - 1];
                this->pagers[j - 1] = temp;
            }
        }
    }
}

void Control::Check(string& command) {
    vector<vector<message>::iterator> buf_iters;
    string buf_str;
    int tact = stoi(command);

    for (Pager* i : pagers) {
        if (i->GetMess()->tact == tact) {
            if (i->GetMess()->in_pager == nullptr){
                i->PutStat(0);
                buf_str = "Subscriber " + i->GetMess()->text.substr(0, i->GetMess()->text.find(' ')) + " not found";
                (this->*(SIGNALL(Status::signal)))(buf_str);
                buf_str = to_string(i->GetMess()->tact) + " " + i->GetMess()->text;
                i->GetMess()->out_pager->PutOutCache(buf_str);
                return;
            }

            i->GetMess()->tact++;
            this->queue.push_back(*i->GetMess());
            i->GetMess()->tact--;
            buf_str = to_string(i->GetMess()->tact) + " " + i->GetMess()->in_pager->GetName() + " " + i->GetMess()->text;
            i->PutOutCache(buf_str);
            i->PutStat(0);
        }
    }

    for (auto i = this->queue.begin(); i < this->queue.end(); i++){
        if (i->tact <= tact && i->in_pager->GetStat() == 0){
            i->in_pager->PutStat(1);
            for (Pager* j : this->pagers) {
                if (j == i->in_pager){
                    buf_str = to_string(tact) + " " + i->out_pager->GetName() + " " + i->text;
                    j->PutInCache(buf_str);
                    success_nums++;
                }
            }
            buf_iters.push_back(i);
        }
    }

    for (auto i : buf_iters){
        i->in_pager->PutStat(0);
        this->queue.erase(i);
    }
}

void Control::SendMessage(string& sig) {
    message new_el;
    Pager* buf = 0;
    string s_buf;
    int tact = stoi(sig.substr(0, sig.find(' ')));
    sig.erase(0, sig.find(' ') + 1);

    for (Pager* i : pagers){
        if (i->GetName() == sig.substr(0, sig.find(' '))){
            if (i->GetStat() == 1) return;

            new_el.out_pager = i;
            i->PutStat(1);
            buf = i;
            break;
        }
    }

    sig.erase(0, sig.find(' ') + 1);
    for (Pager* i : pagers) {
        if (i->GetName() == sig.substr(0, sig.find(' '))) {
            new_el.in_pager = i;
            break;
        }
    }
    if (new_el.in_pager == nullptr)
        new_el.text = sig;
    else {
        sig.erase(0, sig.find(' ') + 1);
        new_el.text = sig;
    }

    sig.size() % 10 == 0 ? new_el.tact = tact + sig.size() / 10 : new_el.tact = tact + sig.size() / 10 + 1;
    buf->PutMess(new_el);
}

void Control::Output(string& sig){
    for ( Pager* i : this->pagers){
        (this->*(HANDLERR(Pager::handler)))(i, sig);
    }
    sig = "Operator " + to_string(success_nums) + " " + to_string(this->queue.size());
    (this->*(SIGNALL(Status::signal)))(sig);
}

void Control::Status(string&command) {
    for (Pager* i : this->pagers) {
        if (i->GetName() == command)
            (i->*(SIGNALL(Pager::Output)))(command);
    }
}