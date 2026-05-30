#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <ctime>
#include <sqlite3.h>
#include <vector>


void init_db(sqlite3 *db);
void create_account();
int switch_account(int acc_num);
void change_account_details(int acc_num);
struct transaction {
    bool ttype;
    unsigned tid;
    unsigned tamount;
    int acc_sent;
    time_t ttime;
};

class account {
    public:
        account();
        void print_acc_details();
        void print_transaction_history(int no = 5);
        void withdraw();
        void deposit();
        ~account();
    private:
        int acc_num;
        int balance;
        std::string name;
        std::vector<transaction> transactions;
};

#endif //ACCOUNT_H
