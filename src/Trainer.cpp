#include "../include/Trainer.h"
#include <vector>
#include <iostream>
#include "../include/Customer.h"

using namespace std;
//constructor
Trainer::Trainer(int t_capacity):capacity(t_capacity),open(false),customersList(),
                                 orderList(),salary(0),orig_capacity(capacity){}

//destructor
Trainer::~Trainer(){
     for(auto i:customersList) {
         if (i != nullptr)
             delete i;
     }
    customersList.clear();
    orderList.clear();
}
//copy constructor
Trainer::Trainer(const Trainer &other) {
    capacity=other.capacity;
    open=other.open;
    for(auto i:other.orderList) {
        orderList.emplace_back(OrderPair (i.first,Workout(i.second.getId(),i.second.getName(),i.second.getPrice(),i.second.getType())));
    }
    for (int i = 0; i <(int) other.customersList.size(); ++i){
        Customer * c= other.customersList[i]->clone();
        customersList.push_back(c);
}
    orig_capacity=other.orig_capacity;
    salary=other.salary;
}
//move constructor
Trainer::Trainer(Trainer &&other) {
    capacity=other.capacity;
    open=other.open;
    for(auto i:other.orderList) {
        orderList.emplace_back(OrderPair (i.first,Workout(i.second.getId(),i.second.getName(),i.second.getPrice(),i.second.getType())));
    }
    customersList=std::vector<Customer*>();
    for (int i = 0; i < (int)other.customersList.size(); ++i)
        customersList.insert(customersList.begin()+i,other.customersList[i]);
    for (int i = 0; i <(int) other.customersList.size(); ++i)
        delete other.customersList[i];
    other.customersList.clear();
    orig_capacity=other.orig_capacity;
    salary=other.salary;
}
//assignment operator
Trainer &Trainer::operator=(const Trainer &other) {
    if (this != &other) {
        for (int i = 0; i <(int) other.customersList.size(); ++i) {
            if(other.customersList[i]!= nullptr)
                delete other.customersList[i];
        }
        customersList.clear();
        capacity = other.capacity;
        open = other.open;
        for (auto i: other.orderList) {
            orderList.emplace_back(OrderPair(i.first, Workout(i.second.getId(), i.second.getName(), i.second.getPrice(),
                                                              i.second.getType())));//This line doesn't work?
        }
        for (int i = 0; i <(int) other.customersList.size(); ++i) {
            Customer *newCus = other.customersList[i]->clone();
            customersList.push_back(newCus);
        }
        orig_capacity = other.orig_capacity;
        salary = other.salary;
    }
        return *this;
}
//move assignment operator
Trainer &Trainer::operator=(Trainer &&other) {
    for (int i = 0; i < (int)customersList.size(); ++i)
        delete customersList[i];
    customersList.clear();
    capacity=other.capacity;
    open=other.open;
    for(auto i:other.orderList) {
        orderList.emplace_back(OrderPair (i.first,Workout(i.second.getId(),i.second.getName(),i.second.getPrice(),i.second.getType())));//This line doesn't work?
    }
    customersList=std::vector<Customer*>();
    for (int i = 0; i < (int)other.customersList.size(); ++i)
        customersList.insert(customersList.begin()+i,other.customersList[i]);
    orig_capacity=other.orig_capacity;
    salary=other.salary;
    for (int i = 0; i <(int) other.customersList.size(); ++i)
        delete other.customersList[i];
    other.customersList.clear();
    return *this;
}

void Trainer::addCustomer(Customer *customer) {
        customersList.push_back(customer);
        capacity--;
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::removeCustomer(int id) {
    for (int i = 0; i < (int)customersList.size(); ++i) {
        if (customersList[i]!= nullptr){
            if (customersList[i]->getId() == id) {
                delete customersList[i];
                customersList.erase(customersList.begin() + i);
                capacity++;
            }
            }
    }
}

Customer* Trainer::getCustomer(int id) {
    for (int i = 0; i <(int) customersList.size(); i++) {
        if (customersList[i]->getId() == id)
            return customersList[i];
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return  customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return (std::vector<OrderPair> &) orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
   for(int i=0;i<(int)workout_ids.size();++i){
       orderList.emplace_back(customer_id,workout_options[workout_ids[i]]);
       salary=salary+workout_options[workout_ids[i]].getPrice();
   }
}
void Trainer::openTrainer() {
    open=true;
}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {
    return open;
}

const int Trainer::getOriginCapacity() {
    return orig_capacity;
}

void Trainer::UpdateSalary(int value) {
    salary=salary+value;
}

void Trainer::closeTrainer() {
    open = false;
    capacity=orig_capacity;
}