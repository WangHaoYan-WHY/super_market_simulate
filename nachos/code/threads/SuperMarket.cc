#include "SuperMarket.h"

Customer::Customer() {
    item_num = rand()%(CUSTOMERS_ITEM_MAX - CUSTOMERS_ITEM_MIN + 1) + CUSTOMERS_ITEM_MIN;
    service_time = CASH_START_TIME + CASH_END_TIME + SCAN_TIME * getNum();
    int round_up_time = service_time/60;
    if(service_time%60 != 0) {
        round_up_time += 1;
    }
    service_time = round_up_time;
    wait_time = 0;
}

Customer::~Customer() {
}

// get the item number
int Customer::getNum() {
    return item_num;
}

// increase this customer's wait time
int Customer::increase_Wait_time() {
    wait_time += 1;
    return wait_time;
}

// get service time of this customer
int Customer::get_Service_Time() {
    return service_time;
}

// get wait time for this customer
int Customer::get_Wait_Time() {
    return wait_time;
}

generateCustomer::generateCustomer(int t) {
    current_time = t;
    if(t > TIME_PEAK_START && t <= TIME_PEAK_END) {
        customer_num = rand()%(CUSTOMERS_PEAK_MAX - CUSTOMERS_PEAK_MIN + 1) + CUSTOMERS_PEAK_MIN;
    }
    else {
        customer_num = rand()%(CUSTOMERS_NORMAL_MAX - CUSTOMERS_NORMAL_MIN + 1) + CUSTOMERS_NORMAL_MIN;
    }
    for(int i = 0; i < customer_num; i++) {
        Customer *new_customer = new Customer();
        customers.Append(new_customer);
    }
}

// get generated customers' list
List<Customer*> generateCustomer::getCustomers() {
    return customers;
}

// get the length of the customers' list
int generateCustomer::getLen() {
    return (int)customers.NumInList();
}

Casher::Casher() {
    start_time = CASH_START_TIME;
    close_time = CASH_END_TIME;
    max_size = CASH_QUEUE_LEN;
    scan_each_time = SCAN_TIME;
}

Casher::~Casher() {
}

// push customer into the queue
bool Casher::push(Customer *c) {
    if(getLen() == max_size) {
        return false;
    }
    customer_q.Append(c);
    int all_serve_time = start_time + close_time + scan_each_time * c->getNum();
    int* roud_up_time = new int(0);
    *roud_up_time = all_serve_time/60;
    if(all_serve_time%60 != 0) {
        *roud_up_time = *roud_up_time + 1;
    }
    service_times.Append(roud_up_time);
    return true;
}

// pop the first customer in the queue
Customer* Casher::pop() {
    Customer *c = customer_q.RemoveFront();
    service_times.RemoveFront();
    return c;
}

// judge the customer queue is empty or not
bool Casher::Empty() {
    return customer_q.IsEmpty();
}

// get the length of the customer queue
int Casher::getLen() {
    return (int)customer_q.NumInList();
}

// get the max size of the customer queue
int Casher::getMaxSize() {
    return max_size;
}

// check whether the service customer in this cashier is completed
bool Casher::checkComplete() {
    if(getLen() == 0) {
        return true;
    }
    int *first_time = service_times.RemoveFront();
    *first_time = *first_time - 1;
    service_times.Prepend(first_time);
    if(*service_times.Front() == 0) {
        pop();
        return true;
    }
    return false;
}

// increase the wait time for the waiting customer
void Casher::increase_wait_time() {
    int len = (int)customer_q.NumInList();
    for (int i = 0; i < len; i++) {
        Customer *one = customer_q.RemoveFront();
        if(i != 0) {
            one->increase_Wait_time();
        }
        customer_q.Append(one);
    }
}

// get the front customer in the queue
Customer* Casher::getFront() {
    return customer_q.Front();
}

Wait_Queue::Wait_Queue() {
    increase_len = WAIT_LEN_INCREASE;
}

// push the customer into the queue
void Wait_Queue::push(Customer *c) {
    wait_q.Append(c);
}

// pop the first customer from the queue
Customer* Wait_Queue::pop() {
    Customer *c = wait_q.RemoveFront();
    return c;
}

// judge the queue is empty or not
bool Wait_Queue::Empty() {
    return wait_q.IsEmpty();
}

// get the length of the wait queue
int Wait_Queue::getLen() {
    return (int)wait_q.NumInList();
}

// get the lenth which means bigger than this
// length should create a new cashier
int Wait_Queue::getIncreaseLen() {
    return increase_len;
}

// increase the wait time of customers in the wait queue
void Wait_Queue::increase_wait_time() {
    int len = (int)wait_q.NumInList();
    for (int i = 0; i < len; i++) {
        Customer *one = wait_q.RemoveFront();
        one->increase_Wait_time();
        wait_q.Append(one);
    }
}

