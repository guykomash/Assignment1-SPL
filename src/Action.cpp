#include "../include/Action.h"
#include <iostream>
#include <vector>
#include <string>
#include "../include/Trainer.h"
#include "../include/Studio.h"

extern Studio*  backup;
//Base Action Methods
BaseAction::BaseAction() {
    status=ERROR;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
void BaseAction::error(std::string errorMsg) {
    this->errorMsg=errorMsg;
    std::cout<<"Error:"+errorMsg;
    status=ERROR;
}

void BaseAction::clone_error() {
    status=ERROR;
}
void BaseAction::complete() {
    status=COMPLETED;
}
ActionStatus BaseAction::getStatus() const {
    return status;
}
//Open trainer Actions
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):
trainerId(id),customers(customersList),names_types(""){}

OpenTrainer *OpenTrainer::clone() {
    OpenTrainer* clone= new OpenTrainer(trainerId,customers);
    clone->names_types=names_types;
    if(getStatus()==ERROR)
        clone->error("Workout session does not exist or is already open.");
    else
        clone->complete();
    return clone;
}

void OpenTrainer::act(Studio &studio) {
    Trainer* t=studio.getTrainer(trainerId);
    if(  t== nullptr) {
        error("Workout session does not exist or is already open.");{
            // std::cout<<":Workout session does not exist or is already open."<<std::endl;
        }
    }
    else if(t->isOpen()==true) {
        int i=0;
        while (t->getCapacity() > 0 && i<(int)customers.size()){
            names_types= names_types+customers[i]->getName()+","+customers[i]->toString()+" ";
            std::cout<<customers[i]->getName()<<std::endl;
            i++;
         }
        error("Workout session does not exist or is already open.");
        // std::cout<<":Workout session does not exist or is already open."<<std::endl;
    }
    else {
        int i=0;
        while (t->getCapacity() > 0 && i<(int)customers.size()){
            t->addCustomer(customers[i]);
            names_types= names_types+customers[i]->getName()+","+customers[i]->toString()+" ";
            i++;
        }
        t->openTrainer();
        complete();
    }
}

std::string OpenTrainer::toString() const {
    std::string s="open "+std::to_string(trainerId)+getNameTypes();
    if(getStatus()==ERROR)
        return s+getStatusString()+getErrorMsg();
    return s+getStatusString();
}

std::string OpenTrainer::getNameTypes() const {
    return names_types;
}
//Order
Order::Order(int id) :
trainerId(id)
{}

