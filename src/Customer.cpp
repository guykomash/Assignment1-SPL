#include "../include/Customer.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

Customer::Customer(std::string c_name, int c_id) :
    name(c_name), id(c_id){}

int Customer::getId() const {
    return id;
}
std::string Customer::getName() const {
    return name;}


//Sweaty Customer
SweatyCustomer::SweatyCustomer(std::string name, int id):
    Customer(name,id){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> wk(0,0);
    for(int i=0;i<(int)workout_options.size();++i){
        if(workout_options[i].getType()==CARDIO)
            wk.push_back(workout_options[i].getId());
    }
    return wk;}

std::string SweatyCustomer::toString() const {
    return "swt";
}

SweatyCustomer *SweatyCustomer::clone() {
    SweatyCustomer *sweatyCustomer=new SweatyCustomer(getName(),getId());
    return sweatyCustomer;
}
//Cheap Customer
CheapCustomer::CheapCustomer(std::string name, int id):
        Customer(name,id)
{}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    int min=workout_options[0].getPrice();
    int min_id;
    std::vector<int> wk(0,0);
    for(int i=1;i<(int)workout_options.size();++i) {
        if (workout_options[i].getPrice() <= min) {
            min = workout_options[i].getPrice();
            min_id = workout_options[i].getId();
        }
        if(workout_options[i].getPrice()==min and workout_options[i].getId()<min_id)
            min_id=workout_options[i].getId();
    }
    wk.push_back(min_id);
    return wk;
}

std::string CheapCustomer::toString() const {
    return "chp";
}

CheapCustomer *CheapCustomer::clone() {
    CheapCustomer *cheapCustomer=new CheapCustomer(getName(),getId());
    return cheapCustomer;
}
//Heavy Muscle Customer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):
        Customer(name,id)
{}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> wk(0,0);
    std::vector <Workout> wo;
    for(int i=0;i<(int)workout_options.size();i++){
        if(workout_options[i].getType()==ANAEROBIC)
            wo.push_back(Workout(workout_options[i].getId(),workout_options[i].getName(),workout_options[i].getPrice(),workout_options[i].getType()));
    }
    std::vector<std::pair<int,int>> wo_pair;//first is the price of the workout,second is the id;sort by new vector
    for( int i=0;i<(int)wo.size();i++)
        wo_pair.emplace_back(wo[i].getPrice(),wo[i].getId());
    std::sort(wo_pair.begin(), wo_pair.end(), [](std::pair<int,int> a, std::pair<int,int> b) {
        if (a.first!= b.first) return a.first>b.first;
        return a.second <b.second;
    });
    wo.clear();
    for(int i=0;i<(int)wo_pair.size();i++)
        wo.push_back(Workout(workout_options[wo_pair[i].second].getId(),workout_options[wo_pair[i].second].getName(),workout_options[wo_pair[i].second].getPrice(),workout_options[wo_pair[i].second].getType()));
    for(int i=0;i<(int)wo.size();i++)
        wk.push_back(wo[i].getId());
    wo.clear();
    return wk;
}

std::string HeavyMuscleCustomer::toString() const {
    return "mcl";
}

HeavyMuscleCustomer *HeavyMuscleCustomer::clone() {
    HeavyMuscleCustomer *heavyMuscleCustomer=new HeavyMuscleCustomer(getName(),getId());
    return heavyMuscleCustomer;
}
//Full Body Customer
FullBodyCustomer::FullBodyCustomer(std::string name, int id) :
        Customer(name,id)
{}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> wk(0,0);
    int min_c,max_m,min_a;//prices
    int min_c_d,max_m_d,min_a_d;//id's
    std::vector<Workout> wo_c,wo_m,wo_a;//workouts
    for(int i=0;i<(int)workout_options.size();i++){
        if(workout_options[i].getType()==CARDIO)
            wo_c.push_back(workout_options[i]);
        if(workout_options[i].getType()==ANAEROBIC)
            wo_a.push_back(workout_options[i]);
        if(workout_options[i].getType()==MIXED)
            wo_m.push_back(workout_options[i]);
    }
    min_c=wo_c[0].getPrice();
    min_c_d=wo_c[0].getId();
    for (int i = 1; i <(int)wo_c.size(); ++i) {
        if(min_c>wo_c[i].getPrice()){
            min_c=wo_c[i].getPrice();
            min_c_d=wo_c[i].getId();
        }
        if(min_c==wo_c[i].getPrice() and min_c_d>wo_c[i].getId() ){
            min_c_d=wo_c[i].getId();
        }
    }
    min_a=wo_a[0].getPrice();
    min_a_d=wo_a[0].getId();
    for (int i = 1; i <(int)wo_a.size(); i++) {
        if(min_a>wo_a[i].getPrice()){
            min_a=wo_a[i].getPrice();
            min_a_d=wo_a[i].getId();
        }
        if(min_a==wo_a[i].getPrice() and min_a_d>wo_a[i].getId() ){
            min_a_d=wo_a[i].getId();
        }
    }
    max_m=wo_m[0].getPrice();
    max_m_d=wo_m[0].getId();
    for (int i = 1; i < (int)wo_m.size(); i++) {
        if(max_m<wo_m[i].getPrice()){
            max_m=wo_m[i].getPrice();
            max_m_d=wo_m[i].getId();
        }
        if(max_m==wo_m[i].getPrice() and max_m_d>wo_m[i].getId() ){
            max_m_d=wo_m[i].getId();
        }
    }
    wk.push_back(min_c_d);
    wk.push_back(max_m_d);
    wk.push_back(min_a_d);
    return wk;
}

std::string FullBodyCustomer::toString() const {
    return "fbd";
}

FullBodyCustomer *FullBodyCustomer::clone() {
FullBodyCustomer *fullBodyCustomer=new FullBodyCustomer(getName(),getId());
return fullBodyCustomer;
}