#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    ~Trainer();//destructor
    Trainer(const Trainer &other);//copy constructor
    Trainer(Trainer &&other);//move constructor
    Trainer& operator=(const Trainer &other);//copy assignment
    Trainer& operator=(Trainer &&other);//move assignment
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    void UpdateSalary(int value);
    bool isOpen();
    const int getOriginCapacity();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int salary;// the salary of the trainer until now
    int orig_capacity;//original capacity that the trainer gets in the beginning
};

#endif