Order *Order::clone() {
    Order* clone= new Order(trainerId);
    if(getStatus()==ERROR)
        clone->error("Trainer does not exist or is not open");
    else
        clone->complete();
    return clone;
}
void Order::act(Studio &studio) {
    Trainer * t=studio.getTrainer(trainerId);
    if(t->isOpen()== false || t== nullptr) {
        error("Trainer does not exist or is not open");
        // std::cout << "Trainer does not exist or is not open"<<std::endl;
    }
    else {
        std::vector<Workout>& workout_opt = studio.getWorkoutOptions();
        for (int i = 0; i < (int)t->getCustomers().size(); i++) {
            std::vector<int> workout_id = t->getCustomers()[i]->order(workout_opt);
            t->order(t->getCustomers()[i]->getId(),workout_id, workout_opt);
            std::string name=t->getCustomers()[i]->getName();
            for(int j=0;j<(int)workout_id.size();j++)
                std::cout<< name <<" is doing: "<<workout_opt[workout_id[j]].getName()<<std::endl;
        }
        t->openTrainer();
        complete();
    }
}
std::string Order::toString() const {
    if(getStatus()==ERROR)
        return "order "+  std::to_string(trainerId)+" "+getStatusString()+getErrorMsg();
    return  "order "+  std::to_string(trainerId)+" "+getStatusString();
}
//Move Customer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) :
srcTrainer(src),dstTrainer(dst),id(customerId)
{}
MoveCustomer *MoveCustomer::clone() {
    MoveCustomer* clone= new MoveCustomer(srcTrainer,dstTrainer,id);
    if(getStatus()==ERROR)
        clone->error("Cannot move customer.");
    else
        clone->complete();
    return clone;
}
void MoveCustomer::act(Studio &studio) {
    Trainer *t_src=studio.getTrainer(srcTrainer);
    Trainer *t_dst=studio.getTrainer(dstTrainer);
    Customer * c=t_src->getCustomer(id);
    if(t_dst== nullptr || t_src== nullptr || t_dst->isOpen()== false|| t_src->isOpen()== false || t_dst->getCapacity()==0 ||
    c== nullptr) {
        error("Cannot move customer.");
       std::cout<<"Cannot move customer."<<std::endl;
    }
    else{
        std::vector<OrderPair> &order_src=t_src->getOrders();
        for(int i=0;i<(int)order_src.size();i++){
            if(order_src[i].first==c->getId()) {
                t_src->UpdateSalary(order_src[i].second.getPrice()*(-1));
                order_src[i].first=-1;
            }
        }
        Customer * c_copy=c->clone();
        t_dst->addCustomer(c_copy);
        bool found=false;
        for(int i=0;i<(int)t_src->getOrders().size() && !found;i++){//move orders of the customer only if he ordered at the src trainer
            if(t_src->getOrders()[i].first==c->getId())
            t_dst->order(c_copy->getId(),c_copy->order(studio.getWorkoutOptions()),studio.getWorkoutOptions());
            found=true;
        }
        t_src->removeCustomer(c->getId());
        if(t_src->getCapacity()==t_src->getOriginCapacity()) {//close the trainer if he doesn't have customers
            Close *a = new Close(srcTrainer);
            a->act(studio);
            studio.pushAction(a);
        }
        complete();
    }
}
std::string MoveCustomer::toString() const {
    if(getStatus()==ERROR)
        return  "Move " + std::to_string(srcTrainer) + " " +std::to_string(dstTrainer)+" " + std::to_string(id)+" "+getStatusString()+getErrorMsg();
    return  "Move " + std::to_string(srcTrainer) + " " +std::to_string(dstTrainer)+" " + std::to_string(id)+" "+getStatusString();
}
//Close
Close::Close(int id) :trainerId(id){}

Close *Close::clone() {
    Close* clone= new Close(trainerId);
    if(getStatus()==ERROR)
        clone->error("Trainer does not exist or is not open");
    else
        clone->complete();
    return clone;
}
void Close::act(Studio &studio){
    Trainer *t=studio.getTrainer(trainerId);
    if(t== nullptr) {
        error("Trainer does not exist or is not open");
        // std::cout <<"Trainer does not exist or is not open"<<std::endl;
    }
    else if (!t->isOpen()){
        error("Trainer does not exist or is not open");
        // std::cout <<"Trainer does not exist or is not open"<<std::endl;
        }
    else {
        int size=(int)t->getCustomers().size();
        while(size!=0){
            t->removeCustomer(t->getCustomers()[0]->getId());
            size--;
        }
        t->getCustomers().clear();
        t->getOrders().clear();
        t->closeTrainer();
        std::cout<<"Trainer" <<trainerId<<"is closed. Salary" <<t->getSalary()<<"NIS"<<std::endl;
        complete();
    }
}

std::string Close::toString() const {
    if(getStatus()==ERROR)
      return "Close"+ std::to_string(trainerId)+" "+ getStatusString()+getErrorMsg();
    return "Close"+ std::to_string(trainerId)+" "+ getStatusString();
}

//Close All
CloseAll::CloseAll() {}

CloseAll *CloseAll::clone() {
    CloseAll* clone= new CloseAll();
    return clone;
}
void CloseAll::act(Studio &studio) {
    for (int i = 0; i <(int) studio.getTrainers().size(); i++) {
        if (studio.getTrainer(i)->isOpen()) {
            Close c = Close(i);
            c.act(studio);
        }
    }
    studio.getWorkoutOptions().clear();
    complete();
    studio.close();
    std::cout<<"The studio is now closed"<<std::endl;
}

