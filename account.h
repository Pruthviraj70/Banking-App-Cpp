#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <ctime>
#include <sqlite3.h>
#include <vector>

struct transaction {
    bool ttype;
    unsigned int tid;
    unsigned int tamount;
    int acc_sent;
    time_t ttime;
};

struct account {
    int acc_num;
    int balance;
    std::string name;
    std::string password;
};

int callback(void* data, int argc, char** argv, char** colName);

void init_db(sqlite3 *db);
void reset_db(sqlite3 *db);
void create_account();
void delete_account(int acc_num);
int switch_account(int acc_num);
void change_account_details(int acc_num);
void print_acc_details();
void print_transaction_history();
void withdraw();
void deposit();

#endif //ACCOUNT_H
