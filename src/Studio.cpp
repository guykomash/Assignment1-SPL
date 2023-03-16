#include "../include/Studio.h"
#include "fstream"
#include <iostream>

using namespace std;
Studio::Studio() {}//constructor
Studio::~Studio() {//destructor
    for (int i = 0; i < (int)trainers.size(); ++i) {
        if (trainers[i] != nullptr)
            delete trainers[i];
    }
    trainers.clear();
    for (int i = 0; i <(int)actionsLog.size(); ++i) {
         if (actionsLog[i] != nullptr) {
             delete actionsLog[i];
         }
    }
    actionsLog.clear();
}
Studio::Studio(const std::string &configFilePath) {// Config Constructor
    open=false;
    std::vector<string> configData;
    ifstream configFile(configFilePath);
    string line;
    while (getline(configFile, line)) {
        if (line.front() != '#' && line.empty() != true) {
            configData.push_back(line);
        }
    }
     NumOfTrainers =stoi(configData[0]);
        for (int i = 0; i < (int)configData[1].size(); i++) {
            std::string capacity_str = "";
            while (i < (int)configData[1].size() && configData[1][i] != ',') {
                capacity_str = capacity_str + configData[1][i];
                i++;
            }
            Trainer *trainer = new Trainer(stoi(capacity_str));;
            trainers.push_back(trainer);
        }
    for (int workout_idx = 2; workout_idx < (int)configData.size(); workout_idx++) {
            int i = 0;
            std::string name = "";
            while (i < (int)configData[workout_idx].size() && configData[workout_idx][i] != ',') {
                name = name + configData[workout_idx][i];
                i++;
            }
            i = i + 2;
            std::string type = "";
            while (i < (int)configData[workout_idx].size() && configData[workout_idx][i] != ',') {
                type = type + configData[workout_idx][i];
                i++;
            }
            i = i + 2;
            std::string price = "";
            while (i < (int)configData[workout_idx].size() && configData[workout_idx][i] != ',') {
                price = price + configData[workout_idx][i];
                i++;
            }
            WorkoutType workout_type;
            if (type == "Mixed")
                workout_type = MIXED;
            if (type == "Anaerobic")
                workout_type = ANAEROBIC;
            if (type == "Cardio")
                workout_type = CARDIO;
            Workout workout(workout_idx - 2, name, stoi(price), workout_type);
            workout_options.push_back(workout);
    }
}
    Studio::Studio(const Studio &other) {//copy constructor
        open = other.open;
        for (auto i: other.workout_options)//copy of the workouts
            workout_options.push_back(Workout(i.getId(), i.getName(), i.getPrice(), i.getType()));
        for (int i = 0; i < (int)other.trainers.size(); ++i){
                trainers.push_back(new Trainer(*other.trainers[i]));
        }
        for (int i = 0; i <(int) other.actionsLog.size(); ++i){
           actionsLog.push_back(other.actionsLog[i]->clone());
        }
        NumOfTrainers=other.NumOfTrainers;
    }
    Studio::Studio(Studio && other)//move constructor
    {
        open = other.open;
        for (auto i: other.workout_options)
            workout_options.push_back(Workout(i.getId(), i.getName(), i.getPrice(), i.getType()));
        trainers=other.trainers;
        for (int i = 0; i <(int) other.trainers.size(); ++i)
            other.trainers[i]= nullptr;
        actionsLog = other.actionsLog;
        for (int i = 0; i < (int)other.actionsLog.size(); ++i)
             other.actionsLog[i]= nullptr;
        other.trainers.clear();
        other.actionsLog.clear();
        NumOfTrainers=other.NumOfTrainers;
    }
    Studio &Studio::operator=(Studio &&other) {//move assignment operator
        //delete this
        for (int i = 0; i < (int)trainers.size(); ++i)
            delete trainers[i];
        trainers.clear();
        for (int i = 0; i <(int ) actionsLog.size(); ++i)
            delete actionsLog[i];
        actionsLog.clear();
        open = other.open;
        for (auto i: other.workout_options)//copy of the workouts
            workout_options.push_back(Workout(i.getId(), i.getName(), i.getPrice(), i.getType()));
        trainers = other.trainers;
        actionsLog = other.actionsLog;
        NumOfTrainers=other.NumOfTrainers;
        for (int i = 0; i <(int) other.trainers.size();++i)
                other.trainers[i]= nullptr;
        other.trainers.clear();
        for (int i = 0; i <(int) other.actionsLog.size();++i)
             other.actionsLog[i]= nullptr;
        other.actionsLog.clear();
        NumOfTrainers=other.NumOfTrainers;
        return *this;
    }
    Studio &Studio::operator=(const Studio &other) {//assignment operator
    if (this != &other) {
            //delete this
            for (int i = 0; i <(int)trainers.size(); ++i)
                delete trainers[i];
            trainers.clear();
            for (int i = 0; i <(int) actionsLog.size(); ++i)
                delete actionsLog[i];
            actionsLog.clear();
            //stack fields
            workout_options.clear();
            open = other.open;
            //copy content from other
            for (auto i: other.workout_options)//copy of the workouts
                workout_options.push_back(Workout(i.getId(), i.getName(), i.getPrice(), i.getType()));
            for (int i = 0; i <(int) other.trainers.size(); ++i){
                trainers.push_back(new Trainer(*other.trainers[i]));
            }
            for (int i = 0; i < (int)other.actionsLog.size(); ++i)
             actionsLog.push_back(other.actionsLog[i]->clone());
            NumOfTrainers=other.NumOfTrainers;
        }
        return *this;
    }//copy assignment operator
    void Studio::start() {
        open = true;
        std::cout << "Studio in now open!";
        std::string action;
        getline(cin >> ws, action);
        int id_customer=0;
        while(action!="closeall"){
            if (action.find("open") != std::string::npos) {//open action
                int j = 5;
                std::string t_id = "";
                while (j < (int)action.size() && action[j] != ' ') {
                    t_id = t_id + action[j];
                    j++;
                }
                j++;
                int trainer_id = stoi(t_id);
                Trainer *trainer = getTrainer(trainer_id);
                std::vector<Customer *> cus;
                int k = 0;
                for (int i = j; i < (int)action.size() && k < trainer->getCapacity(); i++) {
                    std::string name = "";
                    while (i < (int)action.size() && action[i] != ',') {//reading the name
                        name = name + action[i];
                        i++;
                    }
                    i++;
                    std::string type_customer = "";
                    while (i < (int)action.size() && action[i] != ' ') {//reading the type
                        type_customer = type_customer + action[i];
                        i++;
                    }
                    if (type_customer == "swt") {
                        SweatyCustomer *c = new SweatyCustomer(name, id_customer);
                        cus.push_back(c);
                    }
                    if (type_customer == "chp") {
                        CheapCustomer *c = new CheapCustomer(name, id_customer);
                        cus.push_back(c);
                    }

                    if (type_customer == "mcl") {
                        HeavyMuscleCustomer *c = new HeavyMuscleCustomer(name, id_customer);
                        cus.push_back(c);
                    }

                    if (type_customer == "fbd") {
                        FullBodyCustomer *c = new FullBodyCustomer(name, id_customer);
                        cus.push_back(c);
                    }
                    id_customer++;
                    k++;
                }


                OpenTrainer *openTrainer=new OpenTrainer(trainer_id,cus);
                openTrainer->act(*this);
                pushAction(openTrainer);
            }
            if (action.find("order") != std::string::npos){//order action
                int j=6;
                std::string t_id="";
                while(j<(int)action.size() && action[j]!=' ') {
                    t_id=t_id+action[j];
                    j++;
                }
                int trainer_id= stoi(t_id);
                Order *order=new Order(trainer_id);
                order->act(*this);
                pushAction(order);
            }

            if (action.find("move") != std::string::npos){//move action
                int j=5;
                std::string t_src="";
                while(j<(int)action.size() && action[j]!=' ') {
                    t_src=t_src+action[j];
                    j++;
                }
                j++;
                int src= stoi(t_src);
                std::string t_dst="";
                while(j<(int)action.size() && action[j]!=' ') {
                    t_dst=t_dst+action[j];
                    j++;
                }
                int dst= stoi(t_dst);
                j++;
                std::string t_id="";
                while(j<(int)action.size() && action[j]!=' ') {
                    t_id=t_id+action[j];
                    j++;
                }
                int id=stoi(t_id);
                MoveCustomer *moveCustomer=new MoveCustomer(src,dst,id);
                moveCustomer->act(*this);
                pushAction(moveCustomer);

            }
            if (action.find("close") != std::string::npos){//close action
                int j=6;
                std::string t_id="";
                while(j<(int)action.size() && action[j]!=' ') {
                    t_id=t_id+action[j];
                    j++;
                }
                int trainer_id= stoi(t_id);
                Close *close=new Close(trainer_id);
                close->act(*this);
                pushAction(close);
            }
            if (action.find("workout_options") != std::string::npos){//printworkout action
                PrintWorkoutOptions *print=new PrintWorkoutOptions();
                print->act(*this);
                pushAction(print);

            }
            if (action.find("status") != std::string::npos){//print trainer status action
                int j=7;
                std::string t_id="";
                while(j<(int)action.size() && action[j]!=' ') {
                    t_id=t_id+action[j];
                    j++;
                }

                int trainer_id= stoi(t_id);
                PrintTrainerStatus *print=new PrintTrainerStatus(trainer_id);
                print->act(*this);
                pushAction(print);
            }
            if (action.find("log") != std::string::npos){//print log action
                PrintActionsLog *print=new PrintActionsLog();
                print->act(*this);
                pushAction(print);

            }

            if (action.find("backup") != std::string::npos){//backup action
                BackupStudio* back_up=new BackupStudio();
                back_up->act(*this);
                pushAction(back_up);


            }
            if (action.find("restore") != std::string::npos){//restore action
                RestoreStudio* restore=new RestoreStudio();
                restore->act(*this);
                pushAction(restore);
            }
            getline(cin >> ws, action);
        }
        CloseAll *closeAll=new CloseAll();
        closeAll->act(*this);
        delete closeAll;
    }
    int Studio::getNumOfTrainers() const {
        return NumOfTrainers;
    }

    Trainer *Studio::getTrainer(int tid) {
        if (tid >(int) trainers.size() - 1)
            return nullptr;
        else
            return trainers[tid];
    }

    const std::vector<BaseAction *> &Studio::getActionsLog() const {
        return actionsLog;
    }

    std::vector<Workout> &Studio::getWorkoutOptions() {
        return workout_options;
    }

    std::vector<Trainer *> Studio::getTrainers() {
        return trainers;
    }

void Studio::close() {
    open=false;
}

void Studio::pushAction(BaseAction *action) {
    actionsLog.push_back(action);
}