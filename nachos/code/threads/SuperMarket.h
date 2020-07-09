#ifndef SuperMarket_H
#define SuperMarket_H

#include "list.h"
#include<stdio.h>
#include<stdlib.h>
#include <iostream>

#define CUSTOMERS_ITEM_MIN 5
#define CUSTOMERS_ITEM_MAX 40
#define TIME_PEAK_START 120
#define TIME_PEAK_END 240
#define CUSTOMERS_PEAK_MIN 5
#define CUSTOMERS_PEAK_MAX 10
#define CUSTOMERS_NORMAL_MIN 0
#define CUSTOMERS_NORMAL_MAX 5
#define CASH_START_TIME 10
#define CASH_END_TIME 90
#define CASH_QUEUE_LEN 5
#define WAIT_LEN_INCREASE 10
#define SCAN_TIME 5

// store item number, service time and wait time for each customer
class Customer {
public:
    Customer();
    ~Customer();
    // get the item number
    int getNum();
    // increase this customer's wait time
    int increase_Wait_time();
    // get service time of this customer
    int get_Service_Time();
    // get the wait time of this customer
    int get_Wait_Time();
private:
    int item_num;
    int service_time;
    int wait_time;
};

// generate customers each minute according
// to the peak time and normal time
class generateCustomer {
public:
    generateCustomer(int t);
    // get generated customers' list
    List<Customer*> getCustomers();
    // get the length of the customers' list
    int getLen();
private:
    int current_time;
    int customer_num;
    List<Customer*> customers;
};

// store customers' left service time and customers
// queue in this cashier
class Casher {
public:
    Casher();
    ~Casher();
    // push customer into the queue
    bool push(Customer *c);
    // pop the first customer in the queue
    Customer* pop();
    // judge the customer queue is empty or not
    bool Empty();
    // get the length of the customer queue
    int getLen();
    // get the max size of the customer queue
    int getMaxSize();
    // check whether the service customer in this
    // cashier is completed
    bool checkComplete();
    // increase the wait time for the waiting customer
    void increase_wait_time();
    // get the front customer in the queue
    Customer* getFront();
private:
    int scan_each_time;
    int start_time;
    int close_time;
    int max_size;
    List<Customer*> customer_q;
    List<int*> service_times;
};

// store the customer in the wait queue
class Wait_Queue {
public:
    Wait_Queue();
    // push the customer into the queue
    void push(Customer *c);
    // pop the first customer from the queue
    Customer* pop();
    // judge the queue is empty or not
    bool Empty();
    // get the length of the wait queue
    int getLen();
    // get the lenth which means bigger than this
    // length should create a new cashier
    int getIncreaseLen();
    // increase the wait time of customers in the wait queue
    void increase_wait_time();
private:
    List<Customer*> wait_q;
    int increase_len;
};

// make statistics for the total
class Total_Stastics {
public:
    Total_Stastics();
    void set_Total_Complete_Num(int total_num);
    void set_waitT(int lwt, int swt, int twt);
    void set_serviceT(int lst, int sst, int tst);
    void set_maxW(int max_num_wait);
    void print();
private:
    int longest_wait_time, shortest_wait_time, total_wait_time;
    int longest_service_time, shortest_service_time, total_service_time;
    int max_num_waitQ;
    int total_complete_num;
};

//make statistics for each hour
class Hour_Statistics {
public:
    Hour_Statistics();
    void add_Total_Customers(int num);
    void set_Wait_time_service_time(int wait_time, int service_time);
    void set_open_line_num(int open_lines_num);
    void add_time_more3();
    void set_num_waitQ(int num_wait);
    void printStatistic();
    void reset();
    int get_TWT();
    int get_LWT();
    int get_SWT();
    int get_TST();
    int get_LST();
    int get_SST();
    int get_MWQ();
    int get_TCC();
private:
    int total_customers;
    int total_wait_time, total_service_time;
    int longest_wait_time, shortest_wait_time;
    int longest_service_time, shortest_service_time;
    int total_open_lines, max_open_lines;
    int total_time_more3;
    int total_time_waitQ, max_num_waitQ, min_num_waitQ;
    int total_complete_customer;
};


#endif /* SuperMarket_h */