Total_Stastics::Total_Stastics() {
    longest_wait_time = -1;
    shortest_wait_time = 10000;
    total_wait_time = 0;
    longest_service_time = -1;
    shortest_service_time = 10000;
    total_service_time = 0;
    max_num_waitQ = 0;
    total_complete_num = 0;
}
void Total_Stastics::set_Total_Complete_Num(int total_num) {
    total_complete_num += total_num;
}
void Total_Stastics::set_waitT(int lwt, int swt, int twt) {
    longest_wait_time = std::max(longest_wait_time, lwt);
    shortest_wait_time = std::min(shortest_wait_time, swt);
    total_wait_time += twt;
}
void Total_Stastics::set_serviceT(int lst, int sst, int tst) {
    longest_service_time = std::max(longest_service_time, lst);
    shortest_service_time = std::min(shortest_service_time, sst);
    total_service_time += tst;
}
void Total_Stastics::set_maxW(int max_num_wait) {
    max_num_waitQ = std::max(max_num_waitQ, max_num_wait);
}
void Total_Stastics::print() {
    std::cout << "average/shortest/longest waiting time: "
    << (total_complete_num == 0 ? 0 : total_wait_time/total_complete_num)
    << "   " << shortest_wait_time << "   " << longest_wait_time << std::endl;
    std::cout << "average/shortest/longest service time: "
    << (total_complete_num == 0 ? 0 : total_service_time/total_complete_num)
    << "   " << shortest_service_time << "   " << longest_service_time << std::endl;
    std::cout << "maximum number of customers in the waiting queue at any time: "
    << max_num_waitQ << std::endl;
}

Hour_Statistics::Hour_Statistics() {
    total_customers = 0;
    total_wait_time = 0;
    total_service_time = 0;
    longest_wait_time = -1;
    shortest_wait_time = 10000;
    longest_service_time = -1;
    shortest_service_time = 10000;
    total_open_lines = 2;
    max_open_lines = 2;
    total_time_more3 = 0;
    max_num_waitQ = -1;
    min_num_waitQ = 10000;
    total_time_waitQ = 0;
    total_complete_customer = 0;
}

void Hour_Statistics::add_Total_Customers(int num) {
    total_customers += num;
}

void Hour_Statistics::set_Wait_time_service_time(int wait_time, int service_time) {
    total_wait_time += wait_time;
    total_service_time += service_time;
    longest_wait_time = std::max(longest_wait_time, wait_time);
    shortest_wait_time = std::min(shortest_wait_time, wait_time);
    longest_service_time = std::max(longest_service_time, service_time);
    shortest_service_time = std::min(shortest_service_time, service_time);
    total_complete_customer++;
}

void Hour_Statistics::set_open_line_num(int open_lines_num) {
    total_open_lines += open_lines_num;
    max_open_lines = std::max(max_open_lines, open_lines_num);
}

void Hour_Statistics::add_time_more3() {
    total_time_more3++;
}

void Hour_Statistics::set_num_waitQ(int num_wait) {
    max_num_waitQ = std::max(max_num_waitQ, num_wait);
    min_num_waitQ = std::min(min_num_waitQ, num_wait);
    total_time_waitQ += num_wait;
}

void Hour_Statistics::printStatistic() {
    std::cout << "average number of customers arriving for checkout: "
    << total_customers/60 << std::endl;
    std::cout << "average/shortest/longest waiting time: "
    << (total_complete_customer == 0 ? 0 : total_wait_time/total_complete_customer)
    << "   " << shortest_wait_time << "   " << longest_wait_time << std::endl;
    std::cout << "average/shortest/longest service time: "
    << (total_complete_customer == 0 ? 0 : total_service_time/total_complete_customer)
    << "   " << shortest_service_time << "   " << longest_service_time << std::endl;
    std::cout << "average number of open lines: "
    << total_open_lines/60 << std::endl;
    std::cout << "maximum number of open lines: "
    << max_open_lines << std::endl;
    std::cout << "average time each casher will have more than 3 customers standing in line: "
    << total_time_more3/60 << std::endl;
    std::cout << "average/smallest/largest number of customers in the waiting queue: "
    << total_time_waitQ/60 << "   " << min_num_waitQ << "   "
    << max_num_waitQ << std::endl;
}

void Hour_Statistics::reset() {
    total_customers = 0;
    total_wait_time = 0;
    total_service_time = 0;
    longest_wait_time = -1;
    shortest_wait_time = 10000;
    longest_service_time = -1;
    shortest_service_time = 10000;
    total_open_lines = 2;
    max_open_lines = 2;
    total_time_more3 = 0;
    max_num_waitQ = -1;
    min_num_waitQ = 10000;
    total_time_waitQ = 0;
    total_complete_customer = 0;
}

int Hour_Statistics::get_TWT() {
    return total_wait_time;
}

int Hour_Statistics::get_LWT() {
    return longest_wait_time;
}

int Hour_Statistics::get_SWT() {
    return shortest_wait_time;
}

int Hour_Statistics::get_TST() {
    return total_service_time;
}

int Hour_Statistics::get_LST() {
    return longest_service_time;
}

int Hour_Statistics::get_SST() {
    return shortest_service_time;
}

int Hour_Statistics::get_MWQ() {
    return max_num_waitQ;
}

int Hour_Statistics::get_TCC() {
    return total_complete_customer;
}
