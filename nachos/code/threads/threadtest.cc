#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "SuperMarket.h"
#include "list.h"
#include <stdio.h>
#include <iostream>

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < which; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        kernel->currentThread->Yield();
    }
}
void
SuperMarket_Run() {
    List<Casher*> *cashers = new List<Casher*>();
    Total_Stastics total_ss;
    Hour_Statistics hour_ss;
    List<Hour_Statistics*> all_hour_statistics;
    Wait_Queue *w_queue = new Wait_Queue();
    cashers->Append(new Casher());
    cashers->Append(new Casher());
    
    for(int i = 1; i <= 300; i++) {
        
        // generate new customers and add in the casher queue or wait queue, and also judge
        // whether to add more casher queue
        generateCustomer g_customers(i); // generate new customers list
        
        List<Customer*> *customers = new List<Customer*>(g_customers.getCustomers());
        
        ListIterator<Customer*> customers_iter(customers);
        
        for (; !customers_iter.IsDone(); customers_iter.Next()) {
            bool wait_flag = true; // set wait flag is true
            ListIterator<Casher*> cashers_iter(cashers);
            for (; !cashers_iter.IsDone(); cashers_iter.Next()) { // iterate all cashier
                Casher* one = cashers_iter.Item();
                if(one->getLen() < one->getMaxSize()) { // judge this cashier is full or not
                    wait_flag = false;
                    Customer *one_customer = customers_iter.Item();
                    one->push(one_customer); // add the new customer into this cashier
                    break;
                }
            }

            if(wait_flag == true) { // if all cashier are full, then set into wait queue
                w_queue->push(customers_iter.Item());
                if(w_queue->getLen() > w_queue->getIncreaseLen() && (int)cashers->NumInList() < 10) { // if the cashier's number is less than 10 and the wait queue's length is
                    // bigger than 10, open a new cashier and add the customers in the wait
                    // queue into this new cashier queue
                    Casher new_casher;
                    while(w_queue->getLen() > 0 && new_casher.getLen() < new_casher.getMaxSize()) {
                        // add the customers in wait queue to this new open cashier
                        // until the cashier is full or the wait queue's length is 0
                        Customer *one = w_queue->pop();
                        new_casher.push(one);
                    }
                    cashers->Append(new Casher()); // add this new open cashier in the cashers list
                }
            }
        }
        
        // check whether the serve for the customer in each casher been completed
        int len = (int)cashers->NumInList();
        for (int i = 0; i < len; i++) { // iterate all the cashiers in the casher list
            Casher* one = cashers->RemoveFront();
            Customer *temp_stored_customer = new Customer();
            int len = one->getLen();
            if(len > 0) {
                // store the front casher in the casher list
                temp_stored_customer = one->getFront();
            }
            // check whether the service for the customer been completed
            // if completed, then remove this customer from this cashier queue
            bool complete = one->checkComplete();
            // if the service is completed and there are customers in wait queue
            // add one customer from the wait queue to this cashier queue
            if(complete && w_queue->getLen() > 0) {
                Customer *front_wait = w_queue->pop();
                one->push(front_wait);
                
                // make statistics for the wait time and service time
                if(len > 0) {
                    hour_ss.set_Wait_time_service_time(temp_stored_customer->get_Wait_Time(),temp_stored_customer->get_Service_Time());
                }
                
            }
            // if the wait queue's length is 0, this cashier queue's length is 0
            // and the open cashiers' number is bigger than 2, then delete this cashier
            else if(w_queue->getLen() == 0 && one->getLen() == 0 && (int)cashers->NumInList() >= 2) {
                continue;
            }
            cashers->Append(one);
        }
        
        // increase wait time for both cashers queues and wait queue
        ListIterator<Casher*> increase_cash_iter(cashers);
        for (; !increase_cash_iter.IsDone(); increase_cash_iter.Next()) {
            increase_cash_iter.Item()->increase_wait_time();
        }
        w_queue->increase_wait_time();
        
        // make statistics for the customers' number each time
        hour_ss.add_Total_Customers(g_customers.getLen());
        
        // make statistics for the open lines
        int open_lines_num = (int)cashers->NumInList();
        hour_ss.set_open_line_num(open_lines_num);
        
        // make statistics for the cashers more than 3 in line
        ListIterator<Casher*> iter_cash_3(cashers);
        for (; !iter_cash_3.IsDone(); iter_cash_3.Next()) {
            if(iter_cash_3.Item()->getLen() > 3) {
                hour_ss.add_time_more3();
            }
        }
        
        
        // make statistics for the wait queue
        int waitQ_len = w_queue->getLen();
        hour_ss.set_num_waitQ(waitQ_len);
        
        if(i%60 == 0) { // make statistics for each hour
            std::cout << "From hour " << i/60 + 1 << " to hour " << i/60 + 2
            << ", stastics is as following:" << std::endl;
            hour_ss.printStatistic();
            Hour_Statistics *one_Hour = new Hour_Statistics(hour_ss);
            all_hour_statistics.Append(one_Hour);
            // reset the statistics to make statistics for next hour
            hour_ss.reset();
            std::cout << "-------------------------------------------------" << std::endl;
        }
    }
    
    // make statistics for total simulation
    while((int)all_hour_statistics.NumInList() != 0) {
        Hour_Statistics *hs = all_hour_statistics.RemoveFront();
        total_ss.set_waitT(hs->get_LWT(), hs->get_SWT(), hs->get_TWT());
        total_ss.set_serviceT(hs->get_LST(), hs->get_SST(), hs->get_TST());
        total_ss.set_maxW(hs->get_MWQ());
        total_ss.set_Total_Complete_Num(hs->get_TCC());
    }
    std::cout <<"For entire simulation, the stastics is as following:" << std::endl;
    total_ss.print();
}
void
ThreadTest()
{
    SuperMarket_Run();
}
