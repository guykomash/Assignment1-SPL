#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{
public:
    Studio(); //constructor
    ~Studio();//destructor
    Studio(Studio &&other);//move constructor
    Studio& operator=(const Studio &other);//copy assignment
    Studio& operator=(Studio &&other);//move assignment
    Studio(const std::string &configFilePath);//config constructor
    Studio(const Studio &other);//copy constructor
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer *> getTrainers();
    void close();
    void pushAction(BaseAction* action);
private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int NumOfTrainers;
};

#endif