std::string CloseAll::toString() const {
    return "closeall "+getStatusString();
}

//Print Workout options
PrintWorkoutOptions::PrintWorkoutOptions() {}

PrintWorkoutOptions *PrintWorkoutOptions::clone() {
    PrintWorkoutOptions* clone= new PrintWorkoutOptions();
        clone->complete();
    return clone;
}
void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> wk=studio.getWorkoutOptions();
    for(int i=0;i<(int)wk.size();i++)
        std::cout<<wk[i].getName()<<","<<wk[i].getType()<<","<<wk[i].getPrice()<<std::endl;
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options "+getStatusString();
}
//Print Trainer Status
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id){}

PrintTrainerStatus *PrintTrainerStatus::clone() {
    PrintTrainerStatus* clone= new PrintTrainerStatus(trainerId);
        clone->complete();
    return clone;
}
void PrintTrainerStatus::act(Studio &studio) {
    Trainer *t=studio.getTrainer(trainerId);
    if(t!= nullptr){
        std::string open_string;
        if(t->isOpen())
            open_string="Open";
        else open_string="Closed";
        std::cout<<"Trainer "<< trainerId<<" status: "<<open_string<<std::endl;
        if(t->isOpen()){
           std::cout<<"Customers:"<<std::endl;
           for(int i=0;i<(int)t->getCustomers().size();i++){
               std::cout<<t->getCustomers()[i]->getId()<<" "<<t->getCustomers()[i]->getName()<<std::endl;
           }
           std::vector<OrderPair> orders=t->getOrders();
           std::cout<<"Orders:"<<std::endl;
           for(int i=0;i<(int)orders.size();i++) {
               if(orders[i].first!=-1)
               std::cout << orders[i].second.getName() << " " << orders[i].second.getPrice() << "NIS " << orders[i].first << std::endl;
           }
           std::cout<<"Current trainer salary:"<<t->getSalary()<<"NIS"<<std::endl;
           complete();
        }
    }
}

std::string PrintTrainerStatus::toString() const {
    return "status "+std::to_string(trainerId)+" "+getStatusString();
}
//Print Activation Log
PrintActionsLog::PrintActionsLog() {}

PrintActionsLog *PrintActionsLog::clone() {
    PrintActionsLog* clone= new PrintActionsLog();
        clone->complete();
    return clone;
}
void PrintActionsLog::act(Studio &studio) {
    for(int i=0;i<(int)studio.getActionsLog().size();i++)
        std::cout<<studio.getActionsLog()[i]->toString()<<std::endl;
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log";
}
//BackUp
BackupStudio::BackupStudio() {}

BackupStudio *BackupStudio::clone() {
    BackupStudio* clone= new BackupStudio();
        clone->complete();
    return clone;
}
void BackupStudio::act(Studio &studio) {
    if(backup!= nullptr)
        delete backup;
    backup=new Studio(studio);

    complete();
    }

std::string BackupStudio::toString() const {
    return "backup"+getStatusString();
}
//Restore Studio
RestoreStudio::RestoreStudio() {}

RestoreStudio *RestoreStudio::clone() {
    RestoreStudio* clone= new RestoreStudio();
    if(getStatus()==ERROR)
        clone->error("No backup available");
    else
        clone->complete();

    return clone;
}
void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr){
        error("No backup available");
        // std::cout<<"No backup available"<<std::endl;
    }
    else {
       studio =*backup;
        complete();
    }
}

std::string RestoreStudio::toString() const {
    if(getStatus()==ERROR)
        return "restore"+ getStatusString()+getErrorMsg();
    return "restore"+ getStatusString();
}

std::string BaseAction::getStatusString() const {
    if(status==COMPLETED)
        return "COMPLETED";
    else return "ERROR";
}