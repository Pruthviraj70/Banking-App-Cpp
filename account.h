#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
extern "C" {
    #include "sqlite3.h"
}

struct account {
    int acc_num;
    int balance;
    std::string name;
    std::string password;
};

int callback(void* data, int argc, char** argv, char** colName);
int print_callback(void* data, int argc, char** argv, char** colName);

void init_db(sqlite3 **db);
void reset_db(sqlite3 *db);
struct account* create_account(sqlite3 *db);
struct account* delete_account(sqlite3 *db, struct account *accountd);
struct account* switch_account(sqlite3 *db);
void print_acc_details(sqlite3 *db, struct account *accountd);
void print_transaction_history(sqlite3 *db, struct account *accountd);
void withdraw(sqlite3 *db, struct account *accountd);
void deposit(sqlite3 *db, struct account *accountd);

#endif //ACCOUNT_